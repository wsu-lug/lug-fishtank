#ifndef PRIDRAWABLE
#define PRIDRAWABLE
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <string>
#include <queue>



class PriDrawable : public sf::Sprite {
    private:
    
    
    
    public:
    int priority;
    int ticks;
    //std::string identifier;

    int currentTextureIndex;
    std::vector<sf::Texture *> textures;
    PriDrawable(int priority) : sf::Sprite() {
        this->priority = priority;

        currentTextureIndex = 0;
    }



    uint32_t generateRandom() {
        static uint32_t x = 123456789;
        static uint32_t y = 362436069;
        static uint32_t z = 521288629;
        static uint32_t w = 88675123;
        uint32_t t;
        t = x ^ (x << 11);   
        x = y; y = z; z = w;   
        return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
    }

    void nextFrame() {
        currentTextureIndex = (currentTextureIndex + 1) % textures.size();
        setTexture(*textures[currentTextureIndex]);
    }
    
    void resetToDefaultTexture() {
        setTexture(*textures[currentTextureIndex]);
    }
    
    virtual void animate() {
        // does nothing, should be overridden
    }
    virtual bool isRotten() {
        return false;
        // does nothing, should be overridden
    }

    virtual ~PriDrawable() {
        //std::cout <<" --------------------------------DELETED" << std::endl;
    }
};

bool operator< (const PriDrawable &l, const PriDrawable &r) {
        return l.priority < r.priority;
}

bool operator< (const std::shared_ptr<PriDrawable> & l, const std::shared_ptr<PriDrawable> & r) {
        return l->priority < r->priority;
}

struct PriCompare {
    bool operator()(const std::shared_ptr<PriDrawable>& lhs, const std::shared_ptr<PriDrawable>& rhs)
    {
        return lhs->priority > rhs->priority;
    }
};


#endif
