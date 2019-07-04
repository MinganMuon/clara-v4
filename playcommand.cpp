// playcommand.cpp

#include "playcommand.h"

namespace PlayCommand
{

void printPlayCommandHelp(std::vector<std::string> cmdlineoptions)
{
    std::cout << "clara-v4 help for the play command\n";
    std::cout << "----------------------------------\n\n";

    std::cout << "usage: clara play <opponent>\n\n";

    std::cout << "if \"human\" is selected as the opponent, starts a game pitting the user against himself\n";
    std::cout << "if a playable ai (see output of clara ai list-playable) is selected as the opponent, starts a game pitting the user against the selected ai\n\n";

    std::cout << "on the displayed board, white men are represented by \"w\",\n"
              << "black men by \"b\", and kings as the uppercase variants of the men.\n";
}

void play(std::vector<std::string> cmdlineoptions)
{
    std::string opponent;
    if (cmdlineoptions.empty())
    {
        printPlayCommandHelp(cmdlineoptions);
        return;
    }
    else if (cmdlineoptions[0] == "human")
    {
        opponent = "yourself";
    }
    else
    {
        const std::vector<std::string> ailist = AISwitchboard::getListOfAllAIs();
        auto aisiter = std::find(ailist.begin(), ailist.end(), cmdlineoptions[0]);
        if (aisiter != ailist.end())
        {
            opponent = cmdlineoptions[0];
        }
        else
        {
            printPlayCommandHelp(cmdlineoptions);
            return;
        }
        
    }

    std::cout << "you will be playing against " + opponent + ".\n\n";

    Checkers::Game thegame = Checkers::Game();
    bool done = false;
    while (!done)
    {
        Checkers::printboard(thegame.getGameState().piecesonboard);

        std::string playertext = ((thegame.getGameState().playertomove == Checkers::PlayerColor::White) ? "white" : "black");
        std::cout << "\nyou are " +  playertext + ".\n";

        Checkers::Move themove = [&thegame]() -> Checkers::Move {
            while (true)
            {
                std::cout << "select a move with the format of \"<old x> <old y> <new x> <new y>\": ";
                int oldx, oldy, newx, newy;
                std::cin >> oldx >> oldy >> newx >> newy;

                std::vector<Checkers::Move> movelist = thegame.getGameState().getListOfAllLegalMoves();
                auto moveiter = std::find_if(movelist.begin(), movelist.end(),
                                            [oldx,oldy,newx,newy](Checkers::Move m){ return (m.startingpiece.pos.x == oldx)
                                                                                        && (m.startingpiece.pos.y == oldy)
                                                                                        && (m.endingpiece.pos.x == newx)
                                                                                        && (m.endingpiece.pos.y == newy); });
                
                if (moveiter == movelist.end())
                {
                    std::cout << "that was an invalid or illegal move...try again?\n";
                }
                else
                {
                    std::cout << "\n";
                    return *moveiter;
                }
            }
        }();

        thegame.makeMove(themove);

        if (thegame.determineGameStatus() != Checkers::GameStatus::NotCompleted)
            done = true;

        if ((opponent != "human") && (!done))
        {
            if (!thegame.makeMove(AISwitchboard::getMoveFromAI(opponent,thegame.getGameState(),std::vector<std::string>())))
                std::cout << "error: the ai failed to make a move.\n\n";
            if (thegame.determineGameStatus() != Checkers::GameStatus::NotCompleted)
                done = true;
        }
    }

    Checkers::printboard(thegame.getGameState().piecesonboard);

    Checkers::GameStatus gs = thegame.determineGameStatus();
    std::string gstext;
    if (gs == Checkers::GameStatus::BlackWon)
        gstext = "black won!";
    else if (gs == Checkers::GameStatus::WhiteWon)
        gstext = "white won!";
    else if (gs == Checkers::GameStatus::Draw)
        gstext = "the game resulted in a draw.";
    else if (gs == Checkers::GameStatus::NotCompleted)
        gstext = "you exited the game before it had completed.";
    std::cout << "\n" + gstext + "\n";
}

}
