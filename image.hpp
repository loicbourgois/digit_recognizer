#pragma once

#include <QString>
#include <vector>
#include <QImage>

class Digit
{
public:
    Digit(const QString &str);
    ~Digit();

    QImage getImage();
    int getValue();
    std::vector<float> getPixels();

private:
    std::vector<float> pixels;
    int baseDimension;
    int division;
    int newDimension;
    int value;
};
