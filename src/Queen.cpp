#include<iostream>
#include<cmath>
#include<vector>
#include "Piece.h"
#include "Board.h"
#include "Queen.h"

    Queen::Queen(int color,Board* board,int letter,int number):Piece(letter,number,'Q',color){
        this->board=board;
        this->pinning=NULL;
    }

    bool Queen::move(int letter,int number){
        //Saving current position
        int tmp_letter=pos_letter;
        int tmp_number=pos_number;

        if(attacking_squares.find(std::make_pair(letter,number))!=attacking_squares.end()){
            std::cout<<"Entered";
            
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

                //Updating position of a queen
                board->squares[pos_letter][pos_number]=NULL;
                pos_letter=letter;
                pos_number=number;
                board->squares[pos_letter][pos_number]=this;

                std::set<std::pair<int,int> > set_for_iteration=board->pieces_attacking_the_square[tmp_letter][tmp_number];
                
                //For every piece that attacked previous position square of a queen, recalculate attacking squares.
                for(std::pair<int,int> p : set_for_iteration){
                    
                    board->squares[p.first][p.second]->clear_attacking_squares();
                    board->squares[p.first][p.second]->calculate_attacking_squares();
                }

                set_for_iteration=board->pieces_attacking_the_square[pos_letter][pos_number];

                //For every piece that attacks current position square of a queen, recalculate attacking squares. 
                for(std::pair<int,int> p : set_for_iteration){
                    
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
            
            return false;
        }
    } 
        
        bool Queen::is_en_passant_enabled() { return false; }

        void Queen::disable_en_passant(){}

        bool Queen::capture(int letter,int number){
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

        //This method returns true if this piece is pinned and pinner position square is (letter, number)
        //Otherwise returns false
        bool Queen::would_capture_pinner(int letter,int number){
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
        bool Queen :: moves_along_pin(int letter, int number){
            if(pinned_by==NULL){
                return false;
            }
            char pinner_label=pinned_by->get_piece_label();
            int pinner_pos_letter=pinned_by->pos_letter;
            int pinner_pos_number=pinned_by->pos_number;

            int king_pos_letter,king_pos_number;

            if(color==1){
                king_pos_letter=board->White_king.first;
                king_pos_number=board->White_king.second;
            }
            else{
                king_pos_letter=board->Black_king.first;
                king_pos_number=board->Black_king.second;
            }
            //Checks if queen is pinned along diagonal line
            if(pinner_label=='b' || (pinner_label=='Q' && (pinner_pos_letter!=pos_letter && pinner_pos_number!=pinner_pos_number))){
                int difference_letter=letter-king_pos_letter;
                int difference_number=number-king_pos_number;
                if(std::abs(difference_letter)!=std::abs(difference_number)){
                    return false;
                }
                if(pinner_pos_letter>king_pos_letter){
                    if(pinner_pos_number>king_pos_number){
                        if(difference_letter>0 && difference_number>0 && difference_letter<pinner_pos_letter-king_pos_letter){
                            return true;
                        }
                            
                    }
                    else if(pinner_pos_number<king_pos_number){
                        if(difference_letter>0 && difference_number<0 && difference_letter<pinner_pos_letter-king_pos_letter){
                            return true;
                        }
                        
                    }
                    else {
                        return false;
                    }

                }
                else{
                    if(pinner_pos_number>king_pos_number){
                        if(difference_letter<0 && difference_number>0 && difference_number<pinner_pos_number-king_pos_number){
                            return true;
                        }
                        else {
                            return false;
                        }
                            
                   }
                    else if(pinner_pos_number<king_pos_number){
                        if(difference_letter<0 && difference_number<0 && difference_letter>pinner_pos_letter-king_pos_letter){
                            return true;
                        }
                        
                  }
                    else {
                        return false;
                    }
                } 
                    
        
            }
            //Cheks if queen is pinned along vertical line
            else if(pos_letter==pinner_pos_letter){
                
                return (number>std::min(king_pos_number,pinner_pos_number) && number<std::max(king_pos_number,pinner_pos_number));
            }
            //Queen is pinned along horizontal line
            else{
                
                return (letter>std::min(king_pos_letter,pinner_pos_letter) && letter<std::max(king_pos_letter,pinner_pos_letter));
            }
            
        }
        

        //Cheks if opponent's king is in the line of attack of this piece, regardless of other pieces on the board
        bool Queen:: sees_opponent_king(){
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

            if(pos_letter==king_pos_letter || pos_number==king_pos_number){
                return true;
            }
            else{
                int difference_letter=std::abs(pos_letter-king_pos_letter);
                int difference_number=std::abs(pos_number-king_pos_number);

                return difference_letter==difference_number;
            }
            
        }

        //Determines if any opponent's piece should be pinned by this piece. If so, pinnes it
        void Queen :: pin_if_needed(){
            int king_pos_letter,king_pos_number;

            if(color==1){
                king_pos_letter=board->Black_king.first;
                king_pos_number=board->Black_king.second;
            }
            else{
                king_pos_letter=board->White_king.first;
                king_pos_number=board->White_king.second;
            }

            //Candidate is every piece on the line of attack between this piece and opponent's king
            Piece* candidate=NULL;

            //Checks if this queen and opponent's king are on the same diagonal line (down, left)
            if(pos_letter>king_pos_letter && pos_number > king_pos_number){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack
                
                for(int i=king_pos_letter+1,j=king_pos_number+1;i<pos_letter && j< pos_number; i++,j++){
                    Piece* f=board->squares[i][j];
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
            //Checks if this queen and opponent's king are on the same diagonal line (down, right)
            else if(pos_letter<king_pos_letter && pos_number<king_pos_number){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack
                
                for(int i=pos_letter+1,j=pos_number+1;i<king_pos_letter && j< king_pos_number; i++,j++){
                    Piece* f=board->squares[i][j];
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
            //Checks if this queen and opponent's king are on the same diagonal line (up, left)
            else if(pos_letter>king_pos_letter && pos_number<king_pos_number){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack

                for(int i=pos_letter-1,j=pos_number+1;i>king_pos_letter && j<king_pos_number;i--,j++){

                    Piece* f=board->squares[i][j];
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
            //Checks if this queen and opponent's king are on the same diagonal line (down, right)
            else if(pos_letter<king_pos_letter && pos_number>king_pos_number){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack

                for(int i=pos_letter+1,j=pos_number-1;i<king_pos_letter && j>king_pos_number;i++,j--){

                    Piece* f=board->squares[i][j];
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
            //Checks if this queen and opponent's king are on the same vertical line
            else if(king_pos_letter==pos_letter){
                //Candidate, if exists, should only be pinned if it's the only piece on the line of attack
                int bottom=std::min(king_pos_number,pos_number);
                int top=std::max(king_pos_number,pos_number);
                for(int i=bottom+1;i<top;i++){
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
            //Queen and opponent's king are on the same horizontal line
            else{
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

        void Queen :: calculate_attacking_squares(){
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
        
        for(int i=pos_letter+1, j=pos_number+1;i<8 && j<8;i++,j++){
            std::cout<<"\n Now";
            attacking_squares.insert(std::make_pair(i,j));
            board->pieces_attacking_the_square[i][j].insert(std::make_pair(pos_letter,pos_number));

            if(board->squares[i][j]!=NULL){
                break;
            }
        }
        for(int i=pos_letter+1, j=pos_number-1;i<8 &&j>=0;i++,j--){

            attacking_squares.insert(std::make_pair(i,j));
            board->pieces_attacking_the_square[i][j].insert(std::make_pair(pos_letter,pos_number));

            if(board->squares[i][j]!=NULL){
                break;
            }
        }
        for(int i=pos_letter-1, j=pos_number+1;i>=0 && j<8;i--,j++){

            attacking_squares.insert(std::make_pair(i,j));
            board->pieces_attacking_the_square[i][j].insert(std::make_pair(pos_letter,pos_number));

            if(board->squares[i][j]!=NULL){
                break;
            }
        }
        for(int i=pos_letter-1, j=pos_number-1;i>=0 && j>=0;i--,j--){

            attacking_squares.insert(std::make_pair(i,j));
            board->pieces_attacking_the_square[i][j].insert(std::make_pair(pos_letter,pos_number));
           
            if(board->squares[i][j]!=NULL){
                break;
            }
        }

        if(sees_opponent_king()){
            std::cout<<this->get_piece_label()<<" "<< pos_letter<< " "<<pos_number << " sees the king\n";
            pin_if_needed();
        }
            
    }

        void Queen :: clear_attacking_squares(){

            if(pinning!=NULL){
                pinning->unpin();
                pinning=NULL;
            }

            for(std::pair< int, int> p : attacking_squares){
                std::cout<<board->pieces_attacking_the_square[p.first][p.second].erase(std::make_pair(pos_letter,pos_number));
                std::cout<<" Deleted attacking_squares for queen"<<pos_letter<<pos_number<<" \n";
            }

            attacking_squares.clear();
        }