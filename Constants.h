#ifndef AGENTS_CONSTANTS_H
#define AGENTS_CONSTANTS_H

const std::vector<std::vector<std::vector<int>>> I_ROTATIONS = {
        {{1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}},
        {{0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}},
        {{0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}},
        {{0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}},
};

const std::vector<std::vector<std::vector<int>>> J_ROTATIONS = {
        {{1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}},
        {{0, 1, 1},
                {0, 1, 0},
                {0, 1, 0}},
        {{0, 0, 0},
                {1, 1, 1},
                {0, 0, 1}},
        {{0, 1, 0},
                {0, 1, 0},
                {1, 1, 0}},
};

const std::vector<std::vector<std::vector<int>>> L_ROTATIONS = {
        {{0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}},
        {{0, 1, 0},
                {0, 1, 0},
                {0, 1, 1}},
        {{0, 0, 0},
                {1, 1, 1},
                {1, 0, 0}},
        {{1, 1, 0},
                {0, 1, 0},
                {0, 1, 0}},
};

const std::vector<std::vector<std::vector<int>>> O_ROTATIONS = {
        {{1, 1},
         {1, 1}},
};

const std::vector<std::vector<std::vector<int>>> S_ROTATIONS = {
        {{0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}},
        {{0, 1, 0},
                {0, 1, 1},
                {0, 0, 1}},
        {{0, 0, 0},
                {0, 1, 1},
                {1, 1, 0}},
        {{1, 0, 0},
                {1, 1, 0},
                {0, 1, 0}},
};

const std::vector<std::vector<std::vector<int>>> T_ROTATIONS = {
        {{0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}},
        {{0, 1, 0},
                {0, 1, 1},
                {0, 1, 0}},
        {{0, 0, 0},
                {1, 1, 1},
                {0, 1, 0}},
        {{0, 1, 0},
                {1, 1, 0},
                {0, 1, 0}},
};

const std::vector<std::vector<std::vector<int>>> Z_ROTATIONS = {
        {{1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}},
        {{0, 0, 1},
                {0, 1, 1},
                {0, 1, 0}},
        {{0, 0, 0},
                {1, 1, 0},
                {0, 1, 1}},
        {{0, 1, 0},
                {1, 1, 0},
                {1, 0, 0}},
};


enum class Action {
    LEFT,
    RIGHT,
    CW,     // Clockwise
    CCW,    // Counter ClockWise
    HOLD,
    NONE
};


#endif //AGENTS_CONSTANTS_H
