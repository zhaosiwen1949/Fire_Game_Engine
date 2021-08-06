#pragma once
#include <vector>

namespace Alice
{
	class TreeViewNode
	{
	protected:
		TreeViewNode* mParent;
		TreeViewNode* mLeftSibling;
		TreeViewNode* mRightSibling;
		std::vector<TreeViewNode*> mChildren;
	public:
		TreeViewNode();
		int AddChild(TreeViewNode*node, int pos=-1);
		void InsertBefore(TreeViewNode*before);
		void InsertAfter(TreeViewNode*after);
		void RemoveChild(TreeViewNode*node);
		void RemoveChildAt(unsigned int pos);
		TreeViewNode*GetLeftSibling();
		TreeViewNode*GetRightSibling();
		TreeViewNode*GetChild(int pos);
		TreeViewNode*GetParent();
		void SetParent(TreeViewNode*parent);
		int GetChildPos(TreeViewNode*child);
		std::vector<TreeViewNode*>&GetChildren();
	};
}