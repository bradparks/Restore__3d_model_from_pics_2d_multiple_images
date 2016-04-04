// Copyright (c) 2016, Kai Wolf
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

#include "gui/dataset_list_widget_item.hpp"

#include <QFileInfo>
#include <opencv2/imgproc/imgproc.hpp>

#include "common/dataset.hpp"

namespace ret {

DataSetListWidgetItem::DataSetListWidgetItem(
    QListWidget *parent, const std::shared_ptr<DataSet> dataset,
    std::string path)
    : QListWidgetItem(parent), dataset_(dataset), path_(path) {
    setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QFileInfo fi(QString::fromStdString(path));
    setText(fi.fileName());

    // set first image as icon
    cv::Mat tmp = dataset_->getCamera(0).getImage();
    cv::cvtColor(tmp, tmp, CV_BGR2RGB);
    QImage img(static_cast<uchar*>(tmp.data), tmp.cols, tmp.rows,
               static_cast<int>(tmp.step), QImage::Format_RGB888);
    setIcon(QIcon(QPixmap::fromImage(img)));
}

void DataSetListWidgetItem::setDataset(std::shared_ptr<DataSet> dataset) {
    dataset_ = dataset;
}

std::shared_ptr<DataSet> DataSetListWidgetItem::getDataset() const {
    return dataset_;
}

std::string DataSetListWidgetItem::getPath() const {
    return path_;
}

}
