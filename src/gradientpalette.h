#ifndef GRADIENTPALETTE_H
#define GRADIENTPALETTE_H

#include <QColor>

/*
 * 
 * 
 * [ read this ]
 * https://github.com/karlphillip/GraphicsProgramming/blob/master/qtSmoothColorTransition/main.cpp
 * 
 */
class GradientPalette
{
public:
    GradientPalette(QColor startColor, QColor endColor);

    QColor getColor(float ratio);

private:
    QColor startColor;
    QColor endColor;
};


#endif // GRADIENTPALETTE_H
