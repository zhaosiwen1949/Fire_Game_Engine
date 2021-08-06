#pragma once
#include "Runtime/Math/Rect.h"
#include "Runtime/Math/Vector4.h"
namespace Alice
{
	class TextureRectangleNode
	{
	public:
		TextureRectangleNode *mChild[2];
		Rect<int> mRect;
		Vector4f mTexCoord;
		bool mbIsLeaf;
		bool mbFilled;
		bool mbRotated;
		TextureRectangleNode(bool isLeaf);
		TextureRectangleNode* AddTexture(int &width, int &height);
	};
}