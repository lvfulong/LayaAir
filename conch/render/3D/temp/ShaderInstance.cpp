#include "ShaderInstance.h"
#include "CommandUniformMap.h"
#include <render/driver/gles/GLStateMap.h>
#include "RenderState.h"
#include <render/3D/temp/RenderStateContext.h>
#include <render/3D/temp/ShaderPass.h>
#include <render/3D/Shader3D.h>
#include <utils/Log.h>
#include "JCConch.h"
#include "JCConchRender.h"

namespace laya
{
	ShaderInstance::ShaderInstance(WebGLEngine* engine, const char* vs, const char* ps, GLAttributeMap* pAttributeMap, GLStateMap* pStateMap/*, RenderState* pRenderState*/) : ResourceBase(JCConch::s_pConchRender->m_pShaderInstanceManager)
	{
		m_pWebGLEngine = engine;
		m_stateParamsMap = pStateMap;
		//m_renderState = pRenderState;
		m_GLShaderInstance = new GLShaderInstance(engine, vs, ps, pAttributeMap);
		_create();
	}
	ShaderInstance::~ShaderInstance()
	{
		if (m_GLShaderInstance)
		{
			delete m_GLShaderInstance;
			m_GLShaderInstance = nullptr;
		}
	}
	bool ShaderInstance::bind()
	{
		return m_GLShaderInstance->bind();
	}
	void ShaderInstance::destroy()
	{
		m_GLShaderInstance->destroy();
	}
	void ShaderInstance::_create()
	{
		CommandUniformMap* sceneParams = CommandUniformMap::createGlobalUniformMap("Scene3D");
		CommandUniformMap* spriteParms = CommandUniformMap::createGlobalUniformMap("Sprite3D");
		CommandUniformMap* cameraParams = CommandUniformMap::createGlobalUniformMap("BaseCamera");
		CommandUniformMap* customParams = CommandUniformMap::createGlobalUniformMap("Custom");

		std::vector<ShaderVariable*>& data = m_GLShaderInstance->getUniformMap();
		for (int i = 0, n = data.size(); i < n; i++)
		{
			ShaderVariable* one = data[i];
			if (sceneParams->hasPtrID(one->dataOffset))
			{
				m_sceneUniformParamsMap.addShaderUniform(one);
			}
			else if (cameraParams->hasPtrID(one->dataOffset))
			{
				m_cameraUniformParamsMap.addShaderUniform(one);
			}
			else if (spriteParms->hasPtrID(one->dataOffset))
			{
				m_spriteUniformParamsMap.addShaderUniform(one);
			}
			else if (customParams->hasPtrID(one->dataOffset))
			{
				m_customUniformParamsMap[one->dataOffset] = one;
			}
			else
			{
				m_materialUniformParamsMap.addShaderUniform(one);
			}
		}
	}
	void ShaderInstance::_disposeResource()
	{
		//this._renderShaderInstance.destroy();
		m_sceneUniformParamsMap.clear();
		m_cameraUniformParamsMap.clear();
		m_spriteUniformParamsMap.clear();
		m_materialUniformParamsMap.clear();
		m_customUniformParamsMap.clear();
		m_stateParamsMap = nullptr;
		//m_renderState = nullptr;
	}
	/*ShaderData::DataInfo* ShaderInstance::_getRenderState(ShaderData* shaderDatas, int stateIndex)
	{
		int stateID = m_stateParamsMap->getData(stateIndex);
		if (stateID == -1)
			return nullptr;
		else
			return shaderDatas->getData(stateID);
	}*/

	int ShaderInstance::uploadUniforms(CommandEncoder* shaderUniform, ShaderData* shaderDatas, bool uploadUnTexture)
	{
		return m_pWebGLEngine->uploadUniforms(m_GLShaderInstance, shaderUniform, shaderDatas, uploadUnTexture);
	}
	int ShaderInstance::uploadCustomUniforms(int index, char* data, int byteSize)
	{
		return m_pWebGLEngine->uploadCustomUniforms(m_GLShaderInstance, m_customUniformParamsMap, index, data, byteSize);
	}
	void ShaderInstance::uploadRenderStateBlendDepth(ShaderData* shaderDatas)
	{
		if (this->_shaderPass->statefirst)
			this->uploadRenderStateBlendDepthByShader(shaderDatas);
		else
			this->uploadRenderStateBlendDepthByMaterial(shaderDatas);
	}
	void ShaderInstance::uploadRenderStateBlendDepthByShader(ShaderData* shaderDatas)
	{
		//todo
		/*var _a, _b, _c, _d, _e, _f, _g, _h, _j, _k, _l, _m, _o, _p, _q, _r, _s, _t, _u, _v, _w, _x, _y, _z, _0, _1, _2, _3, _4, _5, _6, _7;
		var datas = shaderDatas.getData();
		var renderState = this._shaderPass.renderState;
		var depthWrite = (_b = ((_a = renderState.depthWrite) != nullptr  ? _a : datas[Shader3D::DEPTH_WRITE])) != nullptr ? _b : RenderState::Default.depthWrite;
		RenderStateContext::setDepthMask(depthWrite);
		var depthTest = (_d = ((_c = renderState.depthTest) != nullptr  ? _c : datas[Shader3D::DEPTH_TEST])) != nullptr ? _d : RenderState::Default.depthTest;
		if (depthTest == RenderState::DEPTHTEST_OFF)
			RenderStateContext::setDepthTest(false);
		else {
			RenderStateContext::setDepthTest(true);
			RenderStateContext::setDepthFunc(depthTest);
		}
		bool* stencilWrite = (_f = ((_e = renderState.stencilWrite) != nullptr ? _e : datas[Shader3D::STENCIL_WRITE])) != nullptr ? _f : RenderState::Default.stencilWrite;
		var stencilTest = (_h = ((_g = renderState.stencilTest) != nullptr ? _g : datas[Shader3D::STENCIL_TEST])) != nullptr ? _h : RenderState::Default.stencilTest;
		RenderStateContext::setStencilMask(stencilWrite);
		if (stencilWrite) {
			var stencilOp = (_k = ((_j = renderState.stencilOp) != = null && _j != = void 0 ? _j : datas[Shader3D.STENCIL_Op])) != = null && _k != = void 0 ? _k : RenderState.Default.stencilOp;
			RenderStateContext.setstencilOp(stencilOp.x, stencilOp.y, stencilOp.z);
		}
		if (stencilTest == RenderState::STENCILTEST_OFF) {
			RenderStateContext.setStencilTest(false);
		}
		else {
			var stencilRef = (_m = ((_l = renderState.stencilRef) != = null && _l != = void 0 ? _l : datas[Shader3D.STENCIL_Ref])) != = null && _m != = void 0 ? _m : RenderState.Default.stencilRef;
			RenderStateContext.setStencilTest(true);
			RenderStateContext.setStencilFunc(stencilTest, stencilRef);
		}
		var blend = (_p = ((_o = renderState.blend) != = null && _o != = void 0 ? _o : datas[Shader3D.BLEND])) != = null && _p != = void 0 ? _p : RenderState.Default.blend;
		switch (blend) {
		case RenderState::BLEND_DISABLE:
			RenderStateContext::setBlend(false);
			break;
		case RenderState::BLEND_ENABLE_ALL:
			var blendEquation = (_r = ((_q = renderState.blendEquation) != = null && _q != = void 0 ? _q : datas[Shader3D.BLEND_EQUATION])) != = null && _r != = void 0 ? _r : RenderState.Default.blendEquation;
			var srcBlend = (_t = ((_s = renderState.srcBlend) != = null && _s != = void 0 ? _s : datas[Shader3D.BLEND_SRC])) != = null && _t != = void 0 ? _t : RenderState.Default.srcBlend;
			var dstBlend = (_v = ((_u = renderState.dstBlend) != = null && _u != = void 0 ? _u : datas[Shader3D.BLEND_DST])) != = null && _v != = void 0 ? _v : RenderState.Default.dstBlend;
			RenderStateContext::setBlend(true);
			RenderStateContext::setBlendEquation(blendEquation);
			RenderStateContext::setBlendFunc(srcBlend, dstBlend);
			break;
		case RenderState::BLEND_ENABLE_SEPERATE:
			var blendEquationRGB = (_x = ((_w = renderState.blendEquationRGB) != nullptr && _w != = void 0 ? _w : datas[Shader3D.BLEND_EQUATION_RGB])) != = null && _x != = void 0 ? _x : RenderState.Default.blendEquationRGB;
			var blendEquationAlpha = (_z = ((_y = renderState.blendEquationAlpha) != nullptr && _y != = void 0 ? _y : datas[Shader3D.BLEND_EQUATION_ALPHA])) != = null && _z != = void 0 ? _z : RenderState.Default.blendEquationAlpha;
			var srcRGB = (_1 = ((_0 = renderState.srcBlendRGB) != nullptr && _0 != = void 0 ? _0 : datas[Shader3D.BLEND_SRC_RGB])) != = null && _1 != = void 0 ? _1 : RenderState.Default.srcBlendRGB;
			var dstRGB = (_3 = ((_2 = renderState.dstBlendRGB) != nullptr && _2 != = void 0 ? _2 : datas[Shader3D.BLEND_DST_RGB])) != = null && _3 != = void 0 ? _3 : RenderState.Default.dstBlendRGB;
			var srcAlpha = (_5 = ((_4 = renderState.srcBlendAlpha) != nullptr && _4 != = void 0 ? _4 : datas[Shader3D.BLEND_SRC_ALPHA])) != = null && _5 != = void 0 ? _5 : RenderState.Default.srcBlendAlpha;
			var dstAlpha = (_7 = ((_6 = renderState.dstBlendAlpha) != nullptr && _6 != = void 0 ? _6 : datas[Shader3D.BLEND_DST_ALPHA])) != = null && _7 != = void 0 ? _7 : RenderState.Default.dstBlendAlpha;
			RenderStateContext::setBlend(true);
			RenderStateContext::setBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
			RenderStateContext::setBlendFuncSeperate(srcRGB, dstRGB, srcAlpha, dstAlpha);
			break;
		}*/
	}
	void ShaderInstance::uploadRenderStateBlendDepthByMaterial(ShaderData* shaderDatas) {
		//var datas = shaderDatas.getData();
		//var depthWrite = datas[Shader3D.DEPTH_WRITE];

		bool* depthWrite = (bool*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_WRITE);
		depthWrite = depthWrite != nullptr /*&& depthWrite != = void 0*/ ? depthWrite : RenderState::Default.depthWrite;
		RenderStateContext::setDepthMask(depthWrite);


		//var depthTest = datas[Shader3D.DEPTH_TEST];
		int32_t* depthTest = shaderDatas->getData<int32_t>(Shader3D::DEPTH_TEST);

		depthTest = depthTest != nullptr /* && depthTest != = void 0*/ ? depthTest : RenderState::Default.depthTest;
		if (*depthTest == RenderState::DEPTHTEST_OFF) {
			RenderStateContext::setDepthTest(false);
		}
		else {
			RenderStateContext::setDepthTest(true);
			RenderStateContext::setDepthFunc((CompareFunction)*depthTest);
		}
		//var stencilWrite = datas[Shader3D::STENCIL_WRITE];
		bool* stencilWrite = (bool*)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE);

		stencilWrite = stencilWrite != nullptr /* && stencilWrite != = void 0*/ ? stencilWrite : RenderState::Default.stencilWrite;
		RenderStateContext::setStencilMask(stencilWrite);
		if (stencilWrite) {
			//var stencilOp = datas[Shader3D::STENCIL_Op];
			Vector3* stencilOp = shaderDatas->getData<Vector3>(Shader3D::STENCIL_Op);
			auto* stencilOpX = stencilOp != nullptr /* && stencilOp != = void 0*/ ? &stencilOp->x : RenderState::Default.stencilOpX;
			auto* stencilOpY = stencilOp != nullptr /* && stencilOp != = void 0*/ ? &stencilOp->y : RenderState::Default.stencilOpY;
			auto* stencilOpZ = stencilOp != nullptr /* && stencilOp != = void 0*/ ? &stencilOp->z : RenderState::Default.stencilOpZ;
			RenderStateContext::setstencilOp((StencilOperation)*stencilOpX, (StencilOperation)*stencilOpY, (StencilOperation)*stencilOpZ);
		}
		//var stencilTest = datas[Shader3D::STENCIL_TEST];
		int32_t* stencilTest = shaderDatas->getData<int32_t>(Shader3D::STENCIL_TEST);
		stencilTest = stencilTest != nullptr /* && stencilTest != = void 0*/ ? stencilTest : RenderState::Default.stencilTest;
		if (*stencilTest == RenderState::STENCILTEST_OFF) {
			RenderStateContext::setStencilTest(false);
		}
		else {
			//var stencilRef = datas[Shader3D::STENCIL_Ref];
			int32_t* stencilRef = shaderDatas->getData<int32_t>(Shader3D::STENCIL_Ref);
			stencilRef = stencilRef != nullptr /* && stencilRef != = void 0*/ ? stencilRef : RenderState::Default.stencilRef;
			RenderStateContext::setStencilTest(true);
			RenderStateContext::setStencilFunc((CompareFunction)*stencilTest, *stencilRef);
		}
		//var blend = datas[Shader3D.BLEND];
		int32_t* blend = shaderDatas->getData<int32_t>(Shader3D::BLEND);
		blend = blend != nullptr /* && blend != = void 0*/ ? blend : RenderState::Default.blend;
		switch (*blend) {
		case RenderState::BLEND_ENABLE_ALL:
		{
			//var blendEquation = datas[Shader3D::BLEND_EQUATION];
			int32_t* blendEquation = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION);
			blendEquation = blendEquation != nullptr /* && blendEquation != = void 0*/ ? blendEquation : RenderState::Default.blendEquation;
			//var srcBlend = datas[Shader3D::BLEND_SRC];
			int32_t* srcBlend = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC);
			srcBlend = srcBlend != nullptr /* && srcBlend != = void 0*/ ? srcBlend : RenderState::Default.srcBlend;
			//var dstBlend = datas[Shader3D::BLEND_DST];
			int32_t* dstBlend = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST);
			dstBlend = dstBlend != nullptr /* && dstBlend != = void 0*/ ? dstBlend : RenderState::Default.dstBlend;
			RenderStateContext::setBlend(true);
			RenderStateContext::setBlendEquation((BlendEquationSeparate)*blendEquation);
			RenderStateContext::setBlendFunc((BlendFactor)*srcBlend, (BlendFactor)*dstBlend);
		}
			break;
		case RenderState::BLEND_ENABLE_SEPERATE:
		{
			//var blendEquationRGB = datas[Shader3D::BLEND_EQUATION_RGB];
			int32_t* blendEquationRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_RGB);
			blendEquationRGB = blendEquationRGB != nullptr /* && blendEquationRGB != = void 0*/ ? blendEquationRGB : RenderState::Default.blendEquationRGB;
			//var blendEquationAlpha = datas[Shader3D::BLEND_EQUATION_ALPHA];
			int32_t* blendEquationAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_ALPHA);
			blendEquationAlpha = blendEquationAlpha != nullptr /*&& blendEquationAlpha != = void 0*/ ? blendEquationAlpha : RenderState::Default.blendEquationAlpha;
			//var srcRGB = datas[Shader3D::BLEND_SRC_RGB];
			int32_t* srcRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_RGB);
			srcRGB = srcRGB != nullptr /*&& srcRGB != = void 0*/ ? srcRGB : RenderState::Default.srcBlendRGB;
			//var dstRGB = datas[Shader3D::BLEND_DST_RGB];
			int32_t* dstRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_RGB);
			dstRGB = dstRGB != nullptr /*&& dstRGB != = void 0*/ ? dstRGB : RenderState::Default.dstBlendRGB;
			//var srcAlpha = datas[Shader3D::BLEND_SRC_ALPHA];
			int32_t* srcAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_ALPHA);
			srcAlpha = srcAlpha != nullptr /*&& srcAlpha != = void 0*/ ? srcAlpha : RenderState::Default.srcBlendAlpha;
			//var dstAlpha = datas[Shader3D::BLEND_DST_ALPHA];
			int32_t* dstAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_ALPHA);
			dstAlpha = dstAlpha != nullptr /* && dstAlpha != = void 0*/ ? dstAlpha : RenderState::Default.dstBlendAlpha;
			RenderStateContext::setBlend(true);
			RenderStateContext::setBlendEquationSeparate((BlendEquationSeparate)*blendEquationRGB, (BlendEquationSeparate)*blendEquationAlpha);
			RenderStateContext::setBlendFuncSeperate((BlendFactor)*srcRGB, (BlendFactor)*dstRGB, (BlendFactor)*srcAlpha, (BlendFactor)*dstAlpha);
		}
			break;
		case RenderState::BLEND_DISABLE:
		default:
			RenderStateContext::setBlend(false);
			break;
		};
	}
	void ShaderInstance::uploadRenderStateFrontFace(ShaderData* shaderDatas, bool isTarget, bool invertFront)
	{
		int32_t* _a = nullptr;
		RenderState* renderState = this->_shaderPass->renderState;

		int32_t* cull = shaderDatas->getData<int32_t>(Shader3D::CULL);

		if (this->_shaderPass->statefirst) 
		{
			cull = (_a = renderState->cull) != nullptr /* && _a !== void 0*/ ? _a : cull;
		}
		cull = cull != nullptr /*&& cull != = void 0*/ ? cull : RenderState::Default.cull;

		CullMode forntFace;
		switch (*cull)
		{
		case RenderState::CULL_NONE:
			RenderStateContext::setCullFace(false);
				if (isTarget != invertFront)
					forntFace = CullMode::Front;//gl.CCW
				else
					forntFace = CullMode::Back;
				RenderStateContext::setFrontFace(forntFace);
			break;
		case RenderState::CULL_FRONT:
			RenderStateContext::setCullFace(true);
			if (isTarget == invertFront)
				forntFace = CullMode::Front;//gl.CCW
			else
				forntFace = CullMode::Back;
			RenderStateContext::setFrontFace(forntFace);
			break;
		case RenderState::CULL_BACK:
			RenderStateContext::setCullFace(true);
			if (isTarget != invertFront)
				forntFace = CullMode::Front;//gl.CCW
			else
				forntFace = CullMode::Back;
			RenderStateContext::setFrontFace(forntFace);
			break;
		}
	}
	ShaderInstance* ShaderInstance::getShaderInstance(uint32_t id)
	{
		return JCConch::s_pConchRender->m_pShaderInstanceManager->getResource(id);
	}
}
//------------------------------------------------------------------------------
