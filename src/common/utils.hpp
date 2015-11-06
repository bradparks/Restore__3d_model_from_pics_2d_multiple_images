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

#ifndef RESTORE_COMMON_UTILS_HPP
#define RESTORE_COMMON_UTILS_HPP

// C system files
// none

// C++ system files
#include <cmath>
#include <cstddef>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
// none

namespace ret {

    // std::make_unique implementation taken from N3656 STL
    template <class T>
    struct _Unique_if {
        typedef std::unique_ptr<T> _Single_object;
    };

    template <class T>
    struct _Unique_if<T[]> {
        typedef std::unique_ptr<T[]> _Unknown_bound;
    };

    template <class T, size_t N>
    struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template <class T, class... Args>
    typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <class T>
    typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
        typedef typename std::remove_extent<T>::type U;
        return std::unique_ptr<T>(new U[n]());
    }

    template <class T, class... Args>
    typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;

    template <typename T>
    inline bool isZero(T decimal) {
        auto EPSILON = 0.000001;
        return std::abs(decimal) < EPSILON;
    }

    template <typename T>
    inline T median(std::vector<T>& v) {
        std::size_t n = v.size() / 2;
        std::nth_element(v.begin(), v.begin() + n, v.end());
        return v[n];
    }

    inline std::string getImgType(int img_type_int) {
        // 7 base types, with five channel options each (none or C1, ..., C4)
        // clang-format off
        std::map<int, std::string> cv_types{
            {CV_8U,    "CV_8U"},    {CV_8UC1,  "CV_8UC1"},  {CV_8UC2,  "CV_8UC2"},
            {CV_8UC3,  "CV_8UC3"},  {CV_8UC4,  "CV_8UC4"},  {CV_8S,    "CV_8S"},
            {CV_8SC1,  "CV_8SC1"},  {CV_8SC2,  "CV_8SC2"},  {CV_8SC3,  "CV_8SC3"}, 
            {CV_8SC4,  "CV_8SC4"},  {CV_16U,   "CV_16U"},   {CV_16UC1, "CV_16UC1"},
            {CV_16UC2, "CV_16UC2"}, {CV_16UC3, "CV_16UC3"}, {CV_16UC4, "CV_16UC4"},
            {CV_16S,   "CV_16S"},   {CV_16SC1, "CV_16SC1"}, {CV_16SC2, "CV_16SC2"},
            {CV_16SC3, "CV_16SC3"}, {CV_16SC4, "CV_16SC4"}, {CV_32S,   "CV_32S"},
            {CV_32SC1, "CV_32SC1"}, {CV_32SC2, "CV_32SC2"}, {CV_32SC3, "CV_32SC3"},
            {CV_32SC4, "CV_32SC4"}, {CV_32F,   "CV_32F"},   {CV_32FC1, "CV_32FC1"},
            {CV_32FC2, "CV_32FC2"}, {CV_32FC3, "CV_32FC3"}, {CV_32FC4, "CV_32FC4"},
            {CV_64F,   "CV_64F"},   {CV_64FC1, "CV_64FC1"}, {CV_64FC2, "CV_64FC2"},
            {CV_64FC3, "CV_64FC3"}, {CV_64FC4, "CV_64FC4"},
        };
        // clang-format on

        auto type = cv_types.find(imgTypeInt);
        return type == cv_types.end() ? "unknown image type" : type->second;
    }
}

#endif
