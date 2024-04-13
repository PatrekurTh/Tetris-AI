#ifndef AGENTS_MCTSAGENT_H
#define AGENTS_MCTSAGENT_H

#include "Environment.h"

struct Node {
    TetrisState state;
    std::vector<std::unique_ptr<Node>> children;
    Node* parent;
    int visits{0};
    double wins{0};
    double sumSquare{0};

    Node(TetrisState state, Node* parent) : state(std::move(state)), parent(parent) {};
};

class MCTSAgent : public IAgent {
public:
    std::vector<Action> getBestAction(const TetrisState& state) override;
private:
    Environment env = Environment();
    Node* selection(Node* node);
    Node* expansion(Node* node);
    double simulation(Node* node);
    void backpropagation(Node* node, double result);
    double UCB1(Node* node);
};


#endif //AGENTS_MCTSAGENT_H
