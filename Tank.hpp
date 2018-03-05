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
    sf::RenderWindow * window;
    std::shared_ptr<BackgroundWater> water;
    std::vector<std::shared_ptr<PriDrawable> > drawables;

    public:
    Tank(int width, int height) {
        ticks = 0;
        this->height = height;
        this->width = width;
        water = std::make_shared<BackgroundWater>(width, height);
        drawables.push_back(water);
    };

    void simulate(void) {
        int fishId = 3;
        window = new sf::RenderWindow(sf::VideoMode(width, height), "LUG Fish Tank");
        window->setFramerateLimit(60);
        window->setVerticalSyncEnabled(true);
        for(int i = 0; i < 4; i++) {
            drawables.push_back(std::make_shared<Fish>(1, this->width, this->height, 3 + fishId++));
        }
        

        while(window->isOpen()) {
            sf::Event event;
            drawObjects();
            while (window->pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    window->close();
            }

        }
    };

    void animateObjectsThread(std::shared_ptr<PriDrawable> d) {
        d->animate();
    }

    void drawObjects(void) {
        window->clear();
        std::make_heap(drawables.begin(), drawables.end(), PriCompare());
        std::cout << "_______" << std::endl;
        for(int i = 0; i < drawables.size(); i++) {
            std::shared_ptr<PriDrawable> item = drawables[i];
            //std::cout << "Drew item with priority " << item->priority << std::endl;
            window->draw((*(item)));
            std::thread updater(&PriDrawable::animate, item);
            updater.detach();
        }
        ticks = (ticks + 1) % 60;
        if(ticks % 10 == 0) {
            water->nextFrame();
        }
        
        window->display();

    };

};


#endif
