#include "PriDrawable.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <thread>
#include <mutex>

class BackgroundWater : public PriDrawable {
    private:
    std::string name; 
    std::mutex mtx;
    public:
    BackgroundWater(int width, int height, SDL_Renderer * renderer) : PriDrawable(0, renderer, width, height) {
        name = "water";
        std::cout << "Hello from background water" << std::endl;
        setPosition(Vector2Df{.x = 0, .y = 0});
        ticks = 0;
        populateFrames(10);
    }

    void populateFrames(int frames) {
        for(int i = 0; i < frames; i++) {
            std::cout << "Generating frame " << i << " of " << frames << std::endl;
            std::cout << "texture size is " << textures.size() << std::endl; 
            //std::thread textureThread(&BackgroundWater::addFrameToTextures, this);
            //textureThread.detach();
            addFrameToTextures();
        }
        //resetToDefaultTexture();
    }

    void addFrameToTextures() {
        mtx.lock();
        SDL_Texture * newBackground = regenerate();
        textures.push_back(newBackground);
        mtx.unlock();
    }
        
    void animate() {
        if(ticks == 10) {
            ticks = 0;
            if(textures.size() > currentTextureIndex) {
                //setTexture(*textures[currentTextureIndex]);
                currentTextureIndex = (currentTextureIndex + 1) % 10;
            }
        }
        else {
            ticks++;
        }
        
    }

    ~BackgroundWater() {
        std::cout << "DESTROYED!!!!" << std::endl;
    }

    SDL_Texture * regenerate() {
        SDL_Texture * temp = nullptr;
        SDL_Surface * surface = SDL_CreateRGBSurface(0, getWidth(), getHeight(), 32, 0, 0, 0, 0);
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_int_distribution<int> dist(10, 150);
        SDL_LockSurface(surface);
        //SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
        Color currentColor = getNewColor();
        for(int y = 0; y < getHeight(); y++) {
            int x = 0;
            while(x < getWidth()) {
                int iters = dist(e2);
                currentColor = getNewColor();
                for(int i = 0; i < iters; i++)  {
                    if(x + i < getWidth()) {
                        setPixel(surface, x + i, y, buildColor(currentColor));
                        //setPixel(surface, x, y, 0xFFFFFFFF);
                    }
                }
                x += iters;
            }
        }
        SDL_UnlockSurface(surface);
        temp = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return temp;
    }

    Color getNewColor() {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_int_distribution<uint8_t> distribution(10, 25); // seemed to work well
        return Color {.r = 0, .g = (uint8_t)((220 + distribution(e2)) % 255), .b = (uint8_t)((200 + distribution(e2)) % 255), .a = (uint8_t)255};
    }

};
