// simpleais.h

#include <vector>
#include <string>
#include <random>

#include "checkers.h"

namespace SimpleAIs
{

const std::vector<std::string> simpleAIsList = {"random-ai", "simple-ai"};

Checkers::Move getRandomAIMove(const Checkers::GameState &thegamestate);

Checkers::Move getSimpleEvalSinglePlyAIMove(const Checkers::GameState &thegamestate);

}
