import { GlobalContext, GlobalContextConstants } from "../common/GlobalContext";
import laya from "liblaya.so";

export default class HandleMessageUtils {
    private static context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);
    static RunJS(data: string): void {
        laya.ConchNAPI_RunJS(data);
    }
    /**
    * 同步事件
    * @param eventName 事件名称
    * @param data 数据
    */
    static handleSyncMessage(eventName: string, data: string): string {
        return "sync result";
    }

    /**
    * 异步事件
    * @param eventName 事件名称
    * @param data 数据
    * @param cb callback
    */
    static async handleAsyncMessage(eventName: string, data: string, cb: Function): Promise<void> {
        cb("async result");
    }
}