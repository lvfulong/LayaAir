#include "SkinnedMeshSprite3DProperty.h"
namespace laya
{
uint32_t SkinnedMeshSprite3DProperty::BONES = 0;
void SkinnedMeshSprite3DProperty::__init__()
{
    SkinnedMeshSprite3DProperty::BONES = LayaGL::m_pWebglEngine->propertyNameToID("u_Bones");

}
}; // namespace laya