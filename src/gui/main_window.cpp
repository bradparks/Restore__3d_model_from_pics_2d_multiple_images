// Copyright (c) 2015-2016, Kai Wolf
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

#include "gui/main_window.hpp"

#include <qaction.h>
#include <qfiledialog.h>
#include <qflags.h>
#include <qgridlayout.h>
#include <qkeysequence.h>
#include <qlistwidget.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qwidget.h>

#include "io/dataset_reader.hpp"
#include "gui/dataset_list_widget_item.hpp"
#include "gui/model_widget.hpp"

namespace ret {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    createInterface();
    createActions();
    createMenu();
    createToolBar();
}

void MainWindow::createInterface() {

    setWindowTitle(tr("Restore"));

    // QObjects include ownership semantics, QMainwindow deletes all children,
    // when its parent is deleted
    auto central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    auto grid_layout = new QGridLayout(central_widget);

    dataset_widget_ = new QListWidget(central_widget);
    dataset_widget_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    grid_layout->addWidget(dataset_widget_, 0, 0, Qt::AlignLeft);

    model_widget_ = new ModelWidget(central_widget);
    model_widget_->setMinimumSize(320, 240);
    grid_layout->addWidget(model_widget_, 0, 1);

    scan_button_ = new QPushButton(tr("Scan"), central_widget);
    grid_layout->addWidget(scan_button_, 1, 0);
}

void MainWindow::createActions() {

    open_dataset_action_ = new QAction(tr("Datensatz öffnen..."), this);
    open_dataset_action_->setShortcut(QKeySequence::Open);
    open_dataset_action_->setStatusTip(
        tr("Öffnet einen zuvor verwendeten Datensatz"));

    exit_action_ = menuBar()->addAction(tr("&Beenden"));
    exit_action_->setShortcut(QKeySequence::Close);
    connect(exit_action_, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenu() {

    file_menu_ = menuBar()->addMenu(tr("&Datei"));
    file_menu_->addAction(open_dataset_action_);
    file_menu_->addAction(exit_action_);
    connect(open_dataset_action_, &QAction::triggered, this,
            &MainWindow::openDataset);

    view_menu_ = menuBar()->addMenu(tr("&Ansicht"));

    settings_menu_ = menuBar()->addMenu(tr("&Einstellungen"));

    export_menu_ = menuBar()->addMenu(tr("Export"));
}

void MainWindow::createToolBar() {}

void MainWindow::openDataset() {

    auto dir = QFileDialog::getExistingDirectory(
        this, tr("Datensatz auswählen"), QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    const auto path = dir.toStdString();
    io::DataSetReader dsr(path);
    auto ds = dsr.load(36);
    auto ds_item = new DataSetListWidgetItem(dataset_widget_, ds, path);
    dataset_widget_->addItem(ds_item);
}

}  // namespace ret
