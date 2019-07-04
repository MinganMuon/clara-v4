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
    std::vector<std::string> categorytexts;
    std::vector<std::string> catindexes;
    for (std::array<std::string,2> a : AISwitchboard::getListOfAllAIsByCategory())
    {
        auto catiter = std::find(catindexes.begin(),catindexes.end(),a[0]);
        if (catiter == catindexes.end())
        {
            categorytexts.push_back("playable ais in category " + a[0] + ": " + a[1] + ", ");
            catindexes.push_back(a[0]);
        }
        else
        {
            categorytexts[catiter - catindexes.begin()] += a[1] + ", ";
        }
    }
    for (std::string s : categorytexts)
    {
        s.erase(s.end()-2);
        std::cout << s + "\n";
    }
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
