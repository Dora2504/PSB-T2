
# Gerenciador de Memória - Trabalho 2 (PSB)

## Descrição
Este projeto implementa um **gerenciador de memória estática em C**, utilizando a estratégia de alocação **First Fit**.  
O objetivo é simular um sistema simples de alocação, liberação e visualização de blocos de memória dentro de um pool fixo.

---

## Estruturas de Dados

### `allocation_t`
Representa um bloco alocado dentro do pool.

```c
typedef struct allocation {
    void *start;             // Endereço inicial do bloco
    size_t size;             // Tamanho do bloco em bytes
    struct allocation *next; // Próximo bloco alocado
} allocation_t;
```

### `mymemory_t`
Controla o pool de memória e a lista de alocações.

```c
typedef struct {
    void *pool;              // Memória estática alocada
    size_t total_size;       // Tamanho total do pool
    allocation_t *head;      // Lista encadeada de blocos alocados
} mymemory_t;
```

---

## Funcionalidades

### 1. Inicialização
```c
mymemory_t* mymemory_init(size_t size);
```
- Aloca o pool de memória estática.
- Inicializa as estruturas de controle.

### 2. Alocação (First Fit)
```c
void* mymemory_alloc(mymemory_t *memory, size_t size);
```
- Percorre a lista em busca do **primeiro espaço livre que caiba** no tamanho solicitado.

### 3. Liberação
```c
void mymemory_free(mymemory_t *memory, void *ptr);
```
- Remove o bloco referente ao ponteiro informado.

### 4. Exibição de Alocações
```c
void mymemory_display(mymemory_t *memory);
```
- Mostra todos os blocos alocados.

### 5. Estatísticas de Memória
```c
void mymemory_stats(mymemory_t *memory);
```
- Exibe:
  - Tamanho total
  - Memória livre e alocada
  - Maior bloco livre
  - Fragmentação

### 6. Limpeza
```c
void mymemory_cleanup(mymemory_t *memory);
```
- Libera a estrutura e o pool.

---

## Estratégia First Fit

| Característica | Descrição |
|---|---|
| Busca | Do início ao fim |
| Critério de escolha | Primeiro bloco livre que caiba |
| Vantagens | Simples, rápido |
| Desvantagens | Pode gerar fragmentação |

---

## Makefile

```make
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = memory_manager
SOURCES = main.c mymemory.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: clean run test
```

---

## Como Compilar e Executar

```bash
make          # Compila
make run      # Executa
make test     # Executa com Valgrind
make clean    # Remove binários
```

---

## Exemplo de Uso

```c
#include "mymemory.h"

int main() {
    mymemory_t *mem = mymemory_init(1000);

    int *v = mymemory_alloc(mem, sizeof(int)*32);
    int *v2 = mymemory_alloc(mem, sizeof(int)*16);

    mymemory_free(mem, v);

    char *str = mymemory_alloc(mem, sizeof(char)*16);
    sprintf(str, "teste");

    mymemory_display(mem);
    mymemory_stats(mem);
    mymemory_cleanup(mem);

    return 0;
}
```

---

## Testes Realizados

- Alocações sequenciais e liberação
- Realocação após liberação
- Fragmentação de memória
- Casos de erro:
  - Pool cheio
  - Ponteiros inválidos
  - Liberação de NULL
  - Solicitação de 0 bytes

---

## Apoio no Desenvolvimento
Foram consultados:
- Exemplos básicos de listas encadeadas
- Conceitos de aritmética de ponteiros
- ChatGPT para esclarecimento de dúvidas conceituais
