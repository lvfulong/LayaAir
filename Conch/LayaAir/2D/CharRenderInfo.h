#ifndef __CharRenderInfo_H__
#define __CharRenderInfo_H__

#include <string>
#include <memory>

namespace laya
{
	class TextTexture;
	struct RenderInfo
	{
		static int64_t loopStTm;
		static int64_t loopCount;
	};

	struct CharRenderInfo
	{
		std::string str;
		std::weak_ptr<TextTexture> tex;	
		bool deleted = false; 	// 已经被删除了 
		float uv[8];// [0, 0, 1, 1];		//uv
		int pos = 0;					//数组下标
		int width;					//字体宽度。测量的宽度，用来排版。没有缩放
		int height; 				//字体高度。没有缩放
		int bmpWidth;				//实际图片的宽度。可能与排版用的width不一致。包含缩放和margin
		int bmpHeight;
		int orix = 0;				// 原点位置，通常都是所在区域的左上角
		int oriy = 0;
		uint64_t touchTick = 0;		//
		bool isSpace = false;		//是否是空格，如果是空格，则只有width有效
		bool isoTexture = false;
		void touch();
		~CharRenderInfo();
	};
}
#endif //__CharRenderInfo_H__