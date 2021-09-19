#include <rg/ProgramState.hpp>

namespace rg {
    void ProgramState::saveToDisk(std::string path) const {
//        std::ofstream out(path);
//        out << imGuiEnabled << '\n'
//            << clearColor << '\n'
//            << camera.position << '\n';
    }

    ProgramState *ProgramState::loadFromDisk(const std::string &path) {
        auto *programState = new ProgramState();
//        std::ifstream in(path);
//        if (in) {
//            in >> programState->imGuiEnabled >> programState->clearColor >> programState->camera.position;
//        }

        return programState;
    }
}