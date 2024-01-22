#ifndef __ShaderData_H__
#define __ShaderData_H__
#include <binder/JSInterface.h>
#include <render/3D/temp/DefineDatas.h>
#include <stdio.h>
#include <unordered_map>
#include "ResourceBase.h"
#include <core/math/Color.h>
#include <core/math/Vector4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector2.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Matrix3x3.h>
#include <any>


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

		ShaderData();

		~ShaderData();

		void refreshData(int32_t* pBufferData,int nLength, int counts);

		ShaderData::DataInfo* getData(int key);

        void destroy();
        
        void applyUBOData();
        
		static ShaderData *getShaderData(uint32_t id);
		void setBool(int32_t index, bool value);
		void setInt(int32_t index, int32_t value);
		void setNumber(int32_t index, float value);
		void setVector2(int32_t index, const Vector2& value);
		void setVector(int32_t index, const Vector4& value);
		void setVector3(int32_t index, const Vector3& value);
		void setColor(int32_t index, const Color& value);
		void setMatrix4x4(int32_t index, const Matrix4x4& value);
		void setMatrix3x3(int32_t index, const Matrix3x3& value);
		void setBuffer(int32_t index, uint8_t* data, uint32_t lengthInBytes);
		void addDefine(ShaderDefine* define);
		void removeDefine(ShaderDefine* define);

		void cloneTo(ShaderData* destObject);
	private:

        bool isDestroy{false};
	public:

		std::unordered_map<int, DataInfo*>	m_vData;

		std::unordered_map<uint32_t, std::any>	m_data;
		DefineDatas _defineDatas;
	};
}
#endif //__ShaderData_H__
