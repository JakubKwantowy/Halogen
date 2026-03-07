#include <vm.h>

#include <stdio.h>
#include <string.h>
#include <dictionary.h>

int VM_Init(VM_t *vm) {
    if(vm == NULL) return 1;
    if(Stack_Init(&vm->stack)) return 1;
    if(Stack_Init(&vm->substack)) return 1;

    return 0;
}

void VM_Free(VM_t *vm) {
    if(vm == NULL) return;
    Stack_Free(&vm->stack);
    Stack_Free(&vm->substack);
}

void VM_error(char *err) {
    printf("[ERR] %s\n", err);
}

int VM_Process(VM_t *vm, TOKEN_t *toklist, WORD_t *dict) {
    for(TOKEN_t *token = toklist; token; token = token->next) {
        switch(token->type) {
            case TOKEN_NUMBER:
                Stack_Push(&vm->stack, ((struct Token_Numeric *) token)->value);
            break;

            case TOKEN_STRING: {
                char *s = ((struct Token_String *) token)->value;
                size_t l = strlen(s);
                for(char *p = s + l - 1; p >= s; p--) {
                    Stack_Push(&vm->stack, *p);
                }
            } break;

            case TOKEN_WORD: {
                INT_t hash = ((struct Token_Word *) token)->value;
                WORD_t *word = Dictionary_get(dict, hash);
                if(word == NULL) {
                    fprintf(stderr, "[ERR] Unknown word %s (%08x)\n", ((struct Token_Word *) token)->name, hash);
                    return 1;
                }
                switch (word->type) {
                    case WORD_INTERNAL: {
                        int ret = ((struct Word_Internal *) word)->content(vm);
                        if(ret) return ret;
                    } break;

                    case WORD_CUSTOM: {
                        Stack_Push(&vm->substack, (size_t) token);
                        TOKEN_t *t = ((struct Word_Custom *) word)->content;
                        token = t;
                    } break;
                
                    default:
                        fprintf(stderr, "[WARN] Unknown word type %d\n", word->type);
                    break;
                }
            } break;

            case TOKEN_REDEF: {
                INT_t hash = ((struct Token_Word *) token)->value;
                Dictionary_addCustomWord(&dict, hash, token);
                do {
                    token = token->next;
                } while(token->type != TOKEN_REDEF_END);
            } break;

            case TOKEN_REDEF_END: {
                TOKEN_t *t = (TOKEN_t *) Stack_Pop(&vm->substack);
                if(t == NULL) return 0;
                token = t;
            } break;

            case TOKEN_CONDITIONAL: {
                INT_t v = (INT_t) Stack_Pop(&vm->stack);
                Stack_Push(&vm->stack, v);

                if(!v) {
                    int counter = 0;

                    do {
                        token = token->next;
                        if(token->type == TOKEN_CONDITIONAL) counter++;
                        while( (token->type == TOKEN_CONDITIONAL_END) && counter ) {
                            counter--;
                            token = token->next;
                        }
                    } while(token->type != TOKEN_CONDITIONAL_END);
                    break;
                }

                Stack_Push(&vm->substack, (size_t) token);
            } break;

            case TOKEN_CONDITIONAL_END: {
                TOKEN_t *t = (TOKEN_t *) Stack_Pop(&vm->substack);
                if(t == NULL) {
                    VM_error(") without preceeding (");
                    return 1;
                }

                INT_t v = (INT_t) Stack_Pop(&vm->stack);
                Stack_Push(&vm->stack, v);

                if(!v) break;

                Stack_Push(&vm->substack, (size_t) t);
                token = t;
            } break;

            case TOKEN_COMMENT: {
                do {
                    token = token->next;
                } while(token->type != TOKEN_COMMENT && token->next != NULL);
            } break;

            default:
                fprintf(stderr, "[WARN] Unknown token %d\n", token->type);
            break;
        }
    }
    return 0;
}
