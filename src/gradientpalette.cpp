#include "gradientpalette.h"

GradientPalette::GradientPalette(QColor startColor, QColor endColor)
{
    this->startColor = startColor;
    this->endColor = endColor;
}

QColor GradientPalette::getColor(float ratio){
    // 1.0 >= ratio >= 0.0
    int red = (int)(ratio*this->endColor.red() + (1-ratio)*this->startColor.red()); //in your case, the values are 12 and 122
    int green = (int)(ratio*this->endColor.green() + (1-ratio)*this->startColor.green());  //in your case, the values are 23 and 233
    int blue = (int)(ratio*this->endColor.blue() + (1-ratio)*this->startColor.blue());  //in your case, the values are 24 and 244

    QColor color(red, green, blue);

    /*
    double segmentLength = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    double pdist = sqrt((xp-x1)*(xp-x1) + (yp-y1)*(yp-y1));
    double ratio = pdist/segmentLength;
    int red = (int)(ratio*startRedVal + (1-ratio)*endRedValue); //in your case, the values are 12 and 122
    int green = (int)(ratio*startGreenVal + (1-ratio)*endGreenValue); //in your case, the values are 23 and 233
    int blue = (int)(ratio*startBlueVal + (1-ratio)*endBlueValue); //in your case, the values are 24 and 244
    */

    return color;
}

