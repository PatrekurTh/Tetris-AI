#include "MCTSAgent.h"

#include <random>

std::vector<Action> MCTSAgent::getBestAction(const TetrisState &state) {
    TetrisState tempState = state;
    
    std::vector<char> randomizedBag = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};
    std::mt19937 rng(std::random_device{}());
    std::shuffle(randomizedBag.begin(), randomizedBag.end(), rng);
    tempState.queue.insert(tempState.queue.begin(), randomizedBag.begin(), randomizedBag.end());
    
    const int NUM_ITERATIONS = 500;
    Node* root = new Node(state, nullptr);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        Node* selected = selection(root);
        Node* child = expansion(selected);
        double result = simulation(child);
        backpropagation(child, result);
    }

    for (const auto& s : env.getSuccessorStates(state)) {
        auto child = std::unique_ptr<Node>(new Node(s, root));
        child->parent = root;
        root->children.push_back(std::move(child));
    }

    std::vector<Action> bestActions;
    double bestValue = -std::numeric_limits<double>::max();
    for (const auto& child : root->children) {
        if (child->visits == 0) continue;
        double avg = child->wins / static_cast<double>(child->visits);
        if (avg > bestValue) {
            bestValue = avg;
            bestActions = child->state.actions;
        }
    }

    delete root;
    return bestActions;
}

Node *MCTSAgent::selection(Node *node) {
    while(!node->children.empty()) {
        Node* bestChild = nullptr;
        double bestValue = -std::numeric_limits<double>::max();

        for (const auto& child : node->children) {
            double ucbValue = UCB1(child.get());
            if (ucbValue > bestValue) {
                bestValue = ucbValue;
                bestChild = child.get();
            }
        }
        node = bestChild;
    }
    return node;
}

Node* MCTSAgent::expansion(Node *node) {
    if (node->visits == 0) return node;
    
    auto successors = env.getSuccessorStates(node->state);
    for (const auto& s : successors) {
        auto child = std::unique_ptr<Node>(new Node(s, node));
        child->parent = node;
        node->children.push_back(std::move(child));
    }
    return node->children.front().get();
}

double MCTSAgent::simulation(Node *node) {
    TetrisState tempState = node->state;
    while (!tempState.isTerminal()) {
        auto successors = env.getSuccessorStates(tempState);

        std::mt19937 rng(std::random_device{}());
        tempState = successors[std::uniform_int_distribution<int>(0, successors.size() - 1)(rng)];
    }
    return env.evaluateState(tempState);
}

void MCTSAgent::backpropagation(Node *node, double result) {
    while (node != nullptr) { 
        node->visits++;
        node->wins += result;
        node->sumSquare += result * result;
        node = node->parent;
    }
}

double MCTSAgent::UCB1(Node *node) {
    if (node->visits == 0) return std::numeric_limits<double>::max(); // Handle division by zero

    double Xbar = node->wins / static_cast<double>(node->visits);
    double xSquared = node->sumSquare;
    double D = 10000.0F;
    double C = 0.5F;
    double deviation = std::sqrt((xSquared - node->visits * std::pow(Xbar, 2) + D) / node->visits);

    return Xbar +
              C * std::sqrt(std::log(node->parent->visits) / node->visits) +
                deviation;

}
