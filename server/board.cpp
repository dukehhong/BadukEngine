#include "board.h"
#include <iostream>

Board::Board(int n){
    this->dim = n;
    this->turn = 1;

    this->board = new int*[n+2];
    for(int i = 0; i<n+2; i++){
        this->board[i] = new int[n+2];
        for(int j = 0; j<n+2; j++){
            if(i == 0 || i == n+1){
                this->board[i][j] = -1;
            }else if(j == 0 || j == n+1){
                this->board[i][j] = -1;
            }else{
                this->board[i][j] = 0;
            }
        }
    }
}

void Board::disp(){
    for(int i = 0; i<this->dim+2; i++){
        for(int j = 0; j<this->dim+2; j++){
            if(this->board[i][j] == -1){
                std::cout << "W" << " ";
            }else if(this->board[i][j] == 0){
                std::cout << "_" << " ";
            }else if(this->board[i][j] == 1){
                std::cout << "B" << " ";
            }else{
                std::cout << "W" << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool Board::isValidMove(int color, int i, int j){
    if(this->board[i][j] == 0){
        return true;
    }
    return false;
}

bool Board::addStone(int color, int i, int j){
    if(this->isValidMove(color, i, j)){
        this->board[i][j] = color;
        return true;
    }
    return false;
}
