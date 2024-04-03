#include "NAgent.h"

NAgent::NAgent(int n) : n(n) {}

std::vector<Action> NAgent::getBestAction(const TetrisState &state) {
    std::vector<TetrisState> states = env.getSuccessorStates(state);
    std::vector<Action> bestActions;
    int bestScore = INT_MIN;

    for (const TetrisState& s : states) {
        int score = bestAction(s, n - 1);
        if (score > bestScore) {
            bestScore = score;
            bestActions = s.actions;
        }
    }

    return bestActions;
}

int NAgent::bestAction(const TetrisState &state, int depth) {
    if (depth == 0 || state.isTerminal()) {
        return env.evaluateState(state);
    }

    std::vector<TetrisState> states = env.getSuccessorStates(state);
    int bestScore = INT_MIN;

    for (const TetrisState& s : states) {
        int score = bestAction(s, depth - 1);
        if (score > bestScore) {
            bestScore = score;
        }
    }

    return bestScore;
}
