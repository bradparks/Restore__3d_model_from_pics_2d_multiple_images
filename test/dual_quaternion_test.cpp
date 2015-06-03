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
// C system files
// none

// C++ system files
#include <algorithm>

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
#include "math/dual_quaternion.hpp"
#include "math/math_utils.hpp"

using namespace ret::math;

TEST(DualQuaternionTest, CheckDefaultConstruction) {
    DualQuaternion dq;
    Quaternion q;
    cv::Vec3d origin(0.0, 0.0, 0.0);

    Quaternion rot = dq.getRotation();
    cv::Vec3d trans = dq.getTranslation();

    ASSERT_TRUE(q == rot);
    ASSERT_TRUE(trans == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat P = dq.getPose();
    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction) {
    Quaternion q;
    cv::Vec3d origin(0.0, 0.0, 0.0);

    DualQuaternion dq(q, Quaternion(0.0, origin));

    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat P = dq.getPose();
    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction2) {
    Quaternion q;
    cv::Vec3d origin = cv::Vec3d(0.0, 0.0, 0.0);

    DualQuaternion dq(q, origin);

    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat P = dq.getPose();
    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction3) {
    Quaternion q;
    cv::Vec3d rod = cv::Vec3d(0.0, 0.0, 0.0);
    cv::Vec3d origin = cv::Vec3d(0.0, 0.0, 0.0);

    DualQuaternion dq(rod, origin);

    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat P = dq.getPose();
    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);

    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction4) {
    Quaternion q;
    cv::Mat eye = cv::Mat::eye(3, 3, CV_64FC1);
    cv::Vec3d origin = cv::Vec3d(0.0, 0.0, 0.0);

    DualQuaternion dq(eye, origin);

    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat P = dq.getPose();

    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction5) {
    Quaternion q;
    cv::Vec3d axis = cv::Vec3d(0.0, 0.0, 0.0);
    double angle = 0.0;
    cv::Vec3d origin = cv::Vec3d(0.0, 0.0, 0.0);

    DualQuaternion dq(axis, angle, origin);

    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat P = dq.getPose();

    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction6) {
    DualQuaternion dq0;
    cv::Mat P = dq0.getPose();
    DualQuaternion dq1(P);

    ASSERT_TRUE(std::equal(P.begin<double>(), P.end<double>(),
                           dq1.getPose().begin<double>()));
}

TEST(DualQuaternionTest, CheckConstruction7) {
    Quaternion q0 = Quaternion::fromEulerAngles(cv::Vec3d(3.0, 2.0, 1.0));
    cv::Vec3d t0 = cv::Vec3d(10.0, 30.0, 90.0);

    DualQuaternion dq0(q0, t0);
    DualQuaternion dq;
    cv::Mat P0 = dq.getPose();

    DualQuaternion dq1(P0);
    cv::Mat P1 = dq.getPose();

    ASSERT_TRUE(
        std::equal(P0.begin<double>(), P0.end<double>(), P1.begin<double>()));
}

TEST(DualQuaternionTest, CheckTranslation) {
    cv::Vec3d origin = cv::Vec3d(1.0, 0.0, 1.0);
    DualQuaternion dq(Quaternion(), origin);

    Quaternion q;
    Quaternion r = dq.getRotation();
    cv::Vec3d t = dq.getTranslation();

    ASSERT_TRUE(q == r);
    ASSERT_TRUE(t == origin);

    cv::Mat eye4x4 = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat P = dq.getPose();

    for (auto idx : {0, 1, 2}) eye4x4.at<double>(idx, 3) = origin[idx];

    ASSERT_TRUE(std::equal(eye4x4.begin<double>(), eye4x4.end<double>(),
                           P.begin<double>()));
}

TEST(DualQuaternionTest, CheckEquality) {
    DualQuaternion dq0;
    DualQuaternion dq1(Quaternion(), cv::Vec3d(0.0, 0.0, 0.0));

    ASSERT_TRUE(dq0 == dq1);
}

TEST(DualQuaternionTest, CheckMagnitude) {
    DualQuaternion dq;

    ASSERT_EQ(1.0, dq.magnitude());
}

TEST(DualQuaternionTest, CheckAddition) {
    DualQuaternion dq;
    DualQuaternion dq1;
    DualQuaternion dq2;

    DualQuaternion dq3 = dq1 + dq2;
    dq3.normalize();

    ASSERT_TRUE(dq == dq3);
}

TEST(DualQuaternionTest, CheckRotateAroundSameAxis) {
    double totalRotation = M_PI / 2.;
    DualQuaternion dq0(cv::Vec3d(1.0, 0.0, 0.0), deg2rad(30),
                       cv::Vec3d(0.0, 0.0, 0.0));
    DualQuaternion dq1(cv::Vec3d(1.0, 0.0, 0.0), deg2rad(60),
                       cv::Vec3d(0.0, 0.0, 0.0));

    DualQuaternion dq = dq1 * dq0;

    cv::Vec3d eulerAngles = dq.getRotation().toEulerAngles();

    ASSERT_NEAR(totalRotation, eulerAngles[0], 0.0000001);
    ASSERT_NEAR(0, eulerAngles[1], 0.0000001);
    ASSERT_NEAR(0, eulerAngles[2], 0.0000001);

    dq = dq0 * dq1;

    eulerAngles = dq.getRotation().toEulerAngles();

    ASSERT_NEAR(totalRotation, eulerAngles[0], 0.0000001);
    ASSERT_NEAR(0, eulerAngles[1], 0.0000001);
    ASSERT_NEAR(0, eulerAngles[2], 0.0000001);
}

TEST(DualQuaternionTest, CheckSclerp) {
    Quaternion q0;
    Quaternion q1(1.0, 0.0, 0.0, 1.0);

    DualQuaternion dq0;
    DualQuaternion dq1(q1, cv::Vec3d(0.0, 0.0, 0.0));

    DualQuaternion dq2 = DualQuaternion::sclerp(dq0, dq1, 0.0);
    DualQuaternion dq3 = DualQuaternion::sclerp(dq0, dq1, 1.0);
    DualQuaternion dq4 = DualQuaternion::sclerp(dq0, dq1, 2.0 / 3.0);

    ASSERT_TRUE(dq0 == dq2);
    ASSERT_TRUE(dq1 == dq3);

    q1.normalize();

    Quaternion q = Quaternion::slerp(q0, q1, 2 / 3.0);
    Quaternion r = dq4.getRotation();

    cv::Vec4d qC = q.getComponents();
    cv::Vec4d rC = r.getComponents();

    ASSERT_NEAR(qC[0], rC[0], 0.000001);
    ASSERT_NEAR(qC[1], rC[1], 0.000001);
    ASSERT_NEAR(qC[2], rC[2], 0.0000001);
    ASSERT_NEAR(qC[3], rC[3], 0.000001);
}

TEST(DualQuaternionTest, CheckScalar) {
    cv::Vec3d r = cv::Vec3d(3.0, 2.0, 1.0);
    cv::Vec3d t = cv::Vec3d(10.0, 30.0, 90.0);

    Quaternion q0 = Quaternion::fromEulerAngles(r);
    Quaternion q1 = Quaternion::fromEulerAngles(r);

    DualQuaternion dq0(Quaternion(), t);
    DualQuaternion dq1(Quaternion(), t);

    DualQuaternion dq2 = dq0 / 2.0;
    DualQuaternion dq3 = dq1 * 1.0 / 2.0;

    ASSERT_TRUE(dq2 == dq3);

    dq0 /= 2;
    dq1 *= 1 / 2.;

    ASSERT_TRUE(dq0 == dq2);
    ASSERT_TRUE(dq1 == dq3);
}

TEST(DualQuaternionTest, DISABLED_CheckMatrixVsDualQuaternions) {
    cv::Vec3d r0 = cv::Vec3d(3.0, 2.0, 1.0);
    cv::Vec3d r1 = cv::Vec3d(3.0, 2.0, -1.0);
    cv::Vec3d r2 = cv::Vec3d(3.0, 1.5, 2.0);

    cv::Vec3d t0 = cv::Vec3d(10.0, 30.0, 90.0);
    cv::Vec3d t1 = cv::Vec3d(30.0, 40.0, 190.0);
    cv::Vec3d t2 = cv::Vec3d(5.0, 20.0, 66.0);

    Quaternion q0 = Quaternion::fromEulerAngles(r0);
    Quaternion q1 = Quaternion::fromEulerAngles(r1);
    Quaternion q2 = Quaternion::fromEulerAngles(r2);

    DualQuaternion dq0(q0, t0);
    DualQuaternion dq1(q1, t1);
    DualQuaternion dq2(q2, t2);

    DualQuaternion dq = dq0 * dq1 * dq2;

    cv::Mat P = dq.getPose();

    cv::Mat m0 = q0.toRotMatrix();
    m0.at<double>(0, 3) = t0[0];
    m0.at<double>(1, 3) = t0[1];
    m0.at<double>(2, 3) = t0[2];

    cv::Mat m1 = q1.toRotMatrix();
    m1.at<double>(0, 3) = t1[0];
    m1.at<double>(1, 3) = t1[1];
    m1.at<double>(2, 3) = t1[2];

    cv::Mat m2 = q2.toRotMatrix();
    m2.at<double>(0, 3) = t2[0];
    m2.at<double>(1, 3) = t2[1];
    m2.at<double>(2, 3) = t2[2];

    cv::Mat m = m0 * m1 * m2;

    auto it2 = P.begin<double>();
    for (auto it = m.begin<double>(); it < m.end<double>(); it++) {
        ASSERT_NEAR(*it, *it2, 0.00001);
        it2++;
    }
}

TEST(DualQuaternionTest, CheckExpLog) {
    DualQuaternion dq;
    DualQuaternion dq1 = DualQuaternion::log(dq);
    DualQuaternion dq2 = DualQuaternion::exp(dq1);

    DualQuaternion dq3 = DualQuaternion::exp(dq);
    DualQuaternion dq4 = DualQuaternion::log(dq3);

    cv::Vec4d c = dq.getRotation().getComponents();
    cv::Vec4d c2 = dq2.getRotation().getComponents();
    cv::Vec4d c4 = dq4.getRotation().getComponents();

    for (auto idx : {0, 1, 2, 3}) {
        ASSERT_NEAR(c[idx], c2[idx], 0.000001);
        ASSERT_NEAR(c[idx], c4[idx], 0.000001);
    }

    cv::Vec3d t = dq.getTranslation();
    cv::Vec3d t2 = dq2.getTranslation();
    cv::Vec3d t4 = dq4.getTranslation();

    for (auto idx : {0, 1, 2}) {
        ASSERT_NEAR(t[idx], t2[idx], 0.000001);
        ASSERT_NEAR(t[idx], t4[idx], 0.000001);
    }
}

TEST(DualQuaternionTest, CheckExpLog2) {
    Quaternion q(1.0, 0.0, 0.0, 1.0);
    q.normalize();
    cv::Vec3d t = cv::Vec3d(20, 3, 1);
    DualQuaternion dq(q, t);

    DualQuaternion dq1 = DualQuaternion::log(dq);
    DualQuaternion dq2 = DualQuaternion::exp(dq1);

    DualQuaternion dq3 = DualQuaternion::exp(dq);
    DualQuaternion dq4 = DualQuaternion::log(dq3);

    cv::Vec4d c = q.getComponents();
    cv::Vec4d c2 = dq2.getRotation().getComponents();
    cv::Vec4d c4 = dq4.getRotation().getComponents();

    for (auto idx : {0, 1, 2, 3}) {
        ASSERT_NEAR(c[idx], c2[idx], 0.000001);
        ASSERT_NEAR(c[idx], c4[idx], 0.000001);
    }

    cv::Vec3d t2 = dq2.getTranslation();
    cv::Vec3d t4 = dq4.getTranslation();

    for (auto idx : {0, 1, 2}) {
        ASSERT_NEAR(t[idx], t2[idx], 0.000001);
        ASSERT_NEAR(t[idx], t4[idx], 0.000001);
    }
}

TEST(DualQuaternionTest, DISABLED_CheckExpLog3) {
    double angle = 2 * M_PI * ((double) rand() / RAND_MAX);
    double x = 2.0 * (static_cast<double>(rand()) / RAND_MAX) - 1;
    double y = 2.0 * (static_cast<double>(rand()) / RAND_MAX) - 1;
    double z = 2.0 * (static_cast<double>(rand()) / RAND_MAX) - 1;

    cv::Vec3d t = cv::Vec3d(static_cast<double>(rand()) - RAND_MAX / 2,
                            static_cast<double>(rand()) - RAND_MAX / 2,
                            static_cast<double>(rand()) - RAND_MAX / 2);

    Quaternion q = Quaternion::fromAxisAngle(cv::Vec3d(x, y, z), angle);
    DualQuaternion dq(q, t);

    DualQuaternion dq1 = DualQuaternion::log(dq);
    DualQuaternion dq2 = DualQuaternion::exp(dq1);

    DualQuaternion dq3 = DualQuaternion::exp(dq);
    DualQuaternion dq4 = DualQuaternion::log(dq3);

    cv::Vec4d c = dq.getRotation().getComponents();
    cv::Vec4d c2 = dq2.getRotation().getComponents();
    cv::Vec4d c4 = dq4.getRotation().getComponents();

    for (auto idx : {0, 1, 2, 3}) {
        ASSERT_NEAR(c[idx], c2[idx], 0.000001);
        ASSERT_NEAR(c[idx], c4[idx], 0.000001);
    }

    cv::Vec3d t2 = dq2.getTranslation();
    cv::Vec3d t4 = dq4.getTranslation();

    for (auto idx : {0, 1, 2}) {
        ASSERT_NEAR(t[idx], t2[idx], 0.000001);
        ASSERT_NEAR(t[idx], t4[idx], 0.000001);
    }
}
