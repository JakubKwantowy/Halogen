#include <tokenizer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <murmur.h>

int Tokenizer_iswhitespace(char c) {
    if(c == 0) return 1;
    if(c == ' ') return 1;
    if(c == '\t') return 1;
    if(c == '\n') return 1;
    if(c == '\r') return 1;
    return 0;
}

int Tokenizer_isnumeric(char c) {
    return (c >= '0' && c <= '9');
}

TOKEN_t *Tokenizer_talloc(size_t size) {
    TOKEN_t *token = malloc(size);
    if(token == NULL) return NULL; 
    token->type = TOKEN_INVALID;
    token->next = NULL;
    return token;
}

void Tokenizer_tfree(TOKEN_t *toklist) {
    while(toklist) {
        TOKEN_t *next = toklist->next;
        free(toklist);
        toklist = next;
    }
}

void Tokenizer_enqueueToken(TOKEN_t **toklist, TOKEN_t *token) {
    TOKEN_t *current = *toklist;
    if(current == NULL) {
        *toklist = token;
        return;
    }
    while(current->next) current = current->next;
    current->next = token;
}

void Tokenizer_enqueueGeneric(TOKEN_t **toklist, TOKTYPE_t type) {
    TOKEN_t *token = Tokenizer_talloc(sizeof(*token));
    if(token == NULL) return;
    token->type = type;
    Tokenizer_enqueueToken(toklist, (TOKEN_t *) token);
}

void Tokenizer_enqueueString(TOKEN_t **toklist, TOKTYPE_t type, char *string) {
    struct Token_String *token = (struct Token_String *) Tokenizer_talloc(sizeof(*token));
    if(token == NULL) return;
    token->type = type;
    strncpy(token->value, string, STDSTRLEN);
    Tokenizer_enqueueToken(toklist, (TOKEN_t *) token);
}

void Tokenizer_enqueueHash(TOKEN_t **toklist, TOKTYPE_t type, char *string) {
    INT_t hash = MURMUR_strhash_noseed(string);
    struct Token_Numeric *token = (struct Token_Numeric *) Tokenizer_talloc(sizeof(*token));
    if(token == NULL) return;
    token->type = type;
    token->value = hash;
    Tokenizer_enqueueToken(toklist, (TOKEN_t *) token);
}

void Tokenizer_enqueueWord(TOKEN_t **toklist, TOKTYPE_t type, char *string) {
    INT_t hash = MURMUR_strhash_noseed(string);
    struct Token_Word *token = (struct Token_Word *) Tokenizer_talloc(sizeof(*token));
    if(token == NULL) return;
    token->type = type;
    token->value = hash;
    strncpy(token->name, string, sizeof(token->name) - 1);
    Tokenizer_enqueueToken(toklist, (TOKEN_t *) token);
}

void Tokenizer_enqueueNumeric(TOKEN_t **toklist, TOKTYPE_t type, INT_t num) {
    struct Token_Numeric *token = (struct Token_Numeric *) Tokenizer_talloc(sizeof(*token));
    if(token == NULL) return;
    token->type = type;
    token->value = num;
    Tokenizer_enqueueToken(toklist, (TOKEN_t *) token);
}

TOKEN_t *Tokenizer_parse(char *src) {
    TOKEN_t *toklist = NULL;
    
    char *src_ptr = src;
    char tokenBuffer[STDSTRLEN];
    while(*src_ptr) {
        int strflag = 0;
        size_t buf_idx = 0;
        while(!Tokenizer_iswhitespace(*src_ptr)) {
            if(*src_ptr == '"') {
                buf_idx = 0;
                src_ptr++;
                while(*src_ptr != '"') {
                    if(buf_idx == STDSTRLEN - 1) break;
                    tokenBuffer[buf_idx] = *src_ptr;
                    buf_idx++;
                    src_ptr++;
                }
                src_ptr++;
                strflag = 1;
                break;
            }
            if(buf_idx == STDSTRLEN - 1) break;
            tokenBuffer[buf_idx] = *src_ptr;
            buf_idx++;
            src_ptr++;
        }
        if(buf_idx) {
            tokenBuffer[buf_idx] = 0;
            if(strflag) {
                Tokenizer_enqueueString(&toklist, TOKEN_STRING, tokenBuffer);
                continue;
            }
            switch (tokenBuffer[0]) {
                case ':':
                    Tokenizer_enqueueWord(&toklist, TOKEN_REDEF, tokenBuffer+1);
                break;

                case ';':
                    Tokenizer_enqueueGeneric(&toklist, TOKEN_REDEF_END);
                break;

                case '(':
                    Tokenizer_enqueueGeneric(&toklist, TOKEN_CONDITIONAL);
                break;

                case ')':
                    Tokenizer_enqueueGeneric(&toklist, TOKEN_CONDITIONAL_END);
                break;

                case '#':
                    Tokenizer_enqueueGeneric(&toklist, TOKEN_COMMENT);
                break;
                
                case '-':
                    if(!Tokenizer_isnumeric(tokenBuffer[buf_idx - 1])) {
                        Tokenizer_enqueueWord(&toklist, TOKEN_WORD, tokenBuffer);
                        break;
                    }
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    INT_t n = (INT_t) strtol(tokenBuffer, NULL, 0);
                    Tokenizer_enqueueNumeric(&toklist, TOKEN_NUMBER, n);
                } break;

                default: {
                    Tokenizer_enqueueWord(&toklist, TOKEN_WORD, tokenBuffer);
                }
            }
        }
        src_ptr++;
    }

    return toklist;
}
