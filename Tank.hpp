#ifndef TANK
#define TANK
#include "Fish.hpp"
#include "BackgroundWater.hpp"
#include "WebcamPassthrough.hpp"
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>


#include <thread>


class Tank {
    private:
    int height;
    std::mutex mtx;
    int ticks;
    int width;

    sf::RenderWindow * window;
    std::shared_ptr<WebcamPassthrough> water;
    std::vector<std::shared_ptr<PriDrawable> > drawables;
    std::vector<bool> threadFinishLine;
    int fish_number;
    int last_id;
    public:
    Tank(int width, int height) {
        ticks = 0;
        this->height = height;
        this->width = width;
        last_id = 0;
        //water = std::make_shared<BackgroundWater>(width, height);
        water = std::make_shared<WebcamPassthrough>(width, height);
        drawables.push_back(water);
    }

    void simulate(void) {
        int threadNumber = 3;
        threadFinishLine.resize(threadNumber);
        int fishId = 3;
        window = new sf::RenderWindow(sf::VideoMode(width, height), "LUG Fish Tank");
        sf::Event event;
        while (window->pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->setFramerateLimit(60);
        window->setVerticalSyncEnabled(true);
        int fish_number = 5;
        for(int i = 0; i < fish_number; i++) {
            auto new_fish = std::make_shared<Fish>(1, this->width, this->height, 3 + last_id++);
            if(new_fish == nullptr) {
                std::cout << "Cannot allocate for that many fish, exiting" << std::endl;
                exit(1);
            }
            
            drawables.push_back(std::move(new_fish));
        }
        
        for(int i = 0; i < threadNumber; i++) {
            auto newthread = std::thread(&Tank::animateObjectsThread, this, std::ref(drawables), i, threadNumber, std::ref(threadFinishLine));
            newthread.detach();
        }
        while(window->isOpen()) {
            sf::Event event;
            drawObjects();
            while(window->pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    window->close();
            }

        }
    }

    void animateObjectsThread(std::vector<std::shared_ptr<PriDrawable> > & objects, int id, int threadCount, std::vector<bool> & finish) { 
        while(true) {
            if(!finish[id]) {
                if(id == 0) {
                    if(objects[0] != nullptr) {
                        objects[0]->animate();
                    }
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
                        
                        potentialIndex += (threadCount - 1);
                        //std::cout << "Potential index " << potentialIndex << std::endl;
                        
                        
                    }    
                }
                finish[id] = true;
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

    void drawObjects(void) {
        window->clear();
        while(!threadsFinished()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        std::vector<std::shared_ptr<PriDrawable> > temp;
        for(int i = 0; i < drawables.size(); i++) {
            std::shared_ptr<PriDrawable> & item = drawables[i];
            //std::cout << "Drew item with priority " << item->priority << std::endl;
            if(item.get() != nullptr) {
                window->draw((*(item)));
                temp.push_back(std::move(drawables[i]));
             }
             
        }
        
        window->display();
        std::make_heap(temp.begin(), temp.end(), PriCompare());
        drawables = std::move(temp);
        std::cout << "drawables size " << drawables.size() << std::endl; 
        resetFinishLine();
    }

};


#endif
