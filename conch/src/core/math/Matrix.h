/**
 * @file Matrix.h
 * @brief 表示一个转换矩阵，它确定如何将点从一个坐标空间映射到另一个坐标空间。
 * 您可以对一个显示对象执行不同的图形转换，方法是设置 Matrix 对象的属性，将该 Matrix 对象应用于 Transform 对象的 matrix 属性，
 * 然后应用该 Transform 对象作为显示对象的 transform 属性。这些转换函数包括平移（x 和 y 重新定位）、旋转、缩放和倾斜。
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Point.h"
//#include "../utils/Utils.h"
#include "MathUtils3D.h"

namespace laya {

class Matrix {
public:
    /**
     * @brief 比较两个矩阵是否相等
     * @param a 第一个矩阵
     * @param b 第二个矩阵
     * @return 如果矩阵相等，返回 true，否则返回 false
     */
    static bool equals(const Matrix& a, const Matrix& b);

    /**
     * @brief 从矩阵中提取变换信息，包括X和Y方向的倾斜
     * @param matrix 需要提取的矩阵
     * @return 返回一个包含变换信息的结构体
     */
    struct TransformInfo {
        float x, y;
        float scaleX, scaleY;
        float rotation;
        float skewX, skewY;
    };
    static TransformInfo extractTransformInfo(const Matrix& matrix);

    /**
     * @brief 一个初始化的 Matrix 对象，不允许修改此对象内容
     */
    static const Matrix EMPTY;

    /**
     * @brief 用于中转使用的 Matrix 对象
     */
    static Matrix TEMP;

    /**
     * @brief 缩放或旋转图像时影响像素沿 x 轴定位的值
     */
    float a;

    /**
     * @brief 旋转或倾斜图像时影响像素沿 y 轴定位的值
     */
    float b;

    /**
     * @brief 旋转或倾斜图像时影响像素沿 x 轴定位的值
     */
    float c;

    /**
     * @brief 缩放或旋转图像时影响像素沿 y 轴定位的值
     */
    float d;

    /**
     * @brief 沿 x 轴平移每个点的距离
     */
    float tx;

    /**
     * @brief 沿 y 轴平移每个点的距离
     */
    float ty;

    /**
     * @brief 是否有旋转缩放操作
     */
    bool _bTransform;

    /**
     * @brief 构造函数
     */
    Matrix(float a = 1.0f, float b = 0.0f, float c = 0.0f, float d = 1.0f, float tx = 0.0f, float ty = 0.0f);

    /**
     * @brief 将本矩阵设置为单位矩阵
     * @return 返回当前矩阵
     */
    Matrix& identity();

    /**
     * @brief 设置沿 x 、y 轴平移每个点的距离
     * @param x 沿 x 轴平移每个点的距离
     * @param y 沿 y 轴平移每个点的距离
     * @return 返回对象本身
     */
    Matrix& setTranslate(float x, float y);

    /**
     * @brief 沿 x 和 y 轴平移矩阵
     * @param x 沿 x 轴向右移动的量（以像素为单位）
     * @param y 沿 y 轴向下移动的量（以像素为单位）
     * @return 返回此矩阵对象
     */
    Matrix& translate(float x, float y);

    /**
     * @brief 对矩阵应用缩放转换
     * @param x 用于沿 x 轴缩放对象的乘数
     * @param y 用于沿 y 轴缩放对象的乘数
     * @return 返回矩阵对象本身
     */
    Matrix& scale(float x, float y);

    /**
     * @brief 对矩阵应用旋转转换
     * @param angle 以弧度为单位的旋转角度
     * @return 返回矩阵对象本身
     */
    Matrix& rotate(float angle);

    /**
     * @brief 对矩阵应用倾斜转换
     * @param x 沿着 X 轴的 2D 倾斜弧度
     * @param y 沿着 Y 轴的 2D 倾斜弧度
     * @return 当前矩阵对象
     */
    Matrix& skew(float x, float y);

    /**
     * @brief 对指定的点应用当前矩阵的逆转化
     * @param out 待转化的点 Point 对象
     * @return 返回out
     */
    Point& invertTransformPoint(Point& out);

    /**
     * @brief 将矩阵表示的几何转换应用于指定点
     * @param out 用来设定输出结果的点
     * @return 返回out
     */
    Point& transformPoint(Point& out);

    /**
     * @brief 将矩阵表示的几何转换应用于指定点，忽略tx、ty
     * @param out 用来设定输出结果的点
     * @return 返回out
     */
    Point& transformPointN(Point& out);

    /**
     * @brief 获取 X 轴缩放值
     */
    float getScaleX() const;

    /**
     * @brief 获取 Y 轴缩放值
     */
    float getScaleY() const;

    /**
     * @brief 执行原始矩阵的逆转换
     * @return 当前矩阵对象
     */
    Matrix& invert();

    /**
     * @brief 将矩阵的成员设置为指定值
     * @param a 缩放或旋转图像时影响像素沿 x 轴定位的值
     * @param b 旋转或倾斜图像时影响像素沿 y 轴定位的值
     * @param c 旋转或倾斜图像时影响像素沿 x 轴定位的值
     * @param d 缩放或旋转图像时影响像素沿 y 轴定位的值
     * @param tx 沿 x 轴平移每个点的距离
     * @param ty 沿 y 轴平移每个点的距离
     * @return 当前矩阵对象
     */
    Matrix& setTo(float a, float b, float c, float d, float tx, float ty);

    /**
     * @brief 将指定矩阵与当前矩阵连接
     * @param matrix 要连接到源矩阵的矩阵
     * @return 当前矩阵
     */
    Matrix& concat(const Matrix& matrix);

    /**
     * @brief 将指定的两个矩阵相乘后的结果赋值给指定的输出对象
     * @param m1 矩阵一
     * @param m2 矩阵二
     * @param out 输出对象
     * @return 结果输出对象 out
     */
    static Matrix& mul(const Matrix& m1, const Matrix& m2, Matrix& out);

    /**
     * @brief 将指定的两个矩阵相乘，结果赋值给指定的输出数组
     * @param m1 矩阵一
     * @param m2 矩阵二
     * @param out 输出对象数组
     * @return 结果输出对象 out
     */
    static float* mul16(const Matrix& m1, const Matrix& m2, float* out);

    /**
     * @brief 返回此矩阵对象的副本
     * @return 与原始实例具有完全相同的属性的新矩阵实例
     */
    Matrix clone() const;

    /**
     * @brief 将当前矩阵对象中的所有矩阵数据复制到指定的矩阵对象中
     * @param dec 要复制当前矩阵数据的矩阵对象
     * @return 已复制当前矩阵数据的矩阵对象
     */
    Matrix& copyTo(Matrix& dec) const;

    /**
     * @brief 设置矩阵
     * 输出矩阵 = 平移矩阵*旋转矩阵*斜切矩阵*缩放矩阵*描点矩阵
     */
    Matrix& setMatrix(float x, float y, float sx, float sy, float rotation, float skewX, float skewY, float pivotx, float pivoty);

    /**
     * @brief 返回列出该矩阵对象属性的文本值
     * @return 一个字符串，它包含矩阵对象的属性值：a、b、c、d、tx 和 ty
     */
    std::string toString() const;

    /**
     * @brief 销毁此对象
     */
    void destroy();

    /**
     * @brief 回收到对象池，方便复用
     */
    void recover();

    /**
     * @brief 从对象池中创建一个矩阵对象
     * @return 返回一个矩阵对象
     */
    static Matrix* create();

private:
    bool _checkTransform();
};

} // namespace laya

#endif // __MATRIX_H__ 