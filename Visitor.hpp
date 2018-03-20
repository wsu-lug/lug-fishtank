
#include "BackgroundWater.hpp"
#include "Fish.hpp"
#include "WebcamPassthrough.hpp"

class Visitor {
    public:
    virtual void visit(WebcamPassthrough &) = 0;
    virtual void visit(Fish &) = 0;
    virtual void visit(BackgroundWater &) = 0;
};
