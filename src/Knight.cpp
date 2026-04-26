#include<iostream>
#include<vector>
#include<cmath>
#include "piece.h"
#include "Board.h"
#include "knight.h"

    Knight::Knight(int color,Board* board,int letter,int number):Piece(letter,number,'k',color){
        this->board=board;
        
    }

    bool Knight::move(int letter,int number){
        //Saving current position
        int tmp_letter=pos_letter;
        int tmp_number=pos_number;

        if(pinned || (board->squares[letter][number]!=NULL && board->squares[letter][number]->get_color()==color)){
            return false;
        }
        else if(attacking_squares.find(std::make_pair(letter,number))!=attacking_squares.end()){

            clear_attacking_squares();
            
            //Check if move captures opponent's piece
            if(board->squares[letter][number]!=NULL){
                
                Piece* p=board->squares[letter][number];
                p->clear_attacking_squares();
                char c=p->get_piece_label();
                
                //If captured piece is queen, bishop or rook, we erase that piece from set of possible pinners.
                if(c=='Q' || c=='b' || c=='r'){
                    int b=p->get_color();
                    if(b==1){
                        board->possible_white_pinners.erase(p);
                    }
                    else{
                        board->possible_black_pinners.erase(p);
                    }
                }
            }
            //Updating position of a knight
            board->squares[pos_letter][pos_number]=NULL;
            pos_letter=letter;
            pos_number=number;
            board->squares[pos_letter][pos_number]=this;

            std::set<std::pair<int,int> > set_for_iteration=board->pieces_attacking_the_square[tmp_letter][tmp_number];
            
            //For every piece that attacked previous position square of a knight, recalculate attacking squares.
            for(std::pair<int,int> p : set_for_iteration){
                std::cout<<"Uslo prvo\n"<<p.first<<"  "<<p.second;
                
                board->squares[p.first][p.second]->clear_attacking_squares();
                board->squares[p.first][p.second]->calculate_attacking_squares();
            }
            
            
            //For every piece that attacks current position square of a knight, recalculate attacking squares.
            set_for_iteration=board->pieces_attacking_the_square[pos_letter][pos_number];
            for(std::pair<int,int> p : set_for_iteration){
                std::cout<<"\n"<<pos_letter<<" " << pos_number<<" Uslo drugo\n"<<p.first<<"  "<<p.second;
                
                board->squares[p.first][p.second]->clear_attacking_squares();
                board->squares[p.first][p.second]->calculate_attacking_squares();
            }

            calculate_attacking_squares();

            return true;
        }
        else return false;
    }



    bool Knight::is_en_passant_enabled(){return false;}

    void Knight::disable_en_passant(){}

    void Knight :: calculate_attacking_squares(){

        attacking_squares.clear();

        int candidate_letter=pos_letter+1;
        int candidate_number=pos_number+2;

        if(is_valid_square(candidate_letter,candidate_number)){

            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_number=pos_number-2;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_letter=pos_letter+2;
        candidate_number=pos_number+1;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_number=pos_number-1;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_letter=pos_letter-1;
        candidate_number=pos_number+2;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_number=pos_number-2;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_letter=pos_letter-2;
        candidate_number=pos_number+1;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        candidate_number=pos_number-1;

        if(is_valid_square(candidate_letter,candidate_number)){
            attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
            board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
        }

        std::cout<<"squares attacked by Knight"<<pos_letter<<pos_number<<"\n";
            for(std::pair< int, int> p : attacking_squares){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }


    }

    void Knight :: clear_attacking_squares(){
        
        for(std::pair< int, int> p : attacking_squares){
            board->pieces_attacking_the_square[p.first][p.second].erase(std::make_pair(pos_letter,pos_number));
        }
        
        attacking_squares.clear();
    }

    bool Knight::sees_opponent_king(){}
    