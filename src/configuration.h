#ifndef CONFIGURATION_H
#define CONFIGURATION_H


class Configuration
{
public:
    Configuration();

    int nrow;
    int ncol;
    int cell_width;
    int cell_height;

    int start_color_red;
    int start_color_green;
    int start_color_blue;

    int end_color_red;
    int end_color_green;
    int end_color_blue;

    float rname_wadj;
    float rname_hadj;
    float cname_wadj;
    float cname_hadj;

    // Fusion Pair options
    bool fp_bShortRun;
};

#endif // CONFIGURATION_H
