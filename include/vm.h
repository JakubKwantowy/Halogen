#ifndef VM_H
#define VM_H

#include <common.h>
#include <tokenizer.h>
#include <stack.h>

#define POKEMEM_SIZE 65536

typedef struct VM {
    STACK_t stack;
    STACK_t substack;
    uint8_t pokemem[POKEMEM_SIZE];
} VM_t;

#include <dictionary.h>

extern int VM_Init(VM_t *vm);
extern void VM_Free(VM_t *vm);
extern int VM_Process(VM_t *vm, TOKEN_t *toklist, WORD_t *dict);

extern void VM_error(char *err);

#endif
