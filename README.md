## Exercício Computacional 1

Este exercício é um programa para encontrar a saída de um labirinto.

Link repositório:
https://github.com/leonardodesarocha/ATR_1_maze_runner

### Especificações

- O labirinto é representado por uma matriz em um arquivo de texto.
- A primeira linha do arquivo contém o número de linhas e colunas do labirinto.
- As linhas seguintes representam o conteúdo do labirinto:
  - 'x': caminho válido
  - '#': parede
  - 'e': entrada do labirinto
  - 's': saída do labirinto
- Para cada nova posição explorada, o programa deve imprimir o labirinto atualizado:
  - '.': posições já exploradas
  - 'o': posição corrente

### Condições de Término
O programa deve terminar quando:
- A saída for encontrada
- Não existirem mais posições a serem exploradas

### Compilação e execução
O repositório possui o código principal (maze_runner.cpp) e uma pasta (data), onde são armazenados todos os labirintos de interesse.

Para compilar o código digite no terminal:
- g++ maze_runner.cpp -o maze_runner
  
Para executar o código digite no terminal o seguinte comando e mudando o "maze" para o nome do labirinto que você deseja rodar:
- ./maze_runner data/maze.txt


### Funcionalidades Adicionais 

#### (Exercício 2)

- Durante a exploração do labirinto, se forem encontrados múltiplos caminhos possíveis:
  - A thread atual continua explorando um dos caminhos.
  - Threads adicionais são criadas para explorar os outros caminhos simultaneamente.
- O acesso ao labirinto compartilhado entre threads é protegido por um **mutex** para evitar condições de corrida.
- O programa utiliza **multithreading** para explorar o labirinto de forma eficiente.