#ifndef AGENTS_NAGENT_H
#define AGENTS_NAGENT_H

#include "IAgent.h"
#include "Environment.h"

class NAgent : public IAgent {
public:
    NAgent(int n);
    std::vector<Action> getBestAction(const TetrisState& state) override;
private:
    int n;
    Environment env = Environment();
    int bestAction(const TetrisState& state, int depth);
};


#endif //AGENTS_NAGENT_H
