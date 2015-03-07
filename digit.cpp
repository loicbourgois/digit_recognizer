#include "digit.hpp"

#include <QStringList>
#include <QDebug>
#include <QColor>

Digit::Digit(const QString & str) :
    baseDimension(28),
    division(1),
    newDimension(28)
{
    QStringList list = str.split(',');
    value = list[0].toInt();
    list.removeFirst();
    pixels.clear();
    for(int y = 0 ; y < newDimension ; y++)
    {
        for(int x = 0 ; x < newDimension ; x++)
        {
            float pixel = 0 ;
            pixel += list[(x*division)+(y*baseDimension*division)].toInt();
            /*pixel += list[(x*division)+(y*baseDimension*division) + 1].toInt() ;
            pixel += list[(x*division)+(y*baseDimension*division) + baseDimension].toInt() ;
            pixel += list[(x*division)+(y*baseDimension*division) + baseDimension + 1].toInt();
            pixel /= 4.0f;*/
            pixel /= 255.0f;
            pixels.push_back(pixel);
        }
    }
}

Digit::~Digit()
{

}

QImage Digit::getImage()
{
    QImage img(newDimension,newDimension, QImage::Format_ARGB32);
    for(int y = 0 ; y < newDimension ; y++)
    {
        for(int x = 0 ; x < newDimension ; x++)
        {
            QColor color(pixels[x + y*newDimension]*255,
                    pixels[x + y*newDimension]*255,
                    pixels[x + y*newDimension]*255);
            img.setPixel(x, y, color.rgb());
        }
    }
    return img.scaled(200,200);
}

int Digit::getValue()
{
    return value;
}

std::vector<float> Digit::getPixels()
{
    return pixels;
}

