#include "MainLoop/MainLoop.h"
#include "modes/TerminalTopDown.h"
int main()
{
	static bool keepRunning = true;
	MainLoop::Get()->SetRunCondition([]() {return keepRunning; });
	MainLoop::Get()->AddToOnBegin([]() {TTD::defaultMap(); });
	MainLoop::Get()->AddToOnUpdate([](float dt) {TTD::parseUserInput(dt); TTD::updateAI(dt);});
	MainLoop::Get()->AddToOnPostUpdate([]() {TTD::showscene(); });
	return MainLoop::Get()->Run();
}
