#pragma once
#include <string>
#include <vector>
namespace laya
{
    /**
     * @en The `Rectangle` object is an area defined by its position, as indicated by its top-left corner point (x, y), and by its width and height.
     * The x, y, width, and height properties of the Rectangle class are independent of each other; changing the value of one property has no effect on the others.
     * @zh `Rectangle` 对象是按其位置（由它左上角的点 (x, y) 确定）以及宽度和高度定义的区域。
     * Rectangle 类的 x、y、width 和 height 属性相互独立；更改一个属性的值不会影响其他属性。
     */
    class Rectangle {
    public:
        /**
         * @en Global empty rectangle area with x=0, y=0, width=0, height=0. The content of this object is not allowed to be modified.
         * @zh 全局空的矩形区域，x=0, y=0, width=0, height=0。不允许修改此对象内容。
         */
        static const Rectangle EMPTY;

        /**
         * @en Global temporary rectangle area. This object is used for global reuse to reduce object creation.
         * @zh 全局临时的矩形区域，此对象用于全局复用，以减少对象创建。
         */
        static Rectangle TEMP;

        /**
         * @en Constructor method.
         * @param x The x coordinate of the top-left corner of the rectangle.
         * @param y The y coordinate of the top-left corner of the rectangle.
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        Rectangle(float x = 0, float y = 0, float width = 0, float height = 0);

        // Getters and setters
        float getX() const { return x; }
        void setX(float value) { x = value; }
        float getY() const { return y; }
        void setY(float value) { y = value; }
        float getWidth() const { return width; }
        void setWidth(float value) { width = value; }
        float getHeight() const { return height; }
        void setHeight(float value) { height = value; }

        float getRight() const { return x + width; }
        void setRight(float value) { width = value - x; }
        float getBottom() const { return y + height; }
        void setBottom(float value) { height = value - y; }

        // Methods
        Rectangle& setTo(float x, float y, float width, float height);
        Rectangle& reset();
        void recover();
        static Rectangle create();
        Rectangle& copyFrom(const Rectangle& source);
        bool contains(float x, float y) const;
        bool intersects(const Rectangle& rect) const;
        Rectangle* intersection(const Rectangle& rect, Rectangle* out = nullptr) const;
        Rectangle* union_(const Rectangle& source, Rectangle* out = nullptr) const;
        Rectangle& scale(float scaleX, float scaleY);
        std::string toString() const;
        bool equals(const Rectangle& rect) const;
        Rectangle& addPoint(float x, float y);
        std::vector<float> getBoundPoints(std::vector<float>* out = nullptr) const;
        static Rectangle _getWrapRec(const std::vector<float>& points, Rectangle* out = nullptr);
        static Rectangle minMaxRect(float minX, float minY, float maxX, float maxY, Rectangle* out = nullptr);
        bool isEmpty() const;
        Rectangle clone(Rectangle* out = nullptr) const;
        void cloneTo(Rectangle& destObject) const;

    private:
        float x;
        float y;
        float width;
        float height;
    };
}