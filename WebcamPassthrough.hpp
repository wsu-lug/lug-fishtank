#include<opencv2/opencv.hpp>
#include <thread>
#include "PriDrawable.hpp"
#include <X11/Xlib.h> 

class WebcamPassthrough : public PriDrawable {
    private:
    cv::VideoCapture cap;
    cv::Mat frameRGB, frameRGBA;
    sf::Image image;
    int width, height;
    int counter;
    public:
    WebcamPassthrough(int width, int height) : PriDrawable(0) {
        cap = cv::VideoCapture(-1);
        this->width = width;
        this->height = height;
        counter = 0;
        
        if(!cap.isOpened()) {
            return;
        }
        
        cap.set(CV_CAP_PROP_FRAME_WIDTH,400);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,300);
        cap.retrieve(frameRGB);
        // auto thing = sf::seconds(1);
        // sf::sleep(thing);
        cv::cvtColor(frameRGB,frameRGBA,cv::COLOR_BGR2RGBA); 
        std::cout << "FUCK" << std::endl;
        if(frameRGBA.empty()) {
            std::cout << "MAJOR" << std::endl;
        }
        image.create(frameRGBA.cols, frameRGBA.rows, frameRGBA.ptr());
        std::cout << "FUCK" << std::endl;
        sf::Texture * vidframe = new sf::Texture();
        vidframe->loadFromImage(image);
        
        std::cout << "FUCK2" << std::endl;
        textures.push_back(vidframe);
        setTexture(*textures[currentTextureIndex]);
        float xscale = width / image.getSize().x;
        float yscale = height / image.getSize().y;
        scale(xscale, yscale);
        auto newthread = std::thread(&WebcamPassthrough::frameGrabberThread, this, std::ref(cap));
        newthread.detach();
    }

    void frameGrabberThread(cv::VideoCapture &cap2) {
        cap2.grab();
    }
    
    void animate() {
        if(counter == 10) {
            counter = 0;
            cap.retrieve(frameRGB)
            // auto thing = sf::seconds(1);
            // sf::sleep(thing);
            cv::cvtColor(frameRGB,frameRGBA,cv::COLOR_BGR2RGBA); 
            cv::resize(frameRGBA, frameRGBA, cv::Size(), 1, 1);
            if(frameRGBA.empty() == false) {
                //image.create(frameRGBA.cols, frameRGBA.rows, frameRGBA.ptr());
                textures[0]->update(frameRGBA.ptr());
                
                float xscale = (float)width / image.getSize().x;
                float yscale = (float)height / image.getSize().y;
                
                setScale(xscale, yscale);
                setPosition(width / 2.0, height / 2.0);
                setOrigin(textures[0]->getSize().x / 2, textures[0]->getSize().y / 2);
                scale(-1, 1);
                
            }
            
        }
        else {
            counter += 1;
        }
        
    }
};
