/**
 * @file Point.h
 * @brief Point 对象表示二维坐标系统中的某个位置，其中 x 表示水平轴，y 表示垂直轴。
 */

#ifndef __POINT_H__
#define __POINT_H__

#include <string>

namespace laya {

class Point {
public:
    /**
     * @brief 临时使用的公用对象
     */
    static Point TEMP;

    /**
     * @brief 全局空的point对象(x=0，y=0)，不允许修改此对象内容
     */
    static const Point EMPTY;

    /**
     * @brief 该点的水平坐标
     */
    float x;

    /**
     * @brief 该点的垂直坐标
     */
    float y;

    /**
     * @brief 构造函数
     * @param x 水平坐标，默认为0
     * @param y 垂直坐标，默认为0
     */
    Point(float x = 0.0f, float y = 0.0f);

    /**
     * @brief 从对象池创建
     * @return 从对象池中获取的Point对象
     */
    static Point* create();

    /**
     * @brief 将 Point 的成员设置为指定值
     * @param x 水平坐标
     * @param y 垂直坐标
     * @return 当前 Point 对象
     */
    Point& setTo(float x, float y);

    /**
     * @brief 重置点为(0, 0)
     * @return 当前 Point 对象
     */
    Point& reset();


    /**
     * @brief 计算当前点和目标点(x，y)的距离
     * @param x 目标点的水平坐标
     * @param y 目标点的垂直坐标
     * @return 返回当前点和目标点之间的距离
     */
    float distance(float x, float y) const;

    /**
     * @brief 返回包含 x 和 y 坐标的值的字符串
     */
    std::string toString() const;

    /**
     * @brief 标准化向量
     */
    void normalize();

    /**
     * @brief 复制另一个点的坐标
     * @param point 需要被复制的点
     * @return 当前 Point 对象
     */
    Point& copy(const Point& point);
};

} // namespace laya

#endif // __POINT_H__ 