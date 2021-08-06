#pragma once
#include "AliceGL.h"
class LinkedList {
public:
	LinkedList*mNext;
	LinkedList() :mNext(nullptr) {

	}
	template<typename T>
	T* Next() {
		return (T*)mNext;
	}
	void PushBack(LinkedList*node) {
		if (mNext==nullptr){
			mNext = node;
		}
		else {
			mNext->PushBack(node);
		}
	}
	void Remove(LinkedList*node) {
		if (mNext==node){
			mNext = mNext->mNext;
			node->mNext = nullptr;
		}
		else {
			mNext->Remove(node);
		}
	}
};