#include "BaseCameraProperty.h"
namespace laya
{
uint32_t BaseCameraProperty::VIEWMATRIX = 0;
uint32_t BaseCameraProperty::PROJECTMATRIX = 0;
uint32_t BaseCameraProperty::VIEWPROJECTMATRIX = 0;
void BaseCameraProperty::__init__()
{
    BaseCameraProperty::VIEWMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_View");
    BaseCameraProperty::PROJECTMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_Projection");
    BaseCameraProperty::VIEWPROJECTMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_ViewProjection");
}
}; // namespace laya