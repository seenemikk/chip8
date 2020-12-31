#include "inc/chip8_stack.h"

void
chip8_stack_push(struct chip8_stack *stack, uint8_t *SP, uint16_t PC)
{
    stack->stack[(*SP)++] = PC;
}

uint16_t
chip8_stack_pop(struct chip8_stack *stack, uint8_t *SP)
{
    return stack->stack[--*SP];
}
