#include<iostream>
#include<stdlib.h>
#include "Board.h"
#include "Pawn.h"
#include "Piece.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include<vector>
#include<set>
#include<cmath>


bool is_valid_square(int letter, int number){
    return (letter>=0 && letter<=7 && number>=0 && number<=7);
}

        //Initializes the board: allocates board matrix, creates pieces, and sets them up in starting position
        void Board ::set_up_starting_position(){

            check_on_white_king=false;
            check_on_black_king=false;

            //Allocating memory
            squares.resize(8);
            pieces_attacking_the_square.resize(8);

            //Setting up starting position
            for(int i = 0; i < 8; i++){
                
                squares[i].resize(8);
                pieces_attacking_the_square[i].resize(8);

                for(int j=0 ; j<8 ;j++){
                    if(j==1){
                        Pawn *p = new Pawn(1,this,i,j);
                        squares[i][j]=p;

                    }
                    else if(j==6){
                        Pawn *p=new Pawn(2,this,i,j);
                        squares[i][j]=p;
                    }
                    else if(j==0 && (i==0 || i==7)){
                        Rook *t=new Rook(1,this,i,j);
                        squares[i][j]=t;
                        possible_white_pinners.insert(t);
                    }
                    else if(j==7 && (i==0 || i==7)){
                        Rook *t=new Rook(2,this,i,j);
                        squares[i][j]=t;
                        possible_black_pinners.insert(t);
                    }
                    else if(j==0 && (i==2 || i==5)){
                        Bishop *l=new Bishop(1,this,i,j);
                        squares[i][j]=l;
                        possible_white_pinners.insert(l);
                    }
                    else if(j==7 && (i==2 || i==5)){
                        Bishop *l=new Bishop(2,this,i,j);
                        squares[i][j]=l;
                        possible_black_pinners.insert(l);
                    }
                    else if(j==0 && (i==1 || i==6)){
                        Knight *k=new Knight(1,this,i,j);
                        squares[i][j]=k;
                    }
                    else if(j==7 && (i==1 || i==6)){
                        Knight *k=new Knight(2,this,i,j);
                        squares[i][j]=k;
                    }
                    else if(j==0 && i==3){
                        Queen *Q=new Queen(1,this,i,j);
                        squares[i][j]=Q;
                        possible_white_pinners.insert(Q);
                    }
                    else if(j==7 && i==3){
                        Queen *Q=new Queen(2,this,i,j);
                        squares[i][j]=Q;
                        possible_black_pinners.insert(Q);
                    }
                    else if(j==0 && i==4){
                        King *K=new King(1,this,i,j);
                        squares[i][j]=K;
                        White_king=std::make_pair(i,j);
                    }
                    else if(j==7 && i==4){
                        King *K=new King(2,this,i,j);
                        squares[i][j]=K;
                        Black_king=std::make_pair(i,j);
                    }
                    else squares[i][j]=NULL;
                 
                }
            }
            //Calculating attacking squares for every piece in starting position
            for(int i=0 ; i<8 ; i++){
                for(int j=0 ; j<8 ;j++){
                    if(j==1 || j==6 || j==0 || j==7 ){
                        squares[i][j]->calculate_attacking_squares();
                    }
                }
            }
        }


        void Board::draw_board(){
                    std::cout<<"\t ";
                    for(int i=0;i<40;i++){
                        std::cout<<"-";
                    }
                    

                    for(int i=7;i>=0;i--){
                                                                             
                            std::cout<<"\n\t"<<i+1;
                            for(int k=0;k<40;k++){
                                if(k%5==0){
                                    std::cout<<"|";
                                }
                                else if(k%5==2){
                                    if(squares[k/5][i]==NULL){
                                        std::cout<<" ";
                                    }
                                    else {
                                        std::cout<<squares[k/5][i]->get_color()<<squares[k/5][i]->get_piece_label();
                                        k++;
                                    }
                                }
                                else 
                                    std::cout<<" ";
                                
                            }
                            
                            
                            
                            std::cout<<"|\n\t ";
                            for(int k=0;k<40;k++){
                                std::cout<<"-";
                            }
                            
                        
                    }
                    std::cout<<"\n\t ";
                            char letter='a';
                        for(int k=0;k<40;k++){
                                if(k%5==2){
                                    std::cout<<letter;
                                    letter++;
                                }
                                else{
                                    std::cout<<" ";
                                }
                        }
                    std::cout<<"\n";

                }



        Piece* Board:: piece_on_square(char letter, int number){
            if(number>8 || number <1) {
                std::cout<<"Invalid number";
                exit(0);
            }

            else if(letter<'a' || letter >'h') {
                 std::cout<<"Invalid letter";
                 exit(0);
            }
            
            return squares[letter-'a'][number-1];
        }

//Checks if square [letter, number] is attacked by opponent's piece (piece of different color)
bool Board :: attacked_square(int letter,int number,int color){
    for(std::pair<int,int> p: pieces_attacking_the_square[letter][number]){
        if(squares[p.first][p.second]->get_color()!=color){
            return true;
        }
    }
      
}

//If king is in check, this method determines will moving piece(color) to square[letter,number] defend check (by capturing attacker, or blocking)
//Returns true if check will be blocked, false otherwise
bool Board :: defending_check(int letter,int number, int color){
    int king_pos_number;
    int king_pos_letter;
    if(check_on_white_king){
        king_pos_letter=White_king.first;
        king_pos_number=White_king.second;
    }
    else if(check_on_black_king){
        king_pos_letter=Black_king.first;
        king_pos_number=Black_king.second;
    }

        //Determing set of king's attackers
        std::set<std::pair<int,int> > attackers;
        for(std::pair<int,int> p : pieces_attacking_the_square[king_pos_letter][king_pos_number]){
            if(squares[p.first][p.second]->get_color()!=color){
                attackers.insert(p);
            }
        }

        //If king has multiple attackers, check is undefendable by moving other piece
        if(attackers.size()>1){
            return false;
        }
        //King only has one attacker
        else if(attackers.size()==1){
            for(std:: pair<int , int> p :attackers){

                int attacker_pos_letter=p.first;
                int attacker_pos_number=p.second;
                char label=squares[attacker_pos_letter][attacker_pos_number]->get_piece_label();

                //If the move is capturing attacker of the king, that move defends check (check if pinning should be checked)
                if(attacker_pos_letter==letter && attacker_pos_number==number){
                    return true;
                }
                //If not by capture, check is defendable by other piece only by placing it in between king and attacker - by blocking it
                //If the attacker is knight or pawn, it cannot be blocked
                else if(label=='k' || label=='p'){
                    return false;
                }
                //Checks if line of attack is horizontal or vertical, by rook or queen
                else if(label=='r' || (label=='Q' && (attacker_pos_letter==king_pos_letter || king_pos_number==attacker_pos_number))){
                    //Vertical line of attack
                    if(attacker_pos_letter==king_pos_letter){
                        if(letter!=attacker_pos_letter){
                            return false;
                        }
                        else if(number>king_pos_number && number<attacker_pos_number){
                            return true;
                        }
                        else if(number<king_pos_number && number>attacker_pos_number){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                    //Horizontal line of attack
                    else{
                        if(number!=king_pos_number){
                            return false;
                        }
                        else if(letter>king_pos_letter && letter<attacker_pos_letter){
                            return true;
                        }
                        else if(letter<king_pos_letter && letter>attacker_pos_letter){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                }
                //Checks if line of attack is diagonal, by bishop or queen
                else if(label=='b' || label=='Q'){
                    int difference_letter=letter-king_pos_letter;
                    int difference_number=number-king_pos_number;
                    if(std::abs(difference_letter)!=std::abs(difference_number)){
                        return false;
                    }
                    if(attacker_pos_letter>king_pos_letter){
                        if(attacker_pos_number>king_pos_number){
                            if(difference_letter>0 && difference_number>0 && difference_letter<attacker_pos_letter-king_pos_letter){
                                return true;
                            }
                            
                        }
                        else if(attacker_pos_number<king_pos_number){
                            if(difference_letter>0 && difference_number<0 && difference_letter<attacker_pos_letter-king_pos_letter){
                                return true;
                            }
                        
                        }
                        else {
                            return false;
                        }

                    }
                    else{
                        if(attacker_pos_number>king_pos_number){
                            if(difference_letter<0 && difference_number>0 && difference_number<attacker_pos_number-king_pos_number){
                                return true;
                            }
                            else {
                                return false;
                            }
                            
                        }
                        else if(attacker_pos_number<king_pos_number){
                            if(difference_letter<0 && difference_number<0 && difference_letter>attacker_pos_letter-king_pos_letter){
                                return true;
                            }
                        
                        }
                        else {
                            return false;
                        }
                    } 
                    
                }

            }
        }
        else{
            std::cout<<"\n\nSomething is not right\n\n";
            return false;
        }
}
        //Returns set of squares on diagonal line between square[letter_from, number_from] and square[letter_to, number_to]
        std::set<std::pair<int, int>> Board :: squares_on_diagonal(int letter_from,int number_from, int letter_to,int number_to){
            int tmp_letter,tmp_number;
            if(letter_from<letter_to){
                tmp_letter=1;
            }
            else{
                tmp_letter=-1;
            }
            if(number_from<number_to){
                tmp_number=1;
            }
            else{
                tmp_number=-1;
            }
            std::set<std::pair<int, int > > squares_in_between;
            for(int i=letter_from+tmp_letter,j=number_from+tmp_number;i!=letter_to && j!=number_to;i+=tmp_letter,j+=tmp_number){
                squares_in_between.insert(std::make_pair(i,j));
            }
            return squares_in_between;
        }


//Determines if king(color) is checkmated
bool Board :: checkmate(int color){
    std::cout<<"Entered checkmate method\n";
    King* k;
    if(color==1){
        if(!check_on_white_king){
           return false; 
        }
        
        k=dynamic_cast<King*>(squares[White_king.first][White_king.second]);
    }
    else{
        if(!check_on_black_king){
            return false;
        }
        std::cout<<squares[Black_king.first][Black_king.second]->get_piece_label()<<" label\n";
        k=dynamic_cast<King*>(squares[Black_king.first][Black_king.second]);
    }
    std::cout<<"Entered checkmate method 1.5\n";
    std::cout<<k->get_piece_label()<<"\n";

    //If king can move, it is not checkmate
    if(k->can_move()){
        return false;
    }

    Piece* attacker;
    int counter=0;

    //Determines king's attackers
    for(std::pair<int,int> p : pieces_attacking_the_square[k->pos_letter][k->pos_number]){
        if(squares[p.first][p.second]->get_color()!=color){
            counter++;
            if(counter>1){
                //King has multiple attackers
                //Becuase king can't move, check is undefendable by other piece
                return true;
            }
            attacker=squares[p.first][p.second];
            
        }
    }
    std::cout<<"Entered checkmate method 3\n";

    //Checks if attacker is capturable by other piece
    //If friendly piece, which is not pinned, attacks the king's attacker, check is defendable
    for(std::pair<int,int> p : pieces_attacking_the_square[attacker->pos_letter][attacker->pos_number]){
        Piece* f=squares[p.first][p.second];
        if(f->get_color()==color && !f->pinned && f->get_piece_label()!='K'){
            return false;
        }
    }
    std::cout<<"Entered checkmate method 4\n";

    //King can't move, and attacker is not capturable by any friendy piece
    //Only option to defend check is to block it by friendly piece

    //If knight is attacker, block is not possible
    if(attacker->get_piece_label()=='k'){
        return true;
    }
    //Checks if line of attack is vertical
    else if(attacker->pos_letter==k->pos_letter){
        int min=std::min(attacker->pos_number,k->pos_number);
        int max=std::max(attacker->pos_number,k->pos_number);

        //If friendly piece can be moved to any square in between king and attacker, the check is defendable
        for(int i=min+1;i<max;i++){
            for(std::pair<int,int> p : pieces_attacking_the_square[attacker->pos_letter][i]){
                Piece* f=squares[p.first][p.second];
                if(f->get_color()==color && !f->pinned && f->get_piece_label()!='K'){
                    return false;
                }
            } 
        }
    }
    //Checks if line of attack is horizontal
    else if(attacker->pos_number==k->pos_number){
        std::cout<<"Entered checkmate method 5\n";
        int min=std::min(attacker->pos_letter,k->pos_letter);
        int max=std::max(attacker->pos_letter,k->pos_letter);

        //If friendly piece can be moved to any square in between king and attacker, the check is defendable
        for(int i=min+1;i<max;i++){
            for(std::pair<int,int> p : pieces_attacking_the_square[i][attacker->pos_number]){
                Piece* f=squares[p.first][p.second];
                if(f->get_color()==color && !f->pinned && f->get_piece_label()!='K'){
                    return false;
                }
            } 
        }
    }
    //King is attacked by diagonal line
    else{
        std::cout<<"Entered checkmate method 6\n";
        std::set<std::pair<int, int > > squares_in_between=squares_on_diagonal(attacker->pos_letter,attacker->pos_number,k->pos_letter,k->pos_number);

        //If friendly piece can be moved to any square in between king and attacker, the check is defendable
        for(std::pair<int,int> p1 : squares_in_between){
            for(std::pair<int, int> p2 : pieces_attacking_the_square[p1.first][p1.second] ){
                Piece* f=squares[p2.first][p2.second];
                if(f->get_color()==color && !f->pinned && f->get_piece_label()!='K'){
                    return false;
                }
            }
        }
    }
    //King can't move, attacker is uncapturable, and check is unblockable- it's checkmate
    return true;
    
    
}

