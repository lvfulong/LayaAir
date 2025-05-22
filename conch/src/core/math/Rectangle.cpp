#include "Rectangle.h"
#include <algorithm>
#include <sstream>

const Rectangle Rectangle::EMPTY;
Rectangle Rectangle::TEMP;

Rectangle::Rectangle(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {}

Rectangle& Rectangle::setTo(float x, float y, float width, float height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    return *this;
}

Rectangle& Rectangle::reset() {
    x = y = width = height = 0;
    return *this;
}

void Rectangle::recover() {
    if (this == &TEMP || this == &EMPTY) {
        return;
    }
    // 实现对象池回收逻辑
    reset();
}

Rectangle Rectangle::create() {
    // 实现对象池创建逻辑
    return Rectangle();
}

Rectangle& Rectangle::copyFrom(const Rectangle& source) {
    x = source.x;
    y = source.y;
    width = source.width;
    height = source.height;
    return *this;
}

bool Rectangle::contains(float x, float y) const {
    if (width <= 0 || height <= 0) return false;
    return (x >= this->x && x < getRight() && y >= this->y && y < getBottom());
}

bool Rectangle::intersects(const Rectangle& rect) const {
    return !(rect.x > (x + width) || (rect.x + rect.width) < x || 
             rect.y > (y + height) || (rect.y + rect.height) < y);
}

Rectangle* Rectangle::intersection(const Rectangle& rect, Rectangle* out) const {
    if (!intersects(rect)) return nullptr;

    float newX = std::max(x, rect.x);
    float newY = std::max(y, rect.y);
    float newWidth = std::min(getRight(), rect.getRight()) - newX;
    float newHeight = std::min(getBottom(), rect.getBottom()) - newY;

    if (!out) out = new Rectangle();
    out->setTo(newX, newY, newWidth, newHeight);
    return out;
}

Rectangle* Rectangle::union_(const Rectangle& source, Rectangle* out) const {
    if (!out) out = new Rectangle();
    cloneTo(*out);
    
    if (source.width <= 0 || source.height <= 0) return out;

    out->addPoint(source.x, source.y);
    out->addPoint(source.x + source.width, source.y + source.height);
    return out;
}

Rectangle& Rectangle::scale(float scaleX, float scaleY) {
    x *= scaleX;
    y *= scaleY;
    width *= scaleX;
    height *= scaleY;
    return *this;
}

std::string Rectangle::toString() const {
    std::stringstream ss;
    ss << x << "," << y << "," << width << "," << height;
    return ss.str();
}

bool Rectangle::equals(const Rectangle& rect) const {
    return x == rect.x && y == rect.y && width == rect.width && height == rect.height;
}

Rectangle& Rectangle::addPoint(float x, float y) {
    if (this->x > x) {
        width += this->x - x;
        this->x = x;
    }
    if (this->y > y) {
        height += this->y - y;
        this->y = y;
    }
    if (width < x - this->x) width = x - this->x;
    if (height < y - this->y) height = y - this->y;
    return *this;
}

std::vector<float> Rectangle::getBoundPoints(std::vector<float>* out) const {
    if (!out) out = new std::vector<float>();
    if (width == 0 || height == 0) return *out;
    
    out->push_back(x);
    out->push_back(y);
    out->push_back(x + width);
    out->push_back(y);
    out->push_back(x);
    out->push_back(y + height);
    out->push_back(x + width);
    out->push_back(y + height);
    
    return *out;
}

Rectangle Rectangle::_getWrapRec(const std::vector<float>& points, Rectangle* out) {
    if (!out) out = new Rectangle();
    if (points.empty()) return out->setTo(0, 0, 0, 0);

    float minX = 99999, minY = 99999;
    float maxX = -minX, maxY = -minY;

    for (size_t i = 0; i < points.size(); i += 2) {
        float tx = points[i];
        float ty = points[i + 1];
        minX = std::min(minX, tx);
        minY = std::min(minY, ty);
        maxX = std::max(maxX, tx);
        maxY = std::max(maxY, ty);
    }

    return out->setTo(minX, minY, maxX - minX, maxY - minY);
}

Rectangle Rectangle::minMaxRect(float minX, float minY, float maxX, float maxY, Rectangle* out) {
    if (!out) out = new Rectangle();
    return out->setTo(minX, minY, maxX - minX, maxY - minY);
}

bool Rectangle::isEmpty() const {
    return width <= 0 || height <= 0;
}

Rectangle Rectangle::clone(Rectangle* out) const {
    if (!out) out = new Rectangle();
    cloneTo(*out);
    return *out;
}

void Rectangle::cloneTo(Rectangle& destObject) const {
    destObject.x = x;
    destObject.y = y;
}
