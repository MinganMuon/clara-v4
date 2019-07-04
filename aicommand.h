// aicommand.h

#include <iostream>
#include <string>
#include <vector>

#include "simpleais.h"

namespace AICommand
{

void printAICommandHelp(std::vector<std::string> cmdlineoptions);

void listPlayableAIs();

void ai(std::vector<std::string> cmdlineoptions);

}
