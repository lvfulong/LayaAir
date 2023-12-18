#ifndef __INPUT_TYPES_H__
#define __INPUT_TYPES_H__
namespace laya
{
struct inputEvent
{
    char type[256];
    int nType;
    int key;
    int posX;
    int posY;
    int keyChar;
    int keyCode;
    int nWheel;
    float fTHUMBL_xOffset;
    float fTHUMBL_yOffset;
    float fTHUMBR_xOffset;
    float fTHUMBR_yOffset;
    float fLT_Offset;
    float fRT_Offset;
    bool bCtrl;
    bool bAlt;
    bool bShift;
    int id;
    int nTouchType; // 是用来传给js的
    inputEvent()
    {
        memset(type, 0, 256);
        nType = 0;
        key = 0;
        posX = 0;
        posY = 0;
        keyChar = 0;
        keyCode = 0;
        nWheel = 0;
        fTHUMBL_xOffset = fTHUMBL_yOffset = fTHUMBR_xOffset = fTHUMBR_yOffset = fLT_Offset = fRT_Offset = .0f;
        bCtrl = bAlt = bShift = false;
        id = 0;
        nTouchType = 0;
    }
};
struct DeviceOrientationEvent
{
    char type[256];
    int nType;
    float ra;
    float rb;
    float rg;
};
struct DeviceMotionEvent : public DeviceOrientationEvent
{
    float ax;
    float ay;
    float az;
    float agx;
    float agy;
    float agz;
    float interval;
};
enum EINPUTTYPE
{
    E_ONTOUCHSTART = 0X01,
    E_ONTOUCHMOVE,
    E_ONTOUCHEND,
    E_ONACTION_POINTER_DOWN,
    E_ONACTION_POINTER_UP,
    E_ONMOUSEDOWN,
    E_ONMOUSEMOVE,
    E_ONMOUSEWHEEL,
    E_ONMOUSEUP,
    E_ONRIGHTMOUSEDOWN,
    E_ONRIGHTMOUSEUP,
    E_ONKEYDOWN,
    E_ONKEYUP,
    E_JOYSTICK,
    E_DEVICEMOTION,
    E_DEVICEORIENTATION,
    E_TYPE_COUNT,
};
} // namespace laya
#endif