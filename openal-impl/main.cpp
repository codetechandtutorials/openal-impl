#include "MainLoop/MainLoop.h"
#include "modes/TopDown.h"
int main()
{
	MainLoop::Get()->SetRunCondition([]() {return GridGame::isRunning(); });
	MainLoop::Get()->AddToOnBegin([]() {GridGame::processMapChanges(); GridGame::init(); });
	MainLoop::Get()->AddToOnUpdate([](float dt) {GridGame::processPlayer(dt); /*TTD::processAI(dt);*/ });
	MainLoop::Get()->AddToOnPostUpdate([]() {GridGame::renderScene(); });
	return MainLoop::Get()->Run();
}

