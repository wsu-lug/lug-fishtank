#include <cmath>
#include <random>
#include <iostream>
#include "PriDrawable.hpp"


class Fish : public PriDrawable {
    private:
    int type;
    typedef enum Health {Excellent, Fair, Poor, Dead};
    Health health;
    double age;
    double speed;
    double angle; // range from -90 to 90 degrees
    double acceleration;
    typedef enum Direction {Left, Right};
    Direction dir;
    typedef enum State {Swimming, Eating, Resting, Fighting};
    State state;
    double scaleFactor;
    int windowwidth;
    int windowheight;

    bool naturalAcc;
    
    public:
    Fish(int type, int width, int height, int id) : PriDrawable(id) {
        health = Excellent;
        age = 0;
        naturalAcc = true;
        speed = 0;
        acceleration = 0;
        dir = Left;
        setPosition(width / 2, height / 2);
        windowwidth = width;
        windowheight = height;
        state = Swimming;
        this->type = type;
        if(type == 1) {
            std::shared_ptr<sf::Texture> temp = std::make_shared<sf::Texture>();
            temp->loadFromFile("/home/czhanacek/Coding/lug-fishtank/images/fish/fish1.png");
            textures.push_back(temp);
            setTexture(*textures[currentTextureIndex]);
            
            scaleFactor = 120.0 / textures[currentTextureIndex]->getSize().x;
            scale(sf::Vector2f(scaleFactor, scaleFactor));
        }
        setOrigin(0.5 * textures[currentTextureIndex]->getSize().x, 0.5 * textures[currentTextureIndex]->getSize().y);
    }

    void changeDirection() {
        if(dir == Left) {
            dir = Right;
            
        }
        else if(dir == Right) {
            dir = Left;
            scale(-1.0f, 1.0f);
        }
    }
    void swim(void) {
        std::uniform_int_distribution<int> dist(1, 100);
        std::random_device rd;
        std::mt19937 e2(rd());
        int action = dist(e2);
        if(state == Swimming) {
            swimTick();
        }
    }

    void animate() {
        swim();


    };

    void setPositionAndAngle() {
        speed += (acceleration / 60.0);
        double currentx = getPosition().x;
        double currenty = getPosition().y;
        int directionMultiplier = 1;
        if(speed > 0) {
            if(getScale().x > 0) {
                setScale(getScale().x * -1, getScale().y);
            }
        }
        if(speed < 0) {
            if(getScale().x < 0) {
                setScale(getScale().x * -1, getScale().y);
            }
        }
        setPosition(currentx + (directionMultiplier * speed * cos(getRads(angle))), currenty + (directionMultiplier * speed * sin(getRads(angle))));
        setRotation(angle);
        
    }

    // Precondition: fish is in swimming state
    void swimTick() {
        modifyAcceleration();
        modifyDirection();
        setPositionAndAngle();
    };

    void modifyDirection() {
        std::uniform_int_distribution<int> dist(1, 3);
        std::random_device rd;
        std::mt19937 e2(rd());
        int shouldChange = dist(e2);
        std::uniform_real_distribution<double> realdist(-2,2);
        angle += realdist(e2);
        if(angle > 40) {
            angle = 40;
        } else if (angle < -40) {
            angle = -40;
        }
        if(getPosition().y < (0.1 * windowheight)) {
            if(getScale().x < 0) {
                angle += 2;
            }
            else {
                angle -= 2;
            }
        }
        if(getPosition().y > (windowheight * 0.9)) {
            if(getScale().x < 0) {
                angle -= 2;
            }
            else {
                angle += 2;
            }
        }

    }


    void modifyAcceleration() {
        std::uniform_int_distribution<int> dist(1, 3);
        std::random_device rd;
        std::mt19937 e2(rd());
        int shouldChange = dist(e2);
        std::uniform_real_distribution<double> realdist(-0.1, 0.1);
        acceleration += realdist(e2);
        if(speed > 3) {
            speed = 3;
        }
        if(speed < -3) {
            speed = -3;
        }
       
        if(getPosition().x < (0.1 * windowwidth)) {
            acceleration = 2;
            naturalAcc = false;
            if(getPosition().x < (0.05 * windowwidth)) {
                acceleration = 15;
            }

        }
        else if(getPosition().x > (0.9 * windowwidth)) {
            acceleration = 2 * -1;
            naturalAcc = false;
            if(getPosition().x > (0.95 * windowwidth)) {
                acceleration = -15;
            }
        }

        if(naturalAcc == false && !((getPosition().x > (0.7 * windowwidth)) || getPosition().x < (0.3 * windowwidth) )) {
            naturalAcc = true;
            acceleration = 0;
        }

        
        std::cout << "Acceleration :: " << acceleration << std::endl;
    };

    double getRads(double degrees) {
        return degrees * ((2 * 3.14159) / 180.0);
    };
};
