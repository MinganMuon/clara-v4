// playcommand.cpp

#include "playcommand.h"

namespace PlayCommand
{

void printPlayCommandHelp(std::vector<std::string> cmdlineoptions)
{
    std::cout << "clara-v4 help for the play command\n";
    std::cout << "----------------------------------\n\n";

    std::cout << "usage: clara play\n\n";

    std::cout << "right now, starts a game pitting the user against himself\n";
}

}
