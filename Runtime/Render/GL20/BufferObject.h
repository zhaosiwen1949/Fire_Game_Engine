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
		//1MB~4MB是一个比较理想的数据
		//这里目前暂且定位一个ushort的最大值
		//4MB数据如果顶点占用36Byte，那么可以容纳116508.4个点
		//Index buffer理论上是小于顶点Buffer的，所以只考虑BufferObject的容量即可。
		//理论上一个游戏，一个BufferObject就可以容纳所有的顶点了(11万个顶点可以绘制的三角形已经非常多了，但是也要考虑到容纳不下的时候的处理办法)。只需要生成多个Index Buffer Object就好。
		//AliceAny mVertexDatas[2097152];//2MB
		//追加顶点数据
		//刷新顶点数据
		void SubData(const void*data,int size,int offset=0);
		bool Active();
		bool Restore();
	};
}