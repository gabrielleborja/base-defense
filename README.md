# Base Defender

## Descrição

**Base Defender** é um jogo de defesa de base simples desenvolvido em C++ utilizando a biblioteca gráfica SFML. O jogador controla um herói que precisa defender sua nave de ondas de naves inimigas. O jogo conta com recursos de movimentação, ataque, inimigos com diferentes características, e uma tela inicial.

## Recursos Implementados

### 1. **Movimentação do Jogador**
- **Descrição**: O jogador pode controlar a posição da base utilizando as teclas W A S D.
- **Conceito Utilizado**: Manipulação de eventos de teclado para mover a base.

### 2. **Ataque**
- **Descrição**: O jogador pode atirar projéteis para derrotar os inimigos. A quantidade de balas do herói são gastas, sendo recarregadas ao eliminar uma nave inimiga.
- **Conceito Utilizado**: Criação de projéteis como entidades que se movem e interagem com os inimigos.

### 3. **Inimigos**
- **Descrição**: O jogo possui dois tipos de inimigos:
  - **Inimigo Simples**: Aparece desde o início do jogo com vida básica.
  - **Inimigo Forte**: Surge a partir do nível 3 e possui mais vida.
- **Conceito Utilizado**: Implementação de herança para criar diferentes tipos de inimigos, cada um com atributos próprios.

### 4. **Controle de Nível**
- **Descrição**: O jogo aumenta de nível automaticamente, aumentando a dificuldade conforme o jogador progride.
- **Conceito Utilizado**: Condição baseada no tempo para aumentar o nível e modificar os atributos dos inimigos.

### 5. **Tela Inicial**
- **Descrição**: O jogo começa com uma tela inicial com a mensagem "Pressione ENTER para começar".
- **Conceito Utilizado**: Implementação de uma tela de introdução antes do loop principal do jogo.

## Instruções de Compilação e Execução

### 1. **Pré-requisitos**
- **SFML**: Certifique-se de ter a biblioteca SFML instalada em sua máquina.
- Certifique-se de que o SFML está instalado no seu computador e que os diretórios de inclusão e bibliotecas estão configurados corretamente. O projeto assume que o SFML está localizado nos seguintes diretórios:

  *Cabeçalhos:* C:\SFML\include

   *Bibliotecas:* C:\SFML\lib
- **Compilador C++**: GCC, Clang, ou qualquer outro compatível com C++11 ou superior.

### 2. **Compilação**
- Navegue até o diretório do projeto.
- Compile o projeto utilizando os comando abaixo:

  ```bash
  make clean
  ```
  ```bash
  make
  ```

### 3. **Execução**
- Após a compilação, execute o jogo com o comando:

  ```bash
  ./BaseDefender
  ```

## Controles do Jogo

- **W-A-S-D**: Movimentar a base.
- **Botão esquerdo do mouse**: Atirar.
- **Enter**: Iniciar o jogo.
