#include "Fish.hpp"
#include "BackgroundWater.hpp"
#include "NearbyDetector.hpp"
#include "WebcamPassthrough.hpp"
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#ifndef TANK
#define TANK






class Tank {
    private:
    int height;
    std::mutex mtx;
    int ticks;
    int width;
    SDL_Surface * screen;
    SDL_Texture * screenTexture;
    SDL_Window * window;
    int lastSize;
    SDL_Renderer * renderer;
    std::shared_ptr<WebcamPassthrough> water;
    std::vector<std::shared_ptr<PriDrawable> > drawables;
    std::vector<bool> threadFinishLine;
    int fish_number;
    int last_id;
    public:
    Tank(int width, int height) {
        ticks = 0;
        int imgFlags = IMG_INIT_PNG;
        SDL_Init(SDL_INIT_EVERYTHING);
        //IMG_Init(imgFlags);
        window = SDL_CreateWindow("LUG Fish Tank", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	std::cout << SDL_GetError() << std::endl;
//screen = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        std::cout << "Creating renderer" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        this->height = height;
        this->width = width;
        SDL_SetRenderTarget(renderer, screenTexture);
        screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        water = std::make_shared<WebcamPassthrough>(width, height, renderer);
        
        
        drawables.push_back(std::move(water));
        
        
    };


    void simulate(void) {
        int threadNumber = 3;
        threadFinishLine.resize(threadNumber);
        int fishId = 3;

        int fish_number = 3;
        for(int i = 0; i < fish_number; i++) {
            auto new_fish = std::make_shared<Fish>(1, this->width, this->height, 3 + fishId++, renderer);
            if(new_fish == nullptr) {
                std::cout << "Cannot allocate for that many fish, exiting" << std::endl;
                exit(1);
            }
            
            drawables.push_back(std::move(new_fish));
        }
        lastSize = drawables.size();
        for(int i = 0; i < threadNumber; i++) {
            auto newthread = std::thread(&Tank::animateObjectsThread, this, std::ref(drawables), i, threadNumber, std::ref(threadFinishLine));
            std::cout << "made a thread" << std::endl;
            newthread.detach();
        }
        bool quit = false;
        SDL_Event e;

        while(!quit) {
            drawObjects();
            while (SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }

    };

    void close() {
        SDL_FreeSurface(screen);
        screen = nullptr;
        SDL_DestroyWindow(window);
        window = nullptr;
        //IMG_Quit();
        SDL_Quit();
    }

    void drawObjects(void) {
        SDL_RenderClear(renderer);
        while(!threadsFinished()) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        if(lastSize != drawables.size()) {
            std::make_heap(drawables.begin(), drawables.end());
        }
        for(int i = 0; i < drawables.size(); i++) {
            std::shared_ptr<PriDrawable> & item = drawables[i];
            //std::cout << "Drew item with priority " << item->priority << std::endl;
            if(item.get() != nullptr) {
                item->draw();
                //temp.push_back(std::move(drawables[i]));
             }
             
        }
        
        SDL_RenderPresent(renderer);
        
        resetFinishLine();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }

    void animateObjectsThread(std::vector<std::shared_ptr<PriDrawable> > & objects, int id, int threadCount, std::vector<bool> & finish) { 
        bool renderThreadForBackground = true;
        while(true) {
            if(!finish[id]) {
                if(renderThreadForBackground && id == 0) {
                    objects[0]->animate();
                }
                else {
                    int potentialIndex = id;
                    while(potentialIndex < objects.size()) {
                        if((objects[potentialIndex])->isRotten()) {
                            objects[potentialIndex] = nullptr;
                        }
                        else {
                            objects[potentialIndex]->animate();
                        }
                        if(renderThreadForBackground) {
                            potentialIndex += (threadCount - 1);
                        }
                        else {
                            potentialIndex += 1;
                        }
                        //std::cout << "Potential index " << potentialIndex << std::endl;
                    }
                    
                }
                finish[id] = true;
                
            }
            else {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
           

    }

    bool threadsFinished(void) {
        bool result = true;
        for(int i = 0; i < threadFinishLine.size(); i++) {
            result &= threadFinishLine[i];
        }
        return result;
    }

    void resetFinishLine(void) {
        for(int i = 0; i < threadFinishLine.size(); i++) {
            threadFinishLine[i] = false;
        }
    }

   

};


#endif
