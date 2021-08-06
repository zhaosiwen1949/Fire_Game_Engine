#pragma once
#include "Runtime/Allocator/MemoryLabel.h"
#include "Runtime/Utils/Timer.h"
#include <unordered_map>
namespace Alice {
	struct MemoryObject {
		void*mPtr;
		MemoryLabel mLabel;
	};
	class Profiler{
	public:
		static std::unordered_map<void*, MemoryLabel> mObjects;//pointer to memory label
		static std::unordered_map<void*, size_t> mObjectsSize;//memory size
		static void OnNewObject(void*ptr, MemoryLabel label,size_t size=0);
		static bool OnFreeObject(void*ptr);
		static int GetTotalUsedMemorySize();
	};
}