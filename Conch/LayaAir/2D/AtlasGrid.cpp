#include "AtlasGrid.h"
#include <cassert>
#include <utils/Log.h>

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
		//���û��Ӧ�÷����� ����ֵ����������bRet�Ƿ�ɹ���nX xλ�ã�nY yλ��
		if (!_get(width, height, pt))
			return false;
		//���ݻ�õ�x,y���
		_fill(pt.x, pt.y, width, height, type);
		//this._texCount++;
		//�����Ƿ�ɹ����Լ�Xλ�ú�Yλ��
		return true;
	}
	bool AtlasGrid::_get(int width, int height, Point& pt)
	{
		if (width > m_width || height > m_height)
		{
			return false;
		}
		//���巵�ص�x,y��λ��
		int rx = -1;
		int ry = -1;
		//Ϊ��Ч���ȱ�����ʱ����
		int nWidth = m_width;
		int nHeight = m_height;
		//����һ������Ϊ��ָ�� m_pCells
		uint8_t* pCellBox = m_cells;

		//�������Һ��ʵ�λ��  //TODO ����ķ���Ӧ�ÿ����Ż�
		for (int y = 0; y < nHeight; y++)
		{
			//������еĿհ��� С�� Ҫ����Ŀ��ȷ���
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
				// ��鵱ǰ�����Ƿ�����ȫ���£���x�����ÿ��λ�ö����㹻�ĸ߶ȡ�
				for (int xx = 0; xx < width; xx++)
				{
					if (pCellBox[3 * xx + tm + 2] < height) 
					{
						rx = -1;
						break;
					}
				}
				// ���о�x����ǰ��
				if (rx < 0) 
				{
					x += pCellBox[tm + 1];
					continue;
				}
				// �ҵ���
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
		//����һЩ��ʱ����
		int nWidth = m_width;
		int nHeghit = m_height;
		//������
		_check((x + w) <= nWidth && (y + h) <= nHeghit);

		//���
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
		//�����������ڿհ׸��ӵĿ���������Ϣ����
		if (x > 0) 
		{
			for (int yy = 0; yy < h; ++yy)
			{
				// TODO ����Ӧ�ÿ����Ż�
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
		//�������Ϸ����ڿհ׸��ӵĸ߶�������Ϣ����
		if (y > 0)
		{
			for (int xx = x; xx < (x + w); ++xx)
			{
				// TODO ����Ӧ�ÿ����Ż�
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
			LOGI("xtexMerger ������");
		}
	}
}
//------------------------------------------------------------------------------
