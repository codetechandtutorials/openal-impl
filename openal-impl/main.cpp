#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"
#include "SoundEffectsPlayer.h"
#include <iostream>
#include "MusicBuffer.h"
#include "MainLoop/MainLoop.h"  // for looping
#include <Windows.h>  // for keyboard press

int main()
{
	std::cout << "initializing sound device...\n";
	SoundDevice::Init();
	static SoundEffectsPlayer effectsPlayer1;
	static uint32_t sound1 = SE_LOAD("../res/spellsounds/spell.ogg");
	static SoundEffectsPlayer effectsPlayer2;
	static uint32_t sound2 = SE_LOAD("../res/spellsounds/magicfail.ogg");

	static MusicBuffer myMusic("../res/music/TownTheme.wav");

	static bool keepRunning = true;
	MainLoop::Get()->SetRunCondition([]() {return keepRunning; });
	MainLoop::Get()->AddToOnUpdate([](float dt) {
		static float healcooldown = 3;
		healcooldown += dt;
		static float failcooldown = .8f;
		failcooldown += dt;
		if (GetKeyState('A') & 0x8000)
		{
			if (healcooldown > 3) {
				effectsPlayer1.Play(sound1);
				healcooldown = 0;
				failcooldown = 0;
			}
			else if (effectsPlayer1.isPlaying() && failcooldown > .8f) {
				effectsPlayer2.Play(sound2);
				failcooldown = 0;
			}
		}

		static float musiccontrolcooldown = 1;
		musiccontrolcooldown += dt;
		if (musiccontrolcooldown > 1 && GetKeyState('P') & 0x8000)
		{
			if (myMusic.isPlaying()) // toggle play/pause
			{
				myMusic.Pause();
			}
			else 
			{
				myMusic.Play();
			}
			musiccontrolcooldown = 0;
		}

		if (GetKeyState('Q') & 0x8000)
		{
			keepRunning = false;
		}
		});

	MainLoop::Get()->AddToDelayedUpdate([]() {
		if (myMusic.isPlaying())
		{
			myMusic.UpdateBufferStream();
		}
		});

	return MainLoop::Get()->Run();
}