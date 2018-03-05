#ifndef PRIDRAWABLE
#define PRIDRAWABLE
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <queue>



class PriDrawable : public sf::Sprite {
    private:
    
    
    
    public:
    int priority;

    int currentTextureIndex;
    std::vector<std::shared_ptr<sf::Texture>> textures;
    PriDrawable(int priority) : sf::Sprite() {
        this->priority = priority;

        currentTextureIndex = 0;
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
