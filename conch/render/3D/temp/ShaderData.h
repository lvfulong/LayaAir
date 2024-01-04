#ifndef __ShaderData_H__
#define __ShaderData_H__

#include <stdio.h>
#include <unordered_map>
#include "ResourceBase.h"
#include <core/math/Vector4.h>
#include <core/math/Vector3.h>
#include <core/math/Matrix4x4.h>

namespace laya
{
	enum class ShaderDataType
	{
		Int,
		Bool,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Color,
		Matrix4x4,
		Texture2D,
		TextureCube,
		Buffer,
		Matrix3x3,
	};
    class JSShaderData;
	class ShaderDefine;
	class ShaderData: public ResourceBase<ShaderData>
	{
	public:
		struct DataInfo
		{
			ShaderDataType	type;	//����
			int size;				//�������͵ĸ���
			unsigned char*	data;	//ָ��
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
        
		static ShaderData *getShaderData(uint32_t id);

		void setVector(int32_t index, const Vector4& value);
		void setVector3(int32_t index, const Vector3& value);
		void setMatrix4x4(int32_t index, const Matrix4x4& value);
		void addDefine(ShaderDefine* define);
		void removeDefine(ShaderDefine* define);
	private:

		void printfDataInfoValue();

        bool isDestroy{false};
	public:
        JSShaderData* m_data;
		std::unordered_map<int, DataInfo*>	m_vData;

	};
}
#endif //__ShaderData_H__
