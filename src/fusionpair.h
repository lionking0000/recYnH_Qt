#ifndef FUSIONPAIR_H
#define FUSIONPAIR_H

#include "dwag.h"
#include "runtimeinfo.h"

using namespace std;

class FusionPair
{
public:
    FusionPair();
    FusionPair(int argc, char *argv[]);
    int Search();
    int ReadPairResults(RuntimeInfo* run);

    int _argc;
    char** _argv;
    bool bShortRun;
    dwag start;
    map<pair<int, int>,int> output;
    vector<Record> records;

    vector<string> _colnames;
    vector<string> _rownames;

    map<pair<int, int>,int> _no_selection;
    map<pair<int, int>,int> _selection1;
    map<pair<int, int>,int> _selection2;
};

#endif // FUSIONPAIR_H
