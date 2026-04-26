#include<iostream>
#include<vector>
#include<stdlib.h>
#include<cmath>
#include<string>

#include "Board.h"
#include "Game_flow.h"



int main(){
    
    Board board=Board();

    board.set_up_starting_position();
    
    Game_flow game=Game_flow(&board);
    game.start_the_game();

    
    return 0;
}