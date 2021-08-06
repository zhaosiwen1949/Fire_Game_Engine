#include "AudioListener.h"

namespace Alice
{
	AudioListener::AudioListener()
	{

	}

	void AudioListener::Update(float deltaTime)
	{
		if (mbPositionChanged)
		{
			//
			mbPositionChanged = false;

		}
	}
}