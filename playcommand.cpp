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

void printboard(std::vector<Checkers::Piece> piecesonboard)
{
    std::array<std::string,9> board;
    
    // initialize the board outline
    board[0] = "  0 1 2 3 4 5 6 7 \n";
    for (int i = 1; i < 5; i++)
    {
        board[i*2-1] = std::to_string(i*2-2) + "   x   x   x   x \n";
        board[i*2] = std::to_string(i*2-1) + " x   x   x   x   \n";
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
    printboard(std::vector<Checkers::Piece>());
    printboard(std::vector<Checkers::Piece>{Checkers::Piece(Checkers::TileType::Man,Checkers::PlayerColor::White,Checkers::BoardPos(1,0)),
                                            Checkers::Piece(Checkers::TileType::Man,Checkers::PlayerColor::Black,Checkers::BoardPos(0,1)),
                                            Checkers::Piece(Checkers::TileType::King,Checkers::PlayerColor::White,Checkers::BoardPos(2,1)),
                                            Checkers::Piece(Checkers::TileType::King,Checkers::PlayerColor::Black,Checkers::BoardPos(1,2))});
}

}
