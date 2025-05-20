import { GlobalContext, GlobalContextConstants } from '../common/GlobalContext';
import { Logger } from '../utils/Logger'
import laya from "liblaya.so";
let log: Logger = new Logger(0x0001, "LayaEditBox");

@Observed
export class TextInputInfoNew {
  public viewTag: number = 0;
  public x: number = 100;
  public y: number = 100;
  public w: number = 2;
  public h: number = 2;
  public defaultValue: string = '';
  public maxLength: number = 9999;
  public multiple: boolean = false;
  public confirmHold: boolean = false;
  public confirmType: EnterKeyType = EnterKeyType.Done;
  public prompt: string = '';
  public promptColor: string = '';
  public inputTypeTextInput = InputType.Normal;
  public inputTypeTextArea = TextAreaType.NORMAL;
  public visible: boolean = true;
  public textAreaController: TextAreaController = new TextAreaController();
  public textInputController: TextInputController = new TextInputController();

  constructor(viewTag: number) {
    this.viewTag = viewTag;
  }
}

export class EditBoxNew {
  static MODULE_NAME : string = 'EditBoxNew';

  static show(viewTag: number, defaultValue: string, maxLength: number, multiple: boolean, confirmHold: boolean, confirmType: string, prompt: string, promptColor: string, inputType: string) : boolean {
    log.debug("call EditBoxNew.show, viewTag: %{public}d", viewTag)
    let newTextInputInfo = new TextInputInfoNew(viewTag);



    newTextInputInfo.defaultValue = defaultValue;
    newTextInputInfo.maxLength = maxLength;
    newTextInputInfo.multiple = multiple;
    newTextInputInfo.confirmHold = confirmHold;
    newTextInputInfo.prompt = prompt;
    newTextInputInfo.promptColor = promptColor;
    newTextInputInfo.visible = true;

    switch (confirmType) {
      case 'done':
        newTextInputInfo.confirmType = EnterKeyType.Done;
        break;
      case 'next':
        newTextInputInfo.confirmType = EnterKeyType.Next;
        break;
      case 'search':
        newTextInputInfo.confirmType = EnterKeyType.Search;
        break;
      case "go":
        newTextInputInfo.confirmType = EnterKeyType.Go;
        break;
      case "send":
        newTextInputInfo.confirmType = EnterKeyType.Send;
        break;
      default:
        newTextInputInfo.confirmType = EnterKeyType.Done;
    }

    switch (inputType) {
      case 'email':
        newTextInputInfo.inputTypeTextInput = InputType.Email;
        newTextInputInfo.inputTypeTextArea = TextAreaType.EMAIL;
        break;
      case 'password':
        newTextInputInfo.inputTypeTextInput = InputType.Password;
        newTextInputInfo.inputTypeTextArea = TextAreaType.NORMAL;
        break;
      case 'number':
        newTextInputInfo.inputTypeTextInput = InputType.Number;
        newTextInputInfo.inputTypeTextArea = TextAreaType.NUMBER;
        break;
      case "url":
        newTextInputInfo.inputTypeTextInput = InputType.URL;
        newTextInputInfo.inputTypeTextArea = TextAreaType.URL;
        break;
      case "phone":
        newTextInputInfo.inputTypeTextInput = InputType.PhoneNumber;
        newTextInputInfo.inputTypeTextArea = TextAreaType.PHONE_NUMBER;
        break;
      default:
        newTextInputInfo.inputTypeTextInput = InputType.Normal;
        newTextInputInfo.inputTypeTextArea = TextAreaType.NORMAL;
    }
    let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_ARRAY) as TextInputInfoNew[];
    arr.push(newTextInputInfo);
    let map = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_INDEX_MAP) as Map<number, TextInputInfoNew>;
    map.set(viewTag, newTextInputInfo);
    return true;
  }

  static hide(viewTag: number) : boolean {
    log.debug("call EditBoxNew.hide, viewTag: %{public}d", viewTag)

    let map = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_INDEX_MAP) as Map<number, TextInputInfoNew>;
    let textInputInfo = map.get(viewTag);
    if (textInputInfo) {
      textInputInfo.visible = false;
      laya.ConchNAPI_handleKeyboardComplete(textInputInfo.defaultValue);
      laya.ConchNAPI_handleKeyboardConfirm(textInputInfo.defaultValue);
    }
    let removeIndex = -1;
    let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_NEW_ARRAY) as TextInputInfoNew[];
    arr.forEach((item: TextInputInfoNew, index: number) => {
        if (item.viewTag == viewTag) {
          removeIndex = index;
        }
      });
    arr.splice(removeIndex, 1);
    map.delete(viewTag);
    return true;
  }
}