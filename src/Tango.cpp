#include <Simulator.h>

int main(int argc, char** argv)
{
  const char *filename = (argc > 1) ? argv[1] : "main.lua";
  return startTango(filename);
}
