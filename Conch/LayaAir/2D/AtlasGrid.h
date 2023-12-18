#ifndef __AtlasGrid_H__
#define __AtlasGrid_H__

#include<stdint.h>
#include "Point.h"

namespace laya
{
	class AtlasGrid
	{	
	public:
		AtlasGrid(int width = 0, int height = 0/*, int id = 0*/);
		~AtlasGrid();
		bool addRect(int type, int width, int height, Point& pt);
		float getUsed() { return m_used; }
	private:
		void _check(bool ret);
		bool _get(int width, int height, Point& pt);
		void _clear();
		void _fill(int x, int y, int w, int h, int type);
		int m_width;
		int m_height;
		float m_used;
		uint8_t* m_rowInfo = nullptr;		// 当前行的最大长度
		uint8_t* m_cells = nullptr;	// 每个格子的信息。{type,w,h} 相当于一个距离场. type =0 表示空闲的。不为0的情况下填充的是宽高（有什么用呢）
	};
}
#endif //__AtlasGrid_H__