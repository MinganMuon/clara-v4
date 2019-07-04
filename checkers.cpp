// checkersgame.cpp

#include "checkers.h"

namespace Checkers {

void printboard(std::vector<Piece> piecesonboard)
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
    for (Piece p : piecesonboard)
    {
        std::string pchar = "e";
        if ((p.type == TileType::Man) && (p.color == PlayerColor::White))
            pchar = "w";
        else if ((p.type == TileType::King) && (p.color == PlayerColor::White))
            pchar = "W";
        else if ((p.type == TileType::Man) && (p.color == PlayerColor::Black))
            pchar = "b";
        else if ((p.type == TileType::King) && (p.color == PlayerColor::Black))
            pchar = "B";
        
        board[1+p.pos.y].replace((p.pos.x+1)*2,1,pchar);
    }

    // print the board
    for (std::string s : board)
        std::cout << s;
}

std::vector<Piece> applyMoveToBoard(Move themove, std::vector<Piece> piecesonboard)
{
    // delete the starting piece
    piecesonboard.erase(std::remove(piecesonboard.begin(),
                                                 piecesonboard.end(),
                                                 themove.startingpiece));

    // delete the jumped pieces
    for (Piece p : themove.jumpedpieces)
        piecesonboard.erase(std::remove(piecesonboard.begin(), piecesonboard.end(), p));

    // add the ending piece
    piecesonboard.push_back(themove.endingpiece);

    return piecesonboard;
}

std::vector<Move> GameState::getListOfLegalMoves(const Piece & thepiece) const
{
    // this is the function that has proven very difficult in the past...let's see how well this iteration of it goes
    // update: going better than last time!

    // determine which way(s) the piece can move or jump
    std::vector<int> yoffsets;
    if ((thepiece.type == TileType::Man) && (thepiece.color == PlayerColor::White))
    {
        yoffsets.push_back(1);
    }
    else if ((thepiece.type == TileType::Man) && (thepiece.color == PlayerColor::Black))
    {
        yoffsets.push_back(-1);
    }
    else
    {
        yoffsets.push_back(1);
        yoffsets.push_back(-1);
    }

    // get jump moves
    //
    // general idea:
	// 	    iterate over an expanding movelist
	// 	    finding all the jumps that can be made from a specific move's end tile
	// 	    creating new Moves for those new moves
	// 	    appending them to the end of the movelist
	// 	    then deleting the move that generated those new moves if any new moves were found
	//      repeat until the end of the movelist is reached
    //      i.e. until all moves in the movelist have no possible moves that can be made from them
    std::vector<Move> ml = {{}};
    ml.push_back(Move(thepiece,thepiece,std::vector<Piece>()));
    int i = 0;
    while (i < ml.size())
    {
        // if a piece gets kinged then the turn ends
        if ((ml[i].startingpiece.type == TileType::Man) && (ml[i].endingpiece.type == TileType::King))
        {
            i++;
        }
        else
        {
            bool newjump = false; // have we discovered any new jumps?
            
            for (int xoffset : std::vector<int>{-1,1})
            {
                for (int yoffset : yoffsets)
                {
                    // is there a piece to jump?
                    auto jumpedpieceiter = std::find_if(piecesonboard.begin(),piecesonboard.end(),
                                                        [this,&ml,xoffset,yoffset,i](Piece p){ return (p.pos.x == ml[i].endingpiece.pos.x - xoffset) 
                                                                                                   && (p.pos.y == ml[i].endingpiece.pos.y - yoffset)
                                                                                                   && (p.color != ml[i].startingpiece.color); });
                    if (jumpedpieceiter != piecesonboard.end())
                    {
                        // is there a tile to land on?
                        if ((BoardPos(ml[i].endingpiece.pos.x - 2*xoffset, ml[i].endingpiece.pos.y - 2*yoffset).isValidPos())
                            && (std::find_if(piecesonboard.begin(),piecesonboard.end(),
                        [this,&ml,xoffset,yoffset,i](Piece p){ return (p.pos.x == ml[i].endingpiece.pos.x - 2*xoffset) 
                                                                   && (p.pos.y == ml[i].endingpiece.pos.y - 2*yoffset); }) == piecesonboard.end()))
                        {
                            // we have a move!
                            // update the jumped piece list
                            std::vector<Piece> newjumpedpieces = ml[i].jumpedpieces;
                            newjumpedpieces.push_back(*jumpedpieceiter);
                            // create the move
                            Move m = Move(ml[i].startingpiece,
                                        Piece(ml[i].startingpiece.type, ml[i].startingpiece.color, BoardPos(ml[i].endingpiece.pos.x - 2*xoffset, ml[i].endingpiece.pos.y - 2*yoffset)),
                                        newjumpedpieces);
                            // does the piece get kinged?
                            if (((ml[i].startingpiece.color == PlayerColor::White) && (ml[i].endingpiece.pos.y - 2*yoffset == 0))
                                || ((ml[i].startingpiece.color == PlayerColor::Black) && (ml[i].endingpiece.pos.y - 2*yoffset == 7)))
                            {
                                m.endingpiece.type = TileType::King;
                            }
                            // add the move to the move list
                            ml.push_back(m);
                            newjump = true;
                        }
                    }
                }
            }
            
            if (newjump)
            {
                // we have discovered and added new jumps, so erase the current move and update the iterators
                ml.erase(ml.begin() + i);
            }
            else
            {
                // no new jumps, so proceed to the next move
                i++;
            }
        }
    }

    // if we found possible jump moves, then return them
    // if we didn't find any, then find all possible non-jump moves
    if (!ml.empty() && !(ml[0].endingpiece == thepiece))
    {
        return ml;
    }
    else
    {
        std::vector<Move> movelist = {{}};
        for (int xoffset : std::vector<int>{-1,1})
        {
            for (int yoffset : yoffsets)
            {
                // is there a tile to land on?
                if ((BoardPos(thepiece.pos.x - xoffset, thepiece.pos.y - yoffset).isValidPos())
                    && (std::find_if(piecesonboard.begin(),piecesonboard.end(),
                [this,thepiece,xoffset,yoffset](Piece p){ return (p.pos.x == thepiece.pos.x - xoffset) 
                                                          && (p.pos.y == thepiece.pos.y - yoffset); }) == piecesonboard.end()))
                {
                    // we have a move!
                    // create the move
                    Move m = Move(thepiece,
                                Piece(thepiece.type, thepiece.color, BoardPos(thepiece.pos.x - xoffset, thepiece.pos.y - yoffset)),
                                std::vector<Piece>());
                    // does the piece get kinged?
                    if (((thepiece.color == PlayerColor::White) && (thepiece.pos.y - yoffset == 0))
                        || ((thepiece.color == PlayerColor::Black) && (thepiece.pos.y - yoffset == 7)))
                    {
                        m.endingpiece.type = TileType::King;
                    }
                    // add the move to the move list
                    movelist.push_back(m);
                }
            }
        }
        return movelist;
    }
}

std::vector<Move> GameState::getListOfAllLegalMoves() const
{
    std::vector<Move> movelist = {{}};

    std::vector<Move> jumpmovelist = {{}};
    std::vector<Move> nonjumpmovelist = {{}};

    for (Piece p : piecesonboard)
    {
        if (p.color == playertomove)
        {
            std::vector<Move> piecemovelist = getListOfLegalMoves(p);
            if (!piecemovelist.empty())
            {
                if (piecemovelist[0].jumpedpieces.empty()) // only need to consider the first move because getListOfLegalMoves will remove any non-jump moves if the piece has jump moves
                {
                    nonjumpmovelist.insert(nonjumpmovelist.end(), piecemovelist.begin(), piecemovelist.end());
                }
                else
                {
                    jumpmovelist.insert(jumpmovelist.end(), piecemovelist.begin(), piecemovelist.end());
                }
            }
        }
    }

    if (jumpmovelist.empty())
    {
        movelist.insert(movelist.end(), nonjumpmovelist.begin(), nonjumpmovelist.end());
    }
    else
    {
        movelist.insert(movelist.end(), jumpmovelist.begin(), jumpmovelist.end());
    }

    return movelist;
}

GameStatus GameState::determineGameStatus() const
{
    // if the current player has no pieces or can not play, then the other player won
    if (std::count_if(piecesonboard.begin(), piecesonboard.end(), [this](Piece p){ return p.color == playertomove; }) == 0
            || getListOfAllLegalMoves().empty())
        return (playertomove == PlayerColor::White) ? GameStatus::BlackWon : GameStatus::WhiteWon;
    
    return GameStatus::NotCompleted;
}

bool Game::makeMove(Move themove)
{
    // can't make moves if the game is over!
    if (determineGameStatus() != GameStatus::NotCompleted)
        return false;

    // return false if the pieces in themove don't have valid positions
    // is this required? any invalid moves would be filtered out by checking against the list of legal moves, right?
    if (    !themove.startingpiece.pos.isValidPos()
        ||  !themove.endingpiece.pos.isValidPos()
        || (!(std::count_if(themove.jumpedpieces.begin(), themove.jumpedpieces.end(),
             [this](Piece p){ return !p.pos.isValidPos(); }) == 0)))
        return false;
    
    // return false if themove isn't a legal move for the current player
    std::vector<Move> legalmoves = currentstate.getListOfAllLegalMoves();
    if (std::find(legalmoves.begin(), legalmoves.end(), themove) == legalmoves.end())
        return false;
    
    // now make the move!
    currentstate.piecesonboard = applyMoveToBoard(themove, currentstate.piecesonboard);
    
    currentstate.playertomove = (currentstate.playertomove == PlayerColor::White) ? PlayerColor::Black : PlayerColor::White;

    return true;
}

GameStatus Game::determineGameStatus() const
{
    GameStatus status = currentstate.determineGameStatus();

    if (status == GameStatus::NotCompleted && numberofunproductivemoves > unproductivemovelimit)
        status = GameStatus::Draw;
    
    return status;
}

}
