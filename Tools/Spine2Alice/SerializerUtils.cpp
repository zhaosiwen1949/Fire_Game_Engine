#include "utils.h"
#include "Assets.serializer.h"
#include "SerializerUtils.h"
#pragma comment(lib,"LibProtoBufferLite.lib")
Serializer::GameObject* GetRightSibling(Serializer::GameObject*node) {
	if (node->has_right_sibling()) {
		return GetRightSibling(node->mutable_right_sibling());
	}
	return node->mutable_right_sibling();
}
Serializer::GameObject* GetChild(Serializer::GameObject*parent) {
	if (parent->has_child()) {
		return GetRightSibling(parent->mutable_child());
	}
	else {
		return parent->mutable_child();
	}
}
void SetVector3(Serializer::Vector3f*v, float x, float y, float z) {
	v->set_x(x);
	v->set_y(y);
	v->set_z(z);
}
void SetVector4(Serializer::Vector4f*v, float x, float y, float z, float w) {
	v->set_x(x);
	v->set_y(y);
	v->set_z(z);
	v->set_w(w);
}
void ExportAssetBundle(const char *path, const char * guid, Alice::ResourceType type, int compressed, const std::string &data) {
	Serializer::RuntimeAsset ra;
	ra.set_type(type);
	ra.set_rawdata(data);
	ra.set_guid(guid);
	SaveFile(path, ra.SerializeAsString().c_str(), ra.SerializeAsString().size());
}