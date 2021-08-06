#include "TextureRectangleNode.h"
namespace Alice{
	TextureRectangleNode::TextureRectangleNode(bool isLeaf):mbIsLeaf(isLeaf),mbRotated(false), mbFilled(false){
	}
	TextureRectangleNode* TextureRectangleNode::AddTexture(int &width, int &height){
		if (mbIsLeaf){
			if (mbFilled){
				return nullptr;
			}
			if (mRect.mWidth<width||
				mRect.mHeight<height){
				return nullptr;
			}
			else if (mRect.mWidth == width&& mRect.mHeight ==height){
				mbFilled = true;
				return this;
			}
			//split this to A->B and find a rectangle in A for given image
			mChild[0] = new TextureRectangleNode(true);
			mChild[1] = new TextureRectangleNode(true);
			mbIsLeaf = false;
			int dWidth = mRect.mWidth - width;
			int dHeight = mRect.mHeight - height;
			const int gap = 3;
			if (dWidth>dHeight){//split on x axis
				((TextureRectangleNode*)mChild[0])->mRect.Set(mRect.mLeft, mRect.mBottom,width, mRect.mHeight);
				((TextureRectangleNode*)mChild[1])->mRect.Set(mRect.mLeft + width + gap, mRect.mBottom, dWidth - gap, mRect.mHeight);
			}
			else {//split on y axis
				((TextureRectangleNode*)mChild[0])->mRect.Set(mRect.mLeft, mRect.mBottom, mRect.mWidth, height);
				((TextureRectangleNode*)mChild[1])->mRect.Set(mRect.mLeft, mRect.mBottom + height + gap, mRect.mWidth, dHeight - gap);
			}
			return ((TextureRectangleNode*)mChild[0])->AddTexture(width, height);
		}else{
			TextureRectangleNode*newNode = ((TextureRectangleNode*)mChild[0])->AddTexture(width, height);
			if (newNode != nullptr){
				return newNode;
			}
			return ((TextureRectangleNode*)mChild[1])->AddTexture(width,height);
		}
	}
}