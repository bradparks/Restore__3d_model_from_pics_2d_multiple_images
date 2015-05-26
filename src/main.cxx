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

// C system files
// none

// C++ system files
#include <iostream>

// header files of other libraries
#include <opencv2/highgui/highgui.hpp>

// header files of project libraries
#include <restore/io.hpp>
#include <restore/filtering.hpp>
#include "io/assets_path.hpp"

using ret::DataSet;
using ret::io::DataSetReader;
using ret::filtering::Segmentation;

int main() {

    DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
    DataSet ds = dsr.load(36);

    for (const auto& cam : ds.getCameras()) {
        cv::Point3d pt = cam.getCenter();
        cv::Vec3d vec(pt.x, pt.y, pt.z);
        std::cout << cv::norm(pt) << std::endl;
    }

    std::cout << "ds: " << ds.size() << std::endl;
    cv::imshow("Original", ds.getCamera(0).getImage());
    cv::Mat Binary = Segmentation::binarize(ds.getCamera(0).getImage(),
                                            cv::Scalar(0, 0, 40));
    cv::imshow("Binary", Binary);
    cv::waitKey();

    return 0;
}
