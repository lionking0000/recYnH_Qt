#ifndef RUNTIMEINFO_H
#define RUNTIMEINFO_H

//#include "fusionpair.h"
#include "configuration.h"

class FusionPair;

class RuntimeInfo
{
public:
    RuntimeInfo();
    FusionPair* _pfp;
    Configuration _config;

    ~RuntimeInfo();
};

#endif // RUNTIMEINFO_H
