import { TextInputInfoNew } from '@ohos/libSysCapabilities/src/main/ets/components/EditBoxNew'
import { GlobalContext, GlobalContextConstants, Logger } from '@ohos/libSysCapabilities'
import laya from "liblaya.so";
import display from '@ohos.display';
import { componentUtils, window } from '@kit.ArkUI';
import { BusinessError } from '@kit.BasicServicesKit';

let log: Logger = new Logger(0x0001, "LayaEditBoxComponent");
const heightFactor: number = 0.2
@Component
export struct LayaEditBoxNew {
  @ObjectLink textInputInfo: TextInputInfoNew;
  removeSelf() {
    let removeIndex = -1;
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_ARRAY)
      .forEach((item: TextInputInfoNew, index: number) => {
        if (item.viewTag == this.textInputInfo.viewTag) {
          removeIndex = index;
        }
    });
  GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_ARRAY).splice(removeIndex, 1);
  GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_INDEX_MAP).delete(this.textInputInfo.viewTag);
}
  onComplete() {
    this.getUIContext().getFocusController().clearFocus();
    this.textInputInfo.visible = false;
    this.removeSelf();
    laya.ConchNAPI_handleKeyboardComplete(this.textInputInfo.defaultValue);
    laya.ConchNAPI_handleKeyboardConfirm(this.textInputInfo.defaultValue);
  }
  onSubmit() {
    this.getUIContext().getFocusController().clearFocus();
    this.textInputInfo.visible = false;
    this.removeSelf();
    laya.ConchNAPI_handleKeyboardComplete(this.textInputInfo.defaultValue);
    laya.ConchNAPI_handleKeyboardConfirm(this.textInputInfo.defaultValue);
  }
  aboutToAppear() {
    window.getLastWindow(getContext(this))
      .then(currentWindow => {
        currentWindow.on('avoidAreaChange',
          async data => {
            if (data.type !== window.AvoidAreaType.TYPE_KEYBOARD) {
              return;
            }
            if (data.area.bottomRect.height > 0) {
              let keyboardHeight = data.area.bottomRect.height;
              console.info("keyboardHeight " + keyboardHeight)
              this.textInputInfo.visible = true;
              this.textInputInfo.x = 0;
              this.textInputInfo.y = (display.getDefaultDisplaySync().height - keyboardHeight - heightFactor * keyboardHeight);
              this.textInputInfo.w = (display.getDefaultDisplaySync().width);
              this.textInputInfo.h = (heightFactor * keyboardHeight);
            }
            else {
                this.onComplete();
            }
          }
        )
      })
  }
  build() {
        Stack()  {
          Row({ space: 10 }) {
            if (this.textInputInfo.multiple) {
              TextArea({ placeholder: this.textInputInfo.prompt, text: this.textInputInfo.defaultValue, controller: this.textInputInfo.textAreaController })
                .id('TextAreaNew' + this.textInputInfo.viewTag)
                .height('80%')
                .width('90%')
                .padding({ top: 0, left: 0, bottom: 0, right: 0 })
                .borderRadius(8)
                .maxLength(this.textInputInfo.maxLength)
                .type(this.textInputInfo.inputTypeTextArea)
                .enterKeyType(this.textInputInfo.confirmType)
                .align(Alignment.TopStart)
                .textAlign(TextAlign.Start)
                .fontSize(px2fp(60))
                .fontColor(Color.Black)
                .backgroundColor(Color.White)
                .visibility(Visibility.Visible)
                .focusable(true)
                .onVisibleAreaChange([0.0, 1.0], (isVisible: boolean, currentRadio: number) => {
                  if (isVisible) {
                    this.textInputInfo.textAreaController.caretPosition(this.textInputInfo.defaultValue.length);
                    focusControl.requestFocus('TextArea' + this.textInputInfo.viewTag);
                  }

                })
                .onChange((value: string) => {
                  laya.ConchNAPI_handleKeyboardInput(value);
                  this.textInputInfo.defaultValue = value;
                })
                .onFocus(() => {
                })
                .onBlur(() => {
                })
                .onSubmit(()=>{
                  this.onSubmit();
                })
            }
            else {
              TextInput({ placeholder: this.textInputInfo.prompt, text: this.textInputInfo.defaultValue, controller: this.textInputInfo.textInputController })
                .id('TextInputNew' + this.textInputInfo.viewTag)
                .height('80%')
                .width('90%')
                .padding({ top: 0, left: 0, bottom: 0, right: 0 })
                .borderRadius(8)
                .maxLength(this.textInputInfo.maxLength)
                .type(this.textInputInfo.inputTypeTextInput)
                .enterKeyType(this.textInputInfo.confirmType)
                .align(Alignment.Center)
                .textAlign(TextAlign.Start)
                .fontSize(px2fp(60))
                .fontColor(Color.Black)
                .backgroundColor(Color.White)
                .visibility(Visibility.Visible)
                .defaultFocus(true)
                .focusable(true)
                .showPasswordIcon(false)
                .onVisibleAreaChange([0.0, 1.0], (isVisible: boolean, currentRadio: number) => {
                  if (isVisible) {
                    this.textInputInfo.textInputController.caretPosition(this.textInputInfo.defaultValue.length);
                    focusControl.requestFocus('TextInputNew' + this.textInputInfo.viewTag);
                  }
                })
                .onChange((value: string) => {
                  laya.ConchNAPI_handleKeyboardInput(value);
                  this.textInputInfo.defaultValue = value;
                })
                .onSubmit(()=>{
                  this.onSubmit();
                })
            }
          }
          .id("btnID")
          .position({ x: px2vp(this.textInputInfo.x), y: px2vp(this.textInputInfo.y) })
          .size({ width: px2vp(this.textInputInfo.w), height: px2vp(this.textInputInfo.h) })
          .backgroundColor(0xF7F7F7)
          .alignItems(VerticalAlign.Center)
          .justifyContent(FlexAlign.Center)
          .visibility(this.textInputInfo.visible ? Visibility.Visible : Visibility.None)
          .focusable(true)
        }
        .height('100%')
        .width('100%')
        .visibility(this.textInputInfo.visible ? Visibility.Visible : Visibility.None)
        .backgroundColor(Color.Transparent)
        .focusable(true)
        .onClick(() => {
          this.onComplete();
        })
  }
}