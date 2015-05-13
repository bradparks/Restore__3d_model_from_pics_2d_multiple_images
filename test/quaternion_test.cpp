/*
 * Copyright (c) 2015, Kai Wolf
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

#include <gtest/gtest.h>
#include <restore/math.hpp>

using namespace ret::math;

TEST(QuaternionTest, CheckEquality) {
    Quaternion q1(1.0, cv::Vec3d(0.0, 0.0, 0.0));
    Quaternion q2(1.0, 0.0, 0.0, 0.0);
    Quaternion q3(1.0, 0.5, 0.0, 0.0);
    Quaternion q4(q1);
    Quaternion q5 = q1;

    ASSERT_TRUE(q1 == q2);
    ASSERT_FALSE(q2 == q3);
    ASSERT_TRUE(q1 == q4);
    ASSERT_TRUE(q1 == q5);
}

TEST(QuaternionTest, CheckInequality) {
    Quaternion q1(1.0, cv::Vec3d(0.0, 0.0, 0.0));
    Quaternion q2(1.0, 0.0, 0.0, 0.0);
    Quaternion q3(1.0, 0.5, 0.0, 0.0);

    ASSERT_FALSE(q1 != q2);
    ASSERT_TRUE(q1 != q3);
}

TEST(QuaternionTest, CheckIdentity) {
    Quaternion q;
    cv::Vec4d qC = q.getComponents();

    ASSERT_DOUBLE_EQ(1.0, qC[0]);
    ASSERT_DOUBLE_EQ(0.0, qC[1]);
    ASSERT_DOUBLE_EQ(0.0, qC[2]);
    ASSERT_DOUBLE_EQ(0.0, qC[3]);

    Quaternion qInv = q.inv();
    ASSERT_TRUE(q == qInv);

    cv::Vec3d eulerAngles = q.toEulerAngles();
    ASSERT_TRUE(eulerAngles == cv::Vec3d(0.0, 0.0, 0.0));
}

TEST(QuaternionTest, CheckMagnitude) {
    Quaternion q(0.5, -0.5, 0.5, 0.0);
    ASSERT_DOUBLE_EQ(sqrt(0.75), q.magnitude());
}

TEST(QuaternionTest, CheckNormalizedMagnitude) {
    // reference orientation
    Quaternion q1(1, 0, 0, 0);
    // rotate by 90 degrees about y axis
    Quaternion q2(0.70710678118, 0, 0.70710678118, 0);
    // rotate by 180 degrees about y axis
    Quaternion q3(0, 0, 1, 0);
    // rotate by 270 degrees about y axis
    Quaternion q4(0.70710678118, 0, -0.70710678118, 0);

    ASSERT_NEAR(1.0, q1.magnitude(), 0.00000001);
    ASSERT_NEAR(1.0, q2.magnitude(), 0.00000001);
    ASSERT_NEAR(1.0, q3.magnitude(), 0.00000001);
    ASSERT_NEAR(1.0, q4.magnitude(), 0.00000001);
}

TEST(QuaternionTest, CheckNormalize) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    q1 = q1.normalize();

    cv::Vec4d qC = q1.getComponents();
    ASSERT_NEAR(0.70710678118, qC[0], 0.00000001);
    ASSERT_NEAR(0.0, qC[1], 0.00000001);
    ASSERT_NEAR(0.70710678118, qC[2], 0.00000001);
    ASSERT_NEAR(0.0, qC[3], 0.00000001);
}

TEST(QuaternionTest, CheckNormalize2) {
    double angle = 2 * M_PI * (static_cast<double>(rand()) / (RAND_MAX));
    double x = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double y = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double z = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;

    Quaternion q1 = Quaternion::fromAxisAngle(cv::Vec3d(x, y, z), angle);
    q1 = q1.normalize();
    ASSERT_NEAR(1.0, q1.magnitude(), 00000001);
}

TEST(QuaternionTest, CheckDivisionRing) {
    double angle = 2 * M_PI * (static_cast<double>(rand()) / (RAND_MAX)) - M_PI;
    double x = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double y = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double z = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;

    Quaternion q1 = Quaternion::fromAxisAngle(cv::Vec3d(x, y, z), angle);
    q1 = q1.normalize();

    Quaternion qInv = q1.inv();
    Quaternion result = q1 * qInv;

    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(1, resultC[0], 0.00000001);
    ASSERT_NEAR(0, resultC[1], 0.00000001);
    ASSERT_NEAR(0, resultC[2], 0.00000001);
    ASSERT_NEAR(0, resultC[3], 0.00000001);
}

TEST(QuaternionTest, CheckInverse) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    Quaternion qInv = q1.inv();

    cv::Vec4d qInvC = qInv.getComponents();

    ASSERT_NEAR(0.5, qInvC[0], 0.00000001);
    ASSERT_NEAR(0.0, qInvC[1], 0.00000001);
    ASSERT_NEAR(-0.5, qInvC[2], 0.00000001);
    ASSERT_NEAR(0.0, qInvC[3], 0.00000001);
}

TEST(QuaternionTest, CheckConjugate) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    Quaternion qConj = q1.conj();

    cv::Vec4d qConjC = qConj.getComponents();

    ASSERT_NEAR(1, qConjC[0], 0.00000001);
    ASSERT_NEAR(0, qConjC[1], 0.00000001);
    ASSERT_NEAR(-1, qConjC[2], 0.00000001);
    ASSERT_NEAR(0, qConjC[3], 0.00000001);
}

TEST(QuaternionTest, CheckAddition) {
    Quaternion q1(1.0, 0.0, 0.0, 2.0);
    Quaternion q2(0.5, 3.0, 5.0, 2.0);
    Quaternion q4(q1);

    Quaternion q3 = q1 + q2;
    cv::Vec4d q3C = q3.getComponents();

    ASSERT_NEAR(1.5, q3C[0], 0.00000001);
    ASSERT_NEAR(3.0, q3C[1], 0.00000001);
    ASSERT_NEAR(5.0, q3C[2], 0.00000001);
    ASSERT_NEAR(4.0, q3C[3], 0.00000001);

    q4 += q2;

    ASSERT_TRUE(q3 == q4);
}

TEST(QuaternionTest, CheckMultiplyWithScalar) {
    double scalar = static_cast<double>(rand());
    double s = 1;
    cv::Vec3d vec(2.0, -0.5, 3.0);

    Quaternion q1(s, vec);
    Quaternion q2 = q1 * scalar;
    s *= scalar;
    vec *= scalar;

    cv::Vec4d q2C = q2.getComponents();

    ASSERT_NEAR(s, q2C[0], 0.00000001);
    ASSERT_NEAR(vec[0], q2C[1], 0.00000001);
    ASSERT_NEAR(vec[1], q2C[2], 0.00000001);
    ASSERT_NEAR(vec[2], q2C[3], 0.00000001);

    q1 *= scalar;
    cv::Vec4d q1C = q1.getComponents();

    ASSERT_NEAR(s, q1C[0], 0.00000001);
    ASSERT_NEAR(vec[0], q1C[1], 0.00000001);
    ASSERT_NEAR(vec[1], q1C[2], 0.00000001);
    ASSERT_NEAR(vec[2], q1C[3], 0.00000001);
}

TEST(QuaternionTest, CheckMultiply) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    Quaternion q2(1.0, 0.5, 0.5, 0.75);

    Quaternion result = q1 * q2;
    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(0.5, resultC[0], 0.00000001);
    ASSERT_NEAR(1.25, resultC[1], 0.00000001);
    ASSERT_NEAR(1.5, resultC[2], 0.00000001);
    ASSERT_NEAR(0.25, resultC[3], 0.00000001);

    Quaternion q3 = q1 * q1;
    cv::Vec4d q3C = q3.getComponents();

    ASSERT_NEAR(0.0, q3C[0], 0.00000001);
    ASSERT_NEAR(0.0, q3C[1], 0.00000001);
    ASSERT_NEAR(2.0, q3C[2], 0.00000001);
    ASSERT_NEAR(0.0, q3C[3], 0.00000001);
}

TEST(QuaternionTest, CheckRotateTwice) {
    double rad = deg2rad(45);
    Quaternion q1(cos(rad), 0.0, 0.0, sin(rad));
    Quaternion q2(cos(rad), 0.0, sin(rad), 0.0);
    Quaternion p(0.0, 0.0, 1.0, 0.0);

    Quaternion q = q2 * q1;
    Quaternion result = q * p * q.inv();
    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(0.0, resultC[0], 0.00000001);
    ASSERT_NEAR(0.0, resultC[1], 0.00000001);
    ASSERT_NEAR(0.0, resultC[2], 0.00000001);
    ASSERT_NEAR(1.0, resultC[3], 0.00000001);
}

TEST(QuaternionTest, CheckDot) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    Quaternion q = q1 * q1;

    double dotProd = q1.dot(q1);
    ASSERT_NEAR(q.magnitude(), dotProd, 00000001);
}

TEST(QuaternionTest, CheckDot2) {
    Quaternion q1(1.0, 0.0, 1.0, 0.0);
    Quaternion q2(1.0, 0.5, 0.5, 0.75);

    double dotProd = q1.dot(q2);

    ASSERT_NEAR(1.5, dotProd, 0.00000001);
}

TEST(QuaternionTest, CheckRotatePoint) {
    Quaternion q(1.0, 0.0, 1.0, 0.0);
    Quaternion p(0.0, 1.0, 1.0, 1.0);

    Quaternion result = q * p * q.inv();

    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(0.0, resultC[0], 0.00000001);
    ASSERT_NEAR(1.0, resultC[1], 0.00000001);
    ASSERT_NEAR(1.0, resultC[2], 0.00000001);
    ASSERT_NEAR(-1.0, resultC[3], 0.00000001);
}

TEST(QuaternionTest, CheckRotatePoint2) {
    Quaternion q(1.0, 0.0, 1.0, 0.0);
    q = q.normalize();

    Quaternion p(0.0, 1.0, 0.0, 0.0);

    Quaternion result = q * p * q.inv();

    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(0.0, resultC[0], 0.00000001);
    ASSERT_NEAR(0.0, resultC[1], 0.00000001);
    ASSERT_NEAR(0.0, resultC[2], 0.00000001);
    ASSERT_NEAR(-1.0, resultC[3], 0.00000001);
}

TEST(QuaternionTest, CheckRotatePoint3) {
    Quaternion q(1.0, 0.0, 1.0, 0.0);
    q = q.normalize();

    cv::Vec3d result = q * cv::Vec3d(1.0, 0.0, 0.0);

    ASSERT_NEAR(0.0, result[0], 0.00000001);
    ASSERT_NEAR(0.0, result[1], 0.00000001);
    ASSERT_NEAR(-1.0, result[2], 0.00000001);
}

TEST(QuaternionTest, CheckRotatePoint4) {
    Quaternion p(0.0, cv::Vec3d(0.0, 2.0, 6.0));
    Quaternion q =
        Quaternion::fromAxisAngle(cv::Vec3d(0.0, 0.0, -1.0), deg2rad(60));

    Quaternion result = q * p * q.inv();

    cv::Vec4d resultC = result.getComponents();

    ASSERT_NEAR(1.73, resultC[1], 0.01);
    ASSERT_NEAR(1.0, resultC[2], 0.01);
    ASSERT_NEAR(6.0, resultC[3], 0.01);
}

TEST(QuaternionTest, CheckFromEulerAnglesToEulerAngles) {
    cv::Vec3d expected = cv::Vec3d(deg2rad(30.3), 0.0, 0.0);

    Quaternion q = Quaternion::fromEulerAngles(expected);

    cv::Vec3d actual = q.toEulerAngles();

    ASSERT_NEAR(expected[0], actual[0], 0.0000001);
    ASSERT_NEAR(expected[1], actual[1], 0.0000001);
    ASSERT_NEAR(expected[2], actual[2], 0.0000001);
}

TEST(QuaternionTest, CheckFromAxisAngle) {
    double angle = 2 * M_PI * (static_cast<double>(rand()) / (RAND_MAX)) - M_PI;

    Quaternion q = Quaternion::fromAxisAngle(cv::Vec3d(1, 0, 0), angle);

    cv::Vec3d actual = q.toEulerAngles();

    ASSERT_NEAR(angle, actual[0], 0.0000001);
    ASSERT_NEAR(0, actual[1], 0.0000001);
    ASSERT_NEAR(0, actual[2], 0.0000001);
}

TEST(QuaternionTest, CheckDifference) {
    Quaternion q0 = Quaternion::fromAxisAngle(cv::Vec3d(0.0, 0.0, 1.0), 0.0);
    Quaternion q1 = Quaternion::fromAxisAngle(cv::Vec3d(0.0, 0.0, 1.0), M_PI);

    ASSERT_TRUE(q1.conj() == q1.inv());

    Quaternion qdiff = q0 * q1.inv();

    ASSERT_TRUE(qdiff == q1.inv());

    cv::Vec3d eulerAngles = qdiff.toEulerAngles();

    ASSERT_NEAR(0.0, eulerAngles[0], 0.0000001);
    ASSERT_NEAR(0.0, eulerAngles[1], 0.0000001);
    ASSERT_NEAR(-M_PI, eulerAngles[2], 0.0000001);
}

TEST(QuaternionTest, CheckSlerp) {
    Quaternion q;
    Quaternion q1(0.70710678118, 0, 0, 0.70710678118);

    Quaternion q2 = Quaternion::slerp(q, q1, 0.0);
    Quaternion q3 = Quaternion::slerp(q, q1, 1.0);
    Quaternion q4 = Quaternion::slerp(q, q1, 2.0 / 3.0);

    ASSERT_TRUE(q == q2);
    ASSERT_TRUE(q1 == q3);

    cv::Vec4d q4c = q4.getComponents();

    ASSERT_NEAR(0.866025, q4c[0], 0.000001);
    ASSERT_NEAR(0.0, q4c[1], 0.000001);
    ASSERT_NEAR(0.0, q4c[2], 0.0000001);
    ASSERT_NEAR(0.5, q4c[3], 0.000001);
}

TEST(QuaternionTest, CheckExpLog) {
    Quaternion q;
    Quaternion q1 = Quaternion::log(q);
    Quaternion q2 = Quaternion::exp(q1);

    Quaternion q3 = Quaternion::exp(q);
    Quaternion q4 = Quaternion::log(q3);

    ASSERT_TRUE(q == q2);
    ASSERT_TRUE(q == q4);
    ASSERT_TRUE(q2 == q4);
}

TEST(QuaternionTest, CheckExpLog2) {
    Quaternion q(1.0, 0.0, 0.0, 1.0);
    q = q.normalize();

    Quaternion q1 = Quaternion::log(q);
    Quaternion q2 = Quaternion::exp(q1);

    Quaternion q3 = Quaternion::exp(q);
    Quaternion q4 = Quaternion::log(q3);

    cv::Vec4d c = q.getComponents();
    cv::Vec4d c2 = q2.getComponents();
    cv::Vec4d c4 = q4.getComponents();

    // ASSERT_TRUE(q == q2);
    ASSERT_NEAR(c[0], c2[0], 0.000001);
    ASSERT_NEAR(c[1], c2[1], 0.000001);
    ASSERT_NEAR(c[2], c2[2], 0.000001);
    ASSERT_NEAR(c[3], c2[3], 0.000001);

    // ASSERT_TRUE(q == q4);
    ASSERT_NEAR(c[0], c4[0], 0.000001);
    ASSERT_NEAR(c[1], c4[1], 0.000001);
    ASSERT_NEAR(c[2], c4[2], 0.000001);
    ASSERT_NEAR(c[3], c4[3], 0.000001);

    // ASSERT_TRUE(q2 == q4);
    ASSERT_NEAR(c2[0], c4[0], 0.000001);
    ASSERT_NEAR(c2[1], c4[1], 0.000001);
    ASSERT_NEAR(c2[2], c4[2], 0.000001);
    ASSERT_NEAR(c2[3], c4[3], 0.000001);
}

TEST(QuaternionTest, CheckExpLog5) {
    double angle = 2 * M_PI * (static_cast<double>(rand()) / (RAND_MAX));
    double x = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double y = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;
    double z = 2 * (static_cast<double>(rand()) / (RAND_MAX)) - 1;

    Quaternion q = Quaternion::fromAxisAngle(cv::Vec3d(x, y, z), angle);

    Quaternion q1 = Quaternion::log(q);
    Quaternion q2 = Quaternion::exp(q1);

    Quaternion q3 = Quaternion::exp(q);
    Quaternion q4 = Quaternion::log(q3);

    cv::Vec4d c = q.getComponents();
    cv::Vec4d c2 = q2.getComponents();
    cv::Vec4d c4 = q4.getComponents();

    // ASSERT_TRUE(q == q2);
    ASSERT_NEAR(c[0], c2[0], 0.000001);
    ASSERT_NEAR(c[1], c2[1], 0.000001);
    ASSERT_NEAR(c[2], c2[2], 0.000001);
    ASSERT_NEAR(c[3], c2[3], 0.000001);

    // ASSERT_TRUE(q == q4);
    ASSERT_NEAR(c[0], c4[0], 0.000001);
    ASSERT_NEAR(c[1], c4[1], 0.000001);
    ASSERT_NEAR(c[2], c4[2], 0.000001);
    ASSERT_NEAR(c[3], c4[3], 0.000001);

    // ASSERT_TRUE(q2 == q4);
    ASSERT_NEAR(c2[0], c4[0], 0.000001);
    ASSERT_NEAR(c2[1], c4[1], 0.000001);
    ASSERT_NEAR(c2[2], c4[2], 0.000001);
    ASSERT_NEAR(c2[3], c4[3], 0.000001);
}
