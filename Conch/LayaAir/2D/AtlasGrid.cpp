#include "AtlasGrid.h"
#include <cassert>
#include <Utils/Log.h>

namespace laya
{
	AtlasGrid::AtlasGrid(int width, int height/*, int id*/)
	{
		//this.atlasID = id;
		assert(width != 0 && height != 0);
		m_width = width;
		m_height = height;
		m_cells = new uint8_t[m_width * m_height * 3];
		m_rowInfo = new uint8_t[m_height];
		m_used = 0.0f;
		_clear();
	}
	AtlasGrid::~AtlasGrid()
	{
		if (m_cells)
		{
			delete[] m_cells;
			m_cells = nullptr;
		}

		if (m_rowInfo)
		{
			delete[] m_rowInfo;
			m_rowInfo = nullptr;
		}
	}
	bool AtlasGrid::addRect(int type, int width, int height, Point& pt)
	{
		//调用获得应该放在哪 返回值有三个。。bRet是否成功，nX x位置，nY y位置
		if (!_get(width, height, pt))
			return false;
		//根据获得的x,y填充
		_fill(pt.x, pt.y, width, height, type);
		//this._texCount++;
		//返回是否成功，以及X位置和Y位置
		return true;
	}
	bool AtlasGrid::_get(int width, int height, Point& pt)
	{
		if (width > m_width || height > m_height)
		{
			return false;
		}
		//定义返回的x,y的位置
		int rx = -1;
		int ry = -1;
		//为了效率先保存临时变量
		int nWidth = m_width;
		int nHeight = m_height;
		//定义一个变量为了指向 m_pCells
		uint8_t* pCellBox = m_cells;

		//遍历查找合适的位置  //TODO 下面的方法应该可以优化
		for (int y = 0; y < nHeight; y++)
		{
			//如果该行的空白数 小于 要放入的宽度返回
			if (m_rowInfo[y] < width) 
				continue;
			for (int x = 0; x < nWidth;)
			{

				int tm = (y * nWidth + x) * 3;

				if (pCellBox[tm] != 0 || pCellBox[tm + 1] < width || pCellBox[tm + 2] < height) 
				{
					x += pCellBox[tm + 1];
					continue;
				}
				rx = x;
				ry = y;
				// 检查当前宽度是否能完全放下，即x方向的每个位置都有足够的高度。
				for (int xx = 0; xx < width; xx++)
				{
					if (pCellBox[3 * xx + tm + 2] < height) 
					{
						rx = -1;
						break;
					}
				}
				// 不行就x继续前进
				if (rx < 0) 
				{
					x += pCellBox[tm + 1];
					continue;
				}
				// 找到了
				pt.x = rx;
				pt.y = ry;
				return true;
			}
		}
		return false;
	}
	void AtlasGrid::_clear()
	{
		//this._texCount = 0;
		for (int y = 0; y < m_height; y++) 
		{
			m_rowInfo[y] =m_width;
		}
		for (int i = 0; i < m_height; i++)
		{
			for (int j = 0; j < m_width; j++)
			{
				int tm = (i * m_width + j) * 3;
				m_cells[tm] = 0;
				m_cells[tm + 1] = m_width - j;
				m_cells[tm + 2] = m_width - i;
			}
		}
	}
	void AtlasGrid::_fill(int x, int y, int w, int h, int type)
	{
		//定义一些临时变量
		int nWidth = m_width;
		int nHeghit = m_height;
		//代码检查
		_check((x + w) <= nWidth && (y + h) <= nHeghit);

		//填充
		for (int yy = y; yy < (h + y); ++yy) 
		{
			_check(m_rowInfo[yy] >= w);
			m_rowInfo[yy] -= w;
			for (int xx = 0; xx < w; xx++)
			{
				int tm = (x + yy * nWidth + xx) * 3;
				_check(m_cells[tm] == 0);
				m_cells[tm] = type;
				m_cells[tm + 1] = w;
				m_cells[tm + 2] = h;
			}
		}
		//调整我左方相邻空白格子的宽度连续信息描述
		if (x > 0) 
		{
			for (int yy = 0; yy < h; ++yy)
			{
				// TODO 下面应该可以优化
				int s = 0;
				for (int xx = x - 1; xx >= 0; --xx, ++s)
				{
					if (m_cells[((y + yy) * nWidth + xx) * 3] != 0) 
						break;
				}
				for (int xx = s; xx > 0; --xx)
				{
					m_cells[((y + yy) * nWidth + x - xx) * 3 + 1] = xx;
					_check(xx > 0);
				}
			}
		}
		//调整我上方相邻空白格子的高度连续信息描述
		if (y > 0)
		{
			for (int xx = x; xx < (x + w); ++xx)
			{
				// TODO 下面应该可以优化
				int s = 0;
				for (int yy = y - 1; yy >= 0; --yy, s++)
				{
					if (m_cells[(xx + yy * nWidth) * 3] != 0) 
						break;
				}
				for (int yy = s; yy > 0; --yy)
				{
					m_cells[(xx + (y - yy) * nWidth) * 3 + 2] = yy;
					_check(yy > 0);
				}
			}
		}

		m_used += (float)(w * h) / (float)(m_width * m_height);
	}

	void AtlasGrid::_check(bool ret)
	{
		if (ret == false) 
		{
			LOGI("xtexMerger 错误啦");
		}
	}
}
//------------------------------------------------------------------------------
