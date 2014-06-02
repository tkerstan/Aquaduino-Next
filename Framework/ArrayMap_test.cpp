#include <Framework/ArrayMap.h>
#include <libraries/ArduinoUnit/ArduinoUnit.h>

test(correct)
{
  int x=1;
  assertEqual(x,1);
}

test(incorrect)
{
  int x=1;
  assertNotEqual(x,1);
}

extern "C" void setup()
{
}

extern "C" void loop()
{
	Test::run();
}
