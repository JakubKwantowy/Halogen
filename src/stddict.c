#include <stddict.h>

#include <stdio.h>
#include <vm.h>
#include <stack.h>

int Halogen_print(VM_t *vm) {
    INT_t n = (INT_t) Stack_Pop(&vm->stack);
    printf("%d ", n);

    return 0;
}

int Halogen_putc(VM_t *vm) {
    INT_t n = (INT_t) Stack_Pop(&vm->stack);
    putchar(n);

    return 0;
}

int Halogen_swap(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, v1);
    Stack_Push(&vm->stack, v2);

    return 0;
}

int Halogen_add(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 + v2));

    return 0;
}

int Halogen_sub(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 - v2));

    return 0;
}

int Halogen_mul(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 * v2));

    return 0;
}

int Halogen_div(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    if(v2 == 0) {
        VM_error("Divide by zero!");
        return 1;
    }

    Stack_Push(&vm->stack, (INT_t) (v1 / v2));

    return 0;
}

int Halogen_mod(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 % v2));

    return 0;
}

WORD_t *createDefaultDict() {
    WORD_t *dict = NULL;

    Dictionary_enqueueInternalWord(&dict, "print", (WORD_INTERNAL_t *) &Halogen_print);
    Dictionary_enqueueInternalWord(&dict, "putc", (WORD_INTERNAL_t *) &Halogen_putc);
    Dictionary_enqueueInternalWord(&dict, "swap", (WORD_INTERNAL_t *) &Halogen_swap);
    Dictionary_enqueueInternalWord(&dict, "+", (WORD_INTERNAL_t *) &Halogen_add);
    Dictionary_enqueueInternalWord(&dict, "-", (WORD_INTERNAL_t *) &Halogen_sub);
    Dictionary_enqueueInternalWord(&dict, "*", (WORD_INTERNAL_t *) &Halogen_mul);
    Dictionary_enqueueInternalWord(&dict, "/", (WORD_INTERNAL_t *) &Halogen_div);
    Dictionary_enqueueInternalWord(&dict, "%", (WORD_INTERNAL_t *) &Halogen_mod);

    return dict;
}
