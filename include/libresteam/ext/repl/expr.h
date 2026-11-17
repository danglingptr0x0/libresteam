#ifndef LIBRESTEAM_EXT_REPL_EXPR_H
#define LIBRESTEAM_EXT_REPL_EXPR_H

#include <stdint.h>

#define LIBRESTEAM_EXT_REPL_EXPR_CAP 96U
#define LIBRESTEAM_EXT_REPL_EXPR_TEXT_CAP 256U
#define LIBRESTEAM_EXT_REPL_EXPR_DEPTH_CAP 16U
#define LIBRESTEAM_EXT_REPL_EXPR_VAR_CAP 7U

typedef struct libresteam_ext_repl_expr_val
{
    uint64_t mag;
    double real;
    uint8_t fp;
    uint8_t neg;
    uint8_t pudding[6];
} libresteam_ext_repl_expr_val_t;

typedef struct libresteam_ext_repl_expr_node
{
    libresteam_ext_repl_expr_val_t val;
    uint32_t op;
    uint32_t lhs;
    uint32_t rhs;
    uint32_t depth;
} libresteam_ext_repl_expr_node_t;

typedef struct libresteam_ext_repl_expr
{
    libresteam_ext_repl_expr_node_t nodes[LIBRESTEAM_EXT_REPL_EXPR_CAP];
    char text[LIBRESTEAM_EXT_REPL_EXPR_TEXT_CAP];
    uint32_t cunt;
    uint32_t root;
    uint32_t cursor;
    uint32_t err_pos;
} libresteam_ext_repl_expr_t;

typedef struct libresteam_ext_repl_expr_env
{
    libresteam_ext_repl_expr_val_t vars[LIBRESTEAM_EXT_REPL_EXPR_VAR_CAP];
} libresteam_ext_repl_expr_env_t;

uint32_t libresteam_ext_repl_expr_compile(libresteam_ext_repl_expr_t *expr, const char *text);
uint32_t libresteam_ext_repl_expr_eval(const libresteam_ext_repl_expr_t *expr, const libresteam_ext_repl_expr_env_t *env, libresteam_ext_repl_expr_val_t *out);
uint32_t libresteam_ext_repl_expr_real_get(const libresteam_ext_repl_expr_val_t *val, double *out);
uint8_t libresteam_ext_repl_expr_truth_is(const libresteam_ext_repl_expr_val_t *val);
uint32_t libresteam_ext_repl_expr_decode(uint64_t bits, uint32_t bytes, uint8_t fp, uint8_t signed_val, libresteam_ext_repl_expr_val_t *out);

#endif
