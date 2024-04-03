#ifndef AGENTS_ENVIRONMENT_H
#define AGENTS_ENVIRONMENT_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "TetrisState.h"
#include "IAgent.h"
#include "Constants.h"

class Environment {
public:
    Environment();
    std::vector<TetrisState> getSuccessorStates(const TetrisState& state);
    TetrisState applyAction(const TetrisState& state, const std::vector<Action>& actions);
    static double evaluateState(const TetrisState& state);
private:
    std::map<char, std::vector<std::vector<Action>>> pieceMoves;
    std::map<char, std::vector<std::vector<std::vector<int> > > > pieceRotations;
};

#endif //AGENTS_ENVIRONMENT_H
