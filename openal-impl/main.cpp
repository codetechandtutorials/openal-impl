#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <iostream>

int main()
{
	std::cout << "starting...\n";


	SoundDevice * mysounddevice = SoundDevice::get();

	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("../res/spellsounds/spell.ogg");
	uint32_t /*ALuint*/ sound2 = SoundBuffer::get()->addSoundEffect("../res/spellsounds/magicfail.ogg");

	SoundSource mySpeaker;

	mySpeaker.Play(sound1);
	mySpeaker.Play(sound2);

	std::cout << "got here\n";

	return 0;
}