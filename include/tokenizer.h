#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <common.h>

#define STDSTRLEN 4096

typedef enum TokenType {
    TOKEN_INVALID = -1,
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_REDEF,
    TOKEN_REDEF_END,
    TOKEN_CONDITIONAL,
    TOKEN_CONDITIONAL_END,
    TOKEN_COMMENT
} TOKTYPE_t;

typedef struct Token {
    TOKTYPE_t type;
    struct Token *next;
} TOKEN_t;

struct Token_Numeric {
    TOKTYPE_t type;
    struct Token *next;
    INT_t value;
};

struct Token_Word {
    TOKTYPE_t type;
    struct Token *next;
    INT_t value;
    char name[32];
};

struct Token_String {
    TOKTYPE_t type;
    struct Token *next;
    char value[STDSTRLEN];
};

extern void Tokenizer_tfree(TOKEN_t *toklist);
extern TOKEN_t *Tokenizer_parse(char *src);

#endif
