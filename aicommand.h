// aicommand.h

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "aiswitchboard.h"

namespace AICommand
{

void printAICommandHelp(std::vector<std::string> cmdlineoptions);

void listPlayableAIs();

void ai(std::vector<std::string> cmdlineoptions);

}
