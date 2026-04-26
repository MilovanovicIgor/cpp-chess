#ifndef PIECE_H
#define PIECE_H

#include<set>

class Piece{
    public:
        int color;
        int pos_letter;
        int pos_number;
        std::set<std::pair<int,int> > attacking_squares;

        Piece(int letter,int number,char piece_label, int color);

        char get_piece_label();
        int get_color();
        virtual bool is_en_passant_enabled();
        virtual void disable_en_passant();
        virtual bool move(int letter,int number);
        virtual void calculate_attacking_squares();
        virtual void clear_attacking_squares();
        void pin( Piece* p);
        void unpin();
        bool pinned;
        Piece* pinned_by;
        



    private:
        char piece_label;

        
       
};


#endif