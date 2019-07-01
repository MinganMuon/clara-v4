// main.cpp

#include <iostream>
#include <string>
#include <vector>

#include "playcommand.h"

void printTopLevelHelp()
{
    std::cout << "clara-v4 top level help\n";
    std::cout << "-----------------------\n\n";

    std::cout << "usage: clara <command> <arguments>\n\n";

    std::cout << "<command> can be one of the following:\n";
    std::cout << "help: prints this help when given no arguments or prints help for a command given as an argument\n";
    std::cout << "play: plays a game of checkers. see the help for the play command for details.\n";
}

void handleHelpCommand(std::vector<std::string> cmdlineoptions)
{
    if (cmdlineoptions.empty())
    {
        printTopLevelHelp();
    }
    else if (cmdlineoptions[0] == "play")
    {
        cmdlineoptions.erase(cmdlineoptions.begin());
        PlayCommand::printPlayCommandHelp(cmdlineoptions);
    }
    else
    {
        printTopLevelHelp();
    }
}

int main(int argc, char **argv)
{
    //std::cout << "clara-v4 - plays checkers\n";
    //std::cout << "-------------------------\n\n";

    if (argc < 2)
    {
        printTopLevelHelp();
        return 0;
    }

    // grab the top level command and any other command line options given
    std::string toplevelcommand = std::string(argv[1]);
    std::vector<std::string> cmdlineoptions;
    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            cmdlineoptions.push_back(std::string(argv[i]));
        }
    }

    // switch through the top level commands
    if (toplevelcommand == "help")
    {
        handleHelpCommand(cmdlineoptions);
    }
    else if (toplevelcommand == "play")
    {
        PlayCommand::play(cmdlineoptions);
    }
    else
    {
        printTopLevelHelp();
    }

    return 0;
}
