#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int getRandom(int min, int max);

int windowHeight = 600;
int windowLength = 800;

class Boundary {
public:
    int x1;
    int y1;
    int x2;
    int y2;
};

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

    void move() {
        x += dx;
        y += dy;

        if (x < 0) x = 0;
        if (x >= windowLength) x = windowLength - 1;
        if (y < 0) y = 0;
        if (y >= windowHeight) y = windowHeight - 1;
    }
};

int main() {
    srand(time(nullptr));
    vector<Boundary> boundaries;

    for (int i = 0; i < 5; ++i) {
        auto boundary = Boundary{getRandom(0, windowLength),
                                 getRandom(0, windowHeight),
                                 getRandom(0, windowLength),
                                 getRandom(0, windowHeight)};
        boundaries.push_back(boundary);
    }

    Player player(5, 5);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowLength, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            player.handleEvent(event);
        }

        // Move the player
        player.move();

        // Render
        // You can add your rendering code here

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear with black color
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for(auto boundary: boundaries){
            SDL_RenderDrawLine(renderer, boundary.x1, boundary.y1, boundary.x2, boundary.y2);
        }

        SDL_RenderDrawPoint(renderer, player.x, player.y);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        //SDL_Delay(3);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}
