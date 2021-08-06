#include "AttachmentUtils.h"
#include "utils.h"
extern "C" {
#include "../src/spine/Json.h"
}
void InitSkinData(const char * path, const char * skin_name, Serializer::Skin*spine_skin) {
	int file_size = 0;
	char *file_content = LoadFileContent(path, file_size);
	Json * root_data = Json_create(file_content);
	Json * skins_data = Json_getItem(root_data, "skins");
	if (skins_data) {
		for (Json * skin_data = skins_data->child; skin_data; skin_data = skin_data->next) {
			if (strcmp(skin_name,skin_data->name)!=0){
				continue;
			}
			spine_skin->set_name(skin_data->name);
			for (Json * attachments = skin_data->child; attachments; attachments = attachments->next) {
				Serializer::SlotAvailableAttachmentsInfo*spine_skin_slot = spine_skin->add_slot_available_attachments_info();
				char target_slot_name[256] = {0};
				sprintf(target_slot_name,"s_%s", attachments->name);
				spine_skin_slot->set_name(target_slot_name);
				for (Json * attachment = attachments->child; attachment; attachment = attachment->next) {
					Serializer::Attachment*spine_attachment = spine_skin_slot->add_available_attachments();
					const char* skinAttachmentName = attachment->name;
					const char* attachmentName = Json_getString(attachment, "name", attachment->name);
					spine_attachment->set_name(attachmentName);
					const char* typeString = Json_getString(attachment, "type", "region");
					spAttachmentType type;
					if (strcmp(typeString, "region") == 0)
						type = SP_ATTACHMENT_REGION;
					else if (strcmp(typeString, "mesh") == 0)
						type = SP_ATTACHMENT_MESH;
					else if (strcmp(typeString, "linkedmesh") == 0)
						type = SP_ATTACHMENT_LINKED_MESH;
					else if (strcmp(typeString, "boundingbox") == 0)
						type = SP_ATTACHMENT_BOUNDING_BOX;
					else if (strcmp(typeString, "path") == 0)
						type = SP_ATTACHMENT_PATH;
					else if (strcmp(typeString, "clipping") == 0)
						type = SP_ATTACHMENT_CLIPPING;
					else if (strcmp(typeString, "point") == 0)
						type = SP_ATTACHMENT_POINT;

					spine_attachment->set_type(type);
					switch (type) {
					case SP_ATTACHMENT_REGION: {
						Serializer::RegionAttachment spine_region_attachment;
						spine_region_attachment.add_translate(Json_getFloat(attachment, "x", 0));
						spine_region_attachment.add_translate(Json_getFloat(attachment, "y", 0));
						spine_region_attachment.add_scale(Json_getFloat(attachment, "scaleX", 1));
						spine_region_attachment.add_scale(Json_getFloat(attachment, "scaleY", 1));
						spine_region_attachment.add_rotation(Json_getFloat(attachment, "rotation", 0));
						spine_region_attachment.add_size(Json_getFloat(attachment, "width", 32));
						spine_region_attachment.add_size(Json_getFloat(attachment, "height", 32));

						const char* color = Json_getString(attachment, "color", "ffffffff");
						if (color) {
							spine_region_attachment.add_color(HexToColor(color, 0));
							spine_region_attachment.add_color(HexToColor(color, 1));
							spine_region_attachment.add_color(HexToColor(color, 2));
							spine_region_attachment.add_color(HexToColor(color, 3));
						}
						spine_attachment->set_data(spine_region_attachment.SerializeAsString());
						break;
					}
					case SP_ATTACHMENT_MESH:
					case SP_ATTACHMENT_LINKED_MESH: {
						spine_attachment->set_data("not supported");
						break;
					}
					case SP_ATTACHMENT_BOUNDING_BOX: {
						spine_attachment->set_data("not supported");
						break;
					}
					case SP_ATTACHMENT_PATH: {
						spine_attachment->set_data("not supported");
						break;
					}
					case SP_ATTACHMENT_POINT: {
						spine_attachment->set_data("not supported");
						break;
					}
					case SP_ATTACHMENT_CLIPPING: {
						spine_attachment->set_data("not supported");
						break;
					}
					}
				}
			}
		}
	}
}