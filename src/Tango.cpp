
#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>
#include <GL/glfw.h>

int main(int argc, char** argv)
{

  if (!glfwInit()) return 1;

  if ( argc < 2 ) {
    AKURunScript("main.lua");
  }

  return 0;
}
