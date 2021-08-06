#include "TTree.h"
#pragma warning(disable:4018)
namespace Alice
{
	TTree::TTree():mParent(nullptr),mChild(nullptr),mLeftSibling(nullptr),mRightSibling(nullptr),mLastChild(nullptr)
	{

	}

	TTree::~TTree()
	{

	}
	bool TTree::AppendChild(TTree*node)
	{
		if (IsParent(node))
		{
			return false;
		}
		node->Clean();
		TTree*child = mChild;
		if (mChild ==nullptr)
		{
			mChild = node;
			node->mParent = this;
		}
		else
		{
			while (child->mRightSibling!=nullptr)
			{
				child = child->mRightSibling;
			}
			child->mRightSibling = node;
			node->mParent = this;
			node->mLeftSibling = child;
		}
		mLastChild = node;
		return true;
	}

	bool TTree::InsertBefore(TTree*before)
	{
		if (before->IsParent(this))
		{
			return false;
		}
		Clean();
		if (before->mParent!=nullptr)
		{
			if (before->mLeftSibling==nullptr)
			{
				before->mParent->mChild = this;
			}
			else
			{
				mLeftSibling = before->mLeftSibling;
				mLeftSibling->mRightSibling = this;
			}
			mParent = before->mParent;
			mRightSibling = before;
			before->mLeftSibling = this;
		}
		return true;
	}

	bool TTree::InsertAfter(TTree*after)
	{
		if (after->IsParent(this))
		{
			return false;
		}
		Clean();
		mParent = after->mParent;
		if (after->mRightSibling != nullptr)
		{
			mRightSibling = after->mRightSibling;
			mRightSibling->mLeftSibling = this;
		}
		else
		{
			if(mParent!=nullptr)mParent->mLastChild = this;
		}
		mLeftSibling = after;
		after->mRightSibling = this;
		return true;
	}

	void TTree::Clean()
	{
		if (mParent!=nullptr)
		{
			mParent->RemoveChild(this);
		} else {
			if (mLeftSibling!=nullptr){
				if (mRightSibling != nullptr) {
					mLeftSibling->mRightSibling = mRightSibling;
					mRightSibling->mLeftSibling = mLeftSibling;
				}
				else {
					mLeftSibling->mRightSibling = nullptr;
				}
				mLeftSibling = nullptr;
				mRightSibling = nullptr;
			}
		}
	}

	bool TTree::IsParent(TTree*node)
	{
		bool isParent = false;
		if (mParent != nullptr)
		{
			if (mParent == node)
			{
				isParent = true;
			}
			if (isParent)
			{
				return true;
			}
			isParent = mParent->IsParent(node);
		}
		return isParent;
	}

	void TTree::RemoveChild(TTree*node)
	{
		//����Ǵ���ӣ���ô�Ѷ����ӱ�ɴ���ӽڵ������������
		if (mChild==node)
		{
			if (node->mRightSibling != nullptr)
			{
				mChild = node->mRightSibling;
				node->mRightSibling->mLeftSibling = nullptr;//������ ---> �����
			}
			else
			{
				mChild = nullptr;//����
				mLastChild = nullptr;
			}
		}
		else
		{
			//������Ǵ���ӣ���С�ܵܵĻ����Ͱ�С�ܷŵ��Լ�������λ���ϡ�
			if (node->mRightSibling != nullptr)
			{
				node->mRightSibling->mLeftSibling = node->mLeftSibling;//����Ķ�������ǰ�ƶ�
				node->mLeftSibling->mRightSibling = node->mRightSibling;
			}
			else
			{
				//��С����
				node->mLeftSibling->mRightSibling = nullptr;
				mLastChild = node->mLeftSibling;
			}
		}
		node->mLeftSibling = nullptr;
		node->mRightSibling = nullptr;
		node->mParent = nullptr;
	}
}