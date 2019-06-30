// checkersgame.cpp

#include "checkers.h"

namespace Checkers {

std::vector<Move> GameState::getListOfLegalMoves(const Piece & thepiece) const
{
    // this is the function that has proven very difficult in the past...let's see how well this iteration of it goes

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
    auto enditer = ml.end();
    auto iter = ml.begin();
    while (iter != enditer)
    {
        // if a piece gets kinged then the turn ends
        if ((iter->startingpiece.type == TileType::Man) && (iter->endingpiece.type == TileType::King))
        {
            ++iter;
        }
        else
        {
            bool newjump = false; // have we discovered any new jumps?
            
            for (int xoffset : std::vector<int>{-1,1})
            {
                for (int yoffset : std::vector<int>{-1,1})
                {
                    // is there a tile to jump?
                    auto jumpedpieceiter = std::find_if(piecesonboard.begin(),piecesonboard.end(),
                                                        [this,iter,xoffset,yoffset](Piece p){ return (p.pos.x == iter->endingpiece.pos.x - xoffset) 
                                                                                                  && (p.pos.y == iter->endingpiece.pos.y - yoffset)
                                                                                                  && (p.color != iter->startingpiece.color); });
                    if (jumpedpieceiter != piecesonboard.end())
                    {
                        // is there a tile to land on?
                        if (std::find_if(piecesonboard.begin(),piecesonboard.end(),
                        [this,iter,xoffset,yoffset](Piece p){ return (p.pos.x == iter->endingpiece.pos.x - 2*xoffset) 
                                                                  && (p.pos.y == iter->endingpiece.pos.y - 2*yoffset); }) == piecesonboard.end())
                        {
                            // we have a move!
                            // update the jumped piece list
                            std::vector<Piece> newjumpedpieces = iter->jumpedpieces;
                            newjumpedpieces.push_back(*jumpedpieceiter);
                            // create the move
                            Move m = Move(iter->startingpiece,
                                        Piece(TileType::Man, iter->startingpiece.color, BoardPos(iter->endingpiece.pos.x - 2*xoffset, iter->endingpiece.pos.y - 2*yoffset)),
                                        newjumpedpieces);
                            // does the piece get kinged?
                            if (((iter->startingpiece.color == PlayerColor::White) && (iter->endingpiece.pos.y == 0))
                                || ((iter->startingpiece.color == PlayerColor::Black) && (iter->endingpiece.pos.y == 7)))
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
                iter = ml.erase(iter);
                enditer = ml.end();
            }
            else
            {
                // no new jumps, so proceed to the next move
                ++iter;
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
        std::vector<Move> ml = {{}};
        for (int xoffset : std::vector<int>{-1,1})
        {
            for (int yoffset : std::vector<int>{-1,1})
            {
                // is there a tile to land on?
                if (std::find_if(piecesonboard.begin(),piecesonboard.end(),
                [this,iter,xoffset,yoffset](Piece p){ return (p.pos.x == iter->endingpiece.pos.x - xoffset) 
                                                          && (p.pos.y == iter->endingpiece.pos.y - yoffset); }) == piecesonboard.end())
                {
                    // we have a move!
                    // create the move
                    Move m = Move(iter->startingpiece,
                                Piece(TileType::Man, iter->startingpiece.color, BoardPos(iter->endingpiece.pos.x - xoffset, iter->endingpiece.pos.y - yoffset)),
                                std::vector<Piece>());
                    // does the piece get kinged?
                    if (((iter->startingpiece.color == PlayerColor::White) && (iter->endingpiece.pos.y == 0))
                        || ((iter->startingpiece.color == PlayerColor::Black) && (iter->endingpiece.pos.y == 7)))
                    {
                        m.endingpiece.type = TileType::King;
                    }
                    // add the move to the move list
                    ml.push_back(m);
                }
            }
        }
        return ml;
    }
}

std::vector<Move> GameState::getListOfAllLegalMoves() const
{
    std::vector<Move> movelist = {{}};

    std::vector<Move> jumpmovelist = {{}};
    std::vector<Move> nonjumpmovelist = {{}};

    for (Piece p : piecesonboard)
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
    // delete the starting piece
    currentstate.piecesonboard.erase(std::remove(currentstate.piecesonboard.begin(),
                                                 currentstate.piecesonboard.end(),
                                                 themove.startingpiece));
    // delete the jumped pieces
    for (Piece p : themove.jumpedpieces)
        currentstate.piecesonboard.erase(std::remove(currentstate.piecesonboard.begin(),
                                                     currentstate.piecesonboard.end(),
                                                     p));
    // add the ending piece
    currentstate.piecesonboard.push_back(themove.endingpiece);
    
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
