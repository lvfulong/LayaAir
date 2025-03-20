import resmgr from '@ohos.resourceManager';
import resourceManager from "@ohos.resourceManager";

export interface CPPFunctions {
  onCreate: () => void;
  onShow: () => void;
  onHide: () => void;
  onBackPress: () => void;
  onDestroy: () => void;
  onPageShow: () => void;
  onPageHide: () => void;
  nativeResourceManagerInit: (resourceManager: resmgr.ResourceManager) => void;
  writablePathInit: (writePath: string) => void;
  workerInit: () => void;
  nativeEngineStart: () => void;
  onVideoCallBack: (viewTag: number, event: number) => void;
  onDurationBack: (viewTag: number, duration: number) => void;
  onCurrentTimeBack: (viewTag: number, currentTime: number) => void;
  loadedmetadata: (viewTag: number) => void;
  HttpClientOHOS_onFailure: (ptr: number, code: number) => void;
  HttpClientOHOS_onResponse: (ptr: number, ab: ArrayBuffer, responseCode: number, header: string) => void;
  HttpClientOHOS_onProgress: (ptr: number, total: number, now: number, speed: number) => void;
}

export const getContext: (a: number) => CPPFunctions;

export const ConchNAPI_InitDLib: (rsmgr: resourceManager.ResourceManager, p_strCachePath: string) => void;
export const ConchNAPI_configSetURL: (p_strUrl: string) => void;
export const ConchNAPI_inputChange:(keycode: number) => void;
export const ConchNAPI_RunJS: (js: string) => void;
export const ConchNAPI_handleKeyboardInput:(value: string) => void;
export const ConchNAPI_handleKeyboardConfirm:(value: string) => void;
export const ConchNAPI_handleKeyboardComplete:(value: string) => void;