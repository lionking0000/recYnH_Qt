#include "robot.h"
#include <map>
#include <iostream> // for cout
#include <fstream>  // for ifstream

// For visualization (MainWindow and R)
#include "qtdensity.h"
#include "mainwindow.h"
#include <QApplication>
#include "tabdialog.h"

class COptionFusionPairSearch:public COption{
    public:
        COptionFusionPairSearch() { _command = "FusionPairSearch"; }
        int work(int argc, char** argv, RuntimeInfo* run){
            cout << _command << endl;
            //return fusion_pair_search( argc, argv );
            run->_pfp = new FusionPair( argc, argv );
            run->_pfp->bShortRun = run->_config.fp_bShortRun;
            run->_pfp->Search();
            return 1;
        }
};

class COptionReadPairResults:public COption{
    public:
        COptionReadPairResults() { _command = "ReadPairResults"; }
        int work(int argc, char** argv, RuntimeInfo* run){
            cout << _command << endl;
            run->_pfp = new FusionPair( argc, argv );
            run->_pfp->ReadPairResults( run );
            return 1;
        }
};

CRobot::CRobot( int argc, char* argv[] ): _argc(argc), _argv(argv){
    _option_list.push_back( new COptionFusionPairSearch() );

    _option_list.push_back( new COptionReadPairResults() );
    /*
    // related with fusion search
    _option_list.push_back( new COptionFakeTreeTest() );
    _option_list.push_back( new COptionPerformanceTest() );
    _option_list.push_back( new COptionSearchTest() );
    _option_list.push_back( new COptionFusionPairSearch() );

    // related with compression
    _option_list.push_back( new COptionSplitFastq() );
    _option_list.push_back( new COptionSplitFastqToZst() );
    _option_list.push_back( new COptionReadZst() );

    // related with kmer - compression
    _option_list.push_back( new COptionKmer() );
    _option_list.push_back( new COptionKmerDeCompress() );

    // related with integer packing
    _option_list.push_back( new COptionIntPacking() );

    // related with dwag
    _option_list.push_back( new COptionDWAG() );

    // related with nt + qual
    _option_list.push_back( new COptionCombined() );
    */
}

int CRobot::work(){
    if (_argc <= 1) return 0;
    for( int i = 0; i < _option_list.size(); i ++ ){
        COption* opt = _option_list[i];
        if ( opt->_command == _argv[1] )
            return opt->work( _argc, _argv, &_run );
    }
    return 0;
}

int CRobot::visualize(){
#ifdef USE_R
    RInside R(_argc, _argv);  		// create an embedded R instance

    QApplication app(_argc, _argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window(R,&_run);
    window.show();

    return app.exec();
#else
    QApplication app(_argc, _argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window(&_run, 0);
    //MainWindow window(&_run);
    window.show();

    return app.exec();
#endif
}


CRobot::~CRobot(){
    for( int i = 0; i < _option_list.size(); i ++ ){
        COption* opt = _option_list[i];
        delete opt;
    }
}


/*
 *  Main Dialog for recYnH
 *
 *
 */
int CRobot::visualize_tab(){
    QApplication app(_argc, _argv);
    QString fileName;

    if (_argc >= 2)
        fileName = _argv[1];
    else
        fileName = ".";

    TabDialog tabdialog(this);
    tabdialog.m_robot = this;
    tabdialog.show();

    return app.exec();
}

