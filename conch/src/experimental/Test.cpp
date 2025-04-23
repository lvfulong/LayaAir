#include "Test.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <string>
#include "../JCCmdStruct.h"

namespace laya
{
    static int s_currentID = 0;
    static std::unordered_map<int, TestCommandBuffer*> s_map;    
    JCCommandEncoderBuffer Test::m_renderCmd(128, 128);
    char* Test::m_pBuffer = nullptr;
    Test::Test()
    {
    }

    Test::~Test()
    {
    }   

    int Test::createObject(const std::string& className)
    {
        if (className == "TestCommandBuffer")
        {
            TestCommandBuffer* pTestCommandBuffer = new TestCommandBuffer();
            int id = s_currentID;
            s_map[id] = pTestCommandBuffer;
            s_currentID++;
            return id;
        }
        return -1;
    }

    void Test::destroyObject(const std::string& className, int ptr)
    {
        if (className == "TestCommandBuffer")
        {
            delete s_map[ptr];
            s_map.erase(ptr);
        }       
    }
    void Test::setBuffer(jsbind::ArrayBuffer value)
    {
        Test::m_pBuffer = (char*)value.getData();
    }
    void Test::flush()
    {
        char* pBuffer = Test::m_pBuffer;
        int nLen = (*(int*)pBuffer - 1) * 4;
        m_renderCmd.setShareBuffer(pBuffer + 4, nLen);
        
        Test::dispatchAllCmds(&m_renderCmd);
        
        ((int*)pBuffer)[0] = 1;
        m_renderCmd.clearData();
    }   
    void Test::dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem)
    {
            //循环模板指令流
		pTemplateMem->setReadPos(0);
		int nLen = pTemplateMem->getDataSize();
		while (pTemplateMem->getReadPos() < (size_t)nLen)
		{
			dispatchScriptCmd(*pTemplateMem);
		}
    }
    typedef void(*ProcFunction)(JCCommandEncoderBuffer& layaGLCmd);
    bool Test::dispatchScriptCmd(JCCommandEncoderBuffer& pRenderCmd)
    {
		static int nLastPos = 0;
		static int nFuncID = 0;
        static const ProcFunction g_svProcFunctions[] =
        {
            &Test::_test_testInt32,		
            &Test::_test_testFloat,
            &Test::_test_testString,
            &Test::_test_testBoolean,
            &Test::_test_testArrayBuffer,
            &Test::_test_testArrayBufferView,       
            &Test::_test_testVector4,
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pCmdBuffer = pRenderCmd.getReadPtr();
        if (pCmdBuffer)
        {
#ifdef DEBUG_PARSE_STREAM
			int nCurrentPos = pRenderCmd.getReadPos();
			if (nCurrentPos == 0) {
				nLastPos = 0;
			}
			nLastPos = pRenderCmd.getReadPos();
#endif 
            nFuncID = *(pRenderCmd.popp<int>());
            if (nFuncID >= 0 && nFuncID < nFuncs)
            {
                (g_svProcFunctions[nFuncID])(pRenderCmd);
            }
            else
            {
                LOGE("JCLayaGLDispatch::dispatchScriptCmd Wrong command number!,cmd=%d", nFuncID);
            }
        }
        return true;
    }
    void Test::_test_testInt32(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        pTestCommandBuffer->testInt32(cmd->j);
    }
    void Test::_test_testFloat(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_if* cmd = layaGLCmd.popp<CMD_if>();

        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        pTestCommandBuffer->testFloat(cmd->j);
    }   
    void Test::_test_testString(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        if (cmd->j > 0)
        {
            char* value = layaGLCmd.readBufferAlign(cmd->j);
            pTestCommandBuffer->testString(value);
        }

    }
    void Test::_test_testBoolean(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        pTestCommandBuffer->testBoolean((bool)cmd->j);
    }
    void Test::_test_testArrayBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        if (cmd->j > 0)
        {
            char* value = layaGLCmd.readBufferAlign(cmd->j);
            pTestCommandBuffer->testArrayBuffer(value, cmd->j);
        }
        
    }
    void Test::_test_testArrayBufferView(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        if (cmd->j > 0)
        {
            char* value = layaGLCmd.readBufferAlign(cmd->j);
            pTestCommandBuffer->testArrayBufferView(value, cmd->j);
        }

    }   
    void Test::_test_testVector4(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iffff* cmd = layaGLCmd.popp<CMD_iffff>();
        TestCommandBuffer* pTestCommandBuffer = s_map[cmd->i];
        pTestCommandBuffer->testVector4(Vector4(cmd->x, cmd->y, cmd->w, cmd->h));
    }   

    TestNormal::TestNormal()
    {
    }

    TestNormal::~TestNormal()
    {
    }       

    void TestNormal::testInt32(int32_t value)
    {       
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testInt32"] = str;  
        //LOGI("11 testInt32 %d", value);
    }

    void TestNormal::testFloat(float value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testFloat"] = str;
    }

    void TestNormal::testString(const std::string& value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testString"] = str;
    }

    void TestNormal::testBoolean(bool value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str(); 
        m_map["testBoolean"] = str;
    }   

    void TestNormal::testArrayBuffer(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());


    }

    void TestNormal::testArrayBufferView(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());
    }
    void TestNormal::testVector4(const Vector4& v4)
    {
        m_v4.x = 1.0f;
        m_v4.y = 2.0f;
        m_v4.z = 3.0f;
        m_v4.w = 4.0f;

        //LOGI("testVector4 %f, %f, %f, %f", m_v4.x, m_v4.y, m_v4.z, m_v4.w);
    }
    void TestNormal::destroy()      
    {

        m_map.clear();
    }


    TestCommandBuffer::TestCommandBuffer()
    {
    }

    TestCommandBuffer::~TestCommandBuffer()
    {
    }       

    void TestCommandBuffer::testInt32(int32_t value)
    {       
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testInt32"] = str; 
        //LOGI("testInt32 %d", value);
    }

    void TestCommandBuffer::testFloat(float value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testFloat"] = str;
        //LOGI("testFloat %f", value);
    }

    void TestCommandBuffer::testString(const std::string& value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testString"] = str;
        //LOGI("testString %s", value.c_str());
    }

    void TestCommandBuffer::testBoolean(bool value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str(); 
        m_map["testBoolean"] = str;
        //LOGI("testBoolean %s", str.c_str());
    }   

    void TestCommandBuffer::testArrayBuffer(char* value, int bytes)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(bytes);
        memcpy(buffer.data(), value, bytes);
        //for (int i = 0; i < bytes; i++) 
        //{
        //    LOGI("testArrayBuffer %d", buffer[i]);
        //}   
    }

    void TestCommandBuffer::testArrayBufferView(char* value, int bytes)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(bytes);
        memcpy(buffer.data(), value, bytes);
        //for (int i = 0; i < bytes; i++) 
        //{
        //    LOGI("testArrayBufferView %d", buffer[i]);
        //}
    }
    void TestCommandBuffer::testVector4(const Vector4& v4)
    {
        m_v4.x = 1.0f;
        m_v4.y = 2.0f;
        m_v4.z = 3.0f;
        m_v4.w = 4.0f;
        //LOGI("testVector4 %f, %f, %f, %f", m_v4.x, m_v4.y, m_v4.z, m_v4.w);
    }
    void TestCommandBuffer::destroy()      
    {

        m_map.clear();
    }
} // namespace laya
