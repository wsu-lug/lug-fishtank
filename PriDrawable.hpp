#ifndef PRIDRAWABLE
#define PRIDRAWABLE
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <queue>

typedef struct Vector2Df {
    double x;
    double y;
} Vector2Df;

typedef struct Vector2D {
    uint16_t x;
    uint16_t y;
} Vector2D;


typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

// Everything starts out pointed

class PriDrawable {
    private:
    SDL_Rect position;
    SDL_Rect rect;
    
    
    
    public:
    int priority;
    double angle; 
    double scaleFactor;
    int currentTextureIndex;
    typedef enum Direction {Left, Right};
    Direction dir;
    std::vector<SDL_Texture *> textures;
    SDL_Renderer * renderer;
    SDL_Surface * surface;
    PriDrawable(int priority, SDL_Renderer * theRenderer, int width, int height) {
        rect.x = 0;
        rect.y = 0;
        rect.h = height;
        rect.w = width;
        scaleFactor = 1;
        angle = 0;
        position.h = rect.h;
        position.w = rect.w;
        position.x = 0;
        position.y = 0;
        this->priority = priority;
        this->renderer = theRenderer;
        currentTextureIndex = 0;
        //this->surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    }

    

    // Filepath should be to a png
    bool addNewTexture(std::string filepath) {
        bool returnStatus = true;
        SDL_Surface * newSurface = IMG_Load(filepath.c_str());
        rect.h = newSurface->h;
        rect.w = newSurface->w;
        if(newSurface == nullptr) {
            std::cout << "GRUFK" << std::endl;
        }
        SDL_SetColorKey(newSurface, SDL_TRUE, SDL_MapRGBA(newSurface->format, 0xFF, 0xFF, 0xFF, 0xFF));
        SDL_Texture * newTexture = SDL_CreateTextureFromSurface(renderer, newSurface);
        SDL_FreeSurface(newSurface);
        textures.push_back(newTexture);
        if(newTexture == nullptr) {
            std::cout << "HELP!" << std::endl;
            std::cout << SDL_GetError() << std::endl;
        }
    }

    void flipHorizontal() {
        //SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        //SDL_RenderCopyEx(renderer, textures[currentTextureIndex], &rect, &position, 0, nullptr, flip);

    }

    Vector2Df getScale() {
        return {.x = scaleFactor, .y = scaleFactor};
    }

    void setRotation(double newAngle) {
        newAngle -= angle;
        angle += newAngle;
        
    }

    void rotate(double newAngle) {
        angle += newAngle;
        //SDL_RenderCopyEx(renderer, textures[currentTextureIndex], &rect, &position, angle, nullptr, SDL_FLIP_NONE);
    }

    void setPosition(uint16_t x, uint16_t y) {
        position.x = x;
        position.y = y;

    }

    void setPosition(Vector2Df vec) {
        position.x = vec.x;
        position.y = vec.y;

    }

    void scale(double newScaleFactor) {
        scaleFactor *= newScaleFactor;
    }

    static void setPixel(SDL_Surface * temp, int x, int y, uint32_t color) {
        Uint8 * pixel = (Uint8*)temp->pixels;
        pixel += (y * temp->pitch) + (x * sizeof(Uint32));
        *((Uint32*)pixel) = color;
        //uint32_t * target_pixel_address = (uint32_t *)(temp->pixels) + (y * temp->pitch) + (x * sizeof(uint32_t *));
        //*((uint32_t * )target_pixel_address) = pixel;
    }

    void setPixel(int x, int y, uint32_t pixel) {
        setPixel(this->surface, x, y, pixel);
    }

    static uint32_t buildColor(Color color) {
        uint32_t holder = color.r;
        holder = holder << 8;
        holder = holder | color.g;
        holder = holder << 8;
        holder = holder | color.b;
        holder = holder << 8;
        holder = holder | color.a;
        //std::cout << "Built color " << holder << std::endl;
        return holder;
    }

    const uint16_t getX(void) {
        return position.x;
    }

    const uint16_t getY(void) {
        return position.y;
    }

    const int getWidth(void) {
        return rect.h;
    }

    const int getHeight(void) {
        return rect.w;
    }

    const int getDrawnWidth(void) {
        return position.h;
    }

    const int getDrawnHeight(void) {
        return position.w;
    }

    const Vector2D getPosition(void) {
        return Vector2D {.x = getX(), .y = getY()};
    }
    void draw() {
        SDL_Rect tempSrc = position;
        tempSrc.h *= scaleFactor;
        tempSrc.w *= scaleFactor;
        std::cout << "HELP!!!!!!!!!" << std::endl;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if(dir == Right) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        SDL_RenderCopyEx(renderer, textures[currentTextureIndex], &rect, &position, angle, nullptr, flip);
    }

    void setScale(double x, double y) {
        scaleFactor = x; // ha this doesn't do much
    }

    void nextFrame() {
        currentTextureIndex = (currentTextureIndex + 1) % textures.size();
        //setTexture(*textures[currentTextureIndex]);
    }
    
    void resetToDefaultTexture() {
        //setTexture(*textures[currentTextureIndex]);
    }
    
    ~PriDrawable() {
        for(int i = 0; i < textures.size(); i++) {
            SDL_DestroyTexture(textures[i]);
        }
        SDL_FreeSurface(surface);
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
