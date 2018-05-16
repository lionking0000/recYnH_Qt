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

    double m_color_ratio;
};

#endif // RUNTIMEINFO_H
