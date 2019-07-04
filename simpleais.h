// simpleais.h

#ifndef SIMPLE_AIS_H
#define SIMPLE_AIS_H

#include <vector>
#include <string>
#include <random>

#include "checkers.h"

namespace SimpleAIs
{

Checkers::Move getRandomAIMove(const Checkers::GameState &thegamestate);

Checkers::Move getSimpleEvalSinglePlyAIMove(const Checkers::GameState &thegamestate);

// note: the order of these two vectors must match!
// it is assumed that any one index will refer to the same AI in both lists
const std::vector<std::string> simpleAIsNameList = {"random-ai", "simple-ai"};
const std::vector<Checkers::Move (*)(const Checkers::GameState &thegamestate)> simpleAIsFuncList = {&getRandomAIMove, &getSimpleEvalSinglePlyAIMove};

}

#endif
