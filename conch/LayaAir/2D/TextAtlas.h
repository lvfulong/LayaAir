#ifndef __TextAtlas_H__
#define __TextAtlas_H__

#include <unordered_map>
#include "Point.h"
#include <memory>
#include "CharRenderInfo.h"
namespace laya
{
	class GLESEngine;
	class TextTexture;
	class AtlasGrid;
	class TextAtlas
	{	
	public:
		TextAtlas(GLESEngine* pWebglEngine);
		~TextAtlas();

		std::shared_ptr<CharRenderInfo> find(const char* key);
		void add(const char* key, std::shared_ptr<CharRenderInfo> info);
		bool getAEmpty(int w, int h, Point& pt);//TODO float??
		int getUsedRate();
	private:
		void destroy();
	public:
		int m_texWidth = 1024;
		int	m_texHeight = 1024;
		std::shared_ptr<TextTexture> m_texture;
		AtlasGrid* m_atlasgrid;
		static int atlasGridW;
		typedef std::unordered_map<std::string, std::shared_ptr<CharRenderInfo> > CharMapType;
		CharMapType m_charMaps;		// ����������Ϣ���ֵ�
	};
}
#endif //__TextAtlas_H__