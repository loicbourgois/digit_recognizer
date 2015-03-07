#pragma once

#include <random>



class Util
{
public:
    static float getRandomFloat(float min, float max){
        return ((static_cast <float>(rand())) * (max - min) / (static_cast<float>(RAND_MAX))) + min;
    }
    static int getRandomInt(int min, int max){
        int r = getRandomFloat(min-1, max+1);
        if(r < min || r > max)
            r = getRandomInt(min, max);
        return r;
    }
};
