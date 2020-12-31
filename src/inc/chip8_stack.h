#ifndef CHIP8_CHIP8_STACK_H
#define CHIP8_CHIP8_STACK_H

#include <stdint.h>

#define STACK_SIZE 16

struct chip8_stack {
    uint16_t stack[STACK_SIZE];
};

void chip8_stack_push(struct chip8_stack *stack, uint8_t *SP, uint16_t PC);
uint16_t chip8_stack_pop(struct chip8_stack *stack, uint8_t *SP);

#endif //CHIP8_CHIP8_STACK_H
