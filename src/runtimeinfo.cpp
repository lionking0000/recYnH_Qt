#include "runtimeinfo.h"
#include "fusionpair.h"

RuntimeInfo::RuntimeInfo()
{
    _pfp = NULL;
    m_color_ratio = 1000.0;
}


RuntimeInfo::~RuntimeInfo(){
    if (_pfp != NULL)
        delete _pfp;
}
