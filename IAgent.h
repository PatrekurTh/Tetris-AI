#ifndef AGENTS_IAGENT_H
#define AGENTS_IAGENT_H

#include <vector>
#include "Constants.h"
#include "TetrisState.h"

class IAgent {
public:
    virtual ~IAgent() = default;
    virtual std::vector<Action> getBestAction(const TetrisState& state) = 0;
};


#endif //AGENTS_IAGENT_H
