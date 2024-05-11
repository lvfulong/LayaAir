#ifndef __JSTEXTDECODER_H__
#define __JSTEXTDECODER_H__

#include <string>
#include <v8.h>

namespace laya{
class TextDecoderWrapper {
private:
    std::string encode="utf8";
public:
    TextDecoderWrapper() = default;
    TextDecoderWrapper(const char* encode);
    // 假设decode方法只是简单地返回传入的字节数组（Uint8Array）作为字符串
    //std::string decode(JSValueAsParam args);
    std::string decode(JSValueAsParam args);
    static void exportJS(Context& context);

};
}

#endif