#ifndef __Utils3D_H_
#define __Utils3D_H_
namespace laya
{
class Utils3D
{
  public:
    static void _mulMatrixArray(float *left, float *right, int rightOffset, float *outArray, int outOffset)
    {
        float *l = right;
        float *r = left;
        float *e = outArray;

        float l11 = l[rightOffset], l12 = l[rightOffset + 1], l13 = l[rightOffset + 2], l14 = l[rightOffset + 3];
        float l21 = l[rightOffset + 4], l22 = l[rightOffset + 5], l23 = l[rightOffset + 6], l24 = l[rightOffset + 7];
        float l31 = l[rightOffset + 8], l32 = l[rightOffset + 9], l33 = l[rightOffset + 10], l34 = l[rightOffset + 11];
        float l41 = l[rightOffset + 12], l42 = l[rightOffset + 13], l43 = l[rightOffset + 14],
              l44 = l[rightOffset + 15];

        float r11 = r[0], r12 = r[1], r13 = r[2], r14 = r[3];
        float r21 = r[4], r22 = r[5], r23 = r[6], r24 = r[7];
        float r31 = r[8], r32 = r[9], r33 = r[10], r34 = r[11];
        float r41 = r[12], r42 = r[13], r43 = r[14], r44 = r[15];

        e[outOffset] = (l11 * r11) + (l12 * r21) + (l13 * r31) + (l14 * r41);
        e[outOffset + 1] = (l11 * r12) + (l12 * r22) + (l13 * r32) + (l14 * r42);
        e[outOffset + 2] = (l11 * r13) + (l12 * r23) + (l13 * r33) + (l14 * r43);
        e[outOffset + 3] = (l11 * r14) + (l12 * r24) + (l13 * r34) + (l14 * r44);
        e[outOffset + 4] = (l21 * r11) + (l22 * r21) + (l23 * r31) + (l24 * r41);
        e[outOffset + 5] = (l21 * r12) + (l22 * r22) + (l23 * r32) + (l24 * r42);
        e[outOffset + 6] = (l21 * r13) + (l22 * r23) + (l23 * r33) + (l24 * r43);
        e[outOffset + 7] = (l21 * r14) + (l22 * r24) + (l23 * r34) + (l24 * r44);
        e[outOffset + 8] = (l31 * r11) + (l32 * r21) + (l33 * r31) + (l34 * r41);
        e[outOffset + 9] = (l31 * r12) + (l32 * r22) + (l33 * r32) + (l34 * r42);
        e[outOffset + 10] = (l31 * r13) + (l32 * r23) + (l33 * r33) + (l34 * r43);
        e[outOffset + 11] = (l31 * r14) + (l32 * r24) + (l33 * r34) + (l34 * r44);
        e[outOffset + 12] = (l41 * r11) + (l42 * r21) + (l43 * r31) + (l44 * r41);
        e[outOffset + 13] = (l41 * r12) + (l42 * r22) + (l43 * r32) + (l44 * r42);
        e[outOffset + 14] = (l41 * r13) + (l42 * r23) + (l43 * r33) + (l44 * r43);
        e[outOffset + 15] = (l41 * r14) + (l42 * r24) + (l43 * r34) + (l44 * r44);
    }
};
}
#endif