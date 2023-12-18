#ifndef __ShaderData_H__
#define __ShaderData_H__

#include <stdio.h>
#include <unordered_map>
#include "../RenderEngine/ObjectBase.h"

namespace laya
{
	enum class ShaderDataType
	{
		Number32 = 0,
		Vector2,
		Vector3,
		Vector4,
		Matrix4x4,
		Number32Array,
		Texture,
		ShaderDefine,
		UBO,
	};
    class JSShaderData;
	class ShaderData: public ObjectBase<ShaderData>
	{
	public:
		struct DataInfo
		{
			ShaderDataType	type;	//类型
			int size;				//按照类型的个数
			unsigned char*	data;	//指针
			int byteSize = 0;//
			DataInfo()
			{
				data = nullptr;
			}
			DataInfo(ShaderDataType nType, int nSize, int nByteSize)
			{
				type = nType;
				size = nSize;
				data = nullptr;
				byteSize = nByteSize;
			}
			~DataInfo()
			{
				if (data)
				{
					delete[] data;
					data = nullptr;
				}
			}
		};

	public:

		ShaderData(JSShaderData* data);

		~ShaderData();

		void refreshData(int32_t* pBufferData,int nLength, int counts);

		ShaderData::DataInfo* getData(int key);

        void destroy();
        
        void applyUBOData();
        
	private:

		void printfDataInfoValue();

        bool isDestroy{false};
	public:
        JSShaderData* m_data;
		std::unordered_map<int, DataInfo*>	m_vData;

	};
}
#endif //__ShaderData_H__
