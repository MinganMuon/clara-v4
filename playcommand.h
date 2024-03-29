// playcommand.h

#ifndef PLAY_COMMAND_H
#define PLAY_COMMAND_H

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "checkers.h"
#include "aiswitchboard.h"

namespace PlayCommand
{

void printPlayCommandHelp(std::vector<std::string> cmdlineoptions);

void printboard(std::vector<Checkers::Piece> piecesonboard);

void play(std::vector<std::string> cmdlineoptions);

}

#endif
