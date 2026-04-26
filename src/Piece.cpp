#include<iostream>

#include "piece.h"

        
        Piece::Piece(int letter, int number, char piece_label, int color){
            this->pos_letter=letter;
            this->pos_number=number;
            this->color=color;
            this->piece_label=piece_label;
            this->pinned=false;
            this->pinned_by=NULL;
        }

        char Piece :: get_piece_label(){
            return this->piece_label;
        }
        int Piece :: get_color(){
            return this->color;
        }

        void Piece :: pin(Piece* f){
            this->pinned=true;
            this->pinned_by=f;
            std::cout<<"Pinned "<< this->get_piece_label()<<" "<< this->pos_letter<<" "<<pos_number<<"\n";
        }

        void Piece :: unpin(){
            this->pinned=false;
            this->pinned_by=NULL;
            std::cout<<"Unpinned "<< this->get_piece_label()<<" "<< this->pos_letter<<" "<<pos_number<<"\n";
        }

    
        bool Piece:: is_en_passant_enabled(){
           
        }
        void Piece::disable_en_passant(){}
        

        //Moves the piece to square (letter,number) if the move is legal.
        //Returns true if the move was performed, false otherwise.
        bool Piece:: move(int letter,int number){

        }
        //Calculates squares that piece is attacking based on its current position and other position of other pieces on board.
        //When calculated, inserts them in this piece's attacking_squares  and updates the board's pieces_attacking_the_square.
        void Piece :: calculate_attacking_squares(){}

        //Deletes all elements from this piece's attacking_squares and updates board's pieces_attacking_the_square accordingly
        void Piece :: clear_attacking_squares(){}

       

