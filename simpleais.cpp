// simpleais.cpp

#include "simpleais.h"

namespace SimpleAIs
{

// simple random ai
// note: assumes that there is at least one move that it can make
Checkers::Move getRandomAIMove(const Checkers::GameState &thegamestate)
{
    std::vector<Checkers::Move> moves = thegamestate.getListOfAllLegalMoves();
    std::random_device random_device;
    std::mt19937 engine{random_device()};
    std::uniform_int_distribution<int> dist(0, (int)(moves.size()) - 1);
    Checkers::Move themove = moves[dist(engine)];
    return themove;
}

// simple ai that looks at the resulting board from each legal move it can take,
// figures out how good that board is by a simple evaluation function,
// and then chooses the best board and thus the best move
// note: assumes that there is at least one move that it can make
Checkers::Move getSimpleEvalSinglePlyAIMove(const Checkers::GameState &thegamestate)
{
    std::vector<Checkers::Move> moves = thegamestate.getListOfAllLegalMoves();
    int highestscore = 0;
    Checkers::Move themove = moves[0];
    for (Checkers::Move m : moves)
    {
        std::vector<Checkers::Piece> boardaftermove = Checkers::applyMoveToBoard(m, thegamestate.piecesonboard);
        int score = 0;
        for (Checkers::Piece p : boardaftermove)
        {
            if (thegamestate.playertomove == p.color)
            {
                if (p.type == Checkers::TileType::King)
                    score++;
                score++;
            }
            else
            {
                if (p.type == Checkers::TileType::King)
                    score--;
                score--;
            }
        }
        if (score > highestscore)
        {
            themove.startingpiece = m.startingpiece;
            themove.endingpiece = m.endingpiece;
            themove.jumpedpieces = m.jumpedpieces;
            highestscore = score;
        }
    }
    return themove;
}

}
