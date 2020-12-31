#include "inc/chip8_memory.h"

#include <stdlib.h>
#include <stdio.h>

#define SPRITE_HEIGHT 5
#define SPRITE_MEMORY_START 0

static const uint8_t digit_sprites[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,   /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70,   /* 1 */
    0xf0, 0x10, 0xf0, 0x80, 0xf0,   /* 2 */
    0xf0, 0x10, 0xf0, 0x10, 0xf0,   /* 3 */
    0x90, 0x90, 0xf0, 0x10, 0x10,   /* 4 */
    0xf0, 0x80, 0xf0, 0x10, 0xf0,   /* 5 */
    0xf0, 0x80, 0xf0, 0x90, 0xf0,   /* 6 */
    0xf0, 0x10, 0x20, 0x40, 0x40,   /* 7 */
    0xf0, 0x90, 0xf0, 0x90, 0xf0,   /* 8 */
    0xf0, 0x90, 0xf0, 0x10, 0xf0,   /* 9 */
    0xf0, 0x90, 0xf0, 0x90, 0x90,   /* A */
    0xe0, 0x90, 0xe0, 0x90, 0xe0,   /* B */
    0xf0, 0x80, 0x80, 0x80, 0xf0,   /* C */
    0xe0, 0x90, 0x90, 0x90, 0xe0,   /* D */
    0xf0, 0x80, 0xf0, 0x80, 0xf0,   /* E */
    0xf0, 0x80, 0xf0, 0x80, 0x80    /* F */
};

struct chip8_memory *
chip8_memory_init(void)
{
    struct chip8_memory *memory = calloc(1, sizeof(*memory));
    if (memory == NULL) {
        puts("Error allocating memory!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < sizeof(digit_sprites) / sizeof(digit_sprites[0]); i++) {
        memory->memory[SPRITE_MEMORY_START + i] = digit_sprites[i];
    }
    return memory;
}

uint16_t
chip8_memory_fetch(const struct chip8_memory *memory, uint16_t pc)
{
    return ((uint16_t)memory->memory[pc]) << 8u | ((uint16_t)memory->memory[pc + 1]);
}

void
chip8_memory_load_program(struct chip8_memory *memory, const uint8_t *program, uint32_t size)
{
    for (int i = 0; i < size; i++) {
        memory->memory[PROGRAM_START_ADDR + i] = program[i];
    }
}

uint16_t
chip8_memory_get_digit_sprite(uint16_t digit)
{
    return digit * SPRITE_HEIGHT + SPRITE_MEMORY_START;
}
