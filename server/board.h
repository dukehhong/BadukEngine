#ifndef BOARD_H
#define BOARD_H

class Board{
    public:
        Board(int n);
        void start();
        void disp();
        bool addStone(int color, int i, int j);
        bool isValidMove(int color, int i, int j);
        int calulateLiberty(int i, int j);
    private:
        int dim;
        int turn;
        int** board;
};

#endif 