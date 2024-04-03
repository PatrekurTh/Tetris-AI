#include "Environment.h"

Environment::Environment() {
    // Parse piece moves
    std::ifstream file("PieceMoves.txt");
    std::string line;
    char pieceName;
    int pieceLines, actionCount, action;

    for (int i = 0; i < 7; i++) { // 7 pieces
        file >> pieceName >> pieceLines;
        for (int j = 0; j < pieceLines; j++) {
            file >> actionCount;
            std::vector<Action> actions;
            for (int k = 0; k < actionCount; k++) {
                file >> action;
                actions.push_back(static_cast<Action>(action));
            }
            pieceMoves[pieceName].push_back(actions);
        }
    }

    pieceRotations['I'] = I_ROTATIONS;
    pieceRotations['J'] = J_ROTATIONS;
    pieceRotations['L'] = L_ROTATIONS;
    pieceRotations['O'] = O_ROTATIONS;
    pieceRotations['S'] = S_ROTATIONS;
    pieceRotations['T'] = T_ROTATIONS;
    pieceRotations['Z'] = Z_ROTATIONS;
}

std::vector<TetrisState> Environment::getSuccessorStates(const TetrisState &state) {
    std::vector<TetrisState> states;
    auto actions = pieceMoves[state.currentPiece];
    for (const auto& action : actions) {
        if (state.holdUsed && std::find(action.begin(), action.end(), Action::HOLD) != action.end()) {
            continue;
        }
        TetrisState newState = applyAction(state, action);
        newState.actions = action;
        states.push_back(newState);
    }
    return states;
}

TetrisState Environment::applyAction(const TetrisState &state, const std::vector<Action> &actions) {
    TetrisState newState = state;
    newState.holdUsed = false;

    int move_counter {0};
    int rotation_counter {0};
    for (auto action : actions) {
        if (action == Action::LEFT) {
            move_counter--;
        } else if (action == Action::RIGHT) {
            move_counter++;
        } else if (action == Action::CW) {
            rotation_counter++;
        } else if (action == Action::CCW) {
            rotation_counter--;
        } else if (action == Action::HOLD) {
            newState.hold();
            return newState;
        }
    }

    newState.move(pieceRotations[newState.currentPiece][rotation_counter % pieceRotations[newState.currentPiece].size()], move_counter);
    return newState;
}

double Environment::evaluateState(const TetrisState &state) {
    double score {0};
    int holes {0};
    int aggHeight {0};
    int bumpiness {0};
    std::vector<int> peaks;

    for (int col = 0; col < 10; col++) {
        int peak = 0;
        for (int row = 0; row < 20; row++) {
            if (state.board[row][col]) {
                if ((20 - row) > peak)
                    peak = 20 - row;
            } else {
                if ((20 - row) < peak) {
                    holes += peak - (20 - row);
                }
            }
        }
        aggHeight += peak;
        peaks.push_back(peak);
    }

    for (int i = 0; i < peaks.size() - 2; i++) {
        bumpiness += abs(peaks[i] - peaks[i + 1]);
    }

    const double a = 0.35; // Agg Height
    const double b = 1.1; // Lines Cleared
    const double c = 10.0; // Holes
    const double d = 1.5; // Bumpiness

    double scores[5] = {1, -5, -4, -3, 40};

    score = -a * aggHeight + b * scores[state.linesCleared] - c * holes - d * bumpiness;
    int lastTwo = peaks[peaks.size() - 2] - peaks[peaks.size() - 1];
    if (lastTwo >= 4) {
        score += 3.5F;
    }
    score -= peaks[9] * 2;
    return score;
}
