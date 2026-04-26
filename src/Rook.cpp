
#include<iostream>
#include<vector>
#include<cmath>
#include "Piece.h"
#include "Board.h"
#include "Rook.h"

        Rook::Rook(int color,Board* board,int letter,int number):Piece(letter,number,'r',color){
            this->board=board;
            this->piece_moved=false;
            this->pinning=NULL;
        }

        bool Rook:: move(int letter,int number){
            //Saving current position
            int tmp_letter=pos_letter;
            int tmp_number=pos_number;

            //If target square is not on the same vertical/horizontal line, move is illegal
            if(pos_letter!=letter && pos_number!=number){
                std::cout<<"Prvi if";
                return false;
            }
            else if(attacking_squares.find(std::make_pair(letter,number))!=attacking_squares.end()){

                if((!pinned || would_capture_pinner(letter,number) || moves_along_pin(letter,number)) && (board->squares[letter][number]==NULL || board->squares[letter][number]->get_color()!=this->get_color())){

                    clear_attacking_squares();

                    //Check if move captures opponent's piece
                    if(board->squares[letter][number]!=NULL){
                        
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
                    }
                    //Updating position of a rook
                    board->squares[pos_letter][pos_number]=NULL;
                    pos_letter=letter;
                    pos_number=number;
                    board->squares[pos_letter][pos_number]=this;

                    piece_moved=true;
                    
                    std::set<std::pair<int,int> > set_for_iteration=board->pieces_attacking_the_square[tmp_letter][tmp_number];

                    //For every piece that attacked previous position square of a rook, recalculate attacking squares.
                    for(std::pair<int,int> p : set_for_iteration){
                        std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                        
                        board->squares[p.first][p.second]->clear_attacking_squares();
                        board->squares[p.first][p.second]->calculate_attacking_squares();
                    }
                    
                    set_for_iteration=board->pieces_attacking_the_square[pos_letter][pos_number];

                    //For every piece that attacks current position square of a rook, recalculate attacking squares.
                    for(std::pair<int,int> p : set_for_iteration){
                        std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                        
                        board->squares[p.first][p.second]->clear_attacking_squares();
                        board->squares[p.first][p.second]->calculate_attacking_squares();
                    }
                
                    calculate_attacking_squares();
                    return true;
                }
                else{
                    return false;
                }

            }
            else{
                for(std::pair<int,int> p:attacking_squares){
                    std::cout<<p.first<<p.second<<"\n";
                }
                return false;
            }
            
        }
        bool Rook::is_en_passant_enabled(){return false;}

        void Rook::disable_en_passant(){}
        
        //This method returns true if this piece is pinned and pinner position square is (letter, number)
        //Otherwise returns false
        bool Rook::would_capture_pinner (int letter,int number){
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
        bool Rook:: moves_along_pin(int letter,int number){
            if(pinned_by==NULL){
                return false;
            }
            char pinner_label=pinned_by->get_piece_label();
            int pinner_pos_letter=pinned_by->pos_letter;
            int pinner_pos_number=pinned_by->pos_number;
            //If the rook is pinned by diagonal attack, movement is not possible
            if(pinner_label=='b' || (pinner_label=='Q' && (pinner_pos_letter!=pos_letter && pinner_pos_number!=pinner_pos_number))){
                return false;
            }
            //If the rook is pinned along vertical line
            else if(pos_letter==pinner_pos_letter){

                int king_pos_number;
                if(color==1){
                    king_pos_number=board->White_king.second;
                }
                else{
                    king_pos_number=board->Black_king.second;
                }

                return (number>std::min(king_pos_number,pinner_pos_number) && number<std::max(king_pos_number,pinner_pos_number));
            }
            //Rook is pinned along horizontal line
            else{
                int king_pos_letter;
                if(color==1){
                    king_pos_letter=board->White_king.first;
                }
                else{
                    king_pos_letter=board->Black_king.first;
                }

                return (letter>std::min(king_pos_letter,pinner_pos_letter) && letter<std::max(king_pos_letter,pinner_pos_letter));
            }
            
        }
        
        bool Rook::capture(int letter,int number){
            if(board->squares[letter][number]->get_color()==color){
                return false;
            }
            else if(!pinned || (pinned && would_capture_pinner(letter,number))){
                board->squares[pos_letter][pos_number]=NULL;
                pos_letter=letter;
                pos_number=number;
                board->squares[pos_letter][pos_number]=this;
                return true;
            }
            else
                return false;
        }
        //Cheks if opponent's king is in the line of attack of this piece, regardless of other pieces on the board
        bool Rook:: sees_opponent_king(){
            int king_pos_letter;
            int king_pos_number;
            
            if(color==1){
                king_pos_letter=board->Black_king.first;
                king_pos_number=board->Black_king.second;
            }
            else{
                king_pos_letter=board->White_king.first;
                king_pos_number=board->White_king.second;
            }
            return (pos_letter==king_pos_letter || pos_number==king_pos_number);
        }

        //Determines if any opponent's piece should be pinned by this piece. If so, pinnes it
        void Rook :: pin_if_needed(){
            
            int king_pos_letter,king_pos_number;

            if(color==1){
                king_pos_letter=board->Black_king.first;
                king_pos_number=board->Black_king.second;
            }
            else{
                king_pos_letter=board->White_king.first;
                king_pos_number=board->White_king.second;
            }

            //Candidate is every opponent's piece on the line of attack between this piece and opponent's king
            Piece* candidate=NULL;
            
            //Checks if this rook and opponent's king are on the same vertical line
            if(king_pos_letter==pos_letter){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack
                int bottom=std::min(king_pos_number,pos_number);
                int Rook=std::max(king_pos_number,pos_number);
                for(int i=bottom+1;i<Rook;i++){
                    Piece * f=board->squares[pos_letter][i];
                    if(f!=NULL){
                        if(f->color==color){
                            //On the line of attack is friendly piece
                            return;
                        }
                        else if(candidate!=NULL){
                            //On the line of attack are multipule pieces
                            return;
                        }
                        else{
                            candidate=f;
                        }
                    }
                }
            }

            //Checks if this rook and opponent's king are on the same horizontal line
            else if(king_pos_number==pos_number){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack
                int left=std::min(king_pos_letter,pos_letter);
                int right=std::max(king_pos_letter,pos_letter);

                for(int i=left+1;i<right;i++){
                    Piece* f=board->squares[i][pos_number];
                    if(f!=NULL){
                        if(f->color==color){
                            //On the line of attack is friendly piece
                            return;
                        }
                        else if(candidate!=NULL){
                            //On the line of attack are multipule pieces
                            return;
                        }
                        else{
                            candidate=f;
                        }
                    }
                }
            }
            
            //If valid candidate is found, pin it
            if(candidate!=NULL){
                candidate->pin(this);
                this->pinning=candidate;
            }
        }

        void Rook :: calculate_attacking_squares(){

            attacking_squares.clear();

            for(int i=pos_number+1;i<8;i++){

                attacking_squares.insert(std::make_pair(pos_letter,i));
                board->pieces_attacking_the_square[pos_letter][i].insert(std::make_pair( pos_letter, pos_number));

                if(board->squares[pos_letter][i]!=NULL){
                    break;
                }
            }
            for(int i=pos_number-1;i>=0;i--){

                board->pieces_attacking_the_square[pos_letter][i].insert(std::make_pair( pos_letter, pos_number));
                attacking_squares.insert(std::make_pair( pos_letter,i));

                if(board->squares[pos_letter][i]!=NULL){
                    break;
                }
            }
            for(int i=pos_letter+1;i<8;i++){

                board->pieces_attacking_the_square[i][pos_number].insert(std::make_pair( pos_letter, pos_number));
                attacking_squares.insert(std::make_pair( i,pos_number));

                if(board->squares[i][pos_number]!=NULL){
                    break;
                }
            }
            for(int i=pos_letter-1;i>=0;i--){

                board->pieces_attacking_the_square[i][pos_number].insert(std::make_pair( pos_letter, pos_number));
                attacking_squares.insert(std::make_pair(i , pos_number));

                if(board->squares[i][pos_number]!=NULL){
                    break;
                }
            }

            std::cout<<"squares attacked by Rook"<<pos_letter<<pos_number<<"\n";
            for(std::pair< int, int> p : attacking_squares){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }

            if(sees_opponent_king()){
                std::cout<<this->get_piece_label()<<" "<< pos_letter<< " "<<pos_number << " vidi kralja\n";
                pin_if_needed();
            }

        }

        void Rook :: clear_attacking_squares(){

            if(pinning!=NULL){
                pinning->unpin();
                pinning=NULL;
            }

            for(std::pair< int, int> p : attacking_squares){
                std::cout<<board->pieces_attacking_the_square[p.first][p.second].erase(std::make_pair(pos_letter,pos_number));
                std::cout<<" Obrisano za Rook"<<pos_letter<<pos_number<<" \n";
            }

            attacking_squares.clear();
        }

bool Rook :: get_moved(){
    return piece_moved;
}

void Rook :: set_pos_letter(int letter){
    pos_letter=letter;
}

void Rook :: set_pos_number(int number){
    pos_number=number;
}
