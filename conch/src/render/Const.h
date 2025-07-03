#ifndef __Const_H_
#define __Const_H_
namespace laya
{
enum class Const
{
    ENUM_TEXTALIGN_DEFAULT = 0,
    ENUM_TEXTALIGN_CENTER = 1,
    ENUM_TEXTALIGN_RIGHT = 2,

    INDEX_BYTES = 2,

    MAX_CLIP_SIZE = 99999999,
};

enum class BaseRender2DType {
    empty = -1,
    baseRenderNode = 0,
    spine = 1,
    particle = 2,
    spineSimple = 3,
    graphics = 4
};

} // namespace laya
#endif