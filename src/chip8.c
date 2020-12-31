#include "inc/chip8.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "inc/chip8_display.h"
#include "inc/chip8_stack.h"
#include "inc/chip8_memory.h"
#include "inc/chip8_registers.h"
#include "inc/chip8_keyboard.h"

static void chip8_decode(struct chip8 *chip8, uint16_t instruction);
static void chip8_draw_screen(const struct chip8 *chip8);

static void chip8_instruction_0XXX(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_00E0(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_00EE(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_1nnn(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_2nnn(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_3xkk(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_4xkk(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_5xy0(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_6xkk(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_7xkk(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8XXX(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy0(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy1(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy2(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy3(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy4(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy5(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy6(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xy7(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_8xyE(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_9xy0(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Annn(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Bnnn(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Cxkk(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Dxyn(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_ExXX(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Ex9E(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_ExA1(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_FXXX(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx07(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx0A(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx15(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx18(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx1E(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx29(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx33(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx55(struct chip8 *chip8, uint16_t instruction);
static void chip8_instruction_Fx65(struct chip8 *chip8, uint16_t instruction);

struct chip8 *
chip8_init(struct SDL_Renderer *renderer, uint16_t *keyboard)
{
    struct chip8 *chip = calloc(1, sizeof(*chip));
    if (chip == NULL) {
        puts("Error allocating memory!");
        exit(EXIT_FAILURE);
    }
    chip->memory = chip8_memory_init();
    chip->registers = calloc(1, sizeof(*chip->registers));
    chip->stack = calloc(1, sizeof(*chip->stack));
    chip->keyboard = chip8_keyboard_init(keyboard);
    chip->display = calloc(1, sizeof(*chip->display));
    chip->renderer = renderer;

    chip8_registers_set_PC(chip->registers, PROGRAM_START_ADDR);

    srand(time(NULL));
    return chip;
}

void
chip8_load_program(struct chip8 *chip8, const uint8_t *program, uint32_t size)
{
    chip8_memory_load_program(chip8->memory, program, size);
}

/***
 * Return true if sound should play
 */
bool
chip8_step(struct chip8 *chip8)
{
    for (uint8_t cycle = 0; cycle < CYCLES_PER_SECOND / FRAMES_PER_SECOND; cycle++) {
        chip8_decode(chip8, chip8_memory_fetch(chip8->memory, chip8_registers_get_PC(chip8->registers)));
        chip8_registers_increment_PC(chip8->registers);
    }
    chip8_registers_decrement_DT(chip8->registers);
    chip8_registers_decrement_ST(chip8->registers);
    chip8_draw_screen(chip8);
    return chip8_registers_get_ST(chip8->registers);
}

static void
chip8_decode(struct chip8 *chip8, uint16_t instruction)
{
    void (*instructions[])(struct chip8 *chip8, uint16_t instruction) = {
            chip8_instruction_0XXX, chip8_instruction_1nnn, chip8_instruction_2nnn, chip8_instruction_3xkk,
            chip8_instruction_4xkk, chip8_instruction_5xy0, chip8_instruction_6xkk, chip8_instruction_7xkk,
            chip8_instruction_8XXX, chip8_instruction_9xy0, chip8_instruction_Annn, chip8_instruction_Bnnn,
            chip8_instruction_Cxkk, chip8_instruction_Dxyn, chip8_instruction_ExXX, chip8_instruction_FXXX
    };

    uint8_t opcode = instruction >> (uint8_t)(3 * NIBBLE);
//    printf("0x%04x 0x%04x 0x%02x\n", chip8_registers_get_PC(chip8->registers), instruction, opcode);

    instructions[opcode](chip8, instruction);
}

static void
chip8_draw_screen(const struct chip8 *chip8)
{
    SDL_Rect pixel = {0, 0, SCREEN_WIDTH / DISPLAY_WIDTH, SCREEN_HEIGHT / DISPLAY_HEIGHT};

    SDL_SetRenderDrawColor(chip8->renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(chip8->renderer);

    for (uint8_t row = 0; row < DISPLAY_HEIGHT; row++) {
        for (uint8_t col = 0; col < DISPLAY_WIDTH; col++) {
            uint8_t bit = (chip8->display->display[row] >> (63 - col)) & 1u;
            uint8_t color = bit ? 0xff : 0x20;
            pixel.x = pixel.w * col;
            pixel.y = pixel.h * row;

            SDL_SetRenderDrawColor(chip8->renderer, color, color, color, 0xff);
            SDL_RenderFillRect(chip8->renderer, &pixel);
        }
    }

    SDL_RenderPresent(chip8->renderer);
}

static void
chip8_instruction_0XXX(struct chip8 *chip8, uint16_t instruction)
{
    if (instruction == 0x00e0) {
        chip8_instruction_00E0(chip8, instruction);
    } else {
        chip8_instruction_00EE(chip8, instruction);
    }
}

static void
chip8_instruction_00E0(struct chip8 *chip8, uint16_t instruction)
{
    chip8_display_clear(chip8->display);
}

static void
chip8_instruction_00EE(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t new_pc = chip8_stack_pop(chip8->stack, &chip8->registers->SP);
    chip8_registers_set_PC(chip8->registers, new_pc);
}

static void
chip8_instruction_1nnn(struct chip8 *chip8, uint16_t instruction)
{
    chip8_registers_set_PC(chip8->registers, instruction & 0x0fffu);
    chip8_registers_decrement_PC(chip8->registers);
}

static void
chip8_instruction_2nnn(struct chip8 *chip8, uint16_t instruction)
{
    chip8_stack_push(chip8->stack, &chip8->registers->SP, chip8->registers->PC);
    chip8_registers_set_PC(chip8->registers, instruction & 0x0fffu);
    chip8_registers_decrement_PC(chip8->registers);
}

static void
chip8_instruction_3xkk(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    if (chip8_registers_get_Vx(chip8->registers, x) == (instruction & 0x00ffu)) {
        chip8_registers_increment_PC(chip8->registers);
    }
}

static void
chip8_instruction_4xkk(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    if (chip8_registers_get_Vx(chip8->registers, x) != (instruction & 0x00ffu)) {
        chip8_registers_increment_PC(chip8->registers);
    }
}

static void
chip8_instruction_5xy0(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    if (chip8_registers_get_Vx(chip8->registers, x) == chip8_registers_get_Vx(chip8->registers, y)) {
        chip8_registers_increment_PC(chip8->registers);
    }
}

static void
chip8_instruction_6xkk(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t value = instruction & 0x00ffu;
    chip8_registers_set_Vx(chip8->registers, x, value);
}

static void
chip8_instruction_7xkk(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t value = chip8_registers_get_Vx(chip8->registers, x) + (instruction & 0x00ffu);
    chip8_registers_set_Vx(chip8->registers, x, value);
}

static void
chip8_instruction_8XXX(struct chip8 *chip8, uint16_t instruction)
{
    void (*instructions[])(struct chip8 *chip8, uint16_t instruction) = {
            chip8_instruction_8xy0, chip8_instruction_8xy1, chip8_instruction_8xy2, chip8_instruction_8xy3,
            chip8_instruction_8xy4, chip8_instruction_8xy5, chip8_instruction_8xy6, chip8_instruction_8xy7,
            [14] = chip8_instruction_8xyE
    };

    instructions[instruction & 0x000fu](chip8, instruction);
}

/***
 * Set Vx = Vy.
 */
static void
chip8_instruction_8xy0(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    chip8_registers_set_Vx(chip8->registers, x, chip8_registers_get_Vx(chip8->registers, y));
}

/***
 * Set Vx = Vx OR Vy.
 */
static void
chip8_instruction_8xy1(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint16_t value = chip8_registers_get_Vx(chip8->registers, x) |
                     chip8_registers_get_Vx(chip8->registers, y);
    chip8_registers_set_Vx(chip8->registers, x, value);
}

/***
 * Set Vx = Vx AND Vy.
 */
static void
chip8_instruction_8xy2(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint16_t value = chip8_registers_get_Vx(chip8->registers, x) &
                     chip8_registers_get_Vx(chip8->registers, y);
    chip8_registers_set_Vx(chip8->registers, x, value);
}

/***
 * Set Vx = Vx XOR Vy.
 */
static void
chip8_instruction_8xy3(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint16_t value = chip8_registers_get_Vx(chip8->registers, x) ^
                     chip8_registers_get_Vx(chip8->registers, y);
    chip8_registers_set_Vx(chip8->registers, x, value);
}


/***
 * Set Vx = Vx + Vy, set VF = carry.
 * The values of Vx and Vy are added together.
 * If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
 * Only the lowest 8 bits of the result are kept, and stored in Vx.
 */
static void
chip8_instruction_8xy4(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint16_t value = chip8_registers_get_Vx(chip8->registers, x) +
                     chip8_registers_get_Vx(chip8->registers, y);
    chip8_registers_set_Vx(chip8->registers, 0x0f, (uint8_t)value != value);
    chip8_registers_set_Vx(chip8->registers, x, value);
}

/***
 * Set Vx = Vx - Vy, set VF = NOT borrow.
 * If Vx >= Vy, then VF is set to 1, otherwise 0.
 * Then Vy is subtracted from Vx, and the results stored in Vx.
 */
static void
chip8_instruction_8xy5(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    uint8_t Vy = chip8_registers_get_Vx(chip8->registers, y);

    chip8_registers_set_Vx(chip8->registers, 0x0f, Vx >= Vy);
    chip8_registers_set_Vx(chip8->registers, x, Vx - Vy);
}

/***
 * Vx>>=1
 * Stores the least significant bit of VX in VF and then shifts VX to the right by 1
 */
static void
chip8_instruction_8xy6(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);

    chip8_registers_set_Vx(chip8->registers, 0x0f, Vx & 1u);
    chip8_registers_set_Vx(chip8->registers, x, Vx >> 1u);
}

/***
 * Vx=Vy-Vx
 * Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
 */
static void
chip8_instruction_8xy7(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    uint8_t Vy = chip8_registers_get_Vx(chip8->registers, y);

    chip8_registers_set_Vx(chip8->registers, 0x0f, Vy >= Vx);
    chip8_registers_set_Vx(chip8->registers, x, Vy - Vx);
}

/***
 * Vx<<=1
 * Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
 */
static void
chip8_instruction_8xyE(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);

    chip8_registers_set_Vx(chip8->registers, 0x0f, Vx & 0x80u);
    chip8_registers_set_Vx(chip8->registers, x, Vx << 1u);
}

/***
 * Skip next instruction if(Vx!=Vy)
 * Skips the next instruction if VX doesn't equal VY.
 */
static void
chip8_instruction_9xy0(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    uint8_t Vy = chip8_registers_get_Vx(chip8->registers, y);

    if (Vx != Vy) {
        chip8_registers_increment_PC(chip8->registers);
    }
}

/***
 * I = nnn
 */
static void
chip8_instruction_Annn(struct chip8 *chip8, uint16_t instruction)
{
    chip8_registers_set_I(chip8->registers, instruction & 0x0fffu);
}

/***
 * PC=V0+NNN
 */
static void
chip8_instruction_Bnnn(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t V0 = chip8_registers_get_Vx(chip8->registers, 0x00);
    chip8_registers_set_PC(chip8->registers, V0 + (instruction & 0x0fffu));
    chip8_registers_decrement_PC(chip8->registers);
}

/***
 * Vx=rand()&NN
 */
static void
chip8_instruction_Cxkk(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t kk = instruction & 0xffu;
    chip8_registers_set_Vx(chip8->registers, x, (uint16_t)rand() & kk);
}

/***
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 */
static void
chip8_instruction_Dxyn(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t y = (instruction >> (1u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    uint8_t Vy = chip8_registers_get_Vx(chip8->registers, y);
    uint8_t n = instruction & 0x0fu;

    bool collision = chip8_display_draw(chip8->display, Vx, Vy, n,
                       &chip8->memory->memory[chip8_registers_get_I(chip8->registers)]);
    chip8_registers_set_Vx(chip8->registers, 0x0f, collision);
}

static void
chip8_instruction_ExXX(struct chip8 *chip8, uint16_t instruction)
{
    if ((instruction & 0x00ffu) == 0x9e) {
        chip8_instruction_Ex9E(chip8, instruction);
    } else {
        chip8_instruction_ExA1(chip8, instruction);
    }
}

/***
 * Skips the next instruction if the key stored in VX is pressed.
 * Checks the keyboard, and if the key corresponding to the value of Vx
 * is currently in the down position, PC is increased by 2.
 */
static void
chip8_instruction_Ex9E(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);

    if (chip8_keyboard_key_is_pressed(chip8->keyboard, Vx & 0x0fu)) {
        chip8_registers_increment_PC(chip8->registers);
    }
}

/***
 * Skip next instruction if key with the value of Vx is not pressed.
 * Checks the keyboard, and if the key corresponding to the value of Vx
 * is currently in the up position, PC is increased by 2.
 */
static void
chip8_instruction_ExA1(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);

    if (!chip8_keyboard_key_is_pressed(chip8->keyboard, Vx & 0x0fu)) {
        chip8_registers_increment_PC(chip8->registers);
    }
}


static void
chip8_instruction_FXXX(struct chip8 *chip8, uint16_t instruction)
{
    void (*func)(struct chip8 *chip8, uint16_t instruction) = NULL;
    switch (instruction & 0x00ffu) {
        case 0x07:
            func = chip8_instruction_Fx07;
            break;
        case 0x0a:
            func = chip8_instruction_Fx0A;
            break;
        case 0x15:
            func = chip8_instruction_Fx15;
            break;
        case 0x18:
            func = chip8_instruction_Fx18;
            break;
        case 0x1e:
            func = chip8_instruction_Fx1E;
            break;
        case 0x29:
            func = chip8_instruction_Fx29;
            break;
        case 0x33:
            func = chip8_instruction_Fx33;
            break;
        case 0x55:
            func = chip8_instruction_Fx55;
            break;
        case 0x65:
            func = chip8_instruction_Fx65;
            break;
    }
    func(chip8, instruction);
}

/***
 * Set Vx = delay timer value.
 * The value of DT is placed into Vx.
 */
static void
chip8_instruction_Fx07(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    chip8_registers_set_Vx(chip8->registers, x, chip8_registers_get_DT(chip8->registers));
}

/***
 * Wait for a key press, store the value of the key in Vx.
 * All execution stops until a key is pressed, then the value of that key is stored in Vx.
 */
static void
chip8_instruction_Fx0A(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t key = chip8_keyboard_get_pressed(chip8->keyboard);

    if (!(key >> 1u)) {
        chip8_registers_decrement_PC(chip8->registers);
    } else {
        chip8_registers_set_Vx(chip8->registers, x, key);
    }
}

/***
 * Sets the delay timer to VX.
 */
static void
chip8_instruction_Fx15(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    chip8_registers_set_DT(chip8->registers, Vx);
}

/***
 * Sets the sound timer to VX.
 */
static void
chip8_instruction_Fx18(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    chip8_registers_set_ST(chip8->registers, Vx);
}

/***
 * Set I = I + Vx.
 * The values of I and Vx are added, and the results are stored in I.
 */
static void
chip8_instruction_Fx1E(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    chip8_registers_set_I(chip8->registers, chip8_registers_get_I(chip8->registers) + Vx);
}

/***
 * Set I = location of sprite for digit Vx.
 */
static void
chip8_instruction_Fx29(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    chip8_registers_set_I(chip8->registers, chip8_memory_get_digit_sprite(Vx & 0x0fu));
}

static void
chip8_instruction_Fx33(struct chip8 *chip8, uint16_t instruction)
{
    uint16_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint8_t Vx = chip8_registers_get_Vx(chip8->registers, x);
    uint16_t I = chip8_registers_get_I(chip8->registers);

    uint8_t hundreds = Vx / 100;
    uint8_t tens = (Vx / 10) % 10;
    uint8_t ones = Vx % 10;

    chip8->memory->memory[I] = hundreds;
    chip8->memory->memory[I + 1] = tens;
    chip8->memory->memory[I + 2] = ones;
}

/***
 * Store registers V0 through Vx in memory starting at location I.
 * The interpreter copies the values of registers V0 through Vx into memory,
 * starting at the address in I.
 */
static void
chip8_instruction_Fx55(struct chip8 *chip8, uint16_t instruction)
{
    uint8_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t I = chip8_registers_get_I(chip8->registers);

    for (uint8_t i = 0; i <= x; i++) {
        chip8->memory->memory[I + i] = chip8_registers_get_Vx(chip8->registers, i);
    }
}

/***
 * Read registers V0 through Vx from memory starting at location I.
 * The interpreter reads values from memory starting at location I into registers V0 through Vx.
 */
static void
chip8_instruction_Fx65(struct chip8 *chip8, uint16_t instruction)
{
    uint8_t x = (instruction >> (2u * NIBBLE)) & 0x0fu;
    uint16_t I = chip8_registers_get_I(chip8->registers);

    for (uint8_t i = 0; i <= x; i++) {
        chip8_registers_set_Vx(chip8->registers, i, chip8->memory->memory[I + i]);
    }
}

