#include <SDL.h>

#ifndef ENGINE_PLAYER_H
#define ENGINE_PLAYER_H

class Player {
public:
    double x;
    double y;
    double speed;
    double dx;
    double dy;
    double max_dx;
    double max_dy;

    Player(double given_x, double given_y, double given_speed = .20){
        x = given_x;
        y = given_y;
        speed = given_speed;
        dx = 0;
        dy = 0;
        max_dx = .1;
        max_dy = .1;
    };

    void handleEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    dy -= speed;
                    if(abs(dy) > max_dy)
                        dy = -max_dy;
                    break;
                case SDLK_DOWN:
                    dy += speed;
                    if(abs(dy) > max_dy)
                        dy = max_dy;
                    break;
                case SDLK_LEFT:
                    dx -= speed;
                    if(abs(dx) > max_dx)
                        dx = -max_dx;
                    break;
                case SDLK_RIGHT:
                    dx += speed;
                    if(abs(dx) > max_dx)
                        dx = max_dx;
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    dy = 0;
                    break;
                case SDLK_DOWN:
                    dy = 0;
                    break;
                case SDLK_LEFT:
                    dx = 0;
                    break;
                case SDLK_RIGHT:
                    dx = 0;
                    break;
            }
        }
    }

    void move(int windowHeight, int windowLength) {
        x += dx;
        y += dy;

        if (x < 0) x = 0;
        if (x >= windowLength) x = windowLength - 1;
        if (y < 0) y = 0;
        if (y >= windowHeight) y = windowHeight - 1;
    }
};


#endif
