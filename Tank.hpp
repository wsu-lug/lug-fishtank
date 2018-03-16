#ifndef TANK
#define TANK
#include "Fish.hpp"
#include "BackgroundWater.hpp"
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>

#include <thread>


class Tank {
    private:
    int height;
    int ticks;
    int width;
    SDL_Surface * screen;
    SDL_Texture * screenTexture;
    SDL_Window * window;
    SDL_Renderer * renderer;
    std::shared_ptr<BackgroundWater> water;
    std::vector<std::shared_ptr<PriDrawable> > drawables;

    public:
    Tank(int width, int height) {
        ticks = 0;
        int imgFlags = IMG_INIT_PNG;
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(imgFlags);
        window = SDL_CreateWindow("LUG Fish Tank", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
        //screen = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        std::cout << "Creating renderer" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        this->height = height;
        this->width = width;
        SDL_SetRenderTarget(renderer, screenTexture);
        screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        water = std::make_shared<BackgroundWater>(width, height, renderer);
        
        
        drawables.push_back(water);
        
        
    };

    void simulate(void) {

        
        int fishId = 3;

        for(int i = 0; i < 50; i++) {
            drawables.push_back(std::make_shared<Fish>(1, this->width, this->height, 3 + fishId++, renderer));
        }
        bool quit = false;
        SDL_Event e;

        while(!quit) {
            drawObjects();
            while (SDL_PollEvent(&e) != 0) {
                // Close window: exit
                if (e.type == SDL_QUIT)
                    quit = true;
            }

        }

    };

    void close() {
        SDL_FreeSurface(screen);
        screen = nullptr;
        SDL_DestroyWindow(window);
        window = nullptr;
        IMG_Quit();
        SDL_Quit();
    }

    void animateObjectsThread(std::shared_ptr<PriDrawable> d) {
        d->animate();
    }

    void drawObjects(void) {
        SDL_RenderClear(renderer);
        std::make_heap(drawables.begin(), drawables.end(), PriCompare());
        std::cout << "_______" << std::endl;
        for(int i = 0; i < drawables.size(); i++) {
            std::cout << "index is " << i << std::endl;
            std::shared_ptr<PriDrawable> item = drawables[i];
            //std::cout << "Drew item with priority " << item->priority << std::endl;
            item->draw();
            std::cout << "Drew" << std::endl;
            item->animate();
            std::cout << "Errors:::" << std::endl;
            std::cout << SDL_GetError() << std::endl;
            //std::thread updater(&PriDrawable::animate, item);
            //updater.detach();
        }
        ticks = (ticks + 1) % 60;
        if(ticks % 10 == 0) {
            water->nextFrame();
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0/60);

    };

};


#endif
