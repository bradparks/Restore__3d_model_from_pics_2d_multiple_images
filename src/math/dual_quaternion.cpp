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

#include "math/dual_quaternion.hpp"

// C system files
// none

// C++ system files
#include <cmath>
#include <cstddef>

// header files of other libraries
// none

// header files of project libraries
#include "math/math_utils.hpp"

namespace ret {

namespace math {

    DualQuaternion::DualQuaternion()
        : real_(Quaternion(1.0, 0.0, 0.0, 0.0)),
          dual_(Quaternion(0.0, 0.0, 0.0, 0.0)) {}

    DualQuaternion::DualQuaternion(const Quaternion& real,
                                   const Quaternion& dual)
        : real_(real), dual_(dual) {}

    DualQuaternion::DualQuaternion(const Quaternion& real, const cv::Vec3d vec)
        : real_(initRealPart(real)), dual_(initDualPart(vec)) {}

    DualQuaternion::DualQuaternion(const cv::Vec3d rodrigues,
                                   const cv::Vec3d vec)
        : real_(initRealPart(Quaternion::fromRodriguesVec(rodrigues))),
          dual_(initDualPart(vec)) {}

    DualQuaternion::DualQuaternion(const cv::Mat& Rot, const cv::Vec3d vec)
        : real_(initRealPart(Quaternion::fromRotMatrix(Rot))),
          dual_(initDualPart(vec)) {}

    DualQuaternion::DualQuaternion(const cv::Mat& Pose)
        : real_(initRealPart(Quaternion::fromRotMatrix(
              Pose(cv::Range(0, 3), cv::Range(0, 3)).clone()))),
          dual_(initDualPart(cv::Vec3d(Pose.at<double>(0, 3),
                                       Pose.at<double>(1, 3),
                                       Pose.at<double>(2, 3)))) {}

    DualQuaternion::DualQuaternion(const cv::Vec3d axis, double angle,
                                   const cv::Vec3d vec)
        : real_(initRealPart(Quaternion::fromAxisAngle(axis, angle))),
          dual_(initDualPart(vec)) {}

    Quaternion DualQuaternion::getRotation() const { return real_; }

    cv::Vec3d DualQuaternion::getTranslation() const {
        auto q = (dual_ * real_.conj()) * 2.0;
        auto comps = q.getComponents();
        return cv::Vec3d(comps[1], comps[2], comps[3]);
    }

    cv::Mat DualQuaternion::getPose() const {
        auto dq = DualQuaternion(real_, dual_);
        dq      = dq.normalize();

        auto P     = dq.real_.toRotMatrix4x4();
        auto trans = dq.getTranslation();

        for (auto idx : {0, 1, 2}) {
            P.at<double>(idx, 3) = trans[idx];
        }
        return P;
    }

    bool DualQuaternion::operator==(const DualQuaternion& other) const {
        return ((*this).real_ == other.real_ && (*this).dual_ == other.dual_);
    }

    bool DualQuaternion::operator!=(const DualQuaternion& other) const {
        return !(*this == other);
    }

    DualQuaternion DualQuaternion::operator+(
        const DualQuaternion& other) const {
        return DualQuaternion(real_ + other.real_, dual_ + other.dual_);
    }

    DualQuaternion& DualQuaternion::operator+=(const DualQuaternion& other) {
        return (*this = (*this) + other);
    }

    DualQuaternion DualQuaternion::operator-(
        const DualQuaternion& other) const {
        return DualQuaternion(real_ - other.real_, dual_ - other.dual_);
    }

    DualQuaternion& DualQuaternion::operator-=(const DualQuaternion& other) {
        return (*this = (*this) - other);
    }

    DualQuaternion DualQuaternion::operator*(double scale) const {
        return DualQuaternion(real_ * scale, dual_ * scale);
    }

    DualQuaternion DualQuaternion::operator*=(double scale) {
        return (*this = (*this) * scale);
    }

    DualQuaternion DualQuaternion::operator/(double scale) const {
        return DualQuaternion(real_ / scale, dual_ / scale);
    }

    DualQuaternion& DualQuaternion::operator/=(double scale) {
        return (*this = (*this) / scale);
    }

    DualQuaternion DualQuaternion::operator*(
        const DualQuaternion& other) const {
        return DualQuaternion(real_ * other.real_,
                              real_ * other.dual_ + dual_ * other.real_);
    }

    DualQuaternion& DualQuaternion::operator*=(DualQuaternion& other) {
        return (*this = (*this) * other);
    }

    DualQuaternion DualQuaternion::conj() const {
        return DualQuaternion(real_.conj(), dual_.conj());
    }

    DualQuaternion DualQuaternion::inv() const {

        if (equals((*this).magnitude(), 1.0)) {
            return (*this).conj();
        }
        return DualQuaternion(real_.inv(),
                              (real_.inv() * dual_ * real_.inv()) * -1.0);
    }

    DualQuaternion& DualQuaternion::normalize() {
        double mag = (*this).magnitude();
        if (equals(mag, 0.0)) {
            return *this;
        }

        return (*this = (*this) / mag);
    }

    double DualQuaternion::dot(DualQuaternion& other) const {
        return real_.dot(other.real_);
    }

    double DualQuaternion::magnitude() const {
        DualQuaternion dq = DualQuaternion(*this);
        return sqrt(dq.dot(dq));
    }

    DualQuaternion DualQuaternion::sclerp(const DualQuaternion& dq1,
                                          const DualQuaternion& dq2, double t) {
        if (t <= 0.0) {
            return dq1;
        }

        if (t >= 1.0) {
            return dq2;
        }

        DualQuaternion dqa = DualQuaternion(dq1);
        DualQuaternion dqb = DualQuaternion(dq2);

        // shortest path
        if (dqa.real_.dot(dqb.real_) < 0.0) {
            dqb = dqb * (-1.0);
        }

        DualQuaternion dq = dqa.inv() * dqb;
        return dqa * DualQuaternion::pow(dq, t);
    }

    DualQuaternion DualQuaternion::log(DualQuaternion& dq) {
        Quaternion real   = dq.real_;
        Quaternion dual   = dq.dual_;
        double realLength = real.magnitude();

        cv::Vec4d realComp = real.getComponents();
        cv::Vec4d dualComp = dual.getComponents();
        cv::Vec3d rVec     = cv::Vec3d(realComp[1], realComp[2], realComp[3]);
        cv::Vec3d dVec     = cv::Vec3d(dualComp[1], dualComp[2], dualComp[3]);
        double vecLen      = cv::norm(rVec);
        double rs          = realComp[0];
        double ds          = dualComp[0];

        if (equals(dual.magnitude(), 0.0)) {
            return DualQuaternion(Quaternion::log(real), dual);
        }

        double r = real.dot(dual) / real.dot(real);

        cv::Vec3d vec = (dVec * vecLen - rVec * rVec.dot(dVec) / vecLen) /
                        rVec.dot(rVec) * atan2(vecLen, rs);
        vec -= cv::normalize(rVec) *
               (ds * realLength - rs * real.dot(dual) / realLength) /
               ((real.dot(real) * sqrt(1 - (rs * rs / real.dot(real)))));

        return DualQuaternion(Quaternion::log(real), Quaternion(r, vec));
    }

    DualQuaternion DualQuaternion::exp(DualQuaternion& dq) {
        Quaternion real = dq.real_;
        Quaternion dual = dq.dual_;

        cv::Vec4d realComp = real.getComponents();
        cv::Vec4d dualComp = dual.getComponents();
        cv::Vec3d rVec     = cv::Vec3d(realComp[1], realComp[2], realComp[3]);
        cv::Vec3d dVec     = cv::Vec3d(dualComp[1], dualComp[2], dualComp[3]);
        double vecLen      = cv::norm(rVec);
        double rs          = realComp[0];
        double ds          = dualComp[0];

        if (equals(dual.magnitude(), 0.0)) {
            return DualQuaternion(Quaternion::exp(real), dual);
        }

        double r = (ds * cos(vecLen) - sin(vecLen) * rVec.dot(dVec) / vecLen);

        cv::Vec3d vec = (dVec * vecLen - rVec * rVec.dot(dVec) / vecLen) /
                        rVec.dot(rVec) * sin(vecLen);
        vec += cv::normalize(rVec) *
               (ds * sin(vecLen) + rVec.dot(dVec) / vecLen * cos(vecLen));

        return DualQuaternion(Quaternion::exp(real),
                              Quaternion(r, vec) * cv::exp(rs));
    }

    DualQuaternion DualQuaternion::pow(DualQuaternion& dq, double t) {
        DualQuaternion l = log(dq) * t;
        return exp(l);
    }

    DualQuaternion DualQuaternion::dlb(const std::vector<DualQuaternion>& dqs) {
        DualQuaternion dlb = dqs[0];

        for (std::size_t i = 1; i < dqs.size(); ++i) {
            dlb += dqs[i];
        }
        dlb /= static_cast<double>(dqs.size());
        dlb.normalize();
        return dlb;
    }

    DualQuaternion DualQuaternion::dlb(const std::vector<DualQuaternion>& dqs,
                                       const std::vector<double>& weights) {
        DualQuaternion dlb = dqs[0] * weights[0];

        for (std::size_t i = 1; i < dqs.size(); ++i) {
            dlb += dqs[i] * weights[i];
        }
        dlb.normalize();
        return dlb;
    }

    Quaternion DualQuaternion::initRealPart(const Quaternion& real) {
        return Quaternion(real).normalize();
    }

    Quaternion DualQuaternion::initDualPart(const cv::Vec3d& vec) {
        return (Quaternion(0.0, vec) * real_) * 0.5;
    }

} // namespace math
} // namespace ret
