#include <stack.h>

#include <stdlib.h>

int Stack_InitSize(STACK_t *s, size_t is) {
    if(s == NULL) return 1;
    s->stack = malloc(is * sizeof(*s->stack));
    if(s->stack == NULL) return 1;
    s->size = is;
    s->top = 0;
    return 0;
}

void Stack_Free(STACK_t *s) {
    if(s == NULL) return;
    if(s->stack == NULL) return;
    free(s->stack);
    s->stack = NULL;
    s->size = 0;
    s->top = 0;
}

int Stack_Resize(STACK_t *s, size_t ts) {
    if(s == NULL) return 1;
    if(s->stack == NULL) return 1;
    size_t *new = malloc(ts * sizeof(*s->stack));
    if(new == NULL) return 1;
    size_t tocopy = ts > s->top ? s->top : ts;
    for(size_t i=0;i<tocopy;i++) {
        new[i] = s->stack[i];
    }
    free(s->stack);
    s->stack = new;
    s->size = ts;
    s->top = tocopy;
    return 0;
}

int Stack_Push(STACK_t *s, size_t v) {
    if(s == NULL) return 1;
    if(s->stack == NULL) return 1;
    if(s->top >= s->size) {
        if(Stack_Resize(s, s->size * 2)) return 1;
    }
    s->stack[s->top] = v;
    s->top++;
    return 0;
}

size_t Stack_Pop(STACK_t *s) {
    if(s == NULL) return 0;
    if(s->stack == NULL) return 0;
    if(s->top == 0) return 0;
    s->top--;
    if(s->top >= s->size) return 0;
    return s->stack[s->top];
}
