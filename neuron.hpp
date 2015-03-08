#pragma once

#include <vector>

class Neuron
{
public:
    Neuron(std::vector<float> dna, unsigned int inputCount);
    float getOutput();
    void compute(std::vector<float> inputValues);
    void resetOutput();
private:
    void setCoefficients(std::vector<float> dna);
    std::vector<float> coefficients;
    float outputValue;
};
