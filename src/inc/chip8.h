#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

#define CYCLES_PER_SECOND 540
#define FRAMES_PER_SECOND 60

#define V_REGISTERS 16

struct chip8_memory;
struct chip8_registers;
struct chip8_stack;
struct chip8_keyboard;
struct chip8_display;
struct SDL_Renderer;

struct chip8 {
    struct chip8_memory *memory;
    struct chip8_registers *registers;
    struct chip8_stack *stack;
    struct chip8_keyboard *keyboard;
    struct chip8_display *display;
    struct SDL_Renderer *renderer;
};

struct chip8 *chip8_init(struct SDL_Renderer *renderer, uint16_t *keyboard);
void chip8_load_program(struct chip8 *chip8, const uint8_t *program, uint32_t size);
bool chip8_step(struct chip8 *chip8);

#endif //CHIP8_CHIP8_H
