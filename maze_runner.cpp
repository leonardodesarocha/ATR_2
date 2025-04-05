/*
Exercício computacional 1
Link Repositório:
https://github.com/leonardodesarocha/ATR_1_maze_runner
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <mutex>
#include <stdlib.h>

// Tempo entre telas
#define TIME 100

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
//std::stack<Position> valid_positions;
std::mutex maze_mutex;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string file_name) {
    // 1. Abra o arquivo especificado por file_name usando std::ifstream
    std::ifstream file(file_name);
    
    // 2. Leia o número de linhas e colunas do labirinto
    file >> num_rows >> num_cols;

    // 6. Trate possíveis erros (arquivo não encontrado, formato inválido, etc.)
    if (!file) {
        std::cerr << "Erro: Arquivo não aberto." << file_name << std::endl;
        return {-1, -1}; // Indica erro na abertura do arquivo
    }

    if (file.fail() || num_rows <= 0 || num_cols <= 0) {
        std::cerr << "Erro: Arquivo com formato inválido." << std::endl;
        return {-1, -1};
    }

    // 3. Redimensione a matriz 'maze' de acordo (use maze.resize())
    maze.resize(num_rows, std::vector<char>(num_cols));

    // 4. Leia o conteúdo do labirinto do arquivo, caractere por caractere
    // 5. Encontre e retorne a posição inicial ('e')
    int posX = -1;
    int posY = -1;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            if (maze[i][j] == 'e') {
                posX = i;
                posY = j;
            }
        }
    }
    
    // 7. Feche o arquivo após a leitura
    file.close();

    return {posX, posY};
}

// Função para imprimir o labirinto
void print_maze1(int row, int col) {
    // 1. Percorra a matriz 'maze' usando um loop aninhado
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto
    // 2. Imprima cada caractere usando std::cout
    std::lock_guard<std::mutex> lock(maze_mutex);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (i == row && j == col) {
                std::cout << "o";
            }
            else
                std::cout << maze[i][j];
        }
        std::cout << '\n';
    }
}

// Função para imprimir o labirinto difetente
void print_maze(int row, int col) {
    // 1. Percorra a matriz 'maze' usando um loop aninhado
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto
    // 2. Imprima cada caractere usando std::cout
    std::lock_guard<std::mutex> lock(maze_mutex);
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (i == row && j == col) {
                std::cout << "o";
            }
            else {
                if (maze[i][j] == '#')
                    std::cout << "▓";
                else if (maze[i][j] == 'x')
                    std::cout << "▒";
                else if (maze[i][j] == '.')
                    std::cout << " ";
                else
                    std::cout << maze[i][j];
            }
        }
        std::cout << '\n';
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // 1. Verifique se a posição está dentro dos limites do labirinto
    //    (row >= 0 && row < num_rows && col >= 0 && col < num_cols)
    // 2. Verifique se a posição é um caminho válido (maze[row][col] == 'x')
    // 3. Retorne true se ambas as condições forem verdadeiras, false caso contrário
    if (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's'))
        return true;

    
    return false;
}

// Função principal para navegar pelo labirinto
void walk(Position pos) {
    // 1. Marque a posição atual como visitada (maze[pos.row][pos.col] = '.')
    {
        std::lock_guard<std::mutex> lock(maze_mutex);
        maze[pos.row][pos.col] = '.';
    }

    // 2. Chame print_maze() para mostrar o estado atual do labirinto
    print_maze(pos.row, pos.col);

    // 3. Adicione um pequeno atraso para visualização:
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    
    

    // 4. Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    //    Para cada posição adjacente:
    //    a. Se for uma posição válida (use is_valid_position()), adicione-a à pilha valid_positions
    // std::vector<Position> valid_moves;
    std::vector<Position> valid_positions;

    if (is_valid_position(pos.row + 1, pos.col))
        valid_positions.push_back({pos.row + 1, pos.col});

    if (is_valid_position(pos.row - 1, pos.col))
        valid_positions.push_back({pos.row - 1, pos.col});

    if (is_valid_position(pos.row, pos.col + 1))
        valid_positions.push_back({pos.row, pos.col + 1});

    if (is_valid_position(pos.row, pos.col - 1))
        valid_positions.push_back({pos.row, pos.col - 1});

    // 5. Enquanto houver posições válidas na pilha (!valid_positions.empty()):
    //    a. Remova a próxima posição da pilha (valid_positions.top() e valid_positions.pop())
    //    b. Chame walk recursivamente para esta posição

    if (!valid_positions.empty()) {
        std::system("clear");
    }

    std::vector<std::thread> threads;
    for (size_t i = 1; i < valid_positions.size(); ++i) {
        threads.emplace_back(walk, valid_positions[i]);
    }

    if (!valid_positions.empty()) {
        std::system("clear");
        walk(valid_positions[0]);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main(int argc, char* argv[]) {

    /*
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " data/maze.txt" << std::endl;
        return 1;
    }
    */

    Position initial_pos = load_maze("data/maze4.txt");

    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    print_maze(initial_pos.row, initial_pos.col);

    walk(initial_pos);

    std::cout << "A saída foi encontrada e todos os caminhos possíveis foram explorados." << std::endl;

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.