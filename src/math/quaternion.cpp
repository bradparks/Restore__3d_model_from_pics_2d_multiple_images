// Copyright (c) 2015, Kai Wolf
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "math/quaternion.hpp"

// C system files
// none

// C++ system files
#include <limits>

// header files of other libraries
// none

// header files of project libraries
#include "math/math_utils.hpp"

using namespace ret::math;

Quaternion::Quaternion() : s_(1.0), vec_(cv::Vec3d(0.0, 0.0, 0.0)) {}

Quaternion::Quaternion(double s, double x, double y, double z)
    : s_(s), vec_(cv::Vec3d(x, y, z)) {}

Quaternion::Quaternion(double s, const cv::Vec3d& vec) : s_(s), vec_(vec) {}

Quaternion::Quaternion(const Quaternion& other)
    : s_(other.s_), vec_(other.vec_) {}

Quaternion& Quaternion::operator=(const Quaternion& other) {
    s_ = other.s_;
    vec_ = other.vec_;
    return *this;
}

bool Quaternion::operator==(const Quaternion& other) const {
    return equals(s_, other.s_) && equals(vec_[0], other.vec_[0]) &&
           equals(vec_[1], other.vec_[1]) && equals(vec_[2], other.vec_[2]);
}

bool Quaternion::operator!=(const Quaternion& other) const {
    return !(*this == other);
}

Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(s_ + other.s_, vec_ + other.vec_);
}

Quaternion& Quaternion::operator+=(const Quaternion& other) {
    return (*this = (*this) + other);
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(s_ - other.s_, vec_ - other.vec_);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        s_ * other.s_ - vec_.dot(other.vec_),
        s_ * other.vec_ + other.s_ * vec_ + vec_.cross(other.vec_));
}

cv::Vec3d Quaternion::operator*(const cv::Vec3d& vec) const {
    double qs = s_;
    double qx = vec_[0];
    double qy = vec_[1];
    double qz = vec_[2];

    // clang-format off
    cv::Mat r = (cv::Mat_<double>(3, 3) <<
                 1-2*(qy*qy + qz*qz), 2*(qx*qy - qs*qz),   2*(qx*qz + qs*qy),
                 2*(qx*qy + qs*qz),   1-2*(qx*qx + qz*qz), 2*(qy*qz - qs*qx),
                 2*(qx*qz - qs*qy),   2*(qy*qz + qs*qx),   1-2*(qx*qx + qy*qy));
    // clang-format on

    cv::Mat m = r * cv::Mat(vec);
    return cv::Vec3d(m);
}

Quaternion Quaternion::operator*(double scalar) const {
    return Quaternion(s_ * scalar, vec_ * scalar);
}

Quaternion Quaternion::operator/(double scalar) const {
    return Quaternion(s_ / scalar, vec_ / scalar);
}

Quaternion& Quaternion::operator*=(double scalar) {
    s_ *= scalar;
    vec_ *= scalar;

    return *this;
}

double Quaternion::dot(Quaternion& other) const {
    return s_ * other.s_ + vec_[0] * other.vec_[0] + vec_[1] * other.vec_[1] +
           vec_[2] * other.vec_[2];
}

double Quaternion::magnitude() const {
    Quaternion q = Quaternion(*this);
    return sqrt(q.dot(q));
}

Quaternion& Quaternion::normalize() {
    Quaternion q = Quaternion(*this);
    double n = q.dot(q);

    if (math::equals(n, 1.0)) {
        return *this;
    }

    n = sqrt(n);
    return (*this *= 1.0 / n);
}

Quaternion Quaternion::conj() const {
    return Quaternion(s_, -vec_);
}

Quaternion Quaternion::inv() const {
    Quaternion q = Quaternion(*this);
    double n = q.dot(q);

    if (equals(n, 1.0)) {
        return conj();
    }

    return conj() * (1.0 / n);
}

cv::Vec3d Quaternion::toEulerAngles() const {
    cv::Vec3d eulerAngles;

    eulerAngles[0] = atan2(2 * (s_ * vec_[0] + vec_[1] * vec_[2]),
                           1 - 2 * (vec_[0] * vec_[0] + vec_[1] * vec_[1]));
    eulerAngles[1] = asin(2 * (s_ * vec_[1] - vec_[2] * vec_[0]));
    eulerAngles[2] = atan2(2 * (s_ * vec_[2] + vec_[0] * vec_[1]),
                           1 - 2 * (vec_[1] * vec_[1] + vec_[2] * vec_[2]));

    return eulerAngles;
}

cv::Mat Quaternion::toRotMatrix() const {
    double x = vec_[0];
    double y = vec_[1];
    double z = vec_[2];

    // clang-format off
    return (cv::Mat_<double>(3, 3) <<
            1 - 2*(y*y + z*z),  2*(x*y - s_*z),     2*(x*z + s_*y),
            2*(x*y + s_*z),     1 - 2*(x*x + z*z),  2*(y*z - s_*x),
            2*(x*z - s_*y),     2*(y*z + s_*x),     1 - 2*(x*x + y*y));
    // clang-format on
}

cv::Mat Quaternion::toRotMatrix4x4() const {
    double x = vec_[0];
    double y = vec_[1];
    double z = vec_[2];

    // clang-format off
    return (cv::Mat_<double>(4, 4) <<
            1 - 2*(y*y + z*z), 2*(x*y - s_*z),    2*(x*z + s_*y),     0,
            2*(x*y + s_*z),    1 - 2*(x*x + z*z), 2*(y*z - s_*x),     0,
            2*(x*z - s_*y),    2*(y*z + s_*x),    1 - 2*(x*x + y*y),  0,
            0,                 0,                 0,                  1);
    // clang-format on
}

cv::Vec4d Quaternion::getComponents() const {
    return cv::Vec4d(s_, vec_[0], vec_[1], vec_[2]);
}

Quaternion Quaternion::fromRotMatrix(const cv::Mat& rotMatrix) {
    assert(rotMatrix.cols == 3 && rotMatrix.rows == 3 &&
           rotMatrix.type() == CV_64FC1);

    double m00 = rotMatrix.at<double>(0, 0);
    double m01 = rotMatrix.at<double>(0, 1);
    double m02 = rotMatrix.at<double>(0, 2);

    double m10 = rotMatrix.at<double>(1, 0);
    double m11 = rotMatrix.at<double>(1, 1);
    double m12 = rotMatrix.at<double>(1, 2);

    double m20 = rotMatrix.at<double>(2, 0);
    double m21 = rotMatrix.at<double>(2, 1);
    double m22 = rotMatrix.at<double>(2, 2);

    // trace
    double t = m00 + m11 + m22;

    double m[] = {m00, m11, m22, t};
    int maxIdx = 0;
    for (int i = 1; i < 4; i++) {
        if (m[maxIdx] < m[i]) {
            maxIdx = i;
        }
    }

    double s, x, y, z;

    if (maxIdx == 0) {
        x = sqrt(2 * m00 - t + 1) / 2;
        y = (m10 + m01) / (4 * x);
        z = (m02 + m20) / (4 * x);
        s = (m21 - m12) / (4 * x);
    } else if (maxIdx == 1) {
        y = sqrt(2 * m11 - t + 1) / 2;
        x = (m10 + m01) / (4 * y);
        z = (m21 + m12) / (4 * y);
        s = (m02 - m20) / (4 * y);
    } else if (maxIdx == 2) {
        z = sqrt(2 * m22 - t + 1) / 2;
        x = (m02 + m20) / (4 * z);
        y = (m21 + m12) / (4 * z);
        s = (m10 - m01) / (4 * z);
    } else { // (maxIdx == 3)
        s = sqrt(t + 1) / 2;
        x = (m21 - m12) / (4 * s);
        y = (m02 - m20) / (4 * s);
        z = (m10 - m01) / (4 * s);
    }

    return Quaternion(s, x, y, z);
}

Quaternion Quaternion::fromRodriguesVec(const cv::Vec3d& rodrigues) {
    // get rotation angle
    double angle = norm(rodrigues);

    if (equals(angle, 0.0)) {
        return Quaternion::fromAxisAngle(cv::Vec3d(0, 0, 0), angle);
    }

    // normalize rodrigues vector
    cv::Vec3d rod = rodrigues / angle;

    return Quaternion::fromAxisAngle(rod, angle);
}

Quaternion Quaternion::fromEulerAngles(const cv::Vec3d& eulerAngles) {
    double sx2 = sin(0.5 * eulerAngles[0]);
    double sy2 = sin(0.5 * eulerAngles[1]);
    double sz2 = sin(0.5 * eulerAngles[2]);

    double cx2 = cos(0.5 * eulerAngles[0]);
    double cy2 = cos(0.5 * eulerAngles[1]);
    double cz2 = cos(0.5 * eulerAngles[2]);

    double s = cz2 * cy2 * cx2 + sz2 * sy2 * sx2;
    double x = cz2 * cy2 * sx2 - sz2 * sy2 * cx2;
    double y = cz2 * sy2 * cx2 + sz2 * cy2 * sx2;
    double z = sz2 * cy2 * cx2 - cz2 * sy2 * sx2;

    return Quaternion(s, x, y, z);
}

Quaternion Quaternion::fromAxisAngle(const cv::Vec3d& axis, double angle) {
    // calculate euler parameters

    double halfSin = sin(0.5f * angle);
    double halfCos = cos(0.5f * angle);

    double s = halfCos;
    double x = axis[0] * halfSin;
    double y = axis[1] * halfSin;
    double z = axis[2] * halfSin;

    return Quaternion(s, x, y, z);
}

Quaternion Quaternion::betweenTwoVecs(const cv::Vec3d& a, const cv::Vec3d& b) {
    if (a == b) {
        // avoid zero division error
        return Quaternion();
    }

    cv::Vec3d u = a / norm(a);
    cv::Vec3d v = b / norm(b);

    double cosTheta = u.dot(v);
    double angle = acos(cosTheta);

    cv::Vec3d w = u.cross(v);
    w /= norm(w);

    return Quaternion::fromAxisAngle(w, angle);
}

Quaternion Quaternion::lookAt(const cv::Vec3d& a, const cv::Vec3d& b) {
    cv::Vec3d gFwd(1, 0, 0);
    cv::Vec3d gUp(0, 0, 1);

    cv::Vec3d forward(b - a);
    double lForward = norm(forward);
    if (lForward > 0) {
        forward /= lForward;
    }

    cv::Vec3d right = forward.cross(gUp);
    double lRight = norm(right);
    if (lRight > 0) {
        right /= lRight;
    }

    double dot = gFwd.dot(forward);

    if (fabs(dot - (-1.0)) < std::numeric_limits<double>::epsilon()) {
        return Quaternion::fromAxisAngle(gUp, static_cast<double>(CV_PI));
    }

    if (fabs(dot - (1.0)) < std::numeric_limits<double>::epsilon()) {
        return Quaternion(); // identity
    }

    double angle = acos(dot);

    cv::Vec3d rotAxis = gFwd.cross(forward);
    double lRotAxis = norm(rotAxis);
    if (lRotAxis > 0) {
        rotAxis /= lRotAxis;
    }

    return Quaternion::fromAxisAngle(rotAxis, angle);
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2,
                             double t) {
    if (t <= 0.0) {
        return q1;
    }

    if (t >= 1.0) {
        return q2;
    }

    Quaternion q3 = Quaternion(q2);

    double dot = q1.dot(q3);

    if (dot < 0) {
        dot = -dot;
        q3 *= -1;
    }

    if (dot < 0.95) {
        double angle = acos(dot);
        return Quaternion((q1 * sin(angle * (1 - t)) + q3 * sin(angle * t)) /
                          sin(angle));
    } else {
        return Quaternion(q1 * (1 - t) + q3 * t);
    }
}

Quaternion Quaternion::log(const Quaternion& q) {
    cv::Vec3d qVec = q.vec_;
    double vecLength = cv::norm(qVec);

    if (equals(vecLength, 0.0)) {
        if (q.s_ > 0)
            return Quaternion(cv::log(q.s_), 0.0, 0.0, 0.0);
        else if (q.s_ < 0)
            return Quaternion(cv::log(-q.s_), static_cast<double>(M_PI), 0.0,
                              0.0);
    }

    double qLength = q.magnitude();
    double t = atan2(vecLength, q.s_);

    return Quaternion(cv::log(qLength), t * cv::normalize(qVec));
}

Quaternion Quaternion::exp(const Quaternion& q) {
    cv::Vec3d qVec = q.vec_;
    double angle = cv::norm(qVec);

    return Quaternion(cos(angle), sin(angle) * cv::normalize(qVec)) *
           cv::exp(q.s_);
}

Quaternion Quaternion::pow(const Quaternion& q, double t) {
    Quaternion l = log(q) * t;
    return exp(l);
}
