#include <stdio.h>
#include <SDL2/SDL.h>

#include "inc/chip8.h"

#define ROM_SIZE 4096

static void init_sdl(void);
static SDL_Window *init_window(void);
static SDL_Renderer *init_renderer(struct SDL_Window *window);

static uint16_t read_rom(char *file, uint8_t *buffer);
static uint16_t *make_keyboard(const uint8_t *keyboard_state);
static void update_keyboard(uint16_t *keyboard, const uint8_t *keyboard_state);

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        puts("Usage: chip8 /path/to/rom");
        exit(EXIT_FAILURE);
    }

    init_sdl();
    SDL_Window *window = init_window();
    SDL_Renderer *renderer = init_renderer(window);
    SDL_Event event;

    uint8_t rom[ROM_SIZE];
    int size = read_rom(argv[1], rom);

    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
    uint16_t *keyboard = make_keyboard(keyboard_state);

    struct chip8 *chip8 = chip8_init(renderer, keyboard);
    chip8_load_program(chip8, rom, size);

    bool run = true;
    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT || keyboard_state[SDL_SCANCODE_ESCAPE]) break;
        if (keyboard_state[SDL_SCANCODE_SPACE]) run = !run;

        if (run) {
            update_keyboard(keyboard, keyboard_state);
            chip8_step(chip8);
        }

        SDL_Delay(1000 / FRAMES_PER_SECOND);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

static void
init_sdl(void)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        puts(SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

static SDL_Window *
init_window(void)
{
    SDL_Window *window = SDL_CreateWindow("Chip8 Emulator",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return window;
}

static SDL_Renderer *
init_renderer(struct SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

static void
update_keyboard(uint16_t *keyboard, const uint8_t *keyboard_state)
{
    *keyboard = 0;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_V] << 0xfu;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_F] << 0xeu;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_R] << 0xdu;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_4] << 0xcu;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_C] << 0xbu;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_Z] << 0xau;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_D] << 0x9u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_S] << 0x8u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_A] << 0x7u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_E] << 0x6u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_W] << 0x5u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_Q] << 0x4u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_3] << 0x3u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_2] << 0x2u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_1] << 0x1u;
    *keyboard |= (uint16_t)keyboard_state[SDL_SCANCODE_X] << 0x0u;
}

static uint16_t *
make_keyboard(const uint8_t *keyboard_state)
{
    uint16_t *keyboard = calloc(1, sizeof(uint16_t));
    update_keyboard(keyboard, keyboard_state);
    return keyboard;
}

static uint16_t
read_rom(char *file, uint8_t *buffer)
{
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        puts("File does not exist!");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    uint64_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(buffer, size, 1, fp);

    return size;
}

