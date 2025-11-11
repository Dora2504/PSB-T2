# PSB-T2
# Gerenciador de Memória - Trabalho 2 PSB

## Descrição
Implementação de um gerenciador de memória simples em C usando a estratégia **First Fit**.

## Estrutura
- `mymemory.h` - Definições das estruturas e funções
- `mymemory.c` - Implementação do gerenciador
- `main.c` - Testes e exemplos de uso
- `Makefile` - Compilação automatizada

## Compilação e Execução
```bash
make        # Compilar
make run    # Compilar e executar
make test   # Executar com Valgrind (verificar memory leaks)
make clean  # Limpar arquivos compilados