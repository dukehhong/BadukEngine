#include<iostream>
#include<string>
#include<cmath>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include"../game_files/board.h"

#define DIM 19
#define boxsize 70
#define margin 70

//coordinate struct
struct Pos{
    int x;
    int y;
    Pos(int x, int y){
        this->x = x;
        this->y = y;
    }
};

//pixel to coordinate
Pos p2c(int x, int y){
    int new_x = round((x - (double)margin)/boxsize);
    int new_y = round((y - (double)margin)/boxsize);
    return Pos(abs(new_x), abs(new_y));
}

//coordinate to pixel
Pos c2p(int x, int y){
    int new_x = abs((x*boxsize) + margin);
    int new_y = abs((y*boxsize) + margin);
    return Pos(new_x, new_y);
}

int main(){
    int WIDTH = (boxsize * (DIM - 1)) + (2 * margin);
    int HEIGHT = WIDTH;
    int board_size = WIDTH;
    int cradius = (boxsize/7) + 1;
    int coffest = -2;
    int sradius = boxsize/(2);

    Board game(19);

    sf::Texture btexture;
    sf::Texture wtexture;
    sf::Texture bgtexture;
    //load stone textures
    if(!btexture.loadFromFile("images/bimg.png") || !wtexture.loadFromFile("images/wimg.png") || !bgtexture.loadFromFile("images/bg.png")){
        perror("Texture loading error");
        return EXIT_FAILURE;
    }
    btexture.setSmooth(true);
    // wtexture.setSmooth(true);
    bgtexture.setRepeated(true);

    //init window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Baduk");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    //board background
    sf::RectangleShape board(sf::Vector2f( board_size, board_size));
    board.setFillColor(sf::Color(255, 255, 255));
    board.setTexture(&bgtexture);
    //draw lines
    sf::RectangleShape lat[DIM];
    sf::RectangleShape lon[DIM];
    for(int i = 0; i< DIM; i++){
        lat[i] = sf::RectangleShape(sf::Vector2f(3, 18 * boxsize));
        lat[i].setOrigin(-1*(margin + i*boxsize), -1*margin);
        lat[i].setFillColor(sf::Color(0,0,0));
        lon[i] = sf::RectangleShape(sf::Vector2f(18 * boxsize, 3));
        lon[i].setOrigin(-1 * margin, -1 * (margin + i * boxsize));
        lon[i].setFillColor(sf::Color(0, 0, 0));
    }
    //start points
    sf::CircleShape circ[5];
    int coord[2] = {3, 15};
    circ[4] = sf::CircleShape(cradius);
    Pos temp = c2p(9,9);
    circ[4].setOrigin(-1*temp.x + (cradius) + coffest, -1*temp.y + (cradius) + coffest);
    circ[4].setFillColor(sf::Color(0,0,0));
    for(int i = 0; i<4; i++){
        circ[i] = sf::CircleShape(boxsize/7);
        Pos temp = c2p(coord[i/2], coord[i%2]);
        int new_x = -1*(temp.x - cradius - coffest);
        int new_y = -1*(temp.y - cradius - coffest);
        circ[i].setOrigin(new_x, new_y);
        circ[i].setFillColor(sf::Color(0,0,0));
    }

    while (window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2i click = sf::Mouse::getPosition(window);
                    Pos temp = p2c(click.x, click.y);
                    int x = temp.x + 1;
                    int y = temp.y + 1;
                    game.play(y, x);
                    game.disp();
                }
            }
        }
        window.clear();
        window.draw(board);
        //draw grid
        for(int i = 0; i<DIM; i++){
            window.draw(lat[i]);
            window.draw(lon[i]);
        }
        //draw circles
        for(int i =0; i<5; i++){
            window.draw(circ[i]);
        }
        //update board state
        for(int i = 1; i<21; i++){
            for(int j = 1; j<21; j++){
                int color = game.getBoard()[i][j];
                if(color == 1 || color == 2){
                    sf::CircleShape temp(sradius);
                    Pos coord = c2p(j - 1, i - 1);
                    int xpixel = -1*(coord.x - sradius - coffest);
                    int ypixel = -1*(coord.y - sradius - coffest);
                    temp.setOrigin(xpixel, ypixel);
                    if(color == 1){
                        temp.setTexture(&btexture);
                    }else if(color == 2){
                        temp.setTexture(&wtexture);
                    }
                    window.draw(temp);
                }
            }
        }
        window.display();
    }
    return 0;
}