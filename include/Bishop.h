#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include "Board.h"

class Bishop :public Piece{
    public:

        Bishop (int color,Board* board,int letter,int number);
        Piece* pinning;
        bool move(int letter,int number);
        bool is_en_passant_enabled();
        void disable_en_passant();
        bool capture(int letter,int number);
        bool would_capture_pinner(int letter,int number);
        bool moves_along_pin(int letter,int number);
        void pin_if_needed();
        bool sees_opponent_king();
        void calculate_attacking_squares();
        void clear_attacking_squares();

    private:
        Board* board;
        
};




#endif