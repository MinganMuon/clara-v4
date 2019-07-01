// playcommand.cpp

#include "playcommand.h"
#include "simpleais.h"

namespace PlayCommand
{

void printPlayCommandHelp(std::vector<std::string> cmdlineoptions)
{
    std::cout << "clara-v4 help for the play command\n";
    std::cout << "----------------------------------\n\n";

    std::cout << "usage: clara play <human/ai>\n\n";

    std::cout << "if human is selected, starts a game pitting the user against himself\n";
    std::cout << "if a playable ai (see output of clara ai list-playable) is selected, starts a game pitting the user against the selected ai\n\n";

    std::cout << "on the displayed board, white men are represented by \"w\",\n"
              << "black men by \"b\", and kings as the uppercase variants of the men.\n";
}

void printboard(std::vector<Checkers::Piece> piecesonboard)
{
    std::array<std::string,9> board;
    
    // initialize the board outline
    board[0] = "  0 1 2 3 4 5 6 7 \n";
    for (int i = 1; i < 5; i++)
    {
        board[i*2-1] = std::to_string(i*2-2) + "   .   .   .   . \n";
        board[i*2]   = std::to_string(i*2-1) + " .   .   .   .   \n";
    }

    // add the pieces
    for (Checkers::Piece p : piecesonboard)
    {
        std::string pchar = "e";
        if ((p.type == Checkers::TileType::Man) && (p.color == Checkers::PlayerColor::White))
            pchar = "w";
        else if ((p.type == Checkers::TileType::King) && (p.color == Checkers::PlayerColor::White))
            pchar = "W";
        else if ((p.type == Checkers::TileType::Man) && (p.color == Checkers::PlayerColor::Black))
            pchar = "b";
        else if ((p.type == Checkers::TileType::King) && (p.color == Checkers::PlayerColor::Black))
            pchar = "B";
        
        board[1+p.pos.y].replace((p.pos.x+1)*2,1,pchar);
    }

    // print the board
    for (std::string s : board)
        std::cout << s;
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
        auto simpleaisiter = std::find(SimpleAIs::simpleAIsList.begin(), SimpleAIs::simpleAIsList.end(), cmdlineoptions[0]);
        if (simpleaisiter != SimpleAIs::simpleAIsList.end())
        {
            opponent = (*simpleaisiter);
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
        printboard(thegame.getGameState().piecesonboard);

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
            if (opponent == "random-ai")
            {
                thegame.makeMove(SimpleAIs::getRandomAIMove(thegame.getGameState()));
            }
            else if (opponent == "simple-ai")
            {
                thegame.makeMove(SimpleAIs::getSimpleEvalSinglePlyAIMove(thegame.getGameState()));
            }
            if (thegame.determineGameStatus() != Checkers::GameStatus::NotCompleted)
                done = true;
        }
    }

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
