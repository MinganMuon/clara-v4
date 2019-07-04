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
    std::cout << "list-playable: lists all AIs playable via clara play or clara ai play\n";
    std::cout << "play: plays one ai against another. format:\n"
              << "        clara ai play <ai-white> versus <ai-black> <number-of-games>\n"
              << "      where\n"
              << "        - <ai-white> and <ai-black> are the ais to play white and black respectively and\n"
              << "        - <number-of-games> is the number of games to have the ais play.\n"
              << "      if <number-of-games> is 1, then the full game will be printed upon completion;\n"
              << "      if <number-of-games> is greater than 1, then a summary of wins/losses/percentages/etc. will be printed.\n";
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

void play(std::vector<std::string> cmdlineoptions)
{
    std::vector<std::string> ailist = AISwitchboard::getListOfAllAIs();
    if ((cmdlineoptions.size() != 5)
       || (std::find(ailist.begin(),ailist.end(),cmdlineoptions[1]) == ailist.end())
       || (cmdlineoptions[2] != "versus")
       || (std::find(ailist.begin(),ailist.end(),cmdlineoptions[3]) == ailist.end())
       || (std::stoi(cmdlineoptions[4]) < 1))
    {
        printAICommandHelp(cmdlineoptions);
    }

    int numberofgames = std::stoi(cmdlineoptions[4]);
    std::string aione = cmdlineoptions[1];
    std::string aitwo = cmdlineoptions[3];

    std::cout << "playing " + aione + " against " + aitwo + " for " + std::to_string(numberofgames) + " game" + ((numberofgames == 1) ? "" : "s") + ".\n";

    if (numberofgames == 1)
    {
        Checkers::Game thegame = Checkers::Game();
        bool done = false;
        while (!done)
        {
            Checkers::printboard(thegame.getGameState().piecesonboard);

            bool aioneturn = (thegame.getGameState().playertomove == Checkers::PlayerColor::White);

            std::string colortext = (aioneturn ? "white" : "black");
            std::cout << "\n" + (aioneturn ? aione : aitwo) + " is now playing as " + colortext + ".\n";

            Checkers::Move aimove = AISwitchboard::getMoveFromAI((aioneturn ? aione : aitwo),thegame.getGameState(),std::vector<std::string>());
            if (!thegame.makeMove(aimove))
            {
                std::cout << "error: the ai failed to make a move.\n";
                std::cout << "the problematic move: ";
                std::cout << std::to_string(aimove.startingpiece.pos.x) + " " + std::to_string(aimove.startingpiece.pos.y) + 
                    " to " + std::to_string(aimove.endingpiece.pos.x) + " " + std::to_string(aimove.endingpiece.pos.y);
                if (!aimove.jumpedpieces.empty())
                    std::cout << " jumping the piece(s) at ";
                for (Checkers::Piece p : aimove.jumpedpieces)
                    std::cout << std::to_string(p.pos.x) + " " + std::to_string(p.pos.y) + " ";
                std::cout << "\n\nquitting...\n";
                return;
            }

            if (thegame.determineGameStatus() != Checkers::GameStatus::NotCompleted)
                done = true;
        }

        Checkers::printboard(thegame.getGameState().piecesonboard);

        Checkers::GameStatus gs = thegame.determineGameStatus();
        std::string gstext;
        if (gs == Checkers::GameStatus::BlackWon)
        gstext = aitwo + " (black) won!";
        else if (gs == Checkers::GameStatus::WhiteWon)
        gstext = aione + " (white) won!";
        else if (gs == Checkers::GameStatus::Draw)
        gstext = "the game resulted in a draw.";
        else if (gs == Checkers::GameStatus::NotCompleted)
        gstext = "the game was stopped before it had completed.";
        std::cout << "\n" + gstext + "\n";
    }
    else
    {
        std::cout << "not implemented yet\n";
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
    else if (cmdlineoptions[0] == "play")
    {
        play(cmdlineoptions);
    }
    else
    {
        printAICommandHelp(cmdlineoptions);
    }
}

}
