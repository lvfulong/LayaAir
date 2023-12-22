#include "Sprite3DProperty.h"
namespace laya
{
uint32_t Sprite3DProperty::WORLDMATRIX = 0;
uint32_t Sprite3DProperty::WORLDINVERTFRONT = 0;

void Sprite3DProperty::__init__()
{
    Sprite3DProperty::WORLDMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_WorldMat");
    Sprite3DProperty::WORLDINVERTFRONT = LayaGL::m_pWebglEngine->propertyNameToID("u_WroldInvertFront");
}
}; // namespace laya