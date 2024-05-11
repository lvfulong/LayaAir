#ifndef __GLESInstanceRenderBatch_H__
#define __GLESInstanceRenderBatch_H__

#include <utils/JCSingletonList.h>
#include <unordered_map>
#include <any>
#include <cstdint>

namespace laya
{
	class GLESRenderElement3D;
	class GLESInstanceRenderElement3D;
	struct BatchMark
	{
	public:
		int updateMark;
		int indexInList;
		bool batched;
	};

	class GLESInstanceRenderBatch {
	public:
		GLESInstanceRenderBatch();
		~GLESInstanceRenderBatch();
		BatchMark* getBathMark(GLESRenderElement3D* element);
		void batch(JCSingletonList<GLESRenderElement3D*> &elements);
		void clearRenderData();
		void recoverData();
	private:
		JCSingletonList<GLESInstanceRenderElement3D*> _recoverList;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t,BatchMark*>*> _batchQpaqueMarks;
		uint32_t _updateCountMark = 0;
	};
} // namespace laya
#endif