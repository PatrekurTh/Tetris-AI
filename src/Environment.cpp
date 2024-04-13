#include "Environment.h"

Environment::Environment() {
    // Parse piece moves
    std::ifstream file("utils/PieceMoves.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open Piece Moves" << std::endl;
        exit(1);
    }
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

    int wellScore {0}; // Initialize well score

    for (int i = 1; i < peaks.size() - 3; i++) { // Start from the second column and go to the second last column
        // Check for a well by comparing the current column with its neighbors
        if (peaks[i] < peaks[i - 1] && peaks[i] < peaks[i + 1]) {
            int leftDiff = peaks[i - 1] - peaks[i];
            int rightDiff = peaks[i + 1] - peaks[i];

            // Assess the depth of the well - deeper wells might be scored higher
            if (leftDiff >= 2 && rightDiff >= 1) { // Check if both sides are at least 2 units higher
                wellScore += std::max(leftDiff, rightDiff); // Score based on the smaller of the two heights
            }
        }
    }
    if (peaks[0] < peaks[1]) {
        if (peaks[1] - peaks[0] >= 2) {
            wellScore += peaks[1] - peaks[0];
        }
    }

    for (int i = 0; i < peaks.size() - 2; i++) {
        int diff = abs(peaks[i] - peaks[i + 1]);
        if (diff > 2) {
            bumpiness += diff * 2;
        } else {
            bumpiness += diff;
        }
        // bumpiness += (abs(peaks[i] - peaks[i + 1]) * 2);
    }

    // const double a = 0.35; // Agg Height
    // const double b = 1.1; // Lines Cleared
    // const double c = 10.0; // Holes
    // const double d = 1.5; // Bumpiness

    const double a = 0.90066;
    const double b = 0.720666;
    const double c = 0.72663;
    const double d = 0.18483;
    const double e = 0.218034;

    double scores[5] = {0, -15, -10, -5, 9999};

    // score = -a * aggHeight + b * scores[state.linesCleared] - c * holes - d * bumpiness;
    score = -a * aggHeight + b * scores[state.linesCleared] - c * holes - d * bumpiness - e * wellScore;
    // int lastTwo = peaks[peaks.size() - 2] - peaks[peaks.size() - 1];
    // if (lastTwo >= 4) {
    //     score += 0.05F;
    // }
    score -= peaks[9] * 0.5;
    score -= std::any_of(peaks.begin(), peaks.end(), [](int peak) { return peak >= 18; }) ? 1000.0F : 0;
    return score;
}
