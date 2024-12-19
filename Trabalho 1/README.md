# Analisador Léxico e Sintático
Este repositório contém a implementação de um analisador léxico e sintático para uma linguagem de programação básica, desenvolvida como parte do trabalho prático na disciplina de Compiladores.

Linguagem de Programação Básica

## A linguagem suporta:

- Condicionais (if, then, else)
- Loops (for, while)
- Operadores lógicos (and, or)
- Operadores aritméticos (+, -, *, /, mod)
- Declaração de variáveis e funções (let)

## Estrutura do Projeto

- Lexer (Analisador Léxico): Implementado usando Flex, para identificar tokens da linguagem.
- Parser (Analisador Sintático): Implementado com Bison, para validar a estrutura gramatical da linguagem.
- Tabela de Símbolos: Gerencia identificadores e elementos da linguagem.
- Arquivos de Teste: Conjunto de casos para validação dos analisadores.

## Requisitos para Compilar e Executar
__Certifique-se de ter as seguintes ferramentas instaladas no sistema:__

- Flex
- Bison
- Make
- Compilador pra arquivos C

## Testes
O repositório inclui arquivos de teste que cobrem:

- Casos válidos para todas as funcionalidades da linguagem.
- Casos inválidos para verificar erros sintáticos.
- Execute os testes fornecendo os arquivos de exemplo ao programa compilado para validar o funcionamento do analisador.