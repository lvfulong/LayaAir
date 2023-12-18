#ifndef __Base64_h__
#define __Base64_h__

#include <string>
#include <vector>

namespace laya {
typedef bool (*CharacterMatchFunctionPtr)(uint16_t);
template <typename CharType> inline bool isHTMLSpace(CharType character) {
  return character <= ' ' &&
         (character == ' ' || character == '\n' || character == '\t' ||
          character == '\r' || character == '\f');
}
enum Base64EncodePolicy { Base64DoNotInsertLFs, Base64InsertLFs };

enum Base64DecodePolicy { Base64DoNotValidatePadding, Base64ValidatePadding };

void base64Encode(const char *, unsigned, std::vector<char> &,
                  Base64EncodePolicy = Base64DoNotInsertLFs);

std::string base64Encode(const char *, unsigned,
                         Base64EncodePolicy = Base64DoNotInsertLFs);

bool base64Decode(const std::string &in, std::vector<char> &,
                  CharacterMatchFunctionPtr shouldIgnoreCharacter = 0,
                  Base64DecodePolicy = Base64DoNotValidatePadding);

} // namespace laya
#endif // __Base64_h__
