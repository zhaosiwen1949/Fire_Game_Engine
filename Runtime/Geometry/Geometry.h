#pragma once
#include "AABB.h"
#include "Sphere.h"
namespace Alice {
	enum BoundingVolumeType{
		kBoundingVolumeTypeNone,
		kBoundingVolumeTypeAABB,
		kBoundingVolumeTypeSphere,
		kBoundingVolumeTypeCapsule,
		kBoundingVolumeTypeCount
	};
}