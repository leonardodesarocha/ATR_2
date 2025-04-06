/*
Exercício computacional 2
Link Repositório:
https://github.com/leonardodesarocha/ATR_2
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
std::mutex maze_mutex; // Mutex para sincronizar o acesso ao labirinto

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);
    file >> num_rows >> num_cols;

    if (!file || file.fail() || num_rows <= 0 || num_cols <= 0) {
        std::cerr << "Erro: Arquivo não aberto ou formato inválido: " << file_name << std::endl;
        return {-1, -1};
    }

    maze.resize(num_rows, std::vector<char>(num_cols));
    int posX = -1, posY = -1;

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            file >> maze[i][j];
            if (maze[i][j] == 'e') {
                posX = i;
                posY = j;
            }
        }
    }

    file.close();
    return {posX, posY};
}

// Função para imprimir o labirinto
void print_maze(int row, int col) {
    std::lock_guard<std::mutex> lock(maze_mutex); // Protege o acesso ao labirinto
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (i == row && j == col) {
                std::cout << "o";
            } else {
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
    return row >= 0 && row < num_rows && col >= 0 && col < num_cols &&
           (maze[row][col] == 'x' || maze[row][col] == 's');
}

// Função principal para navegar pelo labirinto
void walk(Position pos) {
    {
        std::lock_guard<std::mutex> lock(maze_mutex);
        maze[pos.row][pos.col] = '.';
    }

    print_maze(pos.row, pos.col);
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    system("clear");

    std::vector<Position> valid_moves;
    if (is_valid_position(pos.row + 1, pos.col))
        valid_moves.push_back({pos.row + 1, pos.col});
    if (is_valid_position(pos.row - 1, pos.col))
        valid_moves.push_back({pos.row - 1, pos.col});
    if (is_valid_position(pos.row, pos.col + 1))
        valid_moves.push_back({pos.row, pos.col + 1});
    if (is_valid_position(pos.row, pos.col - 1))
        valid_moves.push_back({pos.row, pos.col - 1});

    std::vector<std::thread> threads;
    for (size_t i = 1; i < valid_moves.size(); ++i) {
        threads.emplace_back(walk, valid_moves[i]);
    }

    if (!valid_moves.empty()) {
        walk(valid_moves[0]);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " data/maze.txt" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);

    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    print_maze(initial_pos.row, initial_pos.col);
    walk(initial_pos);

    std::cout << "A saída foi encontrada e todos os caminhos possíveis foram explorados." << std::endl;

    return 0;
}