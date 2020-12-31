#ifndef CHIP8_CHIP8_REGISTERS_H
#define CHIP8_CHIP8_REGISTERS_H

#include <stdint.h>

#define V_REGISTERS 16

struct chip8_registers {
    uint8_t V[V_REGISTERS];     /* V registers */
    uint16_t I;                 /* Index register */
    uint16_t PC;                /* Program counter */
    uint8_t DT;                 /* Delay timer */
    uint8_t ST;                 /* Sound timer */
    uint8_t SP;                 /* Stack pointer */
};

void chip8_registers_set_Vx(struct chip8_registers *registers, uint8_t x, uint8_t value);
void chip8_registers_set_I(struct chip8_registers *registers, uint16_t value);
void chip8_registers_set_DT(struct chip8_registers *registers, uint8_t value);
void chip8_registers_set_ST(struct chip8_registers *registers, uint8_t value);
void chip8_registers_set_SP(struct chip8_registers *registers, uint8_t value);
void chip8_registers_set_PC(struct chip8_registers *registers, uint16_t value);

uint8_t chip8_registers_get_Vx(struct chip8_registers *registers, uint8_t);
uint16_t chip8_registers_get_I(struct chip8_registers *registers);
uint8_t chip8_registers_get_DT(struct chip8_registers *registers);
uint8_t chip8_registers_get_ST(struct chip8_registers *registers);
uint8_t chip8_registers_get_SP(struct chip8_registers *registers);
uint16_t chip8_registers_get_PC(struct chip8_registers *registers);

void chip8_registers_increment_PC(struct chip8_registers *registers);

void chip8_registers_decrement_PC(struct chip8_registers *registers);
void chip8_registers_decrement_DT(struct chip8_registers *registers);
void chip8_registers_decrement_ST(struct chip8_registers *registers);

#endif //CHpIP8_CHIP8_REGISTERS_H
