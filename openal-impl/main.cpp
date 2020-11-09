#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <iostream>
#include "MusicBuffer.h"

int main()
{
	std::cout << "starting...\n";

	SoundDevice::init();

	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("../res/spellsounds/spell.ogg");
	uint32_t sound2 = SoundBuffer::get()->addSoundEffect("../res/spellsounds/magicfail.ogg");

	SoundSource mySpeaker;

	std::cout << "playing magic sound...\n";
	mySpeaker.Play(sound1);

	std::cout << "playing magic fail sound...\n";
	mySpeaker.Play(sound2);


	MusicBuffer myMusic("../res/music/TownTheme.wav");

	std::cout << "playing town theme music...\n";
	myMusic.Play();


	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		myMusic.UpdateBufferStream();

		alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
	}


	std::cout << "got here\n";

	return 0;
}