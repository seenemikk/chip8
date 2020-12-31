#include "inc/chip8_registers.h"

void
chip8_registers_set_Vx(struct chip8_registers *registers, uint8_t x, uint8_t value)
{
    registers->V[x] = value;
}

void
chip8_registers_set_I(struct chip8_registers *registers, uint16_t value)
{
    registers->I = value;
}

void
chip8_registers_set_DT(struct chip8_registers *registers, uint8_t value)
{
    registers->DT = value;
}

void
chip8_registers_set_ST(struct chip8_registers *registers, uint8_t value)
{
    registers->ST = value;
}

void
chip8_registers_set_SP(struct chip8_registers *registers, uint8_t value)
{
    registers->SP = value;
}

uint8_t
chip8_registers_get_Vx(struct chip8_registers *registers, uint8_t x)
{
    return registers->V[x];
}

uint16_t
chip8_registers_get_I(struct chip8_registers *registers)
{
    return registers->I;
}

uint8_t
chip8_registers_get_DT(struct chip8_registers *registers)
{
    return registers->DT;
}

uint8_t
chip8_registers_get_ST(struct chip8_registers *registers)
{
    return registers->ST;
}

uint8_t
chip8_registers_get_SP(struct chip8_registers *registers)
{
    return registers->SP;
}

uint16_t
chip8_registers_get_PC(struct chip8_registers *registers)
{
    return registers->PC;
}

void
chip8_registers_set_PC(struct chip8_registers *registers, uint16_t value)
{
    registers->PC = value;
}

void
chip8_registers_increment_PC(struct chip8_registers *registers)
{
    registers->PC += 0x02;
}

void
chip8_registers_decrement_PC(struct chip8_registers *registers)
{
    registers->PC -= 0x02;
}

void
chip8_registers_decrement_DT(struct chip8_registers *registers)
{
    if (registers->DT) {
        registers->DT--;
    }
}

void
chip8_registers_decrement_ST(struct chip8_registers *registers)
{
    if (registers->ST) {
        registers->ST--;
    }
}
