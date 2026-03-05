#include <dictionary.h>

#include <stdlib.h>
#include <murmur.h>

WORD_t *Dictionary_walloc() {
    WORD_t *word = malloc(sizeof(*word));
    if(word == NULL) return NULL;
    word->type = WORD_INTERNAL;
    word->hash = 0;
    word->next = NULL;
    word->content = NULL;
    return word;
}

void Dictionary_dfree(WORD_t *dict) {
    while(dict) {
        WORD_t *next = dict->next;
        free(dict);
        dict = next;
    }
}

WORD_t *Dictionary_get(WORD_t *dict, INT_t hash) {
    WORD_t *word = dict;
    while(word) {
        if(word->hash == hash) break;
        word = word->next;
    }
    return word;
}

void Dictionary_enqueueWord(WORD_t **dict, WORD_t *word) {
    WORD_t *current = *dict;
    if(current == NULL) {
        *dict = word;
        return;
    }
    while(current->next) current = current->next;
    current->next = word;
}

void Dictionary_enqueueInternalWord(WORD_t **dict, const char *name, WORD_INTERNAL_t *f) {
    INT_t hash = MURMUR_strhash_noseed(name);
    struct Word_Internal *word = (struct Word_Internal *) Dictionary_walloc();
    if(word == NULL) return;
    word->type = WORD_INTERNAL;
    word->hash = hash;
    word->content = f;
    Dictionary_enqueueWord(dict, (WORD_t *) word);
}

void Dictionary_enqueueCustomWord(WORD_t **dict, INT_t hash, TOKEN_t *t) {
    struct Word_Custom *word = (struct Word_Custom *) Dictionary_walloc();
    if(word == NULL) return;
    word->type = WORD_CUSTOM;
    word->hash = hash;
    word->content = t;
    Dictionary_enqueueWord(dict, (WORD_t *) word);
}

void Dictionary_addCustomWord(WORD_t **dict, INT_t hash, TOKEN_t *t) {
    WORD_t *word = Dictionary_get(*dict, hash);
    if(word == NULL) {
        Dictionary_enqueueCustomWord(dict, hash, t);
        return;
    }
    word->type = WORD_CUSTOM;
    ((struct Word_Custom *) word)->content = t;
}
