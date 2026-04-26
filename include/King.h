#ifndef KING_H
#define KING_H

#include "Piece.h"
#include "Board.h"

class King :public Piece{
    public:

        King(int color,Board* board,int letter,int number);

        bool move(int letter,int number);
        bool is_en_passant_enabled();
        void disable_en_passant();
        bool sees_opponent_king();
        int get_pos_letter();
        int get_pos_number();
        void calculate_attacking_squares();
        void clear_attacking_squares();
        bool can_move();

    private:
        Board* board;
        bool piece_moved;
        
};




#endif