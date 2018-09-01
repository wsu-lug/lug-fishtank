
#include "Tank.hpp"

int main(void) {
    XInitThreads();
    Tank * tank = new Tank(2400, 1600);
    tank->simulate();
    
}
