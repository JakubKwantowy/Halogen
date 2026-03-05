#include <stdio.h>
#include <stdlib.h>
#include <tokenizer.h>
#include <dictionary.h>
#include <stddict.h>
#include <vm.h>
#include <murmur.h>

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: %s <program.hal>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL) {
        perror("File error");
        goto err_file;
    }

    fseek(f, 0, SEEK_END);
    size_t buffer_size = ftell(f) + 1;
    rewind(f);

    char *buffer = malloc(buffer_size);
    if(buffer == NULL) {
        perror("Buffer error");
        goto err_buffer;
    }

    fread(buffer, 1, buffer_size, f);
    fclose(f);

    TOKEN_t *toklist = Tokenizer_parse(buffer);
    free(buffer);

    WORD_t *dict = createDefaultDict();

    VM_t vm;
    if(VM_Init(&vm)) {
        perror("VM Init Error");
        goto err_vm;
    }

    int ret = VM_Process(&vm, toklist, dict);
    
    VM_Free(&vm);
    Dictionary_dfree(dict);
    Tokenizer_tfree(toklist);

    return ret;

    err_vm:
    return 1;

    err_buffer:
    fclose(f);

    err_file:
    return 1;
}
