#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

#include <cmath>
#include <iostream>

class Vector2D {
public:
    float x, y;

    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}


    Vector2D operator+(const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D& Add(const Vector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& v) const {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D& Subtract(const Vector2D& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }


    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D& Multiply(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }


    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& Divide(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }


    Vector2D normalize() const {
        float mag = magnitude();
        return (mag == 0.0f) ? Vector2D(0.0f, 0.0f) : Vector2D(x / mag, y / mag);
    }


    float dot(const Vector2D& v) const {
        return x * v.x + y * v.y;
    }


    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};


#endif // VECTOR2D_H_INCLUDED
