#include <stdio.h>
#include "mymemory.h"

int main()
{
    // aloca pool memoria
    mymemory_t *memory = mymemory_init(1000);

    //int v[32];
    // int *v = malloc(sizeof(int)*32);
    int *v = mymemory_alloc(memory, sizeof(int)*32);
    printf("Array v alocado em: %p\n", (void*)v);
    
    for (int i = 0; i < 32; i++)
        v[i] = i+1;

    int *v2 = mymemory_alloc(memory, sizeof(int)*16);
    printf("Array v2 alocado em: %p\n", (void*)v2);

    // Mostra estado antes do free
    printf("\n--- Antes de liberar v ---");
    mymemory_display(memory);
    mymemory_stats(memory);

    // free(v);
    mymemory_free(memory, v);
    printf("\n--- Após liberar v ---");
    mymemory_display(memory);
    mymemory_stats(memory);

    char *str = mymemory_alloc(memory, sizeof(char)*16);
    printf("String alocada em: %p\n", (void*)str);
    sprintf(str, "ola mundo");
    printf("Conteúdo da string: %s\n", str);

    // Exibe estado final
    printf("\n--- Estado Final ---");
    mymemory_display(memory);
    mymemory_stats(memory);

    // libera o pool de memoria
    mymemory_cleanup(memory);

    return 0;
}