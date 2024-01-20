#include "RenderElementOBJ.h"

namespace laya
{
RenderElementOBJ::RenderElementOBJ()
{
}
RenderElementOBJ::~RenderElementOBJ()
{
}
void RenderElementOBJ::_render(IRenderContext3D* context) 
{
	//TODO
}
void RenderElementOBJ::_preUpdatePre(IRenderContext3D* context) 
{
	//TODO
}


compileShader(DefineDatas* compileDefine) {
	//根据subpass 和pipeline 得到shaderpass
	//根据shaderpass 和 defineData 宏来得到shader
	//得到shader的步骤：1、getCacheShader。2、没有get到就创建shader（脚本）

}
} // namespace laya
