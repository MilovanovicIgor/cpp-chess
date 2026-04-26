
#ifndef PAWN_H
#define PAWN_H

#include "Board.h"
#include "Piece.h"

class Pawn: public Piece{
    public:
        Pawn(int color,Board* board,int letter,int number);

        bool move(int letter, int number);

        bool move_forward(int letter, int number);

        bool capture(int letter,int number);

        bool capture2(int letter,int number);
                    
        bool promote(int color, int letter);

        bool would_capture_pinner(int letter,int number);

        bool moves_along_pin(int letter,int number);

        bool play_en_passant(int letter,int number);

        bool is_en_passant_enabled();

        void disable_en_passant();

        bool sees_opponent_king();

        void calculate_attacking_squares();

        void clear_attacking_squares();

        
    private:
    Board* board;
    bool En_Passant;
    

    bool piece_moved;
};

#endif