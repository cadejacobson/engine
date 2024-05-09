#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <limits.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NUM_BOUNDARIES 5

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

typedef struct {
    float x, y;
} Vector2D;

typedef struct {
    Vector2D origin;
    Vector2D direction;
} Ray;

typedef struct {
    Vector2D a, b;
} Boundary;

typedef struct {
    float heading;
    int FOV;
    float startAngle;
    float endAngle;
} View;

void drawLine(Vector2D a, Vector2D b) {
    SDL_RenderDrawLine(gRenderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

int intersect(Ray ray, Boundary wall, Vector2D* pt) {
    float x1 = wall.a.x;
    float y1 = wall.a.y;
    float x2 = wall.b.x;
    float y2 = wall.b.y;

    float x3 = ray.origin.x;
    float y3 = ray.origin.y;
    float x4 = ray.origin.x + ray.direction.x;
    float y4 = ray.origin.y + ray.direction.y;

    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (den == 0) {
        return 0;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    if (t > 0 && t < 1 && u > 0) {
        pt->x = x1 + t * (x2 - x1);
        pt->y = y1 + t * (y2 - y1);
        return 1;
    }
    return 0;
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gWindow == NULL || gRenderer == NULL) {
        printf("Window or renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    srand(SDL_GetTicks());

    SDL_Event e;
    int quit = 0;

    Boundary boundaries[NUM_BOUNDARIES];
    for (int i = 0; i < NUM_BOUNDARIES; i++) {
        Vector2D start = { .x = (float)(rand() % SCREEN_WIDTH), .y = (float)(rand() % SCREEN_HEIGHT) };
        Vector2D end = { .x = (float)(rand() % SCREEN_WIDTH), .y = (float)(rand() % SCREEN_HEIGHT) };
        boundaries[i].a = start;
        boundaries[i].b = end;
    }

    Vector2D mousePos = { 0, 0 };

    View view= {0.0, 30, 0.0, 0.0};
    view.startAngle = view.heading - (.5 * view.FOV);
    view.endAngle = view.endAngle + (.5 * view.FOV);

    if(view.startAngle < 0)
        view.startAngle += 360;
    if(view.endAngle > 360)
        view.endAngle -= 360;


    view.startAngle = view.startAngle * (M_PI / 180);
    view.endAngle = view.endAngle * (M_PI / 180);
    // this calculation is in degrees, but the loop below uses radians. Figure your shit out

    printf("%f\n", view.startAngle);
    printf("%f\n", view.endAngle);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                mousePos.x = e.motion.x;
                mousePos.y = e.motion.y;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);

        // Draw walls
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        for (int i = 0; i < NUM_BOUNDARIES; ++i) {
            drawLine(boundaries[i].a, boundaries[i].b);
        }

        // Cast rays from mouse position
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        //for (int angle = view.startAngle; angle < view.endAngle; angle += 1) {
        for(int i = 0; i < 360; i += 1) {
            float angle = i;
            while(angle > 6.2831853071)
                angle -= 6.2831853071;
            if(angle > view.startAngle || angle < view.endAngle){
                Ray ray = { mousePos, {cos(angle), sin(angle)} };
                Vector2D closestPt = { INT_MAX, INT_MAX };
                float closestDist = INFINITY;
                for (int i = 0; i < NUM_BOUNDARIES; ++i) {
                    Vector2D pt;
                    if (intersect(ray, boundaries[i], &pt)) {
                        float dist = sqrt(pow(pt.x - mousePos.x, 2) + pow(pt.y - mousePos.y, 2));
                        if (dist < closestDist) {
                            closestDist = dist;
                            closestPt = pt;
                        }
                    }
                }
                // Draw the line only if a valid intersection point is found
                if (closestPt.x != INT_MAX && closestPt.y != INT_MAX) {
                    drawLine(mousePos, closestPt);
                }
            }
        }

        SDL_RenderPresent(gRenderer);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}
