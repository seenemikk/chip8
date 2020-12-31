#ifndef CHIP8_CHIP8_DISPLAY_H
#define CHIP8_CHIP8_DISPLAY_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SPRITE_WIDTH 8
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define MASK_LEFT_BIT (DISPLAY_WIDTH - SPRITE_WIDTH)

struct chip8_display {
    uint64_t display[DISPLAY_HEIGHT];
};

void chip8_display_clear(struct chip8_display *display);

/**
 * @param display
 * @param x - upper left x coordinate
 * @param y - upper left y coordinate
 * @param n - sprite height
 * @param sprite_ptr - pointer to sprite being drawn
 * @return true if collision occurred
 */
bool chip8_display_draw(struct chip8_display *display, uint8_t x, uint8_t y, uint8_t n, const uint8_t *sprite_ptr);

#endif //CHIP8_CHIP8_DISPLAY_H
