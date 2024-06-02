/**
 * Hacker Disassembler Engine 64
 * Copyright (c) 2008-2009, Vyacheslav Patkov.
 * All rights reserved.
 *
 * hde64.h: C/C++ header file
 *
 */

#ifndef _HDE64_H_
#define _HDE64_H_

#include <stdint.h> // Include stdint.h for fixed-width integer types

#ifdef _MSC_VER
#include "pstdint.h"
#else
#include <stdint.h>
#endif

// ... (other #defines for flags)

#pragma pack(push, 1)

typedef struct {
    uint8_t opcode;
    uint8_t opcode2;
    uint8_t modrm;
    uint8_t modrm_mod;
    uint8_t modrm_reg;
    uint8_t modrm_rm;
    uint8_t sib;
    uint8_t sib_scale : 2;
    uint8_t sib_index : 3;
    uint8_t sib_base : 3;
    uint8_t rex;
    uint8_t rex_w : 1;
    uint8_t rex_r : 1;
    uint8_t rex_x : 1;
    uint8_t rex_b : 1;
    uint8_t p_rep : 2;
    uint8_t p_lock : 1;
    uint8_t p_seg : 3;
    uint8_t p_66 : 1;
    uint8_t p_67 : 1;
    uint8_t len : 4;
    int8_t imm8;
    int16_t imm16;
    int32_t imm32;
    int64_t imm64;
    int8_t disp8;
    int16_t disp16;
    int32_t disp32;
    uint32_t flags;
} hde64_instruction_t;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Disassembles a single x86-64 instruction.
 *
 * @param code Pointer to the start of the instruction code.
 * @param hs Pointer to the hde64_instruction_t structure to store the
 *            disassembled instruction.
 *
 * @return The length of the disassembled instruction in bytes.
 */
unsigned int hde64_disasm(const void* code, hde64_instruction_t* hs);

#ifdef __cplusplus
}
#endif

#endif // _HDE64_H_
