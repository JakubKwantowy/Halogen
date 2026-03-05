#ifndef STACK_H
#define STACK_H

#include <common.h>

typedef struct Stack {
    size_t size;
    size_t top;
    size_t *stack;
} STACK_t;

#define STACK_DEFAULTSIZE 16
#define Stack_Init(s) Stack_InitSize(s, STACK_DEFAULTSIZE)

extern int Stack_InitSize(STACK_t *s, size_t is);
extern void Stack_Free(STACK_t *s);
extern int Stack_Resize(STACK_t *s, size_t ts);
extern int Stack_Push(STACK_t *s, size_t v);
extern size_t Stack_Pop(STACK_t *s);

#endif
