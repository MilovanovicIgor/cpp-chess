#include<iostream>
#include<cmath>
#include "Pawn.h"
#include<vector>
#include "Board.h"
#include "Piece.h"

        Pawn:: Pawn(int color,Board* board,int letter,int number) 
            :Piece(letter,number,'p',color)
        {
            this->board=board;
            this->En_Passant=false;
        }

        

        bool Pawn:: move(int letter, int number){
            //Saving current position
            int tmp_letter=pos_letter;
            int tmp_number=pos_number;

            bool move_made=false;

            if(!is_valid_square(letter,number)){
                return false;
            }
            else if(pos_letter==letter && (!pinned || moves_along_pin(letter,number))){
                move_made = move_forward(letter,number);
                
            }
            else if(attacking_squares.find(std::make_pair(letter,number))!=attacking_squares.end() && (!pinned || would_capture_pinner(letter,number) || moves_along_pin(letter,number))){
                move_made = capture2(letter,number);
            }
            if(move_made){
                std::cout<<"move_made\n";

                std::set<std::pair<int,int> > set_for_iteration=board->pieces_attacking_the_square[tmp_letter][tmp_number];
                //For every piece that attacked previous position square of a pawn, recalculate attacking squares.
                for(std::pair<int,int> p : set_for_iteration){
                    std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                    
                    board->squares[p.first][p.second]->clear_attacking_squares();
                    board->squares[p.first][p.second]->calculate_attacking_squares();
                }

                set_for_iteration=board->pieces_attacking_the_square[pos_letter][pos_number];
                //For every piece that attacks current position square of a pawn, recalculate attacking squares.
                for(std::pair<int,int> p : set_for_iteration){
                    std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                    
                    board->squares[p.first][p.second]->clear_attacking_squares();
                    board->squares[p.first][p.second]->calculate_attacking_squares();
                }
                
                calculate_attacking_squares();

            }
            return move_made;
        }
        
        bool Pawn:: move_forward(int letter, int number){
            std::cout<<"goes forward\n";
            if(board->squares[letter][number]!=NULL){
                return false;
            }
            if(color==1){
                std::cout<<"white\n";
                if(number-pos_number==2 && !piece_moved){
                    if(board->squares[letter][number-1]!=NULL){
                    
                        return false;
                    }
                    else{
                        clear_attacking_squares();
                        
                        board->squares[pos_letter][pos_number]=NULL;
                        pos_number=pos_number+2;
                        board->squares[letter][number]=this;
                        this->En_Passant=true;
                        this->piece_moved=true;
                        
                        return true;
                    }

                }
                else if(number-pos_number==1){
                    piece_moved=true;
                    if(number==7){
                        promote(color,letter);

                    }
                    else{
                        clear_attacking_squares();

                        board->squares[pos_letter][pos_number]=NULL;
                        pos_number=number;
                        board->squares[pos_letter][pos_number]=this;
                        return true;
                    }
                }
                else {
                    std::cout<<"else\n";
                    return false;
                }
            }

            else if(color==2){
                std::cout<<"black\n";
                
                if(pos_number-number==2 && !piece_moved){
                    if(board->squares[letter][number+1]!=NULL){
                        return false;
                    }
                    else{
                        clear_attacking_squares();

                        board->squares[pos_letter][pos_number]=NULL;
                        pos_number=pos_number-2;
                        board->squares[letter][number]=this;
                        this->En_Passant=true;
                        this->piece_moved=true;
                        return true;

                    }

                }
                else if(pos_number-number==1){
                    piece_moved=true;
                    if(number==0){
                        promote(color,letter);

                    }
                    else{
                        clear_attacking_squares();

                        board->squares[pos_letter][pos_number]=NULL;
                        pos_number=number;
                        board->squares[pos_letter][pos_number]=this;
                        return true;
                    }
                }
            }
        }

        bool Pawn:: capture(int letter,int number){
            if(color==1){
                if(number-pos_number!=1){
                    return false;
                }
                if(board->squares[letter][number]!=NULL && board->squares[letter][number]->get_color()==2){
                    
                    if(!pinned || would_capture_pinner(letter,number)){
                        piece_moved=true;
                        std::cout<<"Eating \n";
                        board->squares[letter][number]=NULL;
                        board->squares[pos_letter][pos_number]=NULL;
                        if(number==7){
                            
                            promote(color,letter);
                            return true;
                        }
                        else{
                            std::cout<<"Entering good \n";
                            pos_letter=letter;
                            pos_number=number;
                            board->squares[letter][number]=this;
                            return true;
                        }
                    }
                }
                else if(board->squares[letter][number]==NULL){
                    return play_en_passant(letter,number);
                }
                else return false;
            }

            else if(color==2){
                if(pos_number-number!=1){
                    return false;
                }
                if(board->squares[letter][number]!=NULL && board->squares[letter][number]->get_color()==1){
                    
                    if(!pinned || would_capture_pinner(letter,number)){
                        piece_moved=true;
                        board->squares[letter][number]=NULL;
                        board->squares[pos_letter][pos_number]=NULL;
                        if(number==0){
                            
                            promote(color,letter);
                            return true;
                        }
                        else{
                            pos_letter=letter;
                            pos_number=number;
                            board->squares[letter][number]=this;
                            return true;
                        }
                    }
                }
                else if(board->squares[letter][number]==NULL){
                    return play_en_passant(letter,number);
                }
                else return false;
            }

        }

        bool Pawn:: capture2(int letter,int number){

            if(board->squares[letter][number]==NULL && !pinned){
                return play_en_passant(letter,number);
            }
            else if(board->squares[letter][number]->get_color()==this->get_color()){
                return false;
            }

            else if(!pinned || would_capture_pinner(letter,number)) {
                //Check if promotion is possible
                clear_attacking_squares();
                Piece* f=board->squares[letter][number];
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

                board->squares[pos_letter][pos_number]=NULL;
                pos_letter=letter;
                pos_number=number;
                board->squares[pos_letter][pos_number]=this;
                return true;
            }
            else {
                return false;
            }

        }
        bool Pawn:: promote(int color, int letter){
           /* std::cout<<"Enter desired piece [q,r,b,k]";
                        char c;
                        std::cin>>c;
                        
                        if(c=='q'){
                            Queen new=Queen('q',color);
                        }
                        else if(c=='r'){
                            Rook new=Rook();
                        }
                        else if(c=='b'){
                            Bishop new=Bishop();
                        }
                        else if(c=='k'){
                            Knight new=Knight();
                        }
                        else return false;

                        board.squares[pos_letter][pos_number]=NULL; */
                        return true;
        }

        //This method returns true if this piece is pinned and pinner position square is (letter, number)
        //Otherwise returns false
        bool Pawn ::would_capture_pinner(int letter,int number){
            if(pinned_by==NULL){
                return false;
            }
            int pinner_pos_letter=pinned_by->pos_letter;
            int pinner_pos_number=pinned_by->pos_number;

            return (letter==pinner_pos_letter && number==pinner_pos_number);
        }

        //letter and number define the target square
        //Returns true if piece isn't pinned or it is pinned and target square lays on the line of pin
        //Otherwise returns false
        bool Pawn :: moves_along_pin(int letter,int number){
            if(pinned_by==NULL){
                return false;
            }
            char pinner_label=pinned_by->get_piece_label();
            int pinner_pos_letter=pinned_by->pos_letter;
            int pinner_pos_number=pinned_by->pos_number;

            if(pinner_label=='b' || (pinner_label=='Q' && (pos_letter!=pinner_pos_letter && pos_number!=pinner_pos_number))){
                if(letter==pos_letter){
                    return false;
                }
                else if(pinner_pos_letter>pos_letter && letter>pos_letter){
                    if(pinner_pos_number>pos_number && color==1){
                        return true;
                    }
                    else if(pinner_pos_number<pos_number && color==2){
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else if(pinner_pos_letter<pos_letter && letter<pos_letter){
                    if(pinner_pos_number>pos_number && color==1){
                        return true;
                    }
                    else if(pinner_pos_number<pos_number && color==2){
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
            else if(pinner_pos_letter==pos_letter){
                if(letter==pos_letter){
                    return true;
                }
                else{
                    return false;
                }
            }
            else {
                return false;
            }
        }

        bool Pawn :: play_en_passant(int letter,int number){
            std::cout<<"Game_flow anpasant\n";
            if(color==1 && board->squares[letter][number-1]!=NULL){
                std::cout<<"Assigning piece\n";
                Piece *p=board->squares[letter][number-1];
                
                if(p->get_color()==2 && p->get_piece_label()=='p' && p->is_en_passant_enabled() ){

                    clear_attacking_squares();
                    
                    Piece* f=board->squares[letter][number-1];
                    f->clear_attacking_squares();
                    char c=f->get_piece_label();

                    if(c=='Q' || c=='b' || c=='r'){
                        int b=f->get_color();
                        if(b==1){
                            board->possible_white_pinners.erase(f);
                        }
                        else{
                            board->possible_black_pinners.erase(f);
                        }
                    }
                    

                    board->squares[pos_letter][pos_number]=NULL;
                    board->squares[letter][number-1]=NULL;
                    pos_number=number;
                    pos_letter=letter;
                    board->squares[pos_letter][pos_number]=this;
                    return true;
                }
                return false;
            }
            else if(color==2 && board->squares[letter][number+1]!=NULL) {
                Piece* p=(board->squares[letter][number+1]);
                if(p->get_color()==1 && p->get_piece_label()=='p' && p->is_en_passant_enabled()){

                    clear_attacking_squares();
                    
                    Piece* f=board->squares[letter][number+1];
                    f->clear_attacking_squares();
                    char c=f->get_piece_label();

                    if(c=='Q' || c=='b' || c=='r'){
                        int b=f->get_color();
                        if(b==1){
                            board->possible_white_pinners.erase(f);
                        }
                        else{
                            board->possible_black_pinners.erase(f);
                        }
                    }
                    
                    board->squares[pos_letter][pos_number]=NULL;
                    board->squares[letter][number+1]=NULL;
                    pos_number=number;
                    pos_letter=letter;
                    board->squares[pos_letter][pos_number]=this;
                    return true;
                }
                return false;
            }
            else return false;
        }

        bool Pawn:: is_en_passant_enabled(){
            return  this->En_Passant;
        }
        void Pawn::disable_en_passant(){
            En_Passant=false;
        }

        bool Pawn:: sees_opponent_king(){
           
        }

        void Pawn:: calculate_attacking_squares(){

            attacking_squares.clear();
            
            if(color==1){

                int candidate1_letter=pos_letter-1;
                int candidate1_number=pos_number+1;

                int candidate2_letter=pos_letter+1;
                int candidate2_number=pos_number+1;

                if(is_valid_square(candidate1_letter,candidate1_number)){
                    
                    this->attacking_squares.insert(std::make_pair(candidate1_letter,candidate1_number));
                    board->pieces_attacking_the_square[candidate1_letter][candidate1_number].insert(std::make_pair(pos_letter,pos_number));
                    
                }

                if(is_valid_square(candidate2_letter,candidate2_number)){
                    
                    this->attacking_squares.insert(std::make_pair(candidate2_letter,candidate2_number));
                    board->pieces_attacking_the_square[candidate2_letter][candidate2_number].insert(std::make_pair(pos_letter,pos_number));

                }

            }
            if(color==2){

                int candidate1_letter=pos_letter-1;
                int candidate1_number=pos_number-1;

                int candidate2_letter=pos_letter+1;
                int candidate2_number=pos_number-1;

                if(is_valid_square(candidate1_letter,candidate1_number)){
                    
                    this->attacking_squares.insert(std::make_pair(candidate1_letter,candidate1_number));
                    
                }

                if(is_valid_square(candidate2_letter,candidate2_number)){
                   
                    this->attacking_squares.insert(std::make_pair(candidate2_letter,candidate2_number));
                }

            }

            std::cout<<"squares attacked by Pawn"<<pos_letter<<pos_number<<"\n";
            for(std::pair< int, int> p : attacking_squares){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }
        }

        void Pawn :: clear_attacking_squares(){
            for(std::pair<int,int> p : attacking_squares){
                board->pieces_attacking_the_square[p.first][p.second].erase(std::make_pair(pos_letter,pos_number));
            }

            
            attacking_squares.clear();
        }

        

        
   