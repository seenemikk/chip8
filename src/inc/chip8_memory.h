#ifndef CHIP8_CHIP8_MEMORY_H
#define CHIP8_CHIP8_MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 4096
#define PROGRAM_START_ADDR 0x200

#define NIBBLE 4
#define BYTE 8

struct chip8_memory {
    uint8_t memory[MEMORY_SIZE];
};

struct chip8_memory *chip8_memory_init(void);
uint16_t chip8_memory_fetch(const struct chip8_memory *memory, uint16_t pc);
void chip8_memory_load_program(struct chip8_memory *memory, const uint8_t *program, uint32_t size);
uint16_t chip8_memory_get_digit_sprite(uint16_t digit);

#endif //CHIP8_CHIP8_MEMORY_H
