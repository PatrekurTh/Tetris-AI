#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "TetrisState.h"
#include "IAgent.h"
#include "Constants.h"
#include "MCTSAgent.h"
#include "NAgent.h"

namespace py = pybind11;

std::vector<std::vector<int>> numpyArrayToBoard(py::array_t<int> arr) {
    py::buffer_info buf = arr.request();
    auto *ptr = static_cast<int *>(buf.ptr);
    int height = buf.shape[0];
    int width = buf.shape[1];
    std::vector<std::vector<int>> board(height, std::vector<int>(width, 0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            board[i][j] = ptr[i * width + j];
        }
    }
    return board;
}

std::vector<Action> callAgentGetBestMove(IAgent& agent, py::array_t<int> arr, char current_piece, char hold_piece, std::vector<char> queue, bool hold_used) {
    std::vector<std::vector<int>> board = numpyArrayToBoard(arr);
    TetrisState state(board, current_piece, hold_piece, queue, hold_used);
    return agent.getBestAction(state);
}

PYBIND11_MODULE(agents, m) {
    m.def("call_agent_get_best_move", &callAgentGetBestMove, "Call the agent's getBestMove method", py::arg("agent"), py::arg("board"), py::arg("current_piece"), py::arg("hold_piece"), py::arg("queue"), py::arg("hold_used"));

    py::class_<IAgent>(m, "IAgent")
    .def("getBestAction", &IAgent::getBestAction);

    py::class_<MCTSAgent, IAgent>(m, "MCTSAgent")
    .def(py::init<>());

    py::class_<NAgent, IAgent>(m, "NAgent")
    .def(py::init<int>());

    py::enum_<Action>(m, "Action")
        .value("LEFT", Action::LEFT)
        .value("RIGHT", Action::RIGHT)
        .value("CW", Action::CW)
        .value("CCW", Action::CCW)
        .value("HOLD", Action::HOLD)
        .value("NONE", Action::NONE)
        .export_values();
}