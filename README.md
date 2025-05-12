# Algoritmo do Banqueiro — Trabalho Prático de Sistemas Operacionais

Este projeto implementa o **Algoritmo do Banqueiro** descrito no livro _"Fundamentos de Sistemas Operacionais"_ (Silberschatz, Galvin e Gagne, 9ª edição), utilizando **threads** e **controle de concorrência com mutex**. O objetivo é simular múltiplos clientes (threads) que requisitam e liberam recursos de maneira segura, evitando **deadlocks** e **condições de corrida**.

## 💡 Objetivo

O algoritmo do banqueiro verifica se as requisições de recursos feitas por clientes deixam o sistema em um **estado seguro**. Caso contrário, as requisições são negadas. Este projeto simula essa lógica utilizando `pthread` em C.

## 📦 Estrutura

- `banqueiro.c`: Código-fonte principal do simulador.
- `relatorio_banqueiro.tex`: Relatório em LaTeX com a explicação da implementação.
- `README.md`: Este arquivo com instruções de uso.

## 🛠️ Compilação

> **Requisitos:** GCC com suporte a pthreads (Linux ou WSL no Windows)

Compile o código com:

```bash
gcc -o banqueiro banqueiro.c -lpthread

Execute o código com:

./banqueiro X1 X2 X3

Exemplo: ./banqueiro 10 5 7
