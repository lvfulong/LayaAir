#include "GLRenderDrawContext.h"
#include "GLCapable.h"
#include "GLEnum/WebGLExtension.h"
#include "GLVertexState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>
#include <utils/Preprocessor.h>

namespace laya
{
GLRenderDrawContext::GLRenderDrawContext(WebGLEngine *engine) : GLObject(engine)
{
    if (!m_engine->isWebGL2())
    {
        m_angleInstancedArrays =
            (ANGLEInstancedArraysExt *)m_engine->getExtension(WebGLExtension::ANGLE_instanced_arrays);
    }
}
GLenum GLRenderDrawContext::getMeshTopology(MeshTopology mode)
{
    switch (mode)
    {
    case MeshTopology::Points:
        return GL_POINTS;
    case MeshTopology::Lines:
        return GL_LINES;
    case MeshTopology::LineLoop:
        return GL_LINE_LOOP;
    case MeshTopology::LineStrip:
        return GL_LINE_STRIP;
    case MeshTopology::Triangles:
        return GL_TRIANGLES;
    case MeshTopology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case MeshTopology::TriangleFan:
        return GL_TRIANGLE_FAN;
    default:
        assert("invalid MeshTopology value");
        return GL_POINTS;
    }
}

GLenum GLRenderDrawContext::getIndexType(IndexFormat type)
{
    switch (type)
    {
    case IndexFormat::UInt8:
        return GL_UNSIGNED_BYTE;
    case IndexFormat::UInt16:
        return GL_UNSIGNED_SHORT;
    case IndexFormat::UInt32:
        return GL_UNSIGNED_INT;
    default:
        assert("invalid IndexFormat value");
        return GL_UNSIGNED_BYTE;
    }
}
void GLRenderDrawContext::drawElementsInstanced(int glmode, int count, int /*IndexFormat*/ gltype, int offset,
                                                int instanceCount)
{
    // GLenum glmode = getMeshTopology(mode);
    // GLenum gltype = getIndexType(type);

    if (m_engine->isWebGL2())
        glDrawElementsInstanced(glmode, count, gltype, (const void *)offset, instanceCount);
    else
        m_angleInstancedArrays->drawElementsInstancedANGLE(glmode, count, gltype, (const void *)offset, instanceCount);

    m_engine->_addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::InstanceDrawCall, 1);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::Triangle, count / 3 * instanceCount);
}
void GLRenderDrawContext::drawArraysInstanced(int glmode, int first, int count, int instanceCount)
{
    // GLenum glmode = getMeshTopology(mode);

    if (m_engine->isWebGL2())
        glDrawArraysInstanced(glmode, first, count, instanceCount);
    else
        m_angleInstancedArrays->drawArraysInstancedANGLE(glmode, first, count, instanceCount);

    m_engine->_addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::InstanceDrawCall, 1);
    // TODO glmode
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::Triangle, (count - 2) * instanceCount);
}
void GLRenderDrawContext::drawArrays(int glmode, int first, int count)
{
    // GLenum glmode = getMeshTopology(mode);
    glDrawArrays(glmode, first, count);

    m_engine->_addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
    // TODO glmode
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::Triangle, (count - 2));
}
void GLRenderDrawContext::drawElements(int glmode, int count, int /*IndexFormat*/ gltype, int offset)
{
    // GLenum glmode = getMeshTopology(mode);
    // GLenum gltype = getIndexType(type);
    glDrawElements(glmode, count, gltype, (const void *)offset);

    m_engine->_addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::Triangle, count / 3);
}
void GLRenderDrawContext::drawElements2DTemp(MeshTopology mode, int count, IndexFormat type, int offset)
{
    GLenum glmode = getMeshTopology(mode);
    GLenum gltype = getIndexType(type);
    glDrawElements(glmode, count, gltype, (const void *)offset);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
    m_engine->_addStatisticsInfo(RenderStatisticsInfo::Triangle, count / 3);
}

void GLRenderDrawContext::drawGeometryElement(GLESRenderGeometryElement *geometryElement)
{
    geometryElement->_bufferState->bind();
    std::vector<int> &element = geometryElement->m_pDrawParams->m_vElements;
    int length = geometryElement->m_pDrawParams->getLength();
    switch (geometryElement->m_nDrawType)
    {
    case DrawType::DrawArray:
        for (int i = 0; i < length; i += 2)
        {
            drawArrays(geometryElement->_glmode, element[i], element[i + 1]);
        }
        break;
    case DrawType::DrawElement:
        for (int i = 0; i < length; i += 2)
        {
            drawElements(geometryElement->_glmode, element[i + 1], geometryElement->_glindexFormat, element[i]);
        }
        break;
    case DrawType::DrawArrayInstance:
        for (int i = 0; i < length; i += 2)
        {
            drawArraysInstanced(geometryElement->_glmode, element[i], element[i + 1],
                                geometryElement->m_nInstanceCount);
        }
        break;
    case DrawType::DrawElementInstance:
        for (int i = 0; i < length; i += 2)
        {
            drawElementsInstanced(geometryElement->_glmode, element[i + 1], geometryElement->_glindexFormat, element[i],
                                  geometryElement->m_nInstanceCount);
        }
        break;
    default:
        break;
    }
}
} // namespace laya
