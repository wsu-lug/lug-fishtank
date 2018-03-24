#include<opencv2/opencv.hpp>
#include "PriDrawable.hpp"
//#include <X11/Xlib.h> 

class WebcamPassthrough : public PriDrawable {
    private:
    cv::VideoCapture cap;
    cv::Mat frameRGB;

    int width, height;
    int counter;
    public:
    WebcamPassthrough(int width, int height, SDL_Renderer * renderer) : PriDrawable(0, renderer, width, height) {
        cap = cv::VideoCapture(-1);
        this->width = width;
        this->height = height;
        counter = 0;
        
        if(!cap.isOpened()) {
            return;
        }
        
        //cap.set(CV_CAP_PROP_FRAME_WIDTH,400);
        //cap.set(CV_CAP_PROP_FRAME_HEIGHT,300);
        cap >> frameRGB;
        // auto thing = sf::seconds(1);
        // sf::sleep(thing);
        //cv::cvtColor(frameRGB,frameRGBA,cv::COLOR_BGR2RGBA); 
        std::cout << "FUCK" << std::endl;
        IplImage opencvimg2 = (IplImage)frameRGB;
        IplImage* opencvimg = &opencvimg2;
        SDL_Surface * frameSurface = SDL_CreateRGBSurfaceFrom(
                         (void*)opencvimg->imageData,
                         opencvimg->width, opencvimg->height,
                         opencvimg->depth*opencvimg->nChannels,
                         opencvimg->widthStep,
                         0xff0000, 0x00ff00, 0x0000ff, 0);
        std::cout << "FUCK" << std::endl;
        SDL_Texture * vidframe = SDL_CreateTextureFromSurface(renderer, frameSurface);
        SDL_FreeSurface(frameSurface);
        std::cout << "FUCK2" << std::endl;
        textures.push_back(vidframe);
    }
    void animate() {
        if(counter == 0) {
            counter = 0;
            cap >> frameRGB;
            // auto thing = sf::seconds(1);
            // sf::sleep(thing);
            cap >> frameRGB;
            // auto thing = sf::seconds(1);
            // sf::sleep(thing);
            //cv::cvtColor(frameRGB,frameRGBA,cv::COLOR_BGR2RGBA); 
            //std::cout << "FUCK" << std::endl;

            IplImage opencvimg2 = (IplImage)frameRGB;
            IplImage* opencvimg = &opencvimg2;
            SDL_Surface * frameSurface = SDL_CreateRGBSurfaceFrom(
                            (void*)opencvimg->imageData,
                            opencvimg->width, opencvimg->height,
                            opencvimg->depth*opencvimg->nChannels,
                            opencvimg->widthStep,
                            0xff0000, 0x00ff00, 0x0000ff, 0);
            //std::cout << "FUCK" << std::endl;
            SDL_Texture * vidframe = SDL_CreateTextureFromSurface(renderer, frameSurface);
            SDL_FreeSurface(frameSurface);
            //std::cout << "FUCK2" << std::endl;
            
            textures[0] = vidframe;
            
        }
        else {
            counter += 1;
            cap >> frameRGB;
        }
        
    }

    bool isClose(Vector2D) {
        return false;
    }
};
