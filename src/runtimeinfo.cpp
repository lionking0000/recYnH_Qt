#include "runtimeinfo.h"
#include "fusionpair.h"

RuntimeInfo::RuntimeInfo()
{
    _pfp = NULL;
}


RuntimeInfo::~RuntimeInfo(){
    if (_pfp != NULL)
        delete _pfp;
}
