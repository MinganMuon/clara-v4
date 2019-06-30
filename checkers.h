// checkers.h

#ifndef CHECKERS_H
#define CHECKERS_H

#include <array>
#include <vector>
#include <algorithm>

namespace Checkers {

// tile type and color
enum class TileType {Man, King};
enum class PlayerColor {White, Black};

// a position on the board
// x=0,y=0 is the upper left
// black is always above white
class BoardPos {
    public:
        BoardPos(int _x, int _y) : x(_x), y(_y) { };
        int x;
        int y;
        bool isValidPos() const { return (isInsideBoard() && isValidTile()); };      // is the position a valid position?
    private:
        bool isInsideBoard() const { return (x < 7 && y < 7 && x > -1 && y > -1); }; // is the position inside the board?
        bool isValidTile() const { return (y % 2) ? (!(x % 2)) : (x % 2); };         // is the position describing a valid tile?
    public:
        friend bool operator==(const BoardPos& lhs, const BoardPos& rhs)
            { return (lhs.x == rhs.x) && (lhs.y == rhs.y); };
};

// a piece is a tile of a specific color placed at a certain position on the board
class Piece {
    public:
        Piece(TileType _type, PlayerColor _color, BoardPos _pos) : type(_type), color(_color), pos(_pos) { };
        TileType type;
        PlayerColor color;
        BoardPos pos;

        friend bool operator==(const Piece& lhs, const Piece& rhs)
            { return (lhs.type == rhs.type) && (lhs.color == rhs.color) && (lhs.pos == rhs.pos); };
};

// a move is comprised of a starting piece, an ending piece, and a list of captured pieces
class Move {
    public:
        Move(Piece _startingpiece, Piece _endingpiece, std::vector<Piece> _jumpedpieces) : startingpiece(_startingpiece), endingpiece(_endingpiece), jumpedpieces(_jumpedpieces) { };
        Piece startingpiece;
        Piece endingpiece;
        std::vector<Piece> jumpedpieces; // jumpedpieces is ordered because theoretically one could jump the same pieces and end up in the same place but jump via a different path

        friend bool operator==(const Move& lhs, const Move& rhs)
            { return (lhs.startingpiece == rhs.startingpiece) && (lhs.endingpiece == rhs.endingpiece) && (lhs.jumpedpieces == rhs.jumpedpieces); };
};

enum class GameStatus {WhiteWon, BlackWon, Draw, NotCompleted};

// the game state is basically a board and a player who will make a move
class GameState {
    public:
        GameState(std::vector<Piece> _piecesonboard, PlayerColor _playertomove) : piecesonboard(_piecesonboard), playertomove(_playertomove) { };

        std::vector<Piece> piecesonboard;
        PlayerColor playertomove;

        std::vector<Move> getListOfAllLegalMoves() const;
        GameStatus determineGameStatus() const; // determine whether the current board/playertomove results in a finished game
    private:
        std::vector<Move> getListOfLegalMoves(const Piece & thepiece) const;
};

// a sequence of game states form a full game
// that sequence progresses by players making moves
// and ends when certain parameters are met
class Game {
    public:
        Game(int _unproductivemovelimit=50) : currentstate(GameState(StartingBoard,PlayerColor::White)), numberofunproductivemoves(0), unproductivemovelimit(_unproductivemovelimit) { };
        const GameState getGameState() const { return currentstate; };

        bool makeMove(Move themove); // does not check for legality
        GameStatus determineGameStatus() const;
    private:
        GameState currentstate;
        int numberofunproductivemoves; // how many moves in a row have happened that haven't resulted in a capture?
        int unproductivemovelimit; // how many moves in a row that don't result in a capture should we allow?

        const std::vector<Piece> StartingBoard = {Piece(TileType::Man,PlayerColor::White,BoardPos(1,0))}; // the starting board
};

}

#endif
