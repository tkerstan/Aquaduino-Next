#include <Framework/Aquaduino.h>

Aquaduino *aquaduino;

void setup()
{
    aquaduino = new Aquaduino();

}

void loop()
{
    aquaduino->run();
}
