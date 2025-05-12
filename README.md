# Algoritmo do Banqueiro — Trabalho Prático de Sistemas Operacionais

Este projeto implementa o **Algoritmo do Banqueiro** descrito no livro _"Fundamentos de Sistemas Operacionais"_ (Silberschatz, Galvin e Gagne, 9ª edição), utilizando **threads** e **controle de concorrência com mutex**. O objetivo é simular múltiplos clientes (threads) que requisitam e liberam recursos de maneira segura, evitando **deadlocks** e **condições de corrida**.

## Compilação

> **Requisitos:** GCC com suporte a pthreads (Linux ou WSL no Windows)

Compile o código com:

```bash
gcc -o banqueiro banqueiro.c -lpthread

Execute o código com:

./banqueiro X1 X2 X3

Exemplo:

./banqueiro 10 5 7
