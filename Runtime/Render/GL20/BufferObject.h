#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
namespace Alice{
	class BufferObject{
	public:
		BufferObject(GLenum type,int size,GLenum hint);
		virtual ~BufferObject();
		GLuint mIdentifier;
		GLenum mType;
		int mSize;
		virtual void Init(GLenum type, int size, GLenum hint);
		//1MB~4MB��һ���Ƚ����������
		//����Ŀǰ���Ҷ�λһ��ushort�����ֵ
		//4MB�����������ռ��36Byte����ô��������116508.4����
		//Index buffer��������С�ڶ���Buffer�ģ�����ֻ����BufferObject���������ɡ�
		//������һ����Ϸ��һ��BufferObject�Ϳ����������еĶ�����(11���������Ի��Ƶ��������Ѿ��ǳ����ˣ�����ҲҪ���ǵ����ɲ��µ�ʱ��Ĵ���취)��ֻ��Ҫ���ɶ��Index Buffer Object�ͺá�
		//AliceAny mVertexDatas[2097152];//2MB
		//׷�Ӷ�������
		//ˢ�¶�������
		void SubData(const void*data,int size,int offset=0);
		bool Active();
		bool Restore();
	};
}