#include <stdio.h>
#include "graphics.h"
#include "RenderSystem.h"
#include "RenderQueue.h"
#include "WindowManager.h"
#include "MenuManager.h"
#include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "pedsim/libpedsim/ped_includes.h"


RenderSystem* g_rs = 0;
WindowManager g_windowManager;
MenuManager g_menuManager;

Ped::Tscene * pedscene;


void loop()
{
  g_menuManager.predraw();
  ImGui::Text("%i", (int)pedscene->getAllAgents().size());
    
  for (auto a : pedscene->getAllAgents()) 
  {
    auto v = a->getPosition();
    ImGui::Text("%.1f, %.1f, %.1f", (float)v.x, (float)v.y, (float)v.z);
    if (a->reachedDestination())
      ImGui::Text("stopped");
    else
      ImGui::Text("walking");

  }

  g_menuManager.postdraw();

  // Move all agents for 700 steps (and write their position through the outputwriter)
  pedscene->moveAgents(0.3);
};


extern "C" int main(int argc, char** argv)
{
  g_windowManager.width = 345;
  g_windowManager.height = 490;
  g_windowManager.init("Project");
  g_menuManager.init(g_windowManager);

  g_rs = new RenderSystem();
  g_rs->init();

  // Setup
  pedscene = new Ped::Tscene(-200, -200, 400, 400);

  Ped::Twaypoint *w1 = new Ped::Twaypoint(-100, 0, 24);
  Ped::Twaypoint *w2 = new Ped::Twaypoint(+100, 0, 12);

  Ped::Tobstacle *o = new Ped::Tobstacle(0, -5,  0, +5);
  pedscene->addObstacle(o);

  for (int i = 0; i<10; i++) 
  {
    Ped::Tagent *a = new Ped::Tagent();

    a->addWaypoint(w1);
    a->addWaypoint(w2);

    a->setPosition(-50 + rand()/(RAND_MAX/80)-40, 0 + rand()/(RAND_MAX/20) -10, 0);

    pedscene->addAgent(a);
  } 

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  glfwTerminate();

  return 0;
}
