/*
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
*/
/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "configuration.h"
#ifdef USE_R
#include <RInside.h>
#endif
#include "qtdensity.h"
#include "fusionpair.h"
#include "runtimeinfo.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
#ifdef USE_R
    MainWindow(RInside& R, RuntimeInfo *run, QWidget *parent = 0);
#endif
    MainWindow(RuntimeInfo *run, QWidget *parent);

#ifdef USE_R

    void SetR(RInside& R){ this->_R = R; }
    RInside& GetR(){ return this->_R; }
    RInside& _R;
    void showQtDencityWithR();
#else
    //
#endif

    void SetRuntime(RuntimeInfo *_run) { _run = _run; }

    double m_color_ratio = 1000.0;

private:
    void setupMatrix();
    void populateScene();
    void populateRandomScene(int nrow, int ncol, int width, int height, QColor startColor, QColor endColor);
    void populateFusionPPIScene(int nrow, int ncol, int width, int height, QColor startColor, QColor endColor);
    void createMenus();
    void createActions();

    QGraphicsScene *_scene1;
    QGraphicsScene *_scene2;
    QGraphicsScene *_scene3;
    QGraphicsScene *_scene4;
    QSplitter *_h1Splitter;
    QSplitter *_h2Splitter;

#ifdef USE_R
    QtDensity* _qtdensity;
#endif
    //FusionPair _fp;


    RuntimeInfo* _run;      //  refer to RuntimeInfo of robot


    // MenuBar Related
    QHBoxLayout *layout1;
    QHBoxLayout *layout2;
    QAction* newAct;

    // MenuBar Related
private slots:
    void newFile();
};

#endif   // MAINWINDOW_H
