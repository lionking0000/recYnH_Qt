#include "configuration.h"

Configuration::Configuration()
{
    // default test cell config values
    this->nrow = 12;
    this->ncol = 12;
    this->cell_width = 10;
    this->cell_height = 10;

    // default colors
    /*  orange -> blue-ish
    this->start_color_red = 242;
    this->start_color_green = 177;
    this->start_color_blue = 103;
    this->end_color_red = 124;
    this->end_color_green = 162;
    this->end_color_blue = 248;
    */
    //  white -> dark blue
    this->start_color_red = 255;
    this->start_color_green = 255;
    this->start_color_blue = 255;
    this->end_color_red = 0;
    this->end_color_green = 0;
    this->end_color_blue = 139;

    // row and col names
    /*  when cell_width = 50, cell_height = 50
    this->rname_wadj = 0.1;
    this->rname_hadj = 0.3;
    this->cname_wadj = 0.25;
    this->cname_hadj = -5;
    */
    /* when cell_width = 10, cell_height = 10 */
    this->rname_wadj = 0.0;
    this->rname_hadj = -0.7;
    this->cname_wadj = -0.7;
    this->cname_hadj = -2;

    this->fp_bShortRun = true;
}
