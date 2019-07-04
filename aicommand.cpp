// aicommand.cpp

#include "aicommand.h"

namespace AICommand
{

void printAICommandHelp(std::vector<std::string> cmdlineoptions)
{
    std::cout << "clara-v4 help for the ai command\n";
    std::cout << "--------------------------------\n\n";

    std::cout << "usage: clara ai <command> <arguments>\n\n";
    
    std::cout << "<command> can be one of the following:\n";
    std::cout << "list-playable: lists all AIs playable via clara play <ai>\n";
}

void listPlayableAIs()
{
    std::string simpleailist = "";
    for (std::string s : SimpleAIs::simpleAIsNameList)
        simpleailist = simpleailist + s + ", ";
    simpleailist.erase(simpleailist.end() - 2);
    std::cout << "playable ais from category \"simple AIs\": " + simpleailist;
}

void ai(std::vector<std::string> cmdlineoptions)
{
    if (cmdlineoptions.empty())
    {
        printAICommandHelp(cmdlineoptions);
    }
    else if (cmdlineoptions[0] == "list-playable")
    {
        listPlayableAIs();
    }
    else
    {
        printAICommandHelp(cmdlineoptions);
    }
}

}
