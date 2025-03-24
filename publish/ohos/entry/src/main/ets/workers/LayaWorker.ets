import worker, { MessageEvents, ThreadWorkerGlobalScope } from '@ohos.worker';
import laya from "liblaya.so";
import { ContextType } from "@ohos/libSysCapabilities"
import { Dialog } from "@ohos/libSysCapabilities"
import { WebView } from "@ohos/libSysCapabilities"
import { JumpManager } from "@ohos/libSysCapabilities"
import { NapiHelper } from "@ohos/libSysCapabilities/src/main/ets/napi/NapiHelper"
import { GlobalContext, GlobalContextConstants } from "@ohos/libSysCapabilities"
import { WorkerDataEntity } from './WorkerDataEntity';

const appLifecycle: laya.CPPFunctions = laya.getContext(ContextType.APP_LIFECYCLE);
const workerContext: laya.CPPFunctions = laya.getContext(ContextType.WORKER_INIT);
const videoPlayNapi: laya.CPPFunctions = laya.getContext(ContextType.VIDEOPLAYER_NAPI);
const napiContext: laya.CPPFunctions = laya.getContext(ContextType.NATIVE_API);
workerContext.workerInit();

const workerPort: ThreadWorkerGlobalScope = worker.workerPort;

Dialog.init(workerPort);
JumpManager.init(workerPort);
WebView.init(workerPort);

napiContext.nativeEngineStart();
NapiHelper.registerWorkerFunctions()



workerPort.onmessage = (e: MessageEvents): void => {
  let data: WorkerDataEntity = e.data;
  switch (data.type) {
    case "onXCLoad":
      console.log("[laya] onXCLoad Callback");
      break;
    case "abilityContextInit":
      GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT, data.context);
      break;
    case "onVideoCallBack":
      videoPlayNapi.onVideoCallBack(data.viewTag, data.event);
      break;
    case "loadedmetadata":
      videoPlayNapi.loadedmetadata(data.viewTag);
      break;
    case "onDurationBack":
      videoPlayNapi.onDurationBack(data.viewTag, data.duration);
      break;
    case "onCurrentTimeBack":
      videoPlayNapi.onCurrentTimeBack(data.viewTag, data.currentTime);
      break;
    case "exit":
      appLifecycle.onBackPress();
      break;
    default:
      console.error("laya worker: message type unknown");
  }
}


