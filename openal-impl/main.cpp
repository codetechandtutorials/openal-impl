#include "MainLoop/MainLoop.h"
#include "modes/TerminalTopDown.h"
int main()
{
	MainLoop::Get()->SetRunCondition([]() {return TTD::isRunning(); });
	MainLoop::Get()->AddToOnBegin([]() {TTD::defaultMap();TTD::init(); });
	MainLoop::Get()->AddToOnUpdate([](float dt) {TTD::processPlayer(dt); TTD::processAI(dt);});
	MainLoop::Get()->AddToOnPostUpdate([]() {TTD::renderScene(); });
	return MainLoop::Get()->Run();
}
