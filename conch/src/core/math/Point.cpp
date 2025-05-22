#include "Point.h"
#include <cmath>
#include <sstream>

namespace laya {

Point Point::TEMP;
const Point Point::EMPTY;

Point::Point(float x, float y) : x(x), y(y) {
}

Point& Point::setTo(float x, float y) {
    this->x = x;
    this->y = y;
    return *this;
}

Point& Point::reset() {
    x = y = 0;
    return *this;
}

float Point::distance(float x, float y) const {
    return std::sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
}

std::string Point::toString() const {
    std::stringstream ss;
    ss << x << "," << y;
    return ss.str();
}

void Point::normalize() {
    float d = std::sqrt(x * x + y * y);
    if (d > 0) {
        float id = 1.0f / d;
        x *= id;
        y *= id;
    }
}

Point& Point::copy(const Point& point) {
    return setTo(point.x, point.y);
}

} // namespace laya 