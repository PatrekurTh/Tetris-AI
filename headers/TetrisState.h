#ifndef AGENTS_TETRISSTATE_H
#define AGENTS_TETRISSTATE_H

#include <iostream>
#include <utility>
#include <vector>
#include <numeric>
#include <algorithm>
#include "Constants.h"



struct TetrisState {
    std::vector<std::vector<int>> board;
    char currentPiece;
    char holdPiece;
    std::vector<char> queue;
    bool holdUsed;
    int linesCleared {0};
    std::vector<Action> actions;

    explicit TetrisState(
            std::vector<std::vector<int>> board,
            char currentPiece,
            char holdPiece,
            std::vector<char> queue,
            bool holdUsed
            ) : board(std::move(board)), currentPiece(currentPiece), holdPiece(holdPiece), queue(std::move(queue)), holdUsed(holdUsed) {};

    void clearLines() {
        for (int i = 0; i < 20; i++) {
            if (std::all_of(board[i].begin(), board[i].end(), [](int x) { return x == 1; })) {
                std::fill(board[i].begin(), board[i].end(), 0);
                std::rotate(board.begin(), board.begin() + i, board.begin() + i + 1);
                linesCleared++;
            }
        }
    };

    void hold() {
        if (holdPiece == 'X') {
            if (queue.empty()) {
                return;
            }
            holdPiece = currentPiece;
            currentPiece = queue.back();
            queue.pop_back();
        } else {
            char temp = holdPiece;
            holdPiece = currentPiece;
            currentPiece = temp;
        }
        holdUsed = true;
    };

    void move(std::vector<std::vector<int>> piece, int moveCounter) {
        size_t piece_width = piece[0].size();
        int padding = 3;
        if (piece_width == 2)
            padding = 4;

        // sparsity
        for (auto it = piece.begin(); it != piece.end();)
        {
            int sum = std::accumulate(it->begin(), it->end(), 0);
            if (sum == 0)
            {
                it = piece.erase(it);
            }
            else
            {
                ++it;
            }
        }

        size_t s = piece.size();

        // move down and find collision
        int down_counter = 0;
        bool collision = false;
        while (!collision)
        {
            if (down_counter > 20 - s)
            {
                collision = true;
                down_counter--;
                break;
            }
            for (int i = 0; i < s; i++) // All the lines the piece takes vertically
            {
                auto board_row = board[i + down_counter];
                for (int j = 0; j < piece_width; j++) {
                    if (piece[i][j] + board_row[padding + j + moveCounter] == 2) {
                        collision = true;
                        down_counter--;
                        break;
                    }
                }
            }
            if (!collision) {
                down_counter++;
            }

        }

        for (int row = 0; row < s; row++)
        {
            for (int col = 0; col < piece_width; col++)
            {
                board[std::max(0, row + down_counter)][col + padding + moveCounter] += piece[row][col];
            }
        }

        if (!queue.empty()) {
            currentPiece = queue.back();
            queue.pop_back();
        }

        clearLines();
    };

    bool isTerminal() const {
        return linesCleared == 4 || queue.empty();
    }


    friend std::ostream &operator<<(std::ostream &os, const TetrisState &s)
    {
        os << "Current Piece: " << s.currentPiece << std::endl;
        os << "Hold Piece: " << s.holdPiece << std::endl;
        os << "Queue: ";
        for (auto it = s.queue.rbegin(); it != s.queue.rend(); ++it)
        {
             os << *it << " ";
        }
        os << std::endl;
        os << "Lines Cleared: " << s.linesCleared << std::endl;
        os << "Moves: " << std::endl;
        for (auto &action : s.actions)
        {
            os << static_cast<int>(action) << " ";
        }
        os << std::endl;
        os << "Board: " << std::endl;
        for (auto &row : s.board)
        {
            for (auto &col : row)
            {
                os << col << " ";
            }
            os << std::endl;
        }
        return os;
    }
};


#endif //AGENTS_TETRISSTATE_H
