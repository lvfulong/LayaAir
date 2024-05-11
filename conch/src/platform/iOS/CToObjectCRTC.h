//
//  CToObjectCRTC.h
//  MomoYo
//
//  Created by 范祎楠 on 2022/11/3.
//

#ifndef CToObjectCRTC_h
#define CToObjectCRTC_h
#include <vector>
#include <unordered_map>

void CToObjectCRTCEngineInit(unsigned int appID);
void CToObjectCRTCEngineLoginRoom(const char* roomID, const char* token, const char* userID, const char* userName, bool userUpdate, unsigned int maxMemberCount, std::function<void(bool)> callback);
void CToObjectCRTCEngineStartPublishingStream(const char* streamID, bool videoEnable, bool audioEnable);
void CToObjectCRTCEngineStartPlayingStream(const char* streamID, unsigned int resourceMode);
void CToObjectCRTCEngineRenewToken(const char* token, const char* roomID);
void CToObjectCRTCEngineStartSoundLevelMonitor(const char* config);
void CToObjectCRTCEngineMutePublishStreamVideo(bool isMute);
void CToObjectCRTCEngineMutePublishStreamAudio(bool isMute);
void CToObjectCRTCEngineStopPlayingStream(const char* streamID);
void CToObjectCRTCEngineStopPublishingStream();
void CToObjectCRTCEngineLogoutRoom(const char* streamID);
void CToObjectCRTCEngineDestrory();
void CToObjectCRTCEngineOnRoomStateChanged(std::function<void(const char *roomID, unsigned int reason, int errorCode, const char *extendData)> callback);
void CToObjectCRTCEngineOnRoomUserUpdate(std::function<void(const char *roomID, unsigned int type, std::vector<std::unordered_map<std::string, std::string>> list)> callback);
void CToObjectCRTCEngineOnRoomStreamUpdate(std::function<void(const char *roomID, const char* updateType, std::vector<std::unordered_map<std::string,std::string>> streamList, const char *extendData)> callback);
void CToObjectCRTCEngineOnTokenWillExpire(std::function<void(const char *roomID)> callback);
#endif /* CToObjectCRTC_h */
