#include <cmath>
#include <random>
#include <iostream>
#include "PriDrawable.hpp"
#include <ctime>

#ifndef FISH
#define FISH

class Fish : public PriDrawable {
    private:
    int type;
    typedef enum Health {Excellent, Fair, Poor, Dead};
    Health health;
    double age;
    double presentingAngle;
    double speed;
    double excellentSpeedLimit;
    double fairSpeedLimit;
    double poorSpeedLimit;
    double rotAge;
    double deathAge;
    //double angle; // range from -90 to 90 degrees
    double acceleration;
    double angleacceleration;
    bool floatedToTop;
    typedef enum State {Swimming, Eating, Resting, Fighting};
    State state;
    int breedtimes;
    double scaleFactor;
    int windowwidth;
    int windowheight;

    bool naturalAcc;
    double naturalAge;

    public:
    Fish(int type, int width, int height, int id, SDL_Renderer * renderer) : PriDrawable(id, renderer, 50, 50) {
        health = Excellent;
        breedtimes = 0;
        age = 0;
        std::srand(std::time(0));
        naturalAge = 100;
        naturalAcc = true;
        excellentSpeedLimit = 3;
        fairSpeedLimit = 2;
        poorSpeedLimit = 1;
        speed = 0;
        setPosition(width / 2, height / 2);
        floatedToTop = false;
        acceleration = 0;
        angleacceleration = 0;
        dir = Left;
        setInitialStats();
        //setPosition(width / 2, height / 2);
        windowwidth = width;
        windowheight = height;
        state = Swimming;
        this->type = type;
        if(type == 1) {
            //std::shared_ptr<sf::Texture> temp = std::make_shared<sf::Texture>();
            //temp->loadFromFile("/home/czhanacek/Coding/lug-fishtank/images/fish/fish1.png");
            //textures.push_back(temp);
            addNewTexture("./images/fish/fish1.png");
            currentTextureIndex = 0;
            //setTexture(*textures[currentTextureIndex]);

            //scale(sf::Vector2f(scaleFactor, scaleFactor));
        }
        //setOrigin(0.5 * textures[currentTextureIndex]->getSize().x, 0.5 * textures[currentTextureIndex]->getSize().y);
    }

    // This function is just used to define the conditions of fish garbage collection 
    bool isRotten() {
        return age > rotAge && floatedToTop;
    }

    bool isClose(Vector2D point) {
        if(abs(point.x - getPosition().x) < 20 && abs(point.y - getPosition().y) < 20) {
            return true;
        }
        else {
            return false;
        }
    }


    void setInitialStats() {
        auto deathAgeDist = std::poisson_distribution<int>(naturalAge);
        std::random_device rd;
        std::mt19937 e2(rd());
        deathAge = deathAgeDist(e2);
    }

    ~Fish() {
        //std::cout << "FINALLY DESTROYED!" << std::endl;
    }
    void changeDirection() {
        if(dir == Left) {
            dir = Right;
            
        }
        else if(dir == Right) {
            dir = Left;
        }
    }
    void swim(void) {
        //std::uniform_int_distribution<int> dist(1, 100);
        //std::random_device rd;
        //std::mt19937 e2(rd());
        //int action = dist(e2);
        if(state == Swimming) {
            swimTick();
        }
    }

    void animate() {
        ticks = (ticks + 1) % 60;
        updateHealth();
        if(health == Dead) {
            beDead();
        }
        else {
            swim(); 
        }
        
        if(ticks == 59) {
            age++;
            //std::cout << "fish " << priority << " life " << age / (double)deathAge << std::endl;
        }
    };

    void updateHealth() {
        if(age > deathAge * 0.4) {
            health = Fair;
            if(age > deathAge * 0.7) {
                health = Poor;
                if(age > deathAge) {
                    health = Dead;
                }
            }
        }
    }
    // Precondition: Fish is in dead state
    void beDead() {
        if(getPosition().y > -50  && floatedToTop == false) {
            angleacceleration = 0;
            acceleration = 0;
            
            angle = 90;
            presentingAngle = 0;
            speed = -0.5;
            setDeadPositionAndAngle();
        }
        else {
            floatedToTop = true;
            //speed = 0.03 * sin(3.14 * (ticks + (age * 60)) / 120.0);
            setDeadPositionAndAngle();
        }
        

    }

    void setDeadPositionAndAngle() {
        speed += acceleration / 60.0;
        double currentx = getPosition().x;
        double currenty = getPosition().y;
        setPosition(currentx, currenty + (speed));
        setRotation(180);

    }


    void setSwimPositionAndAngle() {
        speed += (acceleration / 60.0);
        angle += (angleacceleration / 60.0);
        double currentx = getPosition().x;
        double currenty = getPosition().y;
        int directionMultiplier = 1;
        
        setPosition(currentx + (directionMultiplier * speed * cos(getRads(angle))), currenty + (directionMultiplier * speed * sin(getRads(angle))));
        //setPosition(currentx + (directionMultiplier * speed * 1), currenty + (directionMultiplier * speed * 0));

        setRotation(angle);
            
        
    }

    // Precondition: fish is in swimming state
    void swimTick() {
        
        modifyAcceleration();
        modifyDirection();
        if(speed > 0) {
            dir = Right;
        }
        if(speed < 0) {
            dir = Left;
        }
        setSwimPositionAndAngle();
    };

    void modifyDirection() {
        std::uniform_int_distribution<int> dist(1,2);
        std::random_device rd;
        std::mt19937 e2(rd());
        int shouldChange = dist(e2);
        int compensateAngleChange = 30 + (2 * abs(speed));
        float bounds1 = 0.3;
        if(shouldChange == 1) {
            std::uniform_real_distribution<double> realdist(-10,10);
            angleacceleration = realdist(e2);
        }
        if(angle > 40) {
            angle = 40;
        } else if (angle < -40) {
            angle = -40;
        }
        if(getPosition().y < (bounds1 * windowheight)) {
            if(dir == Right) {
                angleacceleration += compensateAngleChange;
            }
            else {
                angleacceleration -= compensateAngleChange;
            }
        }
        if(getPosition().y > (windowheight * (1 - bounds1))) {
            if(dir == Right) {
                angleacceleration -= compensateAngleChange;
            }
            else {
                angleacceleration += compensateAngleChange;
            }
        }

    }

    void enforceSpeedLimit() {
        if(health == Excellent) {
            if(speed > excellentSpeedLimit) {
                speed = excellentSpeedLimit;
            }
            if(speed < -1 * excellentSpeedLimit) {
                speed = -1 * excellentSpeedLimit;
            }
        }
        else if(health == Fair) {
            if(speed > fairSpeedLimit) {
                speed = fairSpeedLimit;
            }
            if(speed < -1 * fairSpeedLimit) {
                speed = -1 * fairSpeedLimit;
            }
            
        }
        else if(health == Poor) {
            if(speed > poorSpeedLimit) {
                speed = poorSpeedLimit;
            }
            if(speed < -1 * poorSpeedLimit) {
                speed = -1 * poorSpeedLimit;
            }
        }
        
    }

    void modifyAcceleration() {
        std::bernoulli_distribution dist(0.33);
        std::random_device rd;
        std::mt19937 e2(rd());
        int shouldChange = dist(e2);
        std::uniform_real_distribution<double> realdist(-10, 10);
        acceleration += realdist(e2);
        float bounds1 = 0.2;
        float bounds2 = 0.1;
        float compAcceleration1 = 2;
        float compAcceleration2 = 12;
        enforceSpeedLimit();
        if(getPosition().x < (bounds1 * windowwidth)) {
            acceleration = compAcceleration1;
            naturalAcc = false;
            if(getPosition().x < (bounds2 * windowwidth)) {
                acceleration = compAcceleration2;
            }

        }
        else if(getPosition().x > (1 - bounds1) * windowwidth) {
            acceleration = compAcceleration1 * -1;
            naturalAcc = false;
            if(getPosition().x > (1 - bounds2) * windowwidth) {
                acceleration = -1 * compAcceleration2;
            }
        }

        if(naturalAcc == false && !((getPosition().x > (bounds1 * windowwidth)) || getPosition().x < (bounds1 * windowwidth) )) {
            naturalAcc = true;
            acceleration = 0;
        }
    };

    double getRads(double degrees) {
        return degrees * ((2 * 3.14159) / 180.0);
    };
};
#endif
