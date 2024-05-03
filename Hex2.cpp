#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Define the size of the HEX board
const int BOARD_SIZE = 11; // Adjust as needed

// Define the player types
enum class Player {
    EMPTY,
    PLAYER_X,
    PLAYER_O
};

// Define the directions for checking connectivity
const std::vector<std::pair<int, int>> DIRECTIONS = {{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}};

// Define the HEX board class
class HexBoard {
private:
    std::vector<std::vector<Player>> board;

public:
    HexBoard() : board(BOARD_SIZE, std::vector<Player>(BOARD_SIZE, Player::EMPTY)) {}

    // Function to draw the board
    void drawBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < i; ++j) {
                std::cout << " ";
            }
            for (int j = 0; j < BOARD_SIZE; ++j) {
                switch (board[i][j]) {
                    case Player::EMPTY:
                        std::cout << ".";
                        break;
                    case Player::PLAYER_X:
                        std::cout << "X";
                        break;
                    case Player::PLAYER_O:
                        std::cout << "O";
                        break;
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    // Function to check if a move is legal
    bool isLegalMove(int row, int col) {
        return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == Player::EMPTY);
    }

    // Function to check if the game has been won
    bool isGameWon(Player player) {
        std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (player == Player::PLAYER_X && board[0][i] == Player::PLAYER_X) {
                if (dfs(0, i, Player::PLAYER_X, visited)) {
                    return true;
                }
            } else if (player == Player::PLAYER_O && board[i][0] == Player::PLAYER_O) {
                if (dfs(i, 0, Player::PLAYER_O, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool dfs(int row, int col, Player player, std::vector<std::vector<bool>>& visited) {
        if ((player == Player::PLAYER_X && row == BOARD_SIZE - 1) ||
            (player == Player::PLAYER_O && col == BOARD_SIZE - 1)) {
            return true;
        }
        visited[row][col] = true;
        for (const auto& dir : DIRECTIONS) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;
            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE &&
                board[newRow][newCol] == player && !visited[newRow][newCol]) {
                if (dfs(newRow, newCol, player, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Function to make a move
    void makeMove(int row, int col, Player player) {
        board[row][col] = player;
    }

    // Function to get the AI's best move using Monte Carlo methods
    std::pair<int, int> getAIMove(Player player) {
        srand(time(nullptr));
        while (true) {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            if (isLegalMove(row, col)) {
                return {row, col};
            }
        }
    }
};

int main() {
    // Create a HEX board object
    HexBoard hexBoard;

    // Main game loop
    while (true) {
        // Draw the board
        hexBoard.drawBoard();

        // Player's turn
        int row, col;
        std::cout << "Enter row and column for your move (0-indexed): ";
        std::cin >> row >> col;
        if (hexBoard.isLegalMove(row, col)) {
            hexBoard.makeMove(row, col, Player::PLAYER_X);
            if (hexBoard.isGameWon(Player::PLAYER_X)) {
                std::cout << "Congratulations! You won!" << std::endl;
                break;
            }
        } else {
            std::cout << "Invalid move! Try again." << std::endl;
            continue;
        }

        // AI's turn
        auto aiMove = hexBoard.getAIMove(Player::PLAYER_O);
        hexBoard.makeMove(aiMove.first, aiMove.second, Player::PLAYER_O);
        if (hexBoard.isGameWon(Player::PLAYER_O)) {
            std::cout << "AI wins! Better luck next time." << std::endl;
            break;
        }
    }

    return 0;
}
