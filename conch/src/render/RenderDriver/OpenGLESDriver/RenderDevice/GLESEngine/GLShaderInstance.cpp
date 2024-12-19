#include "GLShaderInstance.h"
#include "JCConch.h"
#include "JCSystemConfig.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include <assert.h>
#include <unordered_map>
#include <utils/Log.h>
#include <utils/Preprocessor.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBufferBase.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBuffer.h>
namespace laya
{
GLShaderInstance::GLShaderInstance(GLESEngine *engine, const char *vs, const char *ps,
                                   const std::unordered_map<std::string, int32_t> &attributeMap)
    : GLObject(engine)
{
    m_vs = vs;
    m_ps = ps;
    m_attributeMap = attributeMap;
    m_uniformMap.clear();
    _create();
}
GLShaderInstance::~GLShaderInstance()
{
    destroy();
}
void GLShaderInstance::_create()
{
    m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_ShaderCompile, 1);
    //let preTime = performance.now(); 计算时间TODO
    m_program = glCreateProgram();
    m_vshader = _createShader(m_vs, GL_VERTEX_SHADER);
    m_pshader = _createShader(m_ps, GL_FRAGMENT_SHADER);
    glAttachShader(m_program, m_vshader);
    glAttachShader(m_program, m_pshader);

    for (std::unordered_map<std::string, int32_t>::iterator it = m_attributeMap.begin(); it != m_attributeMap.end();
         it++)
    {
        glBindAttribLocation(m_program, it->second, it->first.c_str());
    }

    glLinkProgram(m_program);

    GLint linkStatus = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        this->_complete = false;
        GLint bufLength = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &bufLength);
        if (bufLength)
        {
            char *buf = new char[bufLength + 1];
            buf[bufLength] = '\0';

            glGetProgramInfoLog(m_program, bufLength, NULL, buf);
            std::string info = buf;
            LOGE("GLShaderInstance::compile Could not link program:\n%s\n", info.c_str());
            // assert("'Could not compile WebGL program. \n\n");
            delete[] buf;
        }
    }
    // Uniform
    // Unifrom Objcet
    int nUniformNum = 0;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &nUniformNum);

    useProgram();
    m_curActTexIndex = 0;
    char sVarName[64];
    for (int i = 0; i < nUniformNum; i++)
    {
        // var uniformData : WebGLActiveInfo = gl.getActiveUniform(this._program, i);

        GLenum type;
        int length, count;
        glGetActiveUniform(m_program, i, sizeof(sVarName), &length, &count, &type, sVarName);
        int location = glGetUniformLocation(m_program, sVarName);
        if (location<0 )
            continue;
        ShaderVariable *one = new ShaderVariable();
        one->location = location;

        int nVarNameLenght = strlen(sVarName);
        if (nVarNameLenght > 4 && strcmp(sVarName + (nVarNameLenght - 3), "[0]") == 0)
        {
            sVarName[nVarNameLenght - 3] = 0;

            one->name = sVarName;
            one->isArray = true;
        }
        else
        {
            one->name = sVarName;
            one->isArray = false;
        }
        one->type = type;
        one->count = count;
        _addShaderUnifiormFun(one);
        m_uniformMap.push_back(one);
        one->dataOffset = m_engine->propertyNameToID(sVarName);
    }
    if (m_engine->isWebGL2())
    {
        m_uniformObjectMap.clear();

        GLint nUniformBlock = 0;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &nUniformBlock);

        for (int i = 0; i < nUniformBlock; i++)
        {
            const GLsizei nBufferSize = 256;
            char uniformBlockName[nBufferSize] = {0};
            GLsizei nLength = 0;
          
            glGetActiveUniformBlockName(m_program, i, nBufferSize, &nLength, uniformBlockName);
            ShaderVariable *one = new ShaderVariable();
            one->name = uniformBlockName;
            one->isArray = false;
            one->type = GL_UNIFORM_BUFFER;
            one->dataOffset = m_engine->propertyNameToID(uniformBlockName);
            int location = glGetUniformBlockIndex(m_program, uniformBlockName);
            int bindingPoint = one->location = i;
            glUniformBlockBinding(m_program,location, bindingPoint);
            m_uniformObjectMap[one->name] = one;
            m_uniformMap.push_back(one);
            _addShaderUnifiormFun(one);
        }
    }
    //WebGLEngine.instance._addStatisticsInfo(GPUEngineStatisticsInfo.T_ShaderCompile, (performance.now() - preTime) | 0); TODO
}

GLuint GLShaderInstance::_createShader(const std::string &str, int type)
{
    GLuint shader = glCreateShader(type);
    const char *strShaders[1] = {str.c_str()};
    glShaderSource(shader, 1, strShaders, NULL);
    glCompileShader(shader);
    if (m_engine->m_isShaderDebugMode)
    {
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char *buf = new char[infoLen + 1];
                buf[infoLen] = '\0';

                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                // assert(true);
                std::string info = buf;
                LOGI("GLShaderInstance _createShader %s", info.c_str());
                delete[] buf;

            }
        }
    }
    return shader;
}
void GLShaderInstance::_addShaderUnifiormFun(ShaderVariable *one)
{
    bool isArray = one->isArray;
    switch (one->type)
    {
    case GL_BOOL:
        one->fun = std::bind(&GLShaderInstance::_uniform1i, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLint) * one->count;
        one->uploadedValue.resize(sizeof(GLint), 0);
        break;
    case GL_INT:
        one->fun = isArray
                       ? std::bind(&GLShaderInstance::_uniform1iv, this, std::placeholders::_1, std::placeholders::_2)
                       : std::bind(&GLShaderInstance::_uniform1i, this, std::placeholders::_1,
                                   std::placeholders::_2); 
        one->byteSize = sizeof(GLint) * one->count;
        one->uploadedValue.resize(sizeof(GLint), 0);
        break;
    case GL_FLOAT:
        one->fun = isArray
                       ? std::bind(&GLShaderInstance::_uniform1fv, this, std::placeholders::_1, std::placeholders::_2)
                       : std::bind(&GLShaderInstance::_uniform1f, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * one->count;
        one->uploadedValue.resize(sizeof(GLfloat));
        break;
    case GL_FLOAT_VEC2:
        one->fun =
            isArray ? std::bind(&GLShaderInstance::_uniform_vec2v, this, std::placeholders::_1, std::placeholders::_2)
                    : std::bind(&GLShaderInstance::_uniform_vec2, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 2 * one->count;
        one->uploadedValue.resize(sizeof(GLfloat) * 2, 0);
        break;
    case GL_FLOAT_VEC3:
        one->fun =
            isArray ? std::bind(&GLShaderInstance::_uniform_vec3v, this, std::placeholders::_1, std::placeholders::_2)
                    : std::bind(&GLShaderInstance::_uniform_vec3, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 3 * one->count;
        one->uploadedValue.resize(sizeof(GLfloat) * 3, 0);
        break;
    case GL_FLOAT_VEC4:
        one->fun =
            isArray ? std::bind(&GLShaderInstance::_uniform_vec4v, this, std::placeholders::_1, std::placeholders::_2)
                    : std::bind(&GLShaderInstance::_uniform_vec4, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 4 * one->count;
        one->uploadedValue.resize(sizeof(GLfloat) * 4, 0);
        break;
    case GL_FLOAT_MAT2:
        one->fun = std::bind(&GLShaderInstance::_uniformMatrix2fv, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 4 * one->count;
        break;
    case GL_FLOAT_MAT3:
        one->fun = isArray
            ? std::bind(&GLShaderInstance::_uniformMatrix3f, this, std::placeholders::_1, std::placeholders::_2)
            : std::bind(&GLShaderInstance::_uniformMatrix3fv, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 9 * one->count;
        break;
    case GL_FLOAT_MAT4:
        one->fun =
            isArray
                ? std::bind(&GLShaderInstance::_uniformMatrix4fv, this, std::placeholders::_1, std::placeholders::_2)
                : std::bind(&GLShaderInstance::_uniformMatrix4f, this, std::placeholders::_1, std::placeholders::_2);
        one->byteSize = sizeof(GLfloat) * 16 * one->count;
        // one->uploadedValue.resize(one->byteSize, 0);
        break;
    case GL_SAMPLER_2D:
    case GL_SAMPLER_2D_SHADOW:
        glUniform1i(one->location, m_curActTexIndex);
        one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
        one->fun = std::bind(&GLShaderInstance::_uniform_sampler2D, this, std::placeholders::_1, std::placeholders::_2);
        break;
    case GL_SAMPLER_2D_ARRAY:
        glUniform1i(one->location, m_curActTexIndex);
        one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
        one->fun =
            std::bind(&GLShaderInstance::_uniform_sampler2DArray, this, std::placeholders::_1, std::placeholders::_2);
        break;
    case 0x8b5f: // sampler3D
        glUniform1i(one->location, m_curActTexIndex);
        one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
        one->fun = std::bind(&GLShaderInstance::_uniform_sampler3D, this, std::placeholders::_1, std::placeholders::_2);
        break;
    case GL_SAMPLER_CUBE:
        glUniform1i(one->location, m_curActTexIndex);
        one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
        one->fun =
            std::bind(&GLShaderInstance::_uniform_samplerCube, this, std::placeholders::_1, std::placeholders::_2);
        break;
    case GL_UNIFORM_BUFFER:
        one->fun =
            std::bind(&GLShaderInstance::_uniform_UniformBuffer, this, std::placeholders::_1, std::placeholders::_2);
        break;
    default:
        assert("compile shader err!");
        break;
    }
}
bool GLShaderInstance::bind()
{
    return useProgram();
}
bool GLShaderInstance::useProgram()
{
    if (m_engine->m_glUseProgram == this)
        return false;
    glUseProgram(m_program);
    m_engine->m_glUseProgram = this;
   m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_SetRenderPassCount, 1);
    return true;
}
int GLShaderInstance::_uniform1f(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(float));
    float data = std::any_cast<float>(dataInfo);

    //if (memcmp(one->uploadedValue.data(), &data, sizeof(float)) != 0)
    {
        glUniform1f(one->location, data);
        //memcpy(one->uploadedValue.data(), &data, sizeof(float));
        return 1;
    }
    return 0;
}
int GLShaderInstance::_uniform1fv(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *value = (GLfloat *)info.m_data;
    int count = info.m_lengthInBytes / sizeof(GLfloat);
    /*if (count < 4)
    {
        GLfloat* uploadedValue = (GLfloat*)one->uploadedValue.data();
        if (uploadedValue[0] != value[0] || uploadedValue[1] != value[1] || uploadedValue[2] != value[2] ||
    uploadedValue[3] != value[3])
        {
            glUniform1fv(one->location, count, value);
            uploadedValue[0] = value[0];
            uploadedValue[1] = value[1];
            uploadedValue[2] = value[2];
            uploadedValue[3] = value[3];
            return 1;
        }
        return 0;
    }
    else*/
    {
        glUniform1fv(one->location, count, value);
        return 1;
    }
}
int GLShaderInstance::_uniform_vec2(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(Vector2));
    Vector2 data = std::any_cast<Vector2>(dataInfo);
    //if (memcmp(one->uploadedValue.data(), &data, sizeof(Vector2)) != 0)
    {
        glUniform2f(one->location, data.x, data.y);
        //memcpy(one->uploadedValue.data(), &data, sizeof(Vector2));
        return 1;
    }
    return 0;
}
int GLShaderInstance::_uniform_vec2v(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *value = (GLfloat *)info.m_data;
    int count = info.m_lengthInBytes / (sizeof(GLfloat) * 2);

    /*if (count < 2)
    {
        GLfloat* uploadedValue= (GLfloat*)one->uploadedValue.data();
        if (uploadedValue[0] != value[0] || uploadedValue[1] != value[1] || uploadedValue[2] != value[2] ||
    uploadedValue[3] != value[3])
        {
            glUniform2fv(one->location, count, value);
            uploadedValue[0] = value[0];
            uploadedValue[1] = value[1];
            uploadedValue[2] = value[2];
            uploadedValue[3] = value[3];
            return 1;
        }
        return 0;
    }
    else*/
    {
        glUniform2fv(one->location, count, value);
        return 1;
    }
}
int GLShaderInstance::_uniform_vec3(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(Vector3));
    Vector3 data = std::any_cast<Vector3>(dataInfo);
    //if (memcmp(one->uploadedValue.data(), &data, sizeof(Vector3)) != 0)
    {
        glUniform3f(one->location, data.x, data.y, data.z);
        //memcpy(one->uploadedValue.data(), &data, sizeof(Vector3));
        return 1;
    }

    return 0;
}

int GLShaderInstance::_uniform_vec3v(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *pData = (GLfloat *)info.m_data;
    glUniform3fv(one->location, info.m_lengthInBytes / (sizeof(GLfloat) * 3), pData);
    return 0;
}
int GLShaderInstance::_uniform_vec4(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(Vector4));
    const Vector4& data = std::any_cast<const Vector4&>(dataInfo);
    //if (memcmp(one->uploadedValue.data(), &data, sizeof(Vector4)) != 0)
    {
        glUniform4f(one->location, data.x, data.y, data.z, data.w);
        //memcpy(one->uploa%fdedValue.data(), &data, sizeof(Vector4));
        return 1;
    }
    return 0;
}
int GLShaderInstance::_uniform_vec4v(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *pData = (GLfloat *)info.m_data;
    glUniform4fv(one->location, info.m_lengthInBytes / (sizeof(GLfloat) * 4), pData);
    return 1;
}

int GLShaderInstance::_uniformMatrix2fv(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *pData = (GLfloat *)info.m_data;
    glUniformMatrix2fv(one->location, false, info.m_lengthInBytes / (4 * sizeof(float)), pData);
    return 1;
}

int GLShaderInstance::_uniformMatrix3fv(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *pData = (GLfloat *)info.m_data;
    glUniformMatrix3fv(one->location, false, info.m_lengthInBytes / (9 * sizeof(float)), pData);
    return 1;
}
int GLShaderInstance::_uniformMatrix3f(ShaderVariable* one, const std::any& dataInfo) 
{
    const laya::Matrix3x3& info = std::any_cast<const laya::Matrix3x3&>(dataInfo);
    glUniformMatrix3fv(one->location, 1, false, info.elements);
    return 1;
}

int GLShaderInstance::_uniformMatrix4f(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(ShaderData::BufferDataInfo));
    const laya::Matrix4x4& info = std::any_cast<const laya::Matrix4x4&>(dataInfo);
    glUniformMatrix4fv(one->location, 1, false, info.elements);
    return 1;
}
int GLShaderInstance::_uniformMatrix4fv(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(ShaderData::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    GLfloat *pData = (GLfloat *)(GLfloat *)info.m_data;
    glUniformMatrix4fv(one->location, info.m_lengthInBytes / (16 * sizeof(float)), false, pData);
    return 1;

}

int GLShaderInstance::_uniform1i(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(int32_t));
    int32_t data = std::any_cast<int32_t>(dataInfo);
    //if (memcmp(one->uploadedValue.data(), &data, sizeof(int32_t)) != 0)
    {
        glUniform1i(one->location, data);
        //memcpy(one->uploadedValue.data(), &data, sizeof(GLint));
        return 1;
    }
    return 0;
}

int GLShaderInstance::_uniform1iv(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(laya::BufferDataInfo));
    laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
    // 当前引擎未使用 需要用int32Array
    GLint *pData = (GLint *)info.m_data;
    glUniform1iv(one->location, info.m_lengthInBytes / sizeof(GLint), pData);
    return 1;
}
/*int _uniform_ivec2(one: any, value : any)
{
    var uploadedValue : any[] = one.uploadedValue;
    if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1])
    {
        this._gl.uniform2i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1]);
        return 1;
    }
    return 0;
}
int _uniform_ivec2v(one: any, value : any)
{
    this._gl.uniform2iv(one.location, value);
    return 1;
}

int _uniform_vec3i(one: any, value : any)
{
    var uploadedValue : any[] = one.uploadedValue;
    if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1] || uploadedValue[2] != = value[2])
    {
        this._gl.uniform3i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1], uploadedValue[2] =
value[2]); return 1;
    }
    return 0;
}

int _uniform_vec3vi(one: any, value : any)
{
    this._gl.uniform3iv(one.location, value);
    return 1;
}

int _uniform_vec4i(one: any, value : any)
{
    var uploadedValue : any[] = one.uploadedValue;
    if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1] || uploadedValue[2] != = value[2] ||
uploadedValue[3] != = value[3])
    {
        this._gl.uniform4i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1], uploadedValue[2] =
value[2], uploadedValue[3] = value[3]); return 1;
    }
    return 0;
}
int _uniform_vec4vi(one: any, value : any)
{
    this._gl.uniform4iv(one.location, value);
    return 1;
}*/

int GLShaderInstance::_uniform_sampler2D(ShaderVariable *one, const std::any &dataInfo)
{
    GLESInternalTex* texture = std::any_cast<GLESInternalTex*>(dataInfo);
    if (texture != nullptr)
    {
        _bindTexture(one->textureID, GL_TEXTURE_2D, texture);
    }
    else
    {
        LOGI("_uniform_sampler2D find no texture");
    }
    return 0;
}
int GLShaderInstance::_uniform_sampler2DArray(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(uint32_t));
    GLESInternalTex* texture = std::any_cast<GLESInternalTex*>(dataInfo);
    if (texture != nullptr)
    {
        _bindTexture(one->textureID, GL_TEXTURE_2D_ARRAY, texture);
    }
    else
    {
        LOGI("_uniform_sampler2DArray find no texture");
    }
    return 0;
}
int GLShaderInstance::_uniform_sampler3D(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(uint32_t));
    GLESInternalTex* texture = std::any_cast<GLESInternalTex*>(dataInfo);
    if (texture != nullptr)
    {
        _bindTexture(one->textureID, GL_TEXTURE_3D, texture);
    }
    return 0;
}

int GLShaderInstance::_uniform_samplerCube(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(uint32_t));
    GLESInternalTex* texture = std::any_cast<GLESInternalTex*>(dataInfo);
    if (texture != nullptr)
    {
        _bindTexture(one->textureID, GL_TEXTURE_CUBE_MAP, texture);
    }
    return 0;
}
int GLShaderInstance::_uniform_UniformBuffer(ShaderVariable *one, const std::any &dataInfo)
{
    //linux compile error assert(dataInfo.type == std::typeid(uint32_t));
    uniformDataShell* buffer = std::any_cast<uniformDataShell*>(dataInfo);
    buffer->ubo->bind(one->location);

  ////GLESUniformBuffer
  //  buffer->bind(one->location);
    return 0;
}
void GLShaderInstance::_bindTexture(int textureID, GLenum target, GLESInternalTex *texture)
{
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
    {
        glActiveTexture(textureID);
        glBindTexture(target, texture == nullptr ? 0 : texture->getResource());
    }
    else
    {
        if (m_engine->m_activedTextureID != textureID)
        {
            glActiveTexture(textureID);
            m_engine->m_activedTextureID = textureID;
        }
        const int texID = m_engine->m_activedTextureID - GL_TEXTURE0;
        if (m_engine->m_activeTextures[texID] != texture)
        {
            glBindTexture(target, texture == nullptr ? 0 : texture->getResource());
            m_engine->m_activeTextures[texID] = texture;
        }
    }
}

void GLShaderInstance::destroy()
{
    if (m_program != 0)
    {
        glDeleteShader(m_vshader);
        glDeleteShader(m_pshader);
        glDeleteProgram(m_program);
        m_vshader = 0;
        m_pshader = 0;
        m_program = 0;
        for (int i = 0, size = m_uniformMap.size(); i < size; i++)
        {
            ShaderVariable *one = m_uniformMap[i];
            delete one;
        }
        m_uniformMap.clear();
        // m_attributeMap = null;
        // m_uniformMap = null;
        // m_uniformObjectMap = null;
        // m_engine = null;
    }
}
} // namespace laya
