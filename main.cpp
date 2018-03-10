
#include "Tank.hpp"

int main(void) {
    XInitThreads();
    Tank * tank = new Tank(1200, 800);
    tank->simulate();
    
}
