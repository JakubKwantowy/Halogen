#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <common.h>
#include <tokenizer.h>

typedef void REPLACE_WITH_VM_t;
typedef int WORD_INTERNAL_t(REPLACE_WITH_VM_t *vm);

typedef enum WordType {
    WORD_INVALID = -1,
    WORD_INTERNAL,
    WORD_CUSTOM
} WORDTYPE_t;

typedef struct Word {
    WORDTYPE_t type;
    INT_t hash;
    struct Word *next;
    void *content;
} WORD_t;

struct Word_Internal {
    WORDTYPE_t type;
    INT_t hash;
    struct Word *next;
    WORD_INTERNAL_t *content;
};

struct Word_Custom {
    WORDTYPE_t type;
    INT_t hash;
    struct Word *next;
    TOKEN_t *content;
};

extern void Dictionary_dfree(WORD_t *dict);
extern WORD_t *Dictionary_get(WORD_t *dict, INT_t hash);
extern void Dictionary_enqueueInternalWord(WORD_t **dict, const char *name, WORD_INTERNAL_t *f);
extern void Dictionary_addCustomWord(WORD_t **dict, INT_t hash, TOKEN_t *t);

#endif
