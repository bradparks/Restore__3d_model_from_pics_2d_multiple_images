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
#include <iterator>

// header files of other libraries
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

// header files of project libraries
#include "common/dataset.hpp"
#include "io/dataset_reader.hpp"

namespace fs = boost::filesystem;
using ret::DataSet;
using ret::io::DataSetReader;

DataSetReader::DataSetReader(const std::string& directory)
    : directory_(directory) {}

DataSet DataSetReader::load(const std::size_t numImages) const {

    cv::Mat K, dist;
    if (fs::exists(directory_ + "/K.xml")) {
        K = loadMatrixFromFile("/K.xml", "K_matrix");
    }

    if (fs::exists(directory_ + "/dist.xml")) {
        dist = loadMatrixFromFile("/dist.xml", "dist_coeff");
    }
    auto projMats = loadProjectionMatrices(numImages, "/viff.xml");

    fs::path dir(directory_);
    DataSet ds;
    if (fs::exists(dir) && fs::is_directory(dir)) {
        // sorting paths, since directory listing is not sorted under Linux
        using paths = std::vector<fs::path>;
        paths stored_paths;
        std::copy(fs::directory_iterator(dir), fs::directory_iterator(),
                  std::back_inserter(stored_paths));
        std::sort(stored_paths.begin(), stored_paths.end());

        std::size_t camIdx = 0;
        for (paths::const_iterator iter(stored_paths.begin());
             iter != stored_paths.end(); ++iter) {
            if (fs::is_regular_file(*iter) && iter->extension() == ".png") {
                cv::Mat P = projMats[camIdx++];
                cv::Mat R, K2, t;
                cv::decomposeProjectionMatrix(P, K2, R, t);

                Camera cam(cv::imread(iter->string()));
                cam.setCalibrationMatrix(K.empty() ? K2 : K);
                if (!dist.empty()) cam.setDistortionCoeffs(dist);
                cam.setProjectionMatrix(P);
                cam.setRotationMatrix(R);
                cam.setTranslationVector(t);

                ds.addCamera(cam);
            }
        }
    }

    assert(ds.size() == numImages);
    return ds;
}

cv::Mat DataSetReader::loadMatrixFromFile(const std::string& filename,
                                          const std::string& matname) const {
    cv::Mat M;
    cv::FileStorage Fs(directory_ + filename, cv::FileStorage::READ);
    Fs[matname] >> M;
    return M;
}

std::vector<cv::Mat> DataSetReader::loadProjectionMatrices(
    const std::size_t numMatrices, const std::string& filename) const {

    std::vector<cv::Mat> projMats;
    boost::format fmt("viff%03d_matrix");
    cv::FileStorage fs(directory_ + filename, cv::FileStorage::READ);
    for (std::size_t idx = 0; idx < numMatrices; ++idx) {
        cv::Mat P;
        fs[(fmt % idx).str()] >> P;
        projMats.push_back(P);
    }

    return projMats;
}
