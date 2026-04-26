#ifndef GAME_FLOW_H
#define GAME_FLOW_H

#include "Board.h"
#include "Piece.h"
class Game_flow{
    public:
    Game_flow(Board* t);

    void start_the_game();

    private:
        Board *board;
        int player_on_the_move;
        Piece* black_en_passant;
        Piece* white_en_passant;

};


#endif