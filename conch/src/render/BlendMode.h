#ifndef __BlendMode_H_
#define __BlendMode_H_

namespace laya
{
enum BlendMode
{
    Invalid = -1,
    Normal = 0,
    Add,
    Multiply,
    Screen,
    Overlay,
    Light,
    Lighter,
    Mask,
    DestinationOut,
    AddOld,
    LighterOld,
    SourceAlpha,
};
} // namespace laya
#endif