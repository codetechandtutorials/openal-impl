#include "SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>

static SoundDevice* _instance = nullptr;

SoundDevice* SoundDevice::get()
{
	init();
	return _instance;
}

void SoundDevice::init()
{
	if (_instance == nullptr)
		_instance = new SoundDevice();
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if (!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}
