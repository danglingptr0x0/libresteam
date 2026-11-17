#include <stdint.h>

#if defined(__x86_64__) || defined(_M_X64)
#include <hde64.h>
#endif

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/ext/repl/xref.h>

#define XREF_DECODER_BUFF_CAP 64U
#define XREF_SIGN_REL32 0x80000000U
#define XREF_SIGN_REL8 0x80U
#define XREF_LEA 0x8dU
#define XREF_CALL 0xe8U
#define XREF_JMP 0xe9U
#define XREF_JMP_SHORT 0xebU
#define XREF_MOV_REG_IMM_MIN 0xb8U
#define XREF_MOV_REG_IMM_MAX 0xbfU
#define XREF_MOV_OFFSET_MIN 0xa0U
#define XREF_MOV_OFFSET_MAX 0xa3U
#define XREF_RIP_MODRM_RM 5U

uint32_t libresteam_ext_repl_xref_decode(const uint8_t *bytes, uint32_t len, uint64_t addr, libresteam_ext_repl_xref_t *out)
{
    uint32_t res = LDG_ERR_AOK;
#if defined(__x86_64__) || defined(_M_X64)
    hde64s insn = LDG_STRUCT_ZERO_INIT;
    uint8_t buff[XREF_DECODER_BUFF_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t next = 0x0;
    uint64_t delta = 0x0;
    uint32_t disp = 0x0;
    uint32_t decoded = 0x0;
    uint8_t neg = LDG_TRUTH_FALSE;
#endif

    if (LDG_UNLIKELY(!bytes || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(out, sizeof(*out));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (LDG_UNLIKELY(!len || addr > UINT64_MAX - len)) { return LDG_ERR_RANGE; }

#if defined(__x86_64__) || defined(_M_X64)
    if (len > LIBRESTEAM_EXT_REPL_XREF_INSN_CAP) { len = LIBRESTEAM_EXT_REPL_XREF_INSN_CAP; }

    /* HDE can read beyond an invalid instruction before reporting its length error. */
    res = ldg_mem_secure_cpy(buff, bytes, len);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    decoded = (uint32_t)hde64_disasm(buff, &insn);
    if (!decoded || decoded > len || (insn.flags & F_ERROR) || insn.p_67 || insn.p_seg == PREFIX_SEGMENT_FS || insn.p_seg == PREFIX_SEGMENT_GS) { return LDG_ERR_NOT_FOUND; }

    next = addr + decoded;
    if ((insn.flags & (F_MODRM | F_DISP32)) == (F_MODRM | F_DISP32) && insn.modrm_mod == 0 && insn.modrm_rm == XREF_RIP_MODRM_RM)
    {
        disp = insn.disp.disp32;
        neg = (disp & XREF_SIGN_REL32) != 0 ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
        delta = neg ? (uint32_t)(~disp + 1U) : disp;
        out->kind = insn.opcode == XREF_LEA ? "rip-address" : "rip-memory";
    }
    else if (insn.flags & F_RELATIVE)
    {
        if (insn.p_66) { return LDG_ERR_NOT_FOUND; }

        if (insn.flags & F_IMM32)
        {
            disp = insn.imm.imm32;
            neg = (disp & XREF_SIGN_REL32) != 0 ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
            delta = neg ? (uint32_t)(~disp + 1U) : disp;
        }
        else if (insn.flags & F_IMM8)
        {
            disp = insn.imm.imm8;
            neg = (disp & XREF_SIGN_REL8) != 0 ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
            delta = neg ? (uint8_t)(~disp + 1U) : disp;
        }
        else { return LDG_ERR_NOT_FOUND; }

        out->kind = insn.opcode == XREF_CALL ? "call" : (insn.opcode == XREF_JMP || insn.opcode == XREF_JMP_SHORT ? "jump" : "branch");
    }
    else if ((insn.flags & F_IMM64) && insn.opcode >= XREF_MOV_REG_IMM_MIN && insn.opcode <= XREF_MOV_REG_IMM_MAX)
    {
        out->tgt = insn.imm.imm64;
        out->kind = "immediate";
        out->len = decoded;
        return LDG_ERR_AOK;
    }
    else if ((insn.flags & F_IMM64) && insn.opcode >= XREF_MOV_OFFSET_MIN && insn.opcode <= XREF_MOV_OFFSET_MAX)
    {
        out->tgt = insn.imm.imm64;
        out->kind = "absolute-memory";
        out->len = decoded;
        return LDG_ERR_AOK;
    }
    else { return LDG_ERR_NOT_FOUND; }

    if (LDG_UNLIKELY((neg && delta > next) || (!neg && delta > UINT64_MAX - next))) { return LDG_ERR_OVERFLOW; }

    out->tgt = neg ? next - delta : next + delta;
    out->len = decoded;
    return LDG_ERR_AOK;
#else
    return LDG_ERR_UNSUPPORTED;
#endif
}
