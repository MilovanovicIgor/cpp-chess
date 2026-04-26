#ifndef BOARD_H
#define BOARD_H

#include<vector>
#include<set>
#include<unordered_set>
#include "Piece.h"

bool is_valid_square(int letter, int number);

class Board{
    public:
        
        std::vector<std::vector<Piece*> > squares;
        std::vector<std::vector<std::set<std::pair<int,int> > > > pieces_attacking_the_square;
        std::pair<int,int> White_king;
        bool check_on_white_king;
        std::pair<int,int> Black_king;
        bool check_on_black_king;
        std::pair<int,int> pozicija_en_passant;

        std::unordered_set<Piece *> possible_white_pinners;
        std::unordered_set<Piece *> possible_black_pinners;

        bool attacked_square(int letter,int number,int color);
        bool defending_check(int letter,int number,int color);
        bool checkmate(int color);
        std::set<std::pair<int, int>> squares_on_diagonal(int letter_from,int number_from, int letter_to,int number_to);

    
        void set_up_starting_position();

        void draw_board();



        Piece* piece_on_square(char letter, int number);


};

#endif