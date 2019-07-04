// aiswitchboard.h

#ifndef AI_SWITCHBOARD_H
#define AI_SWITCHBOARD_H

#include <string>
#include <vector>
#include <array>

#include "checkers.h"
#include "simpleais.h"

namespace AISwitchboard {

const std::vector<std::string> getListOfAllAIs();
const std::vector<std::array<std::string,2>> getListOfAllAIsByCategory();

Checkers::Move getMoveFromAI(const std::string selectedAI, const Checkers::GameState &thegamestate, const std::vector<std::string> aiparameters);

}

#endif
