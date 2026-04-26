#include<iostream>
#include<vector>
#include "Piece.h"
#include "Board.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "Game_flow.h"


    
        Game_flow ::Game_flow(Board* t){
            this->board=t;
            this->player_on_the_move=1;
            this->white_en_passant=NULL;
            this->black_en_passant=NULL;
        }

        void Game_flow:: start_the_game(){
            board->draw_board();
            
            while(true){

                if(board->check_on_white_king){
                    std::cout<<"Check on white king\n";
                }
                else if(board->check_on_black_king){
                    std::cout<<"Check on black king\n";
                }

              

                /*for(int i=0;i<8;i++){
                    for(int j=0;j<8;j++){
                        std::cout<<"Polje "<< i<< " "<< j<<" napadaju:\n";
                        for(std::pair<int,int> p: board->pieces_attacking_the_square[i][j]){
                            std::cout<< p.first<<" "<< p.second<< "\n";
                        }
                    }
                }*/
                    if(player_on_the_move==1){
                        std::cout<<"White";
                        if(white_en_passant!=NULL){
                            white_en_passant->disable_en_passant();
                            white_en_passant=NULL;
                        }
                    }
                    else if(player_on_the_move==2){
                        std::cout<<"Black";
                        if(black_en_passant!=NULL){
                            black_en_passant->disable_en_passant();
                            black_en_passant=NULL;
                        }
                    }
                    
                    std::cout<<" on the move\n";
                    std::string s1,s2;
                    std::cin>>s1>>s2;
                    char label=s1[0];
                    int pos_letter=s1[1]-'a';
                    int pos_number=s1[2]-'1';

                    int letter=s2[0]-'a';
                    int number=s2[1]-'1';

                    

                    if(board->squares[pos_letter][pos_number]!=NULL){
                        Piece *p=board->squares[pos_letter][pos_number];
                        

                        if(p->get_color()!=player_on_the_move){
                            std::cout<<"You can't move opponent's piece\n";
                            board->draw_board();
                            continue;
                        }

                        if(player_on_the_move==1 && board->check_on_white_king && p->get_piece_label()!='K'){
                            if(!board->defending_check(letter,number,player_on_the_move)){
                                std::cout<<"You must defend check\n";

                                board->draw_board();
                                continue;
                            }
                        }
                        
                        else if(player_on_the_move==2 && board->check_on_black_king && p->get_piece_label()!='K'){
                            if(!board->defending_check(letter,number,player_on_the_move)){
                                std::cout<<"You must defend check\n";

                                board->draw_board();
                                continue;
                            }
                        }

                        bool move_made=p->move(letter,number);
                        if(!move_made){
                            std::cout<<"Invalid move\n";
                            board->draw_board();
                        }
                        else{
                            //Move was successful

                            std::unordered_set<Piece *> possible_pinners;

                            //If the king is successfully moved, we must recalculate pinned pieces and their pinners
                            if(label=='K'){
                                if(player_on_the_move==1){
                                    possible_pinners=board->possible_black_pinners;
                                }
                                else{
                                    possible_pinners=board->possible_white_pinners;
                                }

                                for(Piece * p: possible_pinners){

                                    if(p->get_piece_label()=='b'){

                                        Bishop* l=dynamic_cast<Bishop*> (p);

                                        if(l->pinning!=NULL){
                                            l->pinning->unpin();
                                            l->pinning=NULL;
                                        }
                                        if(l->sees_opponent_king()){
                                            l->pin_if_needed();
                                        }
                                    }
                                    else if(p->get_piece_label()=='r'){

                                        Rook* t=dynamic_cast<Rook*> (p);

                                        if(t->pinning!=NULL){
                                            t->pinning->unpin();
                                            t->pinning=NULL;
                                        }
                                        if(t->sees_opponent_king()){
                                            t->pin_if_needed();
                                        }
                                    }
                                    else if(p->get_piece_label()=='Q'){

                                        Queen* q=dynamic_cast<Queen*> (p);

                                        if(q->pinning!=NULL){
                                            q->pinning->unpin();
                                            q->pinning=NULL;
                                        }
                                        if(q->sees_opponent_king()){
                                            q->pin_if_needed();
                                        }
                                    }
                                }
                            }

                            
                            if(board->attacked_square(board->Black_king.first,board->Black_king.second,2)){
                                board->check_on_black_king=true;
                                if(board->checkmate(2)){
                                    std::cout<<"CHECKMATE!\n"<<"WHITE HAS WON!\n";
                                    break;
                                }
                            }
                            else{
                                board->check_on_black_king=false;
                                
                            }
                            

                            
                            if(board->attacked_square(board->White_king.first,board->White_king.second,1)){
                                board->check_on_white_king=true;
                                if(board->checkmate(1)){
                                    std::cout<<"CHECKMATE!\n"<<"BLACK HAS WON!\n";
                                    break;
                                }
                            }
                            else{
                                board->check_on_white_king=false;
                            }

                            
                            //If moved piece was pawn, check is en passant is possible
                            if(s1[0]=='p'){
                                if(p->is_en_passant_enabled() && player_on_the_move==1){
                                    white_en_passant=p;
                                }
                                else if(p->is_en_passant_enabled() && player_on_the_move==2){
                                    black_en_passant=p;
                                }
                            }
                            //Changing player on the move after successful move
                            player_on_the_move=3-player_on_the_move;
                            board->draw_board();
                        }
                    }
                    else{
                        std::cout<<"Empty square\n";
                    }

                
            }
        }
    
    
