#include<iostream>
#include<vector>

#include "Board.h"
#include "Piece.h"
#include "Rook.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"

King::King(int color,Board* board,int letter,int number):Piece(letter,number,'K',color){
    this->board=board;
    this->piece_moved=false;
}

bool King::move(int letter,int number){
    //Saving current position
    int tmp_letter=pos_letter;
    int tmp_number=pos_number;

    if(board->attacked_square(letter,number,color)){
        return false;
    }
            
    if(attacking_squares.find(std::make_pair(letter,number))!=attacking_squares.end()){
            
        if((board->squares[letter][number]==NULL || board->squares[letter][number]->get_color()!=this->get_color())){
                    
            //Check if move captures opponent's piece
            if(board->squares[letter][number]!=NULL){

                Piece* f=board->squares[letter][number];

                if(board->attacked_square(letter,number,color)){
                    return false;
                }

                f->clear_attacking_squares();
                char c=f->get_piece_label();

                //If captured piece is queen, bishop or rook, we erase that piece from set of possible pinners.
                if(c=='Q' || c=='b' || c=='r'){
                    int b=f->get_color();
                    if(b==1){
                        board->possible_white_pinners.erase(f);
                    }
                    else{
                        board->possible_black_pinners.erase(f);
                    }
                }

            }

            clear_attacking_squares();

            //Updating position of a king
            board->squares[pos_letter][pos_number]=NULL;
            pos_letter=letter;
            pos_number=number;
            board->squares[pos_letter][pos_number]=this;
            piece_moved=true;

            std::set<std::pair<int,int> > set_for_iteration=board->pieces_attacking_the_square[tmp_letter][tmp_number];

            //For every piece that attacked previous position square of a king, recalculate attacking squares.
            for(std::pair<int,int> p : set_for_iteration){

                board->squares[p.first][p.second]->clear_attacking_squares();    
                board->squares[p.first][p.second]->calculate_attacking_squares();
            }
            
            set_for_iteration=board->pieces_attacking_the_square[pos_letter][pos_number];

            //For every piece that attacks current position square of a king, recalculate attacking squares.
            for(std::pair<int,int> p : set_for_iteration){
                
                board->squares[p.first][p.second]->clear_attacking_squares();
                board->squares[p.first][p.second]->calculate_attacking_squares();
            }
                    

            calculate_attacking_squares();

            if(color==1){
                board->White_king=std::make_pair(pos_letter, pos_number);
                
            }
            else{
                board->Black_king=std::make_pair(pos_letter,pos_number);
            }

            return true;
         }
         else{
            return false;
        }

    }
    else if(!piece_moved && number==pos_number){
        
        if(letter==6 && board->squares[7][number]->get_piece_label()=='r'){
            if(!board->attacked_square(5,number,color) && !board->attacked_square(6,number,color) && !board->attacked_square(4,number,color)){

                Rook* t=dynamic_cast<Rook*>(board->squares[7][number]);
                if(!(t->get_moved()) && board->squares[5][number]==NULL && board->squares[6][number]==NULL){

                    t->clear_attacking_squares();
                    clear_attacking_squares();

                    board->squares[7][number]=NULL;
                    board->squares[pos_letter][pos_number]=NULL;

                    pos_letter=letter;
                    board->squares[6][number]=this;
                    
                    t->set_pos_letter(5);
                    board->squares[5][number]=t;

                    calculate_attacking_squares();
                    t->calculate_attacking_squares();
                    std::set<std::pair<int,int> > set_for_iteration;
                    for(int i=4;i<8;i++){
                        set_for_iteration=board->pieces_attacking_the_square[i][number];
                        for(std::pair<int,int> p : set_for_iteration){
                            board->squares[p.first][p.second]->calculate_attacking_squares();
                        }
                    }
                    piece_moved=true;

                    if(color==1){
                        board->White_king=std::make_pair(pos_letter, pos_number);
                    }
                    else{
                        board->Black_king=std::make_pair(pos_letter,pos_number);
                    }

                    return true;

                }

            }
        }
        else if(letter==2 && board->squares[0][number]->get_piece_label()=='r'){

            if(!board->attacked_square(2,number,color) && !board->attacked_square(3,number,color) && !board->attacked_square(4,number,color)){

                Rook* t=dynamic_cast<Rook*>(board->squares[0][number]);
                if(!(t->get_moved()) && board->squares[1][number]==NULL && board->squares[2][number]==NULL && board->squares[3][number]==NULL){

                    t->clear_attacking_squares();
                    clear_attacking_squares();

                    board->squares[0][number]=NULL;
                    board->squares[pos_letter][pos_number]=NULL;

                    pos_letter=letter;
                    board->squares[2][number]=this;
                        
                    t->set_pos_letter(3);
                    board->squares[3][number]=t;

                    calculate_attacking_squares();
                    t->calculate_attacking_squares();
                    
                    std::set<std::pair<int,int> > set_for_iteration;
                    for(int i=0;i<5;i++){
                        set_for_iteration=board->pieces_attacking_the_square[i][number];
                        for(std::pair<int,int> p : set_for_iteration){
                            board->squares[p.first][p.second]->calculate_attacking_squares();
                        }
                    }
                    piece_moved=true;

                    if(color==1){
                        board->White_king=std::make_pair(pos_letter, pos_number);
                        }
                    else{
                        board->Black_king=std::make_pair(pos_letter,pos_number);
                    }

                    return true;

                }
            }
        }

    }
    else{
            
        return false;
    }
}
bool King::is_en_passant_enabled(){
    return false;
    }

void King :: disable_en_passant(){}

bool King:: sees_opponent_king(){
    return false;
}

int King::get_pos_letter(){
    return pos_letter;
}

int King::get_pos_number(){
    return pos_number;
}

void King :: calculate_attacking_squares(){
    attacking_squares.clear();

    int candidate_letter=pos_letter;
    int candidate_number=pos_number+1;

    if(is_valid_square(candidate_letter,candidate_number)){

        attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
        board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
    }

    candidate_number=pos_number-1;

    if(is_valid_square(candidate_letter,candidate_number)){
        attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
        board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
    }

    candidate_letter=pos_letter+1;
    candidate_number=pos_number;

    if(is_valid_square(candidate_letter,candidate_number)){
        attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
        board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
    }

    candidate_letter=pos_letter-1;

    if(is_valid_square(candidate_letter,candidate_number)){
        attacking_squares.insert(std::make_pair(candidate_letter,candidate_number));
        board->pieces_attacking_the_square[candidate_letter][candidate_number].insert( std::make_pair(pos_letter , pos_number));
    }

    candidate_letter=pos_letter+1;
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

    std::cout<<"squares attacked by King"<<pos_letter<<pos_number<<"\n";
    for(std::pair< int, int> p : attacking_squares){
        std::cout<<p.first<<"  "<<p.second<<"\n";
    }

}

void King :: clear_attacking_squares(){
    for(std::pair< int, int> p : attacking_squares){
        board->pieces_attacking_the_square[p.first][p.second].erase(std::make_pair(pos_letter,pos_number));
    }
        
    attacking_squares.clear();
}

bool King :: can_move(){
    std::cout<<"Entered can_move method\n";
    
    

    for(std::pair<int,int> p : attacking_squares){
        std::cout<<p.first<<" "<<p.second<<"\n";
        Piece* f=board->squares[p.first][p.second];
        if(f==NULL){
            if(!board->attacked_square(p.first,p.second,color)){
                //If square is unattacked by opponent's pieces, square is safe for king to move
                return true;
            }
        }
        else if(f->get_color()==color){
            //On this square is friendly piece
            //continue probably
        }
        else{
            //If an opponent's piece lays on this square, king can capture it, if that piece is not defended (by another opponent's piece)
            if(!board->attacked_square(p.first,p.second,color)){
                return true;
            }
        }
    }
    std::cout<<"Done\n";
    return false;

}