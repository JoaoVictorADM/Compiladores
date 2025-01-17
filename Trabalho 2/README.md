# Tradutor de JSON para MASPY

Este projeto implementa um tradutor que converte uma especificação em JSON para a linguagem de programação de agentes MASPY. O trabalho foi desenvolvido com o objetivo de explorar conceitos de compiladores, como análise léxica e sintática, ao mesmo tempo que promove o estudo de sistemas multiagentes baseados na arquitetura BDI (Belief-Desire-Intention).

---

## Objetivo do Projeto

O tradutor foi projetado para processar uma estrutura JSON que representa agentes, suas crenças, objetivos e planos, transformando-a em um código Python que pode ser executado utilizando a biblioteca MASPY. Essa conversão automatiza a criação de programas para sistemas multiagentes, permitindo que desenvolvedores foquem no design do comportamento dos agentes, enquanto o código final é gerado de forma eficiente e padronizada.

A ideia central é possibilitar que a definição de agentes e seus comportamentos em um formato amigável (JSON) seja traduzida para uma linguagem executável, garantindo compatibilidade e funcionalidade com a MASPY, uma ferramenta especializada em sistemas multiagentes.

---

## Como o Projeto Funciona

### Estrutura Geral

1. **Análise Léxica**: 
   - Foi utilizado o **Flex** para criar um analisador léxico capaz de identificar os tokens do JSON, como chaves, colchetes, strings e valores.
   - O analisador léxico é responsável por "ler" o arquivo JSON e identificar as partes que compõem a especificação.

2. **Análise Sintática**:
   - Com o **Bison**, foi desenvolvida uma gramática baseada na BNF (Backus-Naur Form) que define como o JSON deve ser estruturado para descrever agentes.
   - O analisador sintático verifica se o arquivo JSON segue essa gramática e organiza os dados em uma estrutura compreensível para o programa.

3. **Estrutura de Dados**:
   - Foi implementada uma estrutura de dados em C, baseada em **listas encadeadas**, para armazenar as informações dos agentes, como crenças, objetivos e planos.
   - Essa estrutura permite gerenciar os dados de forma dinâmica e eficiente durante a execução do programa.

4. **Geração de Código**:
   - Após a análise do JSON e o preenchimento da estrutura de dados, o programa gera um arquivo Python contendo o código dos agentes na linguagem MASPY.
   - Esse código é estruturado de forma a ser executado diretamente pelo interpretador MASPY.

---

## Utilidade do Projeto

### Para Sistemas Multiagentes

Sistemas multiagentes são amplamente utilizados em simulações, jogos, inteligência artificial e até mesmo em robótica. Esses sistemas permitem modelar interações complexas entre entidades autônomas (os agentes), que podem cooperar, competir ou agir de forma independente.

O JSON foi escolhido como formato de entrada pela sua simplicidade e legibilidade, permitindo que os agentes sejam definidos facilmente por pessoas que talvez não sejam especialistas em programação. O tradutor automatiza a transformação dessa definição em código Python, que pode ser executado diretamente para simular ou implementar o comportamento dos agentes.

### Exemplos de Aplicação

- **Simulações Educacionais**: Modelar agentes que representam alunos, professores ou recursos em uma simulação de sala de aula.
- **Jogos**: Criar NPCs (personagens não jogáveis) com comportamentos dinâmicos e adaptativos.
- **Sistemas de Suporte à Decisão**: Modelar entidades autônomas que auxiliem em tomadas de decisão em cenários complexos, como gerenciamento de recursos ou resposta a desastres.

---

## Detalhes do Código Gerado

O código Python gerado utiliza a biblioteca **MASPY**, que oferece suporte para a criação de agentes baseados na arquitetura BDI. Essa arquitetura modela o comportamento dos agentes em três componentes principais:

- **Crenças (Beliefs)**: Informações que o agente possui sobre o ambiente.
- **Objetivos (Goals)**: As metas que o agente busca alcançar.
- **Planos (Plans)**: Sequências de ações que o agente executa para atingir seus objetivos.

No código gerado, cada agente é representado como uma classe Python, com seus planos definidos como métodos decorados. Isso facilita a execução e a simulação dos agentes, bem como a integração com outros sistemas baseados em MASPY.

---

## Reflexões

Este projeto destaca como ferramentas como Flex e Bison podem ser utilizadas para construir soluções práticas em áreas específicas, como sistemas multiagentes. Além de cumprir com os objetivos acadêmicos de explorar conceitos de compiladores, ele também demonstra como linguagens e bibliotecas podem ser integradas para resolver problemas reais.

A transformação de um formato descritivo (JSON) para um formato executável (Python com MASPY) é um exemplo prático de como abstrações podem ser usadas para reduzir a complexidade no desenvolvimento de sistemas inteligentes. 

Por meio desse trabalho, é possível entender melhor a importância da automação na criação de soluções computacionais e como ferramentas bem projetadas podem impactar positivamente no desenvolvimento de software.
