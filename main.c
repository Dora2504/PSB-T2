#include <stdio.h>
#include "mymemory.h"

int main() {
    printf("=== TESTE DO GERENCIADOR DE MEMÓRIA ===\n");
    
    // 1. Inicialização
    printf("\n1. INICIALIZANDO POOL DE 1000 BYTES\n");
    mymemory_t *memory = mymemory_init(1000);
    if (!memory) {
        printf("Erro ao inicializar memória\n");
        return 1;
    }
    
    // 2. Primeiras alocações
    printf("\n2. PRIMEIRAS ALOCAÇÕES\n");
    int *v = mymemory_alloc(memory, sizeof(int) * 32);  // 128 bytes
    printf("Array v[32] alocado em: %p\n", (void*)v);
    
    for (int i = 0; i < 32; i++) {
        v[i] = i + 1;
    }
    printf("Valores em v: %d, %d, ..., %d\n", v[0], v[1], v[31]);
    
    int *v2 = mymemory_alloc(memory, sizeof(int) * 16);  // 64 bytes
    printf("Array v2[16] alocado em: %p\n", (void*)v2);
    
    // Usar v2 para evitar warning
    for (int i = 0; i < 16; i++) {
        v2[i] = i * 2;
    }
    printf("Valores em v2: %d, %d, ..., %d\n", v2[0], v2[1], v2[15]);
    
    // Estado atual
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 3. Liberação e fragmentação
    printf("\n3. TESTE DE LIBERAÇÃO E FRAGMENTAÇÃO\n");
    printf("Liberando array v...\n");
    mymemory_free(memory, v);
    
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 4. Nova alocação (First Fit)
    printf("\n4. NOVA ALOCAÇÃO NO ESPAÇO LIVRE\n");
    char *str = mymemory_alloc(memory, sizeof(char) * 16);  // 16 bytes
    printf("String alocada em: %p\n", (void*)str);
    
    sprintf(str, "ola mundo");
    printf("Conteúdo da string: '%s'\n", str);
    
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 5. Teste com múltiplos blocos
    printf("\n5. TESTE COM MÚLTIPLOS BLOCOS\n");
    void* blocks[5];
    size_t sizes[] = {50, 100, 25, 75, 200};
    
    for (int i = 0; i < 5; i++) {
        blocks[i] = mymemory_alloc(memory, sizes[i]);
        if (blocks[i]) {
            printf("Bloco %d: %zu bytes alocados em %p\n", i, sizes[i], blocks[i]);
        } else {
            printf("Falha ao alocar bloco %d de %zu bytes\n", i, sizes[i]);
        }
    }
    
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 6. Liberação seletiva para criar fragmentação
    printf("\n6. CRIANDO FRAGMENTAÇÃO\n");
    printf("Liberando blocos 1 e 3...\n");
    mymemory_free(memory, blocks[1]);  // Libera 100 bytes
    mymemory_free(memory, blocks[3]);  // Libera 75 bytes
    
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 7. Tentativa de alocar bloco grande
    printf("\n7. TESTE DE ALOCAÇÃO COM FRAGMENTAÇÃO\n");
    void* large_block = mymemory_alloc(memory, 150);
    if (large_block) {
        printf("Bloco grande de 150 bytes alocado em %p\n", large_block);
    } else {
        printf("Não foi possível alocar 150 bytes (fragmentação!)\n");
    }
    
    // 8. Alocação em fragmentos menores
    void* small1 = mymemory_alloc(memory, 40);
    void* small2 = mymemory_alloc(memory, 30);
    if (small1) printf("Bloco pequeno 1 (40 bytes) alocado em %p\n", small1);
    if (small2) printf("Bloco pequeno 2 (30 bytes) alocado em %p\n", small2);
    
    mymemory_display(memory);
    mymemory_stats(memory);
    
    // 9. Limpeza final
    printf("\n8. LIMPEZA FINAL\n");
    mymemory_cleanup(memory);
    
    printf("\n=== TESTE CONCLUÍDO ===\n");
    return 0;
}