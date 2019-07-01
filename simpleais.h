// simpleais.h

#include <vector>
#include <string>
#include <random>

#include "checkers.h"

namespace SimpleAIs
{

const std::vector<std::string> simpleAIsList = {"Random AI", "Simple Eval Single-ply AI"};

Checkers::Move getRandomAIMove(const Checkers::GameState &thegamestate);

Checkers::Move getSimpleEvalSinglePlyAIMove(const Checkers::GameState &thegamestate);

}
