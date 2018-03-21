#include "Fish.hpp"

class NearbyDetector {
    private:
    std::vector<std::shared_ptr<PriDrawable> > * draws;
    int width;
    int height;

    public:
    NearbyDetector(std::vector<std::shared_ptr<PriDrawable> > * drawables, int screen_width, int screen_height) {
        draws = drawables;
        height = screen_height;
        width = screen_width;
    }

    Vector2D pickRandomPointInScreen(void) {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_int_distribution<int> dist((int)(width * 0.1), (int)(width * 0.9));
        Vector2D thePoint;
        thePoint.x = dist(e2);
        dist = std::uniform_int_distribution<int>((int)(height * 0.1), (int)(height * 0.9));
        thePoint.y = dist(e2);
        return thePoint;
    }

    bool pointsClose(Vector2D point1, Vector2D point2) {
        if(abs((point1.x - point2.x)) < width * 0.1 && abs(point1.y - point2.y)) {
            return true;
        }
    }
    
    void detect() {
        std::queue<std::shared_ptr<PriDrawable> > things = std::queue<std::shared_ptr<PriDrawable> >();
        Vector2D thePoint = pickRandomPointInScreen();
        Vector2D comparePoint;
        for(int i = 0; i < draws->size(); i++) {
            
            if((*draws)[i]->isClose()) {
                things.push((*draws)[i]);
            }
        }
        
    }

};
