#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inicializa o gerenciador de memória
mymemory_t* mymemory_init(size_t size) {
    mymemory_t* memory = (mymemory_t*)malloc(sizeof(mymemory_t));
    if (!memory) return NULL;
    
    memory->pool = malloc(size);
    if (!memory->pool) {
        free(memory);
        return NULL;
    }
    
    memory->total_size = size;
    memory->head = NULL;
    
    printf("Pool de memória inicializado com %zu bytes em %p\n", size, memory->pool);
    return memory;
}

// Aloca memória (First Fit)
void* mymemory_alloc(mymemory_t *memory, size_t size) {
    if (!memory || size == 0 || size > memory->total_size) {
        printf("Erro: tentativa de alocar %zu bytes inválida\n", size);
        return NULL;
    }
    
    allocation_t* new_alloc = (allocation_t*)malloc(sizeof(allocation_t));
    if (!new_alloc) {
        printf("Erro: não foi possível alocar estrutura de controle\n");
        return NULL;
    }
    
    printf("Tentando alocar %zu bytes...\n", size);
    
    // Caso 1: Lista vazia - aloca no início do pool
    if (memory->head == NULL) {
        new_alloc->start = memory->pool;
        new_alloc->size = size;
        new_alloc->next = NULL;
        memory->head = new_alloc;
        printf("Alocado no início: %zu bytes em %p\n", size, new_alloc->start);
        return new_alloc->start;
    }
    
    // Caso 2: Verifica espaço antes da primeira alocação
    size_t space_before_first = (char*)memory->head->start - (char*)memory->pool;
    if (space_before_first >= size) {
        new_alloc->start = memory->pool;
        new_alloc->size = size;
        new_alloc->next = memory->head;
        memory->head = new_alloc;
        printf("Alocado antes do primeiro: %zu bytes em %p\n", size, new_alloc->start);
        return new_alloc->start;
    }
    
    // Caso 3: Procura espaço entre alocações existentes
    allocation_t* current = memory->head;
    while (current->next != NULL) {
        void* current_end = (char*)current->start + current->size;
        size_t gap_size = (char*)current->next->start - (char*)current_end;
        
        printf("Verificando gap entre %p e %p: %zu bytes\n", 
               current_end, current->next->start, gap_size);
        
        if (gap_size >= size) {
            new_alloc->start = current_end;
            new_alloc->size = size;
            new_alloc->next = current->next;
            current->next = new_alloc;
            printf("Alocado no gap: %zu bytes em %p\n", size, new_alloc->start);
            return new_alloc->start;
        }
        current = current->next;
    }
    
    // Caso 4: Tenta alocar no final
    void* last_end = (char*)current->start + current->size;
    size_t remaining_space = (char*)memory->pool + memory->total_size - (char*)last_end;
    
    printf("Espaço restante no final: %zu bytes\n", remaining_space);
    
    if (remaining_space >= size) {
        new_alloc->start = last_end;
        new_alloc->size = size;
        new_alloc->next = NULL;
        current->next = new_alloc;
        printf("Alocado no final: %zu bytes em %p\n", size, new_alloc->start);
        return new_alloc->start;
    }
    
    // Não há espaço suficiente
    printf("Falha: não há espaço suficiente para alocar %zu bytes\n", size);
    free(new_alloc);
    return NULL;
}

// Libera memória
void mymemory_free(mymemory_t *memory, void *ptr) {
    if (!memory || !ptr) {
        printf("Erro: parâmetros inválidos para free\n");
        return;
    }
    
    printf("Tentando liberar memória em %p\n", ptr);
    
    allocation_t* current = memory->head;
    allocation_t* prev = NULL;
    
    while (current != NULL) {
        if (current->start == ptr) {
            if (prev == NULL) {
                memory->head = current->next;
            } else {
                prev->next = current->next;
            }
            
            printf("Memória liberada: %zu bytes em %p\n", current->size, current->start);
            free(current);
            return;
        }
        
        prev = current;
        current = current->next;
    }
    
    printf("Erro: ponteiro %p não encontrado para liberação\n", ptr);
}

// Exibe alocações atuais
void mymemory_display(mymemory_t *memory) {
    if (!memory) {
        printf("Erro: memória não inicializada\n");
        return;
    }
    
    printf("\n=== Alocações Atuais ===\n");
    allocation_t* current = memory->head;
    int count = 0;
    
    while (current != NULL) {
        printf("Alocação %d: %p - %zu bytes\n", 
               count++, current->start, current->size);
        current = current->next;
    }
    
    if (count == 0) {
        printf("Nenhuma alocação ativa\n");
    }
}

// Exibe estatísticas CORRIGIDA
void mymemory_stats(mymemory_t *memory) {
    if (!memory) {
        printf("Erro: memória não inicializada\n");
        return;
    }
    
    printf("\n=== Estatísticas de Memória ===\n");
    
    int total_allocations = 0;
    size_t total_allocated = 0;
    size_t largest_free_block = 0;
    int free_fragments = 0;
    
    allocation_t* current = memory->head;
    
    // Calcula memória alocada
    while (current != NULL) {
        total_allocations++;
        total_allocated += current->size;
        current = current->next;
    }
    
    // CALCULA FRAGMENTOS E MAIOR BLOCO LIVRE CORRETAMENTE
    current = memory->head;
    void* current_pos = memory->pool;
    
    while (current != NULL) {
        // Espaço livre antes desta alocação
        size_t free_space = (char*)current->start - (char*)current_pos;
        if (free_space > 0) {
            free_fragments++;
            if (free_space > largest_free_block) {
                largest_free_block = free_space;
            }
        }
        current_pos = (char*)current->start + current->size;
        current = current->next;
    }
    
    // Espaço livre após a última alocação
    size_t final_free_space = (char*)memory->pool + memory->total_size - (char*)current_pos;
    if (final_free_space > 0) {
        free_fragments++;
        if (final_free_space > largest_free_block) {
            largest_free_block = final_free_space;
        }
    }
    
    // Se não há fragmentos mas há memória livre, considera como 1 fragmento
    if (free_fragments == 0) {
        size_t free_memory = memory->total_size - total_allocated;
        if (free_memory > 0) {
            free_fragments = 1;
            largest_free_block = free_memory;
        }
    }
    
    size_t free_memory = memory->total_size - total_allocated;
    
    printf("Total de alocações: %d\n", total_allocations);
    printf("Memória total alocada: %zu bytes\n", total_allocated);
    printf("Memória total livre: %zu bytes\n", free_memory);
    printf("Maior bloco livre: %zu bytes\n", largest_free_block);
    printf("Fragmentos livres: %d\n", free_fragments);
}

// Libera todos os recursos
void mymemory_cleanup(mymemory_t *memory) {
    if (!memory) return;
    
    printf("Limpando todos os recursos...\n");
    
    allocation_t* current = memory->head;
    while (current != NULL) {
        allocation_t* next = current->next;
        printf("Liberando alocação: %p - %zu bytes\n", current->start, current->size);
        free(current);
        current = next;
    }
    
    free(memory->pool);
    free(memory);
    
    printf("Todos os recursos liberados\n");
}