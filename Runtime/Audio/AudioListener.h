#pragma once
#include "Runtime/Math/Vector3.h"
#include "Runtime/Base/Component.h"

namespace Alice
{
	class AudioListener:public Component
	{
	public:
		AudioListener();
		Vector3f mPos;
		void Update(float deltaTime);
	public:
		DEF_ALICE_EXPORTED_TYPE_NEW_METHOD(AudioListener,AudioListener)
		DECLEAR_ALICE_CLASS(AudioListener)
	};
}