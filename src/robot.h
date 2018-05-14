#ifndef ROBOT_H
#define ROBOT_H

// If you want to use graphic user interface, uncomment following line
// If commented, then you can use only command line interface
#define USE_GRAPHIC_INTERFACE

// If you want to use R, uncomment following line
//#define USE_R   // for R


#include "dwag.h"
#include "fusionpair.h"
#include "configuration.h"
#include "runtimeinfo.h"

class COption{
    public:
    std::string _command;
    COption(){
    }
    virtual int work(int argc, char** _argv, RuntimeInfo* run) = 0;
};


class CRobot
{
private:
    std::vector<COption*> _option_list;
    int _argc;
    char** _argv;


public:
    CRobot( int argc, char* argv[] );
    int work();
    int visualize();        // example
    int visualize_tab();    // tab view

    ~CRobot();

    RuntimeInfo _run;   // robot has RuntimeInfo
};






#endif // ROBOT_H
