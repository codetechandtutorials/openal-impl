#include "SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>
#include <vector>
#include "OpenALErrorCheck.h"

static SoundDevice* _instance = nullptr;

SoundDevice* SoundDevice::Get()
{
	Init();
	return _instance;
}

void SoundDevice::Init()
{
	if (_instance == nullptr)
		_instance = new SoundDevice();
}

/// <summary>
/// Gets the listeners current locaiton.
/// </summary>
/// <param name="x">return value x</param>
/// <param name="y">return value y</param>
/// <param name="z">return value z</param>
void SoundDevice::GetLocation(float& x, float& y, float& z)
{
	alGetListener3f(AL_POSITION, &x, &y, &z);
	AL_CheckAndThrow();
}

/// <summary>
/// Gets the current listener Orientation as 'at' and 'up'
/// </summary>
/// <param name="ori"> Return value: Must be a float array with 6 slots available ex: float myvar[6]</param>
void SoundDevice::GetOrientation(float& ori)
{
	alGetListenerfv(AL_ORIENTATION, &ori);
	AL_CheckAndThrow();
}

/// <summary>
/// Gets the current volume of our listener.
/// </summary>
/// <returns>current volume</returns>
float SoundDevice::GetGain()
{
	float curr_gain;
	alGetListenerf(AL_GAIN, &curr_gain);
	AL_CheckAndThrow();
	return curr_gain;
}

/// <summary>
/// Sets the Attunation Model.
/// </summary>
/// <param name="key">
///#define AL_INVERSE_DISTANCE                      0xD001
///#define AL_INVERSE_DISTANCE_CLAMPED              0xD002
///#define AL_LINEAR_DISTANCE                       0xD003
///#define AL_LINEAR_DISTANCE_CLAMPED               0xD004
///#define AL_EXPONENT_DISTANCE                     0xD005
///#define AL_EXPONENT_DISTANCE_CLAMPED             0xD006
/// </param>
void SoundDevice::SetAttunation(int key)
{
	if (key < 0xD001 || key > 0xD006)
		throw("bad attunation key");

	alDistanceModel(key);
	AL_CheckAndThrow();
}

/// <summary>
/// Sets the location of our listener
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void SoundDevice::SetLocation(const float& x, const float& y, const float& z)
{
	alListener3f(AL_POSITION, x, y, z);
	AL_CheckAndThrow();
}

/// <summary>
/// Sets the orientation of our listener using 'look at' (or foward vector) and 'up' direction
/// </summary>
/// <param name="atx"></param>
/// <param name="aty"></param>
/// <param name="atz"></param>
/// <param name="upx"></param>
/// <param name="upy"></param>
/// <param name="upz"></param>
void SoundDevice::SetOrientation(const float& atx, const float& aty, const float& atz, const float& upx, const float& upy, const float& upz)
{
	std::vector<float> ori;
	ori.push_back(atx);
	ori.push_back(aty);
	ori.push_back(atz);
	ori.push_back(upx);
	ori.push_back(upy);
	ori.push_back(upz);
	alListenerfv(AL_ORIENTATION, ori.data());
	AL_CheckAndThrow();
}

/// <summary>
/// Sets the master volume of our listeners. capped between 0 and 5 for now.
/// </summary>
/// <param name="val"></param>
void SoundDevice::SetGain(const float& val)
{
	// clamp between 0 and 5
	float newVol = val;
	if (newVol < 0.f)
		newVol = 0.f;
	else if (newVol > 5.f)
		newVol = 5.f;

	alListenerf(AL_GAIN, newVol);
	AL_CheckAndThrow();
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
