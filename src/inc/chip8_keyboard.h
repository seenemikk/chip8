#ifndef CHIP8_CHIP8_KEYBOARD_H
#define CHIP8_CHIP8_KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

struct chip8_keyboard {
    uint16_t *keyboard;
};

struct chip8_keyboard *chip8_keyboard_init(uint16_t *keyboard);
bool chip8_keyboard_key_is_pressed(const struct chip8_keyboard *keyboard, uint8_t key);
uint8_t chip8_keyboard_get_pressed(const struct chip8_keyboard *keyboard);

#endif //CHIP8_CHIP8_KEYBOARD_H
