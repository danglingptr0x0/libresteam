#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <dangling/core/arith.h>
#include <dangling/core/bits.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/expr.h>

#define EXPR_EXACT_REAL_MAX UINT64_C(9007199254740992)
#define EXPR_UNARY_PREC 6U
#define EXPR_FP32_BYTES 4U
#define EXPR_FP64_BYTES 8U
#define EXPR_FP32_FRACTION_BITS 23U
#define EXPR_FP32_EXPONENT_MASK 0xffU
#define EXPR_FP32_FRACTION_MASK UINT64_C(0x7fffff)
#define EXPR_FP32_IMPLICIT_BIT UINT64_C(0x800000)
#define EXPR_FP32_SIGN_BIT UINT64_C(0x80000000)
#define EXPR_FP32_SCALED_BIAS 150
#define EXPR_FP32_SUBNORMAL_EXPONENT (-149)

typedef enum expr_op
{
    EXPR_LITERAL,
    EXPR_VAR,
    EXPR_NEG,
    EXPR_NOT,
    EXPR_ABS,
    EXPR_SIN,
    EXPR_COS,
    EXPR_SQRT,
    EXPR_FLOOR,
    EXPR_MIN,
    EXPR_MAX,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_MOD,
    EXPR_POW,
    EXPR_LT,
    EXPR_LE,
    EXPR_GT,
    EXPR_GE,
    EXPR_EQ,
    EXPR_NE,
    EXPR_AND,
    EXPR_OR
} expr_op_t;

uint8_t libresteam_ext_repl_expr_truth_is(const libresteam_ext_repl_expr_val_t *val)
{
    if (LDG_UNLIKELY(!val)) { return LDG_ERR_FUNC_ARG_NULL; }

    return (val->fp ? val->real != 0.0 : val->mag != 0) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

uint32_t libresteam_ext_repl_expr_real_get(const libresteam_ext_repl_expr_val_t *val, double *out)
{
    if (LDG_UNLIKELY(!val || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0.0;
    if (!val->fp && val->mag > EXPR_EXACT_REAL_MAX) { return LDG_ERR_RANGE; }

    *out = val->fp ? val->real : (val->neg ? -(double)val->mag : (double)val->mag);
    return isfinite(*out) ? LDG_ERR_AOK : LDG_ERR_RANGE;
}

uint32_t libresteam_ext_repl_expr_decode(uint64_t bits, uint32_t bytes, uint8_t fp, uint8_t signed_val, libresteam_ext_repl_expr_val_t *out)
{
    uint64_t sign = 0x0;
    uint64_t mask = UINT64_MAX;
    uint64_t fraction = 0x0;
    uint32_t exponent = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(out, sizeof(*out));
    if (res != LDG_ERR_AOK) { return res; }

    if (bytes != 1U && bytes != 2U && bytes != EXPR_FP32_BYTES && bytes != EXPR_FP64_BYTES) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (bytes < sizeof(bits)) { mask = (UINT64_C(1) << (bytes * LDG_BYTE_BITS)) - 1U; }

    bits &= mask;
    if (!fp)
    {
        sign = UINT64_C(1) << (bytes * LDG_BYTE_BITS - 1U);
        out->neg = signed_val && (bits & sign) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
        out->mag = out->neg ? ((~bits & mask) + 1U) : bits;
        return LDG_ERR_AOK;
    }

    out->fp = LDG_TRUTH_TRUE;
    if (bytes == EXPR_FP64_BYTES) { res = ldg_mem_secure_cpy(&out->real, &bits, sizeof(bits)); }
    else if (bytes == EXPR_FP32_BYTES)
    {
        exponent = (uint32_t)((bits >> EXPR_FP32_FRACTION_BITS) & EXPR_FP32_EXPONENT_MASK);
        fraction = bits & EXPR_FP32_FRACTION_MASK;
        if (exponent == EXPR_FP32_EXPONENT_MASK) { return LDG_ERR_RANGE; }

        out->real = exponent ? ldexp((double)(fraction | EXPR_FP32_IMPLICIT_BIT), (int)exponent - EXPR_FP32_SCALED_BIAS) : ldexp((double)fraction, EXPR_FP32_SUBNORMAL_EXPONENT);
        if (bits & EXPR_FP32_SIGN_BIT) { out->real = -out->real; }
    }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    return res != LDG_ERR_AOK ? res : (isfinite(out->real) ? LDG_ERR_AOK : LDG_ERR_RANGE);
}

static void expr_space_skip(libresteam_ext_repl_expr_t *expr)
{
    while (expr->text[expr->cursor] == ' ' || expr->text[expr->cursor] == '\t') { expr->cursor++; }
}

static uint8_t expr_word_is(const char *text, uint32_t len, const char *word)
{
    uint32_t idx = 0x0;

    for (idx = 0; idx < len; idx++) { if (!word[idx] || text[idx] != word[idx]) { return LDG_TRUTH_FALSE; } }
    return word[len] == LDG_STR_TERM ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t expr_number_parse(libresteam_ext_repl_expr_t *expr, libresteam_ext_repl_expr_val_t *out)
{
    uint32_t start = expr->cursor;
    uint32_t base = LDG_BASE_DEC;
    uint32_t res = LDG_ERR_AOK;
    char *end = 0x0;
    uint8_t fp = LDG_TRUTH_FALSE;

    if (expr->text[expr->cursor] == '0' && (expr->text[expr->cursor + 1U] == 'x' || expr->text[expr->cursor + 1U] == 'X'))
    {
        expr->cursor += 2U;
        start = expr->cursor;
        base = LDG_BASE_HEX;
        while (ldg_char_hex_is((uint8_t)expr->text[expr->cursor]) == LDG_TRUTH_TRUE) { expr->cursor++; }
    }
    else
    {
        while (ldg_char_digit_is((uint8_t)expr->text[expr->cursor]) == LDG_TRUTH_TRUE) { expr->cursor++; }
        if (expr->text[expr->cursor] == '.')
        {
            fp = LDG_TRUTH_TRUE;
            expr->cursor++;
            while (ldg_char_digit_is((uint8_t)expr->text[expr->cursor]) == LDG_TRUTH_TRUE) { expr->cursor++; }
        }

        if (expr->text[expr->cursor] == 'e' || expr->text[expr->cursor] == 'E')
        {
            fp = LDG_TRUTH_TRUE;
            expr->cursor++;
            if (expr->text[expr->cursor] == '+' || expr->text[expr->cursor] == '-') { expr->cursor++; }

            if (ldg_char_digit_is((uint8_t)expr->text[expr->cursor]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }

            while (ldg_char_digit_is((uint8_t)expr->text[expr->cursor]) == LDG_TRUTH_TRUE) { expr->cursor++; }
        }
    }

    if (expr->cursor == start) { return LDG_ERR_STR_INVALID; }

    if (!fp) { return ldg_str_uint_parse(expr->text + start, base, expr->cursor - start, &out->mag); }

    errno = 0;
    out->real = strtod(expr->text + start, &end);
    out->fp = LDG_TRUTH_TRUE;
    if (errno == ERANGE || end != expr->text + expr->cursor || !isfinite(out->real)) { res = LDG_ERR_RANGE; }

    return res;
}

static uint32_t expr_node_add(libresteam_ext_repl_expr_t *expr, uint32_t op, uint32_t lhs, uint32_t rhs, uint32_t *out)
{
    uint32_t depth = 1U;

    if (expr->cunt == LIBRESTEAM_EXT_REPL_EXPR_CAP) { return LDG_ERR_FULL; }

    if (lhs != UINT32_MAX) { depth = expr->nodes[lhs].depth + 1U; }

    if (rhs != UINT32_MAX && expr->nodes[rhs].depth >= depth) { depth = expr->nodes[rhs].depth + 1U; }

    if (depth > LIBRESTEAM_EXT_REPL_EXPR_DEPTH_CAP) { return LDG_ERR_FULL; }

    *out = expr->cunt++;
    expr->nodes[*out].op = op;
    expr->nodes[*out].lhs = lhs;
    expr->nodes[*out].rhs = rhs;
    expr->nodes[*out].depth = depth;
    return LDG_ERR_AOK;
}

static uint32_t expr_op_get(const char *text, uint32_t *op, uint32_t *prec, uint32_t *len)
{
    *prec = 0;
    *len = 1U;
    if (text[0] == '|' && text[1] == '|') { *op = EXPR_OR; *prec = 1U; *len = 2U; }
    else if (text[0] == '&' && text[1] == '&') { *op = EXPR_AND; *prec = 2U; *len = 2U; }
    else if (text[0] == '=' && text[1] == '=') { *op = EXPR_EQ; *prec = 3U; *len = 2U; }
    else if (text[0] == '!' && text[1] == '=') { *op = EXPR_NE; *prec = 3U; *len = 2U; }
    else if (text[0] == '<') { *op = text[1] == '=' ? EXPR_LE : EXPR_LT; *prec = 3U; *len = text[1] == '=' ? 2U : 1U; }
    else if (text[0] == '>') { *op = text[1] == '=' ? EXPR_GE : EXPR_GT; *prec = 3U; *len = text[1] == '=' ? 2U : 1U; }
    else if (text[0] == '+') { *op = EXPR_ADD; *prec = 4U; }
    else if (text[0] == '-') { *op = EXPR_SUB; *prec = 4U; }
    else if (text[0] == '*') { *op = EXPR_MUL; *prec = 5U; }
    else if (text[0] == '/') { *op = EXPR_DIV; *prec = 5U; }
    else if (text[0] == '%') { *op = EXPR_MOD; *prec = 5U; }
    else if (text[0] == '^') { *op = EXPR_POW; *prec = 7U; }

    return LDG_ERR_AOK;
}

static uint32_t expr_parse(libresteam_ext_repl_expr_t *expr, uint32_t min_prec, uint32_t depth, uint32_t *out)
{
    uint32_t lhs = UINT32_MAX;
    uint32_t rhs = UINT32_MAX;
    uint32_t op = EXPR_LITERAL;
    uint32_t prec = 0x0;
    uint32_t len = 0x0;
    uint32_t start = 0x0;
    uint32_t var = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    char ch = 0x0;

    if (depth > LIBRESTEAM_EXT_REPL_EXPR_DEPTH_CAP) { return LDG_ERR_FULL; }

    expr_space_skip(expr);
    ch = expr->text[expr->cursor];
    if (ch == '+' || ch == '-' || ch == '!')
    {
        expr->cursor++;
        res = expr_parse(expr, EXPR_UNARY_PREC, depth + 1U, &lhs);
        if (res == LDG_ERR_AOK && ch != '+') { res = expr_node_add(expr, ch == '-' ? EXPR_NEG : EXPR_NOT, lhs, UINT32_MAX, &lhs); }
    }
    else if (ch == '(')
    {
        expr->cursor++;
        res = expr_parse(expr, 1U, depth + 1U, &lhs);
        expr_space_skip(expr);
        if (res == LDG_ERR_AOK && expr->text[expr->cursor] != ')') { return LDG_ERR_STR_INVALID; }

        if (res == LDG_ERR_AOK) { expr->cursor++; }
    }
    else if (ldg_char_digit_is((uint8_t)ch) == LDG_TRUTH_TRUE || ch == '.')
    {
        res = expr_node_add(expr, EXPR_LITERAL, UINT32_MAX, UINT32_MAX, &lhs);
        if (res == LDG_ERR_AOK) { res = expr_number_parse(expr, &expr->nodes[lhs].val); }
    }
    else
    {
        start = expr->cursor;
        while (ldg_char_alpha_is((uint8_t)expr->text[expr->cursor]) == LDG_TRUTH_TRUE) { expr->cursor++; }
        len = expr->cursor - start;
        if (!len) { return LDG_ERR_STR_INVALID; }

        if (expr_word_is(expr->text + start, len, "v") == LDG_TRUTH_TRUE) { var = 0U; }
        else if (expr_word_is(expr->text + start, len, "p") == LDG_TRUTH_TRUE) { var = 1U; }
        else if (expr_word_is(expr->text + start, len, "b") == LDG_TRUTH_TRUE) { var = 2U; }
        else if (expr_word_is(expr->text + start, len, "d") == LDG_TRUTH_TRUE) { var = 3U; }
        else if (expr_word_is(expr->text + start, len, "t") == LDG_TRUTH_TRUE) { var = 4U; }
        else if (expr_word_is(expr->text + start, len, "dt") == LDG_TRUTH_TRUE) { var = 5U; }
        else if (expr_word_is(expr->text + start, len, "n") == LDG_TRUTH_TRUE) { var = 6U; }

        if (var != UINT32_MAX)
        {
            res = expr_node_add(expr, EXPR_VAR, UINT32_MAX, UINT32_MAX, &lhs);
            if (res == LDG_ERR_AOK) { expr->nodes[lhs].val.mag = var; }
        }
        else
        {
            if (expr_word_is(expr->text + start, len, "abs") == LDG_TRUTH_TRUE) { op = EXPR_ABS; }
            else if (expr_word_is(expr->text + start, len, "sin") == LDG_TRUTH_TRUE) { op = EXPR_SIN; }
            else if (expr_word_is(expr->text + start, len, "cos") == LDG_TRUTH_TRUE) { op = EXPR_COS; }
            else if (expr_word_is(expr->text + start, len, "sqrt") == LDG_TRUTH_TRUE) { op = EXPR_SQRT; }
            else if (expr_word_is(expr->text + start, len, "floor") == LDG_TRUTH_TRUE) { op = EXPR_FLOOR; }
            else if (expr_word_is(expr->text + start, len, "min") == LDG_TRUTH_TRUE) { op = EXPR_MIN; }
            else if (expr_word_is(expr->text + start, len, "max") == LDG_TRUTH_TRUE) { op = EXPR_MAX; }
            else { return LDG_ERR_NOT_FOUND; }

            expr_space_skip(expr);
            if (expr->text[expr->cursor] != '(') { return LDG_ERR_STR_INVALID; }

            expr->cursor++;
            res = expr_parse(expr, 1U, depth + 1U, &lhs);
            if (res != LDG_ERR_AOK) { return res; }

            expr_space_skip(expr);
            if (op == EXPR_MIN || op == EXPR_MAX)
            {
                if (expr->text[expr->cursor] != ',') { return LDG_ERR_STR_INVALID; }

                expr->cursor++;
                res = expr_parse(expr, 1U, depth + 1U, &rhs);
                if (res != LDG_ERR_AOK) { return res; }

                expr_space_skip(expr);
            }

            if (expr->text[expr->cursor] != ')') { return LDG_ERR_STR_INVALID; }

            expr->cursor++;
            res = expr_node_add(expr, op, lhs, rhs, &lhs);
        }
    }

    if (res != LDG_ERR_AOK) { return res; }

    while (expr->text[expr->cursor])
    {
        expr_space_skip(expr);
        res = expr_op_get(expr->text + expr->cursor, &op, &prec, &len);
        if (res != LDG_ERR_AOK) { return res; }

        if (prec < min_prec) { break; }

        expr->cursor += len;
        res = expr_parse(expr, op == EXPR_POW ? prec : prec + 1U, depth + 1U, &rhs);
        if (res == LDG_ERR_AOK) { res = expr_node_add(expr, op, lhs, rhs, &lhs); }

        if (res != LDG_ERR_AOK) { return res; }
    }
    *out = lhs;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_expr_compile(libresteam_ext_repl_expr_t *expr, const char *text)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!expr || !text)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(expr, sizeof(*expr));
    if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(expr->text, text, sizeof(expr->text)); }

    if (res == LDG_ERR_AOK) { res = expr_parse(expr, 1U, 0U, &expr->root); }

    expr_space_skip(expr);
    if (res == LDG_ERR_AOK && expr->text[expr->cursor]) { res = LDG_ERR_STR_INVALID; }

    expr->err_pos = expr->cursor;
    if (res != LDG_ERR_AOK) { expr->cunt = 0; }

    return res;
}

static uint32_t expr_cmp(const libresteam_ext_repl_expr_val_t *lhs, const libresteam_ext_repl_expr_val_t *rhs, uint32_t *out)
{
    double a = 0x0;
    double b = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = LDG_ARITH_ORD_EQ;
    if (lhs->fp || rhs->fp)
    {
        res = libresteam_ext_repl_expr_real_get(lhs, &a);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_real_get(rhs, &b); }

        if (res != LDG_ERR_AOK) { return res; }

        *out = a < b ? LDG_ARITH_ORD_LT : (a > b ? LDG_ARITH_ORD_GT : LDG_ARITH_ORD_EQ);
    }
    else if (lhs->neg != rhs->neg) { *out = lhs->neg ? LDG_ARITH_ORD_LT : LDG_ARITH_ORD_GT; }
    else if (lhs->mag != rhs->mag) { *out = (lhs->mag < rhs->mag) != lhs->neg ? LDG_ARITH_ORD_LT : LDG_ARITH_ORD_GT; }

    return LDG_ERR_AOK;
}

static uint32_t expr_arith(uint32_t op, libresteam_ext_repl_expr_val_t lhs, libresteam_ext_repl_expr_val_t rhs, libresteam_ext_repl_expr_val_t *out)
{
    double a = 0x0;
    double b = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (lhs.fp || rhs.fp || op == EXPR_DIV || op == EXPR_POW)
    {
        res = libresteam_ext_repl_expr_real_get(&lhs, &a);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_real_get(&rhs, &b); }

        if (res != LDG_ERR_AOK) { return res; }

        out->fp = LDG_TRUTH_TRUE;
        if ((op == EXPR_DIV || op == EXPR_MOD) && b == 0.0) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (op == EXPR_ADD) { out->real = a + b; }
        else if (op == EXPR_SUB) { out->real = a - b; }
        else if (op == EXPR_MUL) { out->real = a * b; }
        else if (op == EXPR_DIV) { out->real = a / b; }
        else if (op == EXPR_MOD) { out->real = fmod(a, b); }
        else { out->real = pow(a, b); }

        return isfinite(out->real) ? LDG_ERR_AOK : LDG_ERR_RANGE;
    }

    if (op == EXPR_SUB && rhs.mag) { rhs.neg = !rhs.neg; }

    if (op == EXPR_ADD || op == EXPR_SUB) { if (lhs.neg == rhs.neg) { out->neg = lhs.neg; res = ldg_arith_uint64_add(lhs.mag, rhs.mag, &out->mag); }
        else if (lhs.mag >= rhs.mag) { out->neg = lhs.neg; out->mag = lhs.mag - rhs.mag; }
        else { out->neg = rhs.neg; out->mag = rhs.mag - lhs.mag; } }
    else if (op == EXPR_MUL) { out->neg = lhs.neg != rhs.neg; res = ldg_arith_uint64_mul(lhs.mag, rhs.mag, &out->mag); }
    else if (op == EXPR_MOD)
    {
        if (!rhs.mag) { return LDG_ERR_FUNC_ARG_INVALID; }

        out->mag = lhs.mag % rhs.mag;
        out->neg = lhs.neg;
    }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    if (!out->mag) { out->neg = LDG_TRUTH_FALSE; }

    return res;
}

static uint32_t expr_eval_node(const libresteam_ext_repl_expr_t *expr, const libresteam_ext_repl_expr_env_t *env, uint32_t idx, uint32_t depth, libresteam_ext_repl_expr_val_t *out)
{
    const libresteam_ext_repl_expr_node_t *node = 0x0;
    libresteam_ext_repl_expr_val_t lhs = LDG_STRUCT_ZERO_INIT;
    libresteam_ext_repl_expr_val_t rhs = LDG_STRUCT_ZERO_INIT;
    double real = 0x0;
    uint32_t cmp = LDG_ARITH_ORD_EQ;
    uint32_t res = LDG_ERR_AOK;

    if (idx >= expr->cunt || depth > LIBRESTEAM_EXT_REPL_EXPR_DEPTH_CAP) { return LDG_ERR_BOUNDS; }

    node = &expr->nodes[idx];
    res = ldg_mem_secure_zero(out, sizeof(*out));
    if (res != LDG_ERR_AOK) { return res; }

    if (node->op == EXPR_LITERAL) { *out = node->val; return LDG_ERR_AOK; }

    if (node->op == EXPR_VAR)
    {
        if (node->val.mag >= LIBRESTEAM_EXT_REPL_EXPR_VAR_CAP) { return LDG_ERR_BOUNDS; }

        if (node->val.mag == 3U) { return expr_arith(EXPR_SUB, env->vars[0], env->vars[1], out); }

        *out = env->vars[node->val.mag];
        return LDG_ERR_AOK;
    }

    res = expr_eval_node(expr, env, node->lhs, depth + 1U, &lhs);
    if (res != LDG_ERR_AOK) { return res; }

    if (node->op == EXPR_AND && libresteam_ext_repl_expr_truth_is(&lhs) == LDG_TRUTH_FALSE) { return LDG_ERR_AOK; }

    if (node->op == EXPR_OR && libresteam_ext_repl_expr_truth_is(&lhs) == LDG_TRUTH_TRUE) { out->mag = 1U; return LDG_ERR_AOK; }

    if (node->rhs != UINT32_MAX)
    {
        res = expr_eval_node(expr, env, node->rhs, depth + 1U, &rhs);
        if (res != LDG_ERR_AOK) { return res; }
    }

    if (node->op == EXPR_AND || node->op == EXPR_OR) { out->mag = libresteam_ext_repl_expr_truth_is(&rhs) == LDG_TRUTH_TRUE ? 1U : 0U; return LDG_ERR_AOK; }

    if (node->op == EXPR_NOT) { out->mag = libresteam_ext_repl_expr_truth_is(&lhs) == LDG_TRUTH_FALSE ? 1U : 0U; return LDG_ERR_AOK; }

    if (node->op == EXPR_NEG || node->op == EXPR_ABS)
    {
        *out = lhs;
        if (lhs.fp) { out->real = node->op == EXPR_NEG ? -lhs.real : fabs(lhs.real); }
        else { out->neg = lhs.mag && node->op == EXPR_NEG ? !lhs.neg : LDG_TRUTH_FALSE; }

        return LDG_ERR_AOK;
    }

    if (node->op >= EXPR_SIN && node->op <= EXPR_FLOOR)
    {
        res = libresteam_ext_repl_expr_real_get(&lhs, &real);
        if (res != LDG_ERR_AOK) { return res; }

        out->fp = LDG_TRUTH_TRUE;
        if (node->op == EXPR_SIN) { out->real = sin(real); }
        else if (node->op == EXPR_COS) { out->real = cos(real); }
        else if (node->op == EXPR_SQRT) { out->real = sqrt(real); }
        else { out->real = floor(real); }

        return isfinite(out->real) ? LDG_ERR_AOK : LDG_ERR_RANGE;
    }

    if ((node->op >= EXPR_LT && node->op <= EXPR_NE) || node->op == EXPR_MIN || node->op == EXPR_MAX)
    {
        res = expr_cmp(&lhs, &rhs, &cmp);
        if (res != LDG_ERR_AOK) { return res; }

        if (node->op == EXPR_MIN) { *out = cmp == LDG_ARITH_ORD_GT ? rhs : lhs; }
        else if (node->op == EXPR_MAX) { *out = cmp == LDG_ARITH_ORD_LT ? rhs : lhs; }
        else if (node->op == EXPR_LT) { out->mag = cmp == LDG_ARITH_ORD_LT; }
        else if (node->op == EXPR_LE) { out->mag = cmp != LDG_ARITH_ORD_GT; }
        else if (node->op == EXPR_GT) { out->mag = cmp == LDG_ARITH_ORD_GT; }
        else if (node->op == EXPR_GE) { out->mag = cmp != LDG_ARITH_ORD_LT; }
        else if (node->op == EXPR_EQ) { out->mag = cmp == LDG_ARITH_ORD_EQ; }
        else { out->mag = cmp != LDG_ARITH_ORD_EQ; }

        return LDG_ERR_AOK;
    }

    return expr_arith(node->op, lhs, rhs, out);
}

uint32_t libresteam_ext_repl_expr_eval(const libresteam_ext_repl_expr_t *expr, const libresteam_ext_repl_expr_env_t *env, libresteam_ext_repl_expr_val_t *out)
{
    if (LDG_UNLIKELY(!expr || !env || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!expr->cunt || expr->cunt > LIBRESTEAM_EXT_REPL_EXPR_CAP) { return LDG_ERR_NOT_INIT; }

    return expr_eval_node(expr, env, expr->root, 0U, out);
}
