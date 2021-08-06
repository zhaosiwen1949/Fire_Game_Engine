#include "TreeViewNode.h"
#pragma warning(disable:4018)
namespace Alice
{
	TreeViewNode::TreeViewNode():mParent(nullptr),mLeftSibling(nullptr),mRightSibling(nullptr)
	{

	}

	int TreeViewNode::AddChild(TreeViewNode*node, int pos/* =-1 */)
	{
		int nRet = 1;
		if (pos==-1)
		{
			//push end
			mChildren.push_back(node);
			node->SetParent(this);
			nRet = (int)mChildren.size();
		}
		else
		{
			unsigned int nChildCount = mChildren.size();
			if (nChildCount-1>=pos)
			{
				TreeViewNode*end = mChildren[nChildCount - 1];
				for (int i = nChildCount - 1; i > pos;i--)
				{
					mChildren[i] = mChildren[i-1];
				}
				mChildren[pos]=node;
				mChildren.push_back(end);
				if (pos!=0)
				{
					node->mLeftSibling = mChildren[pos - 1];
				}
				node->mRightSibling = mChildren[pos + 1];
				mChildren[pos + 1]->mLeftSibling = node;
				node->SetParent(this);
				nRet = pos;
			}
			else
			{
				mChildren.push_back(node);
				node->SetParent(this);
				nRet = (int)mChildren.size();
			}
		}
		return nRet;
	}

	void TreeViewNode::InsertBefore(TreeViewNode*before)
	{
		if (before->GetParent()!=nullptr)
		{
			before->GetParent()->AddChild(this, before->GetParent()->GetChildPos(before));
		}
	}

	void TreeViewNode::InsertAfter(TreeViewNode*after)
	{
		if (after->GetParent()!=nullptr)
		{
			after->GetParent()->AddChild(this, after->GetParent()->GetChildPos(after) + 1);
		}
	}

	void TreeViewNode::RemoveChildAt(unsigned int pos)
	{
		if (mChildren.size()>pos)
		{
			RemoveChild(mChildren[pos]);
		}
	}

	void TreeViewNode::RemoveChild(TreeViewNode*node)
	{
		for (std::vector<TreeViewNode*>::iterator iter = mChildren.begin(); iter != mChildren.end();iter++)
		{
			if ((*iter)==node)
			{
				mChildren.erase(iter);
				break;
			}
		}
	}

	TreeViewNode* TreeViewNode::GetChild(int pos)
	{
		if (mChildren.size()>pos)
		{
			return mChildren[pos];
		}
		return nullptr;
	}

	std::vector<TreeViewNode*>& TreeViewNode::GetChildren()
	{
		return mChildren;
	}

	void TreeViewNode::SetParent(TreeViewNode*parent)
	{
		mParent = parent;
	}

	TreeViewNode* TreeViewNode::GetParent()
	{
		return mParent;
	}

	int TreeViewNode::GetChildPos(TreeViewNode*child)
	{
		int nIndex = 0;
		int nCount = (int)mChildren.size();
		for (; nIndex < nCount;nIndex++)
		{
			if (mChildren[nIndex]==child)
			{
				return nIndex;
			}
		}
		return -1;
	}

	TreeViewNode* TreeViewNode::GetLeftSibling()
	{
		return mLeftSibling;
	}

	TreeViewNode* TreeViewNode::GetRightSibling()
	{
		return mRightSibling;
	}
}