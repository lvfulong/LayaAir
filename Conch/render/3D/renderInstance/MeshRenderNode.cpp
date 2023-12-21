#include "MeshRenderNode.h"
namespace laya {
	MeshRenderNode::MeshRenderNode()
	{
	}

	MeshRenderNode::~MeshRenderNode()
	{
	}

	void MeshRenderNode::_calculateGeometryBoundingBox() {
		geometryBounds._tranform(transform->getWorldMatrix(), *compose.bounds);
		boundsChange = false;
	}

	void MeshRenderNode::_renderUpdate(NodeContext3DData data) {
		if (lightmapdata.lightmapDirtyFlag == data.sceneLightmapDirtyFlag)
			_applyLightmap();
		_applyReflection();
		_applyLightProb();
		//TODO set ShaderData
		//this._setShaderValue(Sprite3D.WORLDMATRIX, ShaderDataType.Matrix4x4, trans.worldMatrix);
		//this._worldParams.x = trans.getFrontFaceValue();
		//this._setShaderValue(Sprite3D.WORLDINVERTFRONT, ShaderDataType.Vector4, this._worldParams);
		return;
	}
}
