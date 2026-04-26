#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include "Board.h"

class Knight :public Piece{
    public:

        Knight(int color,Board* board,int letter,int number);

        bool move(int letter,int number);
        bool is_en_passant_enabled();
        void disable_en_passant();
        bool sees_opponent_king();
        void calculate_attacking_squares();
        void clear_attacking_squares();
        

    private:
        Board* board;
       
};




#endif