#include "inc/chip8_keyboard.h"

#include <stdlib.h>
#include <stdio.h>

struct chip8_keyboard *
chip8_keyboard_init(uint16_t *keyboard)
{
    struct chip8_keyboard *chip8_keyboard = malloc(sizeof(*chip8_keyboard));
    if (chip8_keyboard == NULL) {
        puts("Error allocating memory!");
        exit(EXIT_FAILURE);
    }
    chip8_keyboard->keyboard = keyboard;
    return chip8_keyboard;
}

bool
chip8_keyboard_key_is_pressed(const struct chip8_keyboard *keyboard, uint8_t key)
{
    return (*keyboard->keyboard >> key) & 1u;
}

uint8_t
chip8_keyboard_get_pressed(const struct chip8_keyboard *keyboard)
{
    for (uint8_t i = 0; i <= 0x0f; i++) {
        if ((*keyboard->keyboard >> i) & 1u) {
            return i;
        }
    }
    return 0;
}
