#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <conio.h> // Para capturar as teclas de seta no Windows

const int WIDTH = 10;
const int HEIGHT = 20;
int grid[WIDTH][HEIGHT] = {0};

const int PIECES[7][4][4] = {
    // Linha
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // L
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // L invertido
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    },
    // T
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Quadrado
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // S
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // S invertido
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }
};

struct Piece {
    int type;
    int rotation;
    int x;
    int y;
};

Piece currentPiece;
Piece nextPiece;

int score = 0;
bool gameOver = false;

void drawGrid() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << (grid[x][y] == 0 ? '.' : '#') << " ";
        }
        std::cout << std::endl;
    }
}

void drawPiece(Piece piece, bool draw) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (PIECES[piece.type][piece.rotation][i * 4 + j] != 0) {
                int posX = piece.x + j;
                int posY = piece.y + i;
                if (posX >= 0 && posX < WIDTH && posY >= 0 && posY < HEIGHT) {
                    grid[posX][posY] = draw ? 1 : 0;
                }
            }
        }
    }
}

bool checkCollision(int newX, int newY, int newRotation) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (PIECES[currentPiece.type][newRotation][i * 4 + j] != 0) {
                int posX = newX + j;
                int posY = newY + i;
                if (posX < 0 || posX >= WIDTH || posY >= HEIGHT || (posY >= 0 && grid[posX][posY] != 0)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void movePiece(int deltaX, int deltaY, int deltaRotation) {
    drawPiece(currentPiece, false);
    if (!checkCollision(currentPiece.x + deltaX, currentPiece.y + deltaY, (currentPiece.rotation + deltaRotation) % 4)) {
        currentPiece.x += deltaX;
        currentPiece.y += deltaY;
        currentPiece.rotation = (currentPiece.rotation + deltaRotation) % 4;
    }
    drawPiece(currentPiece, true);
}

bool isGameOver() {
    return checkCollision(currentPiece.x, currentPiece.y + 1, currentPiece.rotation);
}

void fixPiece() {
    drawPiece(currentPiece, false);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (PIECES[currentPiece.type][currentPiece.rotation][i * 4 + j] != 0) {
                int posX = currentPiece.x + j;
                int posY = currentPiece.y + i;
                if (posX >= 0 && posX < WIDTH && posY >= 0 && posY < HEIGHT) {
                    grid[posX][posY] = 1;
                }
            }
        }
    }
}

void spawnPiece() {
    currentPiece = nextPiece;
    nextPiece.type = rand() % 7;
    nextPiece.rotation = 0;
    nextPiece.x = WIDTH / 2 - 2;
    nextPiece.y = 0;
}

void checkLines() {
    for (int y = 0; y < HEIGHT; ++y) {
        bool lineComplete = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[x][y] == 0) {
                lineComplete = false;
                break;
            }
        }
        if (lineComplete) {
            for (int yy = y; yy > 0; --yy) {
                for (int x = 0; x < WIDTH; ++x) {
                    grid[x][yy] = grid[x][yy - 1];
                }
            }
            for (int x = 0; x < WIDTH; ++x) {
                grid[x][0] = 0;
            }
            score += 100;
        }
    }
}

void displayNextPiece() {
    std::cout << "Next piece:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << (PIECES[nextPiece.type][nextPiece.rotation][i * 4 + j] != 0 ? '#' : '.') << " ";
        }
        std::cout << std::endl;
    }
}

void handleInput() {
    if (_kbhit()) { // Verifica se uma tecla foi pressionada
        char key = _getch(); // Captura a tecla pressionada
        switch(key) {
            case 'a': // Movimento para a esquerda
                movePiece(-1, 0, 0);
                break;
            case 'd': // Movimento para a direita
                movePiece(1, 0, 0);
                break;
            case 's': // Movimento para baixo
                movePiece(0, 1, 0);
                break;
            case ' ': // Rotação
                movePiece(0, 0, 1);
                break;
            default:
                break;
        }
    }
}

int main() {
    nextPiece.type = rand() % 7;
    spawnPiece();

    while (!gameOver) {
        drawGrid();
        drawPiece(currentPiece, true);
        displayNextPiece();

        handleInput(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (checkCollision(currentPiece.x, currentPiece.y + 1, currentPiece.rotation)) {
            fixPiece();
            checkLines();
            spawnPiece();
            if (isGameOver()) {
                gameOver = true;
            }
        } else {
            movePiece(0, 1, 0);
        }
    }

    std::cout << "Game Over! Score: " << score << std::endl;

    return 0;
}
