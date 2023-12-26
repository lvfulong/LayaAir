#include "JSImageBitmap.h"
#include "../../JCScriptRuntime.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>

namespace laya
{
JSImageBitmap::JSImageBitmap()
{
    AdjustAmountOfExternalAllocatedMemory(128);
    JCMemorySurvey::GetInstance()->newClass("JSImageBitmap", 10240, this);
}
JSImageBitmap::~JSImageBitmap()
{
    JCMemorySurvey::GetInstance()->releaseClass("JSImageBitmap", this);
}
int JSImageBitmap::getWidth()
{
    return m_ImageBitmap.getWidth();
}
int JSImageBitmap::getHeight()
{
    return m_ImageBitmap.getHeight();
}
void JSImageBitmap::close()
{
    m_ImageBitmap.close();
}
int JSImageBitmap::getImageID()
{
    return m_ImageBitmap.getImageID();
}
void JSImageBitmap::exportJS(Context &context)
{
    class_<JSImageBitmap> class_binding;
    class_binding.constructor<>();
    class_binding.property("width", &JSImageBitmap::getWidth);
    class_binding.property("height", &JSImageBitmap::getHeight);
    class_binding.property("conchImgId", &JSImageBitmap::getImageID);
    context.class_("ImageBitmap", class_binding);
}
} // namespace laya
