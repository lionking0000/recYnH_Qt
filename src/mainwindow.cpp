/*#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/

/****************************************************************************
**
** Copyright (C) 2017 Luis Serrano group
**
** Developed by Jae-Seong Yang
**
****************************************************************************/

#include "chip.h"
#include "title.h"
#include "robot.h"
#include "fusionpair.h"

#include "mainwindow.h"
#include "view.h"
#include "gradientpalette.h"

#include <QHBoxLayout>
#include <QSplitter>

#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>


void run_fusion_test(){
    // /Volumes/jyang/work/Mireia/src/run.2016-12-22_MiSeq.sh
    // time ./kmer FusionPairSearch ../data/roth2016_control_set_plus_control.sorted.fa ../2016-12-22_MiSeq/17543_S1_L001_R1_001.fastq.gz ../2016-12-22_MiSeq/17543_S1_L001_R2_001.fastq.gz ./output/17543.txt

}

#ifdef USE_R
MainWindow::MainWindow(RInside & R, RuntimeInfo *run, QWidget *parent ) : _R(R), _run(run), QWidget(parent)
#else
MainWindow::MainWindow(RuntimeInfo *run, QWidget *parent ) : _run(run), QWidget(parent)
#endif
{
    //populateScene();
    m_color_ratio = 1000.0;

    QColor startColor(_run->_config.start_color_red, _run->_config.start_color_green, _run->_config.start_color_blue);
    QColor endColor(_run->_config.end_color_red, _run->_config.end_color_green, _run->_config.end_color_blue);


    if (false){ /* 2 x 2 sites */
    _h1Splitter = new QSplitter;
    _h2Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(_h1Splitter);
    vSplitter->addWidget(_h2Splitter);

    View *view = new View("Top left view");
    view->view()->setScene(_scene1);
    _h1Splitter->addWidget(view);

    view = new View("Top right view");
    view->view()->setScene(_scene1);
    _h1Splitter->addWidget(view);

    view = new View("Bottom left view");
    view->view()->setScene(_scene1);
    _h2Splitter->addWidget(view);

    view = new View("Bottom right view");
    view->view()->setScene(_scene1);
    _h2Splitter->addWidget(view);

    layout1 = new QHBoxLayout;
    layout1->addWidget(vSplitter);
    setLayout(layout1);
    }
    if (true){
        /* 1 x 1 sites */
        /*   run.2016-12-22_MiSeq.sh
        run_fusion_test
        */
        populateFusionPPIScene(_run->_config.nrow, _run->_config.ncol, _run->_config.cell_height, _run->_config.cell_width, startColor, endColor);
        View *view = new View("Fusion PPI View - 2016-12-22_MiSeq");
        view->view()->setScene(_scene1);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget( view );
        setLayout( layout );

        /*   default cells
        populateRandomScene(_config.nrow, _config.ncol, _config.cell_height, _config.cell_width, startColor, endColor);
        View *view = new View("Fusion PPI View");
        view->view()->setScene(scene);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(view);
        setLayout(layout); */
    }
    if (false){
        /* 1 x 2 sites */
        /*   run.2016-12-22_MiSeq.sh
        run_fusion_test
        */
        //QSplitter *vSplitter = new QSplitter;
        //vSplitter->setOrientation(Qt::Vertical);

        populateFusionPPIScene(_run->_config.nrow, _run->_config.ncol, _run->_config.cell_height, _run->_config.cell_width, startColor, endColor);


        //vSplitter->addWidget(view);

        //QHBoxLayout *layout = new QHBoxLayout;
        //layout->addWidget( vSplitter );
        //setLayout( layout );

        _h1Splitter = new QSplitter;
        _h2Splitter = new QSplitter;

        QSplitter *vSplitter = new QSplitter;
        vSplitter->setOrientation(Qt::Vertical);
        vSplitter->addWidget(_h1Splitter);
        vSplitter->addWidget(_h2Splitter);

        View *view = new View("Fusion PPI View - 2016-12-22_MiSeq");
        view->view()->setScene(_scene1);
        _h1Splitter->addWidget(view);

        //m_svg = new QSvgWidget();
        //runRandomDataCmd();         // also calls plot()

        view = new View("Bottom left view");
        view->view()->setScene(_scene1);
        _h2Splitter->addWidget(view);

        view = new View("Bottom right view");
        view->view()->setScene(_scene1);
        _h2Splitter->addWidget(view);

        layout2 = new QHBoxLayout;
        layout2->addWidget(vSplitter);
        setLayout(layout2);
    }

    createActions();
    createMenus();
    setWindowTitle(tr("Fusion MAP"));
}

#ifdef USE_R
void MainWindow::showQtDencityWithR(){
    //view = new View("Top right view");
    Rcpp::IntegerVector mylist(this->_config.nrow*this->_config.ncol);
    int item = 0;
    for (int i = 0; i < this->_config.nrow; i ++) {
        for (int j = 0; j < this->_config.ncol; j ++) {
            mylist[item] = _fp.output[ pair<int,int>(i,j) ];
            std::cout << mylist[item] << std::endl;
            item++;
        }
    }
    //view->view()->setScene(_scene2);
    //_h1Splitter->addWidget(qd);

    QtDensity* qd = new QtDensity(_R, mylist);
    //qd->show();
}
#endif

void MainWindow::populateFusionPPIScene(int nrow, int ncol, int width, int height, QColor startColor, QColor endColor)
{
    //return;
    // /Volumes/jyang/work/Mireia/src/run.2016-12-22_MiSeq.sh
    // time ./kmer FusionPairSearch ../data/roth2016_control_set_plus_control.sorted.fa ../2016-12-22_MiSeq/17543_S1_L001_R1_001.fastq.gz ../2016-12-22_MiSeq/17543_S1_L001_R2_001.fastq.gz ./output/17543.txt

    int argc = 7;


    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/jyang/work/Mireia/2015-05-28/40_40.upper.fasta",
    //                 "/Volumes/jyang/work/Mireia/src/data/2015-05-28/3A2E_10337_GGAGCC_read1.fastq.gz",
    //                 "/Volumes/jyang/work/Mireia/src/data/2015-05-28/3A2E_10337_GGAGCC_read2.fastq.gz",
    //                 //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2_10336_AGGCCA_read1.fastq.gz",
    //                 //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2_10336_AGGCCA_read2.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-05-28.3A2E.txt"};

    char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/users/lserrano/jyang/work/Mireia/2015-05-28/40_40.upper.fasta",
                     //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2E_10337_GGAGCC_read1.fastq.gz",
                     //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2E_10337_GGAGCC_read2.fastq.gz",
                     "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read1.fastq.gz",
                     "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read2.fastq.gz",
                     "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-07-07.A2.txt", "125"};

    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/jyang/work/Mireia/2015-05-28/40_40.upper.fasta",
    //                 //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2E_10337_GGAGCC_read1.fastq.gz",
    //                 //"/users/lserrano/sequencing_data/Jae-Seong_Yang/2015-05-28/3A2E_10337_GGAGCC_read2.fastq.gz",
    //                 "/Volumes/users/smaurer/sequencing_data/Mireia_Garriga/2016-02-12/24_1_K_12850_CAGGCT_read1.fastq.gz",
    //                 "/Volumes/users/smaurer/sequencing_data/Mireia_Garriga/2016-02-12/24_1_K_12850_CAGGCT_read2.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2016-02-12.24_1_K.txt"};

    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/jyang/work/Mireia/data/roth2016_control_set_plus_control.sorted.fa",
    //                 "/Volumes/jyang/work/Mireia/2016-12-22_MiSeq/17543_S1_L001_R1_001.fastq.gz",
    //                 "/Volumes/jyang/work/Mireia/2016-12-22_MiSeq/17543_S1_L001_R2_001.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/ouput/2016-12-22.17543.txt"};

    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/jyang/work/Mireia/data/roth2016_control_set_plus_control.sorted.fa",
    //                 "/Volumes/jyang/work/Mireia/2016-12-22_MiSeq/17544_S2_L001_R1_001.fastq.gz",
    //                 "/Volumes/jyang/work/Mireia/2016-12-22_MiSeq/17544_S2_L001_R2_001.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/ouput.17544.txt"};

    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/jyang/work/Mireia/data/roth2016_control_set_plus_control.sorted.fa",
    //                 "/Volumes/jyang/work/Mireia/2017-02-04_MiSeq/17861-48-WCC-R10D10_S1_L001_R1_001.fastq.gz",
    //                 "/Volumes/jyang/work/Mireia/2017-02-04_MiSeq/17861-48-WCC-R10D10_S1_L001_R2_001.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/ouput.17861.txt"};

    if (_run->_pfp == NULL){
        FusionPair* _fp = new FusionPair( argc, argv );
        _fp->bShortRun = _run->_config.fp_bShortRun;
        _fp->Search();
        _run->_pfp = _fp;
    }

    FusionPair* _fp = _run->_pfp;

    //nrow = _fp->records.size();
    //ncol = _fp->records.size();
    nrow = _fp->_rownames.size();
    ncol = _fp->_colnames.size();

    _run->_config.nrow = nrow;
    _run->_config.ncol = ncol;

    //fusion_pair_search( argc, argv );

    nrow = _fp->_rownames.size();
    ncol = _fp->_colnames.size();

    _scene1 = new QGraphicsScene(this);

    GradientPalette gp( startColor, endColor );

    // Populate scene
    int yy = 0;
    int nitems = 0;

    Chip::SetWidth( width );
    Chip::SetHeight( height );

    //double test = 1000.0;
    for (int i = 0; i < nrow; i ++) {
        ++yy;
        int xx = 0;
        for (int j = 0; j < ncol; j ++) {
            ++xx;

            int pair_cnt = _fp->output[ pair<int,int>(i,j) ];
            float ratio = min( pair_cnt / _run->m_color_ratio, 1.0 );
            //float ratio = min( pair_cnt / 1000.0, 1.0 );
            //float ratio = min( pair_cnt / test, 1.0 );
            QColor color = gp.getColor(ratio);

            QGraphicsItem *item = new Chip(color, xx, yy);
            item->setPos(QPointF(j*width, i*height));
            _scene1->addItem(item);

            ++nitems;
        }
    }

    // Add row names
    for (int i = 0; i < nrow; i ++) {
        //QGraphicsItem *item = new Title( _fp->records[i].header.c_str() );
        QGraphicsItem *item = new Title( _fp->_rownames[i].c_str() );
        item->setScale(0.75);
        item->setPos(QPointF((nrow+_run->_config.rname_wadj)*width, (i+_run->_config.rname_hadj)*height+4.0));
        _scene1->addItem(item);
    }

    // Add col names
    for (int i = 0; i < ncol; i ++) {
        //QGraphicsItem *item = new Title( _fp->records[i].header.c_str() );
        QGraphicsItem *item = new Title( _fp->_colnames[i].c_str() );
        item->setScale(0.75);
        item->setRotation(-90);
        item->setPos(QPointF((i+_run->_config.cname_wadj)*width+4.0, _run->_config.cname_hadj));
        _scene1->addItem(item);
    }

    //QtDensity qtdensity(R);		// pass R inst. by reference

    std::cout << "item counts: " << nitems << std::endl;
}

void MainWindow::populateScene()
{
    _scene1 = new QGraphicsScene(this);

    //GradientPalette gp(QColor(255,255,255), QColor(255,0,0));
    //GradientPalette gp(QColor(125,125,0), QColor(125,125,0));
    GradientPalette gp(QColor(242, 177, 103), QColor(124, 162, 248)); // orange -> blue-ish

    QImage image(":/qt4logo.png");

    // Populate scene
    int xx = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i += 110) {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += 70) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            //QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            float ratio = (x+y)/2.0;

            QColor color = gp.getColor((x+y)/2.0);

            QGraphicsItem *item = new Chip(color, xx, yy);
            item->setPos(QPointF(i, j));
            _scene1->addItem(item);

            ++nitems;
        }
    }
    std::cout << "item counts: " << nitems << std::endl;
}

void MainWindow::newFile(){
    // Test for QMessageBox
    /*if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "title", "Question", QMessageBox::Yes|QMessageBox::No).exec())
    {

    }*/

    // Test for RandomPattern
    /*
     */
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
}

void MainWindow::createMenus()
{
    QMenuBar* menu_bar = new QMenuBar();
    QMenu *fileMenu = menu_bar->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    this->layout()->setMenuBar( menu_bar );
    //fileMenu->addAction(openAct);
    //fileMenu->addAction(saveAct);
    //fileMenu->addAction(printAct);
    //fileMenu->addSeparator();
    //fileMenu->addAction(exitAct);

    //QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    //editMenu->addAction(undoAct);
    //editMenu->addAction(redoAct);
}

void MainWindow::populateRandomScene(int nrow, int ncol, int width, int height, QColor startColor, QColor endColor)
{
    _scene1 = new QGraphicsScene(this);

    GradientPalette gp( startColor, endColor );

    // Populate scene
    int xx = 0;
    int nitems = 0;

    Chip::SetWidth( width );
    Chip::SetHeight( height );

    for (int i = 0; i < nrow; i ++) {
        ++xx;
        int yy = 0;
        for (int j = 0; j < ncol; j ++) {
            ++yy;
            qreal x = i / qreal(nrow);
            qreal y = j / qreal(ncol);

            //QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            float ratio = (x+y)/2.0;

            QColor color = gp.getColor((x+y)/2.0);

            QGraphicsItem *item = new Chip(color, xx, yy);
            item->setPos(QPointF(i*width, j*height));
            _scene1->addItem(item);

            ++nitems;
        }
    }

    // Add row names
    char buf[100];
    for (int i = 0; i < nrow; i ++) {
        sprintf( buf, "%d", i );
        QGraphicsItem *item = new Title(buf);
        item->setPos(QPointF((nrow+_run->_config.rname_wadj)*width, (i+_run->_config.rname_hadj)*height));
        _scene1->addItem(item);
    }

    // Add col names
    for (int i = 0; i < ncol; i ++) {
        sprintf( buf, "%d", i );
        QGraphicsItem *item = new Title(buf);
        item->setRotation(-90);
        item->setPos(QPointF((i+_run->_config.cname_wadj)*width, _run->_config.cname_hadj));
        _scene1->addItem(item);
    }

    std::cout << "item counts: " << nitems << std::endl;
}

