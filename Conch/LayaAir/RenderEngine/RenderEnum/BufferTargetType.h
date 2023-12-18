#ifndef __BufferTargetType_H__
#define __BufferTargetType_H__

namespace laya
{
	enum class BufferTargetType
	{
		ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER,
		UNIFORM_BUFFER,
		COPY_READ_BUFFER,
		COPY_WRITE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER,
		PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER,
	};
	enum class BufferUsage
	{
		Static,
		Dynamic,
		Stream
	};
}
#endif