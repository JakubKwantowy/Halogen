#include <stddict.h>

#include <stdio.h>
#include <vm.h>
#include <stack.h>

static inline INT_t tobool(INT_t v) {
    return (v ? ~0 : 0);
}

int Halogen_print(VM_t *vm) {
    INT_t n = (INT_t) Stack_Pop(&vm->stack);
    printf("%d", n);

    return 0;
}

int Halogen_putc(VM_t *vm) {
    INT_t n = (INT_t) Stack_Pop(&vm->stack);
    putchar(n);

    return 0;
}

int Halogen_dup(VM_t *vm) {
    INT_t v = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, v);
    Stack_Push(&vm->stack, v);

    return 0;
}

int Halogen_pop(VM_t *vm) {
    Stack_Pop(&vm->stack);

    return 0;
}

int Halogen_swap(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, v1);
    Stack_Push(&vm->stack, v2);

    return 0;
}

int Halogen_input(VM_t *vm) {
    INT_t v;
    scanf("%d", &v);
    
    Stack_Push(&vm->stack, v);

    return 0;
}

int Halogen_getc(VM_t *vm) {
    char c = getchar();
    
    Stack_Push(&vm->stack, (INT_t) c);

    return 0;
}

int Halogen_top(VM_t *vm) {
    Stack_Push(&vm->stack, (INT_t) vm->stack.top);

    return 0;
}

int Halogen_grab(VM_t *vm) {
    size_t v1 = Stack_Pop(&vm->stack);
    size_t idx = vm->stack.top - v1 - 1;

    if(idx < 0) {
        VM_error("Bad stack index");
        return 1;
    }

    if(idx >= vm->stack.top) {
        VM_error("Bad stack index");
        return 1;
    }

    Stack_Push(&vm->stack, (INT_t) vm->stack.stack[idx]);

    return 0;
}

int Halogen_place(VM_t *vm) {
    size_t v1 = Stack_Pop(&vm->stack);
    size_t v2 = Stack_Pop(&vm->stack);
    size_t idx = vm->stack.top - v2 - 1;

    if(idx < 0) {
        VM_error("Bad stack index");
        return 1;
    }

    if(idx >= vm->stack.top) {
        VM_error("Bad stack index");
        return 1;
    }

    vm->stack.stack[idx] = (size_t) v1;

    return 0;
}

int Halogen_dswap(VM_t *vm) {
    size_t v1 = Stack_Pop(&vm->stack);
    size_t v2 = Stack_Pop(&vm->stack);
    size_t i1 = vm->stack.top - v1 - 1;
    size_t i2 = vm->stack.top - v2 - 1;

    if(i1 < 0) {
        VM_error("Bad stack index");
        return 1;
    }

    if(i1 >= vm->stack.top) {
        VM_error("Bad stack index");
        return 1;
    }

    if(i2 < 0) {
        VM_error("Bad stack index");
        return 1;
    }

    if(i2 >= vm->stack.top) {
        VM_error("Bad stack index");
        return 1;
    }

    size_t temp = vm->stack.stack[i1];
    vm->stack.stack[i1] = vm->stack.stack[i2];
    vm->stack.stack[i2] = temp;

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

int Halogen_bool(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1));

    return 0;
}

int Halogen_and(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 & v2));

    return 0;
}

int Halogen_or(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 | v2));

    return 0;
}

int Halogen_xor(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 ^ v2));

    return 0;
}

int Halogen_not(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (~v1));

    return 0;
}

int Halogen_shiftleft(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 << v2));

    return 0;
}

int Halogen_shiftright(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, (INT_t) (v1 >> v2));

    return 0;
}

int Halogen_eq(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1 == v2));

    return 0;
}

int Halogen_gt(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1 > v2));

    return 0;
}

int Halogen_lt(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1 < v2));

    return 0;
}

int Halogen_gteq(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1 >= v2));

    return 0;
}

int Halogen_lteq(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    Stack_Push(&vm->stack, tobool(v1 <= v2));

    return 0;
}

int Halogen_poke(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    INT_t v2 = (INT_t) Stack_Pop(&vm->stack);
    
    vm->pokemem[v2 & (POKEMEM_SIZE - 1)] = (uint8_t) v1; // If you're changing POKEMEM_SIZE, change it to [v2 % POKEMEM_SIZE] if POKEMEM_SIZE is not a power of 2

    return 0;
}

int Halogen_peek(VM_t *vm) {
    INT_t v1 = (INT_t) Stack_Pop(&vm->stack);
    
    INT_t v = (INT_t) vm->pokemem[v1 & (POKEMEM_SIZE - 1)]; // If you're changing POKEMEM_SIZE, change it to [v1 % POKEMEM_SIZE] if POKEMEM_SIZE is not a power of 2
    Stack_Push(&vm->stack, v);

    return 0;
}

WORD_t *createDefaultDict() {
    WORD_t *dict = NULL;

    Dictionary_enqueueInternalWord(&dict, "print", (WORD_INTERNAL_t *) &Halogen_print);
    Dictionary_enqueueInternalWord(&dict, "putc", (WORD_INTERNAL_t *) &Halogen_putc);
    Dictionary_enqueueInternalWord(&dict, "dup", (WORD_INTERNAL_t *) &Halogen_dup);
    Dictionary_enqueueInternalWord(&dict, "pop", (WORD_INTERNAL_t *) &Halogen_pop);
    Dictionary_enqueueInternalWord(&dict, "swap", (WORD_INTERNAL_t *) &Halogen_swap);
    Dictionary_enqueueInternalWord(&dict, "input", (WORD_INTERNAL_t *) &Halogen_input);
    Dictionary_enqueueInternalWord(&dict, "getc", (WORD_INTERNAL_t *) &Halogen_getc);
    Dictionary_enqueueInternalWord(&dict, "top", (WORD_INTERNAL_t *) &Halogen_top);
    Dictionary_enqueueInternalWord(&dict, "grab", (WORD_INTERNAL_t *) &Halogen_grab);
    Dictionary_enqueueInternalWord(&dict, "place", (WORD_INTERNAL_t *) &Halogen_place);
    Dictionary_enqueueInternalWord(&dict, "dswap", (WORD_INTERNAL_t *) &Halogen_dswap);

    Dictionary_enqueueInternalWord(&dict, "+", (WORD_INTERNAL_t *) &Halogen_add);
    Dictionary_enqueueInternalWord(&dict, "-", (WORD_INTERNAL_t *) &Halogen_sub);
    Dictionary_enqueueInternalWord(&dict, "*", (WORD_INTERNAL_t *) &Halogen_mul);
    Dictionary_enqueueInternalWord(&dict, "/", (WORD_INTERNAL_t *) &Halogen_div);
    Dictionary_enqueueInternalWord(&dict, "%", (WORD_INTERNAL_t *) &Halogen_mod);

    Dictionary_enqueueInternalWord(&dict, "bool", (WORD_INTERNAL_t *) &Halogen_bool);
    Dictionary_enqueueInternalWord(&dict, "&", (WORD_INTERNAL_t *) &Halogen_and);
    Dictionary_enqueueInternalWord(&dict, "|", (WORD_INTERNAL_t *) &Halogen_or);
    Dictionary_enqueueInternalWord(&dict, "^", (WORD_INTERNAL_t *) &Halogen_xor);
    Dictionary_enqueueInternalWord(&dict, "~", (WORD_INTERNAL_t *) &Halogen_not);
    Dictionary_enqueueInternalWord(&dict, "<<", (WORD_INTERNAL_t *) &Halogen_shiftleft);
    Dictionary_enqueueInternalWord(&dict, ">>", (WORD_INTERNAL_t *) &Halogen_shiftright);

    Dictionary_enqueueInternalWord(&dict, "=", (WORD_INTERNAL_t *) &Halogen_eq);
    Dictionary_enqueueInternalWord(&dict, ">", (WORD_INTERNAL_t *) &Halogen_gt);
    Dictionary_enqueueInternalWord(&dict, "<", (WORD_INTERNAL_t *) &Halogen_lt);
    Dictionary_enqueueInternalWord(&dict, ">=", (WORD_INTERNAL_t *) &Halogen_lteq);
    Dictionary_enqueueInternalWord(&dict, "<=", (WORD_INTERNAL_t *) &Halogen_gteq);

    Dictionary_enqueueInternalWord(&dict, "poke", (WORD_INTERNAL_t *) &Halogen_poke);
    Dictionary_enqueueInternalWord(&dict, "peek", (WORD_INTERNAL_t *) &Halogen_peek);

    return dict;
}
