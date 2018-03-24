#include <cmath>
#include <random>
#include <iostream>
#include "PriDrawable.hpp"
#include <ctime>


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
    double angle; // range from -90 to 90 degrees
    double acceleration;
    double angleacceleration;
    typedef enum Direction {Left, Right};
    bool floatedToTop;
    Direction dir;
    typedef enum State {Swimming, Eating, Resting, Fighting};
    State state;
    int breedtimes;
    double scaleFactor;
    int windowwidth;
    int windowheight;

    bool naturalAcc;
    double naturalAge;

    public:
    
    Fish(int type, int width, int height, int id) : PriDrawable(id) {
        //std::cout << "made a new fish" << std::endl;
        health = Excellent;
        breedtimes = 0;
        age = 0;
        std::srand(std::time(0));
        excellentSpeedLimit = 6;
        fairSpeedLimit = 5;
        poorSpeedLimit = 2;
        naturalAge = 100;
        naturalAcc = true;
        speed = 0;
        floatedToTop = false;
        acceleration = 0;
        angleacceleration = 0;
        dir = Left;
        setPosition(width / 2, height / 2);
        windowwidth = width;
        windowheight = height;
        state = Swimming;
        this->type = type;
        if(type == 1) {
            sf::Image img;
            img.loadFromFile(std::string("./images/fish/fish1.png").c_str());
            for(int y = 0; y < img.getSize().y; y++) {
                int x = 0;
                while(x < img.getSize().x) {
                    if(img.getPixel(x, y).b > 200) {
                        img.setPixel(x, y, sf::Color::Red);
                    }
                    x++;
                }
            }
            sf::Texture * temp = new sf::Texture();
            temp->loadFromImage(img);
            textures.push_back(std::move(temp));
            setTexture(*textures[currentTextureIndex]);
            
            scaleFactor = 100.0 / textures[currentTextureIndex]->getSize().x;
            scale(sf::Vector2f(scaleFactor, scaleFactor));

        }
        setOrigin(0.5 * textures[currentTextureIndex]->getSize().x, 0.5 * textures[currentTextureIndex]->getSize().y);
        determineInitialStats();
    }

    void determineInitialStats() {
        std::poisson_distribution<int> dist((int)naturalAge);
        std::random_device rd;
        std::mt19937 e2(rd());
        deathAge = dist(e2);
        rotAge = deathAge * 1.05;
    }

    bool isRotten() {
        //if(age > rotAge && floatedToTop)
            //std::cout << "fish is rotten" << std::endl;
        return age > rotAge && floatedToTop;
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
        ticks = (ticks + 1) % 60;
        updateHealth();
        if(health == Dead) {
            beDead();
        }
        else {
            swim(); 
        }
        
        if(ticks == 0) {
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
            speed = -1;
            setDeadPositionAndAngle();
        }
        else {
            floatedToTop = true;
            speed = 0.03 * sin(3.14 * (ticks + (age * 60)) / 120.0);
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
        setRotation(angle);
        
    }

    // Precondition: fish is in swimming state
    void swimTick() {
        
        modifyAcceleration();
        modifyDirection();
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
        setSwimPositionAndAngle();
    };

    void modifyDirection() {
        std::uniform_int_distribution<int> dist(1,2);
        std::random_device rd;
        std::mt19937 e2(rd());
        int shouldChange = dist(e2);
        int compensateAngleChange = 30 + (2 * abs(speed));
        float bounds1 = 0.2;
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
            if(getScale().x < 0) {
                angleacceleration += compensateAngleChange;
            }
            else {
                angleacceleration -= compensateAngleChange;
            }
        }
        if(getPosition().y > (windowheight * (1 - bounds1))) {
            if(getScale().x < 0) {
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
        std::uniform_real_distribution<double> realdist(-0.1, 0.1);
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
