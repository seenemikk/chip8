#include "inc/chip8_display.h"

static uint64_t
chip8_display_get_mask(uint8_t x, uint8_t sprite_row)
{
    return (uint64_t)sprite_row >> (uint64_t)(x - MASK_LEFT_BIT) |
           (uint64_t)sprite_row << (uint64_t)(DISPLAY_WIDTH - (x - MASK_LEFT_BIT));
}

static bool
chip8_display_check_collision(uint64_t display_row, uint64_t mask)
{
    return (display_row & mask) != mask;
}

void
chip8_display_clear(struct chip8_display *display)
{
    uint64_t *ptr = display->display;
    while (ptr < &display->display[DISPLAY_HEIGHT]) {
        *ptr++ = 0;
    }
}

bool
chip8_display_draw(struct chip8_display *display, uint8_t x, uint8_t y, uint8_t n, const uint8_t *sprite_ptr)
{
    bool collision = false;
    for (int i = 0; i < n; i++) {
        int row = (y + i) % DISPLAY_HEIGHT;
        uint64_t mask = chip8_display_get_mask(x, sprite_ptr[i]);
        display->display[row] ^= mask;
        collision |= chip8_display_check_collision(display->display[row], mask);
    }
    return collision;
}
