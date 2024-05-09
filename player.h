#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "boundary.h"
#include "ray.h"
#include <cmath>
#include <iostream>

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
    std::vector<Ray> rays;

    Player(double given_x, double given_y, double given_speed = .20){
        x = given_x;
        y = given_y;
        speed = given_speed;
        dx = 0;
        dy = 0;
        max_dx = .1;
        max_dy = .1;
        for(int i = 0; i < 360; i++){
            rays.push_back( Ray(x, y, (i * 3.1415926553) / 180) );
        }
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

    std::vector<std::vector<int>> look(std::vector<Boundary> &boundaries){
        std::vector<std::vector<int>> lines;
        for (int i = 0; i < rays.size(); i++) {
            Ray ray = rays[i];
            //std::cout << "ray: " << ray.x << " " << ray.y << " " << ray.dir << std::endl;
            Point closest = Point(-1, -1);
            double record = INT16_MAX;
            for (auto boundary: boundaries) {
                Point pt = ray.cast(boundary);
                if (pt != Point(INT16_MAX, INT16_MAX)) {
                    double dist = std::sqrt((pt.x - x) * (pt.x - x) + (pt.y - y) * (pt.y - y));
                    if (dist < record) {
                        record = dist;
                        closest = pt;
                    }
                }
            }
            //std::cout << "closest: " << closest.x << " " << closest.y << std::endl;
            if (closest != Point(-1,-1)) {
                //std::cout << "closest: " << closest.x << " " << closest.y << std::endl;

                std::vector<int> line;

                line.push_back(x);
                line.push_back(y);
                line.push_back(closest.x);
                line.push_back(closest.y);
                lines.push_back(line);
            }
        }
        return lines;
    }
};

#endif
