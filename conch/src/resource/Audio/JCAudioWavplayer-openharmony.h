#ifndef __JCAudioWavPlayer_OHOS_H__
#define __JCAudioWavPlayer_OHOS_H__

#include <stdio.h>
#include <thread>
#include <utils/JCCommonMethod.h>
#include "JCWaveInfo.h"
#include "JCAudioInterface.h"
#include <unordered_map>
#include <vector>
#include <ohaudio/native_audiostreambuilder.h>
#include <ohaudio/native_audiorenderer.h>

namespace laya{
    class JCFileResManager;
    class AudioRenderInfo
    {
        public:
            OH_AudioRenderer* _audioRender;
            OH_AudioStreamBuilder* _builder;
            JCAudioInterface* m_pAudio; //Audio 为了回调使用
            bool m_bPlaying; //是否为播放状态
            const char* pcmBuffer;
            int32_t m_pBufferSize;
            int32_t writeOffset = 0;
            std::function<int32_t(OH_AudioRenderer* renderer, void* userData, void* buffer, int32_t bufferLen)> writeDataCallback;
        public:
            void operator=(const AudioRenderInfo &other){
                m_bPlaying = other.m_bPlaying;
                _audioRender = other._audioRender;
                m_pAudio = other.m_pAudio;
            }
    };
    class JCAudioWavPlayer{
        public:
            typedef std::unordered_map<std::string, JCWaveInfo*> MapWaveInfo;
            typedef MapWaveInfo::iterator MapWaveInfoIter;
        public:
            JCAudioWavPlayer(JCFileResManager* pFileResManager);
            ~JCAudioWavPlayer();
            void Release();
        public:
            JCWaveInfo* AddWaveInfoMp3(const std::string& p_sUrl, const std::string& p_sFilePath, void* p_pExternalMark);
            AudioRenderInfo* playAudioMp3(JCAudioInterface* p_pAudio, const std::string& p_sSrc, const char* p_sFilePath, float currentTime);
            AudioRenderInfo* playAudio(JCAudioInterface* p_pAudio, const std::string& p_sSrc, bool bIsOgg, float currentTime);
            void delAudio(JCAudioInterface* p_pAudio);
            AudioRenderInfo* playAudioFromBuffer(JCAudioInterface* p_pAudio, const char* p_pBuffer, unsigned int p_nBufferSize, int p_nRate, int p_nBitsPerSample, int nChannels);
            void setVolume(AudioRenderInfo* pAudioRenderInfo, float p_nVolume);
            void stop(AudioRenderInfo* pAudioRenderInfo);
            void setAllVolume(float p_nVolume);
            void stopAll();
            void checkWavePlayEnd();
            void pause();
            void resume();
            static int32_t AudioRendererOnWriteData(OH_AudioRenderer *renderer, void *userData, void *buffer, int32_t bufferLen);
            float getCurrentTime(AudioRenderInfo* pOpenALInfo);
        public:
            /**
             * @brief 添加资源
             * @return
            */
            JCWaveInfo *AddWaveInfo(const std::string &p_sUrl, unsigned char *p_pBuffer, int p_nSize, const char *p_sFilePath, void *p_pExternalMark, bool p_bIsOgg);
            /**
             * @brief 查找资源根据ulid
             * @param[in] ulid
             * @return
            */
           JCWaveInfo* FindWaveInfo(const std::string& p_sUrl);
            /**
             * @brief 清空所有
             */
            bool ClearAllWaveInfo(void);
            void autoGarbageCollection();
        public:
            static int s_nGarbageCollectionTime;
            MapWaveInfo m_vWaveInfos; //wave的信息
        public:
            JCFileResManager* m_pFileResManager;
            OH_AudioRenderer *audioRenderer;
            std::vector<AudioRenderInfo*> m_pAudioRenderSource; //播放声音用的
    };
}
#endif //__JCAudioWavPlayer_H__