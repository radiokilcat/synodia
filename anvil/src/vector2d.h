#pragma once

#include <cmath>
#include "json_serializer/json_serializer.h"

namespace anvil {
    class Vector2D
    {
    public:
        Vector2D() :x_(0.0f), y_(0.0f)  {}
        Vector2D(float x, float y)
            : x_(x), y_(y)
        {}

        float x() const { return x_; }

        float y() const { return y_; }

        void setX(float x) { x_ = x; }

        void setY(float y) { y_ = y; }

        float length() const {
            return sqrt((x_ * x_) + (y_ * y_));
        }


        void normalize() {
            float l = length();

            if (l > 0) {
                (*this) *= 1 / l;
            }
        }

        Vector2D operator+ (const Vector2D& v2) const {
            return Vector2D(x_ + v2.x_, y_ + v2.y_);
        }

        Vector2D operator*(float scalar) {
            return Vector2D(x_ * scalar, y_ * scalar);
        }

        Vector2D& operator*=(float scalar) {
            x_ *= scalar;
            y_ *= scalar;
            return *this;
        }

        Vector2D operator-(const Vector2D& v2) const {
            return Vector2D(x_ - v2.x_, y_ - v2.y_);
        }

        Vector2D operator/(float scalar) {
            return Vector2D(x_ / scalar, y_ / scalar);
        }

        Vector2D& operator/=(float scalar) {
            x_ /= scalar;
            y_ /= scalar;
            return *this;
        }

        bool operator==(const Vector2D& other) const {
            return (std::fabs(x_ - other.x_) < std::numeric_limits<float>::epsilon() &&
                    std::fabs(y_ - other.y_) < std::numeric_limits<float>::epsilon());
        }

        bool isZero() const {
            constexpr float epsilon = std::numeric_limits<float>::epsilon();
            return (std::fabs(x_) < epsilon && std::fabs(y_) < epsilon);
        }
        
        friend Vector2D& operator+=(Vector2D& v1, Vector2D& v2) {
            v1.x_ += v2.x_;
            v1.y_ += v2.y_;

            return v1;
        }

        friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2) {
            v1.x_ -= v2.x_;
            v1.y_ -= v2.y_;
            return v1;
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2D, x_, y_)
    private:
        float x_;
        float y_;
    };
}