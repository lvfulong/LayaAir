#ifndef __BlendMode_H_
#define __BlendMode_H_

namespace laya
{
enum BlendMode
{
    Invalid = 0,
    Normal,
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