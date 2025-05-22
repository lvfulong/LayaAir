#include "Matrix.h"
#include <cmath>
#include <sstream>
#include "MathUtils3D.h"
#include <utils/Preprocessor.h>
namespace laya {

    const Matrix Matrix::EMPTY;
    Matrix Matrix::TEMP;

    bool Matrix::equals(const Matrix& a, const Matrix& b) {
        return MathUtils3D::nearEqual(a.a, b.a) &&
            MathUtils3D::nearEqual(a.b, b.b) &&
            MathUtils3D::nearEqual(a.c, b.c) &&
            MathUtils3D::nearEqual(a.d, b.d) &&
            MathUtils3D::nearEqual(a.tx, b.tx) &&
            MathUtils3D::nearEqual(a.ty, b.ty);
    }

    Matrix::TransformInfo Matrix::extractTransformInfo(const Matrix& matrix) {
        TransformInfo info;
        float x = matrix.tx;
        float y = matrix.ty;

        // 计算缩放和旋转前，先检查行列式
        float det = matrix.a * matrix.d - matrix.b * matrix.c;
        float sign = det < 0 ? -1 : 1;

        // 提取缩放
        info.scaleX = sqrt(matrix.a * matrix.a + matrix.b * matrix.b);
        info.scaleY = sign * sqrt(matrix.c * matrix.c + matrix.d * matrix.d);

        // 提取旋转（角度）
        info.rotation = MathUtils3D::toAngle(atan2(matrix.b, matrix.a));

        // 计算X和Y方向的倾斜
        info.skewX = 0;
        info.skewY = 0;

        if (info.scaleX != 0 && info.scaleY != 0) {
            // 归一化矩阵的第一行和第二行
            float norm_a = matrix.a / info.scaleX;
            float norm_b = matrix.b / info.scaleX;
            float norm_c = matrix.c / info.scaleY;
            float norm_d = matrix.d / info.scaleY;

            // 旋转角的余弦和正弦
            float cos = norm_a;
            float sin = norm_b;

            // 计算去除旋转后的矩阵元素
            float derot_c = norm_c * cos + norm_d * sin;
            float derot_d = -norm_c * sin + norm_d * cos;

            // X方向倾斜角
            info.skewX = MathUtils3D::toAngle(atan2(derot_c, derot_d));

            // Y方向倾斜角
            float dotProduct = matrix.a * matrix.c + matrix.b * matrix.d;
            float len1 = sqrt(matrix.a * matrix.a + matrix.b * matrix.b);
            float len2 = sqrt(matrix.c * matrix.c + matrix.d * matrix.d);

            if (len1 != 0 && len2 != 0) {
                float cosTheta = dotProduct / (len1 * len2);
                // 防止数值误差导致的超出[-1,1]范围
                float clampedCosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
                info.skewY = MathUtils3D::toAngle(PI / 2 - acos(clampedCosTheta));
            }
        }

        info.x = x;
        info.y = y;
        return info;
    }

    Matrix::Matrix(float a, float b, float c, float d, float tx, float ty)
        : a(a), b(b), c(c), d(d), tx(tx), ty(ty), _bTransform(false) {
        _checkTransform();
    }

    Matrix& Matrix::identity() {
        a = d = 1;
        b = tx = ty = c = 0;
        _bTransform = false;
        return *this;
    }

    bool Matrix::_checkTransform() {
        return _bTransform = (a != 1 || b != 0 || c != 0 || d != 1);
    }

    Matrix& Matrix::setTranslate(float x, float y) {
        tx = x;
        ty = y;
        return *this;
    }

    Matrix& Matrix::translate(float x, float y) {
        tx += x;
        ty += y;
        return *this;
    }

    Matrix& Matrix::scale(float x, float y) {
        a *= x;
        d *= y;
        c *= x;
        b *= y;
        tx *= x;
        ty *= y;
        _bTransform = true;
        return *this;
    }

    Matrix& Matrix::rotate(float angle) {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        float a1 = a;
        float c1 = c;
        float tx1 = tx;

        a = a1 * cos - b * sin;
        b = a1 * sin + b * cos;
        c = c1 * cos - d * sin;
        d = c1 * sin + d * cos;
        tx = tx1 * cos - ty * sin;
        ty = tx1 * sin + ty * cos;
        _bTransform = true;
        return *this;
    }

    Matrix& Matrix::skew(float x, float y) {
        float sinx = std::sin(x);
        float cosx = std::cos(x);
        float siny = std::sin(y);
        float cosy = std::cos(y);
        float a1 = a;
        float c1 = c;
        float tx1 = tx;

        a = cosy * a1 + sinx * b;
        b = siny * a1 + cosx * b;
        c = cosy * c1 + sinx * d;
        d = siny * c1 + cosx * d;
        tx = cosy * tx1 + sinx * ty;
        ty = siny * tx1 + cosx * ty;
        _bTransform = true;
        return *this;
    }

    Point& Matrix::invertTransformPoint(Point& out) {
        float a1 = a;
        float b1 = b;
        float c1 = c;
        float d1 = d;
        float tx1 = tx;
        float n = a1 * d1 - b1 * c1;

        float a2 = d1 / n;
        float b2 = -b1 / n;
        float c2 = -c1 / n;
        float d2 = a1 / n;
        float tx2 = (c1 * ty - d1 * tx1) / n;
        float ty2 = -(a1 * ty - b1 * tx1) / n;
        return out.setTo(a2 * out.x + c2 * out.y + tx2, b2 * out.x + d2 * out.y + ty2);
    }

    Point& Matrix::transformPoint(Point& out) {
        return out.setTo(a * out.x + c * out.y + tx, b * out.x + d * out.y + ty);
    }

    Point& Matrix::transformPointN(Point& out) {
        return out.setTo(a * out.x + c * out.y, b * out.x + d * out.y);
    }

    float Matrix::getScaleX() const {
        return b == 0 ? a : std::sqrt(a * a + b * b);
    }

    float Matrix::getScaleY() const {
        return c == 0 ? d : std::sqrt(c * c + d * d);
    }

    Matrix& Matrix::invert() {
        float a1 = a;
        float b1 = b;
        float c1 = c;
        float d1 = d;
        float tx1 = tx;
        float n = a1 * d1 - b1 * c1;

        a = d1 / n;
        b = -b1 / n;
        c = -c1 / n;
        d = a1 / n;
        tx = (c1 * ty - d1 * tx1) / n;
        ty = -(a1 * ty - b1 * tx1) / n;
        return *this;
    }

    Matrix& Matrix::setTo(float a, float b, float c, float d, float tx, float ty) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->tx = tx;
        this->ty = ty;
        return *this;
    }

    Matrix& Matrix::concat(const Matrix& matrix) {
        float a1 = a;
        float c1 = c;
        float tx1 = tx;

        a = a1 * matrix.a + b * matrix.c;
        b = a1 * matrix.b + b * matrix.d;
        c = c1 * matrix.a + d * matrix.c;
        d = c1 * matrix.b + d * matrix.d;
        tx = tx1 * matrix.a + ty * matrix.c + matrix.tx;
        ty = tx1 * matrix.b + ty * matrix.d + matrix.ty;
        return *this;
    }

    Matrix& Matrix::mul(const Matrix& m1, const Matrix& m2, Matrix& out) {
        float aa = m1.a, ab = m1.b, ac = m1.c, ad = m1.d, atx = m1.tx, aty = m1.ty;
        float ba = m2.a, bb = m2.b, bc = m2.c, bd = m2.d, btx = m2.tx, bty = m2.ty;
        out.a = aa * ba + ab * bc;
        out.b = aa * bb + ab * bd;
        out.c = ac * ba + ad * bc;
        out.d = ac * bb + ad * bd;
        out.tx = atx * ba + aty * bc + btx;
        out.ty = atx * bb + aty * bd + bty;
        return out;
    }

}