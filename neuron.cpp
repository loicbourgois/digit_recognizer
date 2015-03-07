#include "neuron.hpp"

#include <QDebug>
#include <math.h>

Neuron::Neuron(std::vector<float> dna, int inputCount)
{
    if(inputCount != dna.size())
    {
        qDebug() << "InputCount != dna.size()";
    }
    else
    {
        setCoefficients(dna);
    }

}

float Neuron::getOutput()
{
    return outputValue;
}

void Neuron::compute(std::vector<float> inputValues)
{
    if(inputValues.size() == coefficients.size())
    {
        float divisor = 0.0f;
        outputValue = 0.0f;
        for(unsigned int i = 0 ; i < inputValues.size() ; i++)
        {
            if(inputValues[i]>1 || inputValues[i]<-1)
                qDebug() << "Input value >  < 1 : " << inputValues[i];
            outputValue += inputValues[i] * coefficients[i];
            divisor += fabs(coefficients[i]);
        }
        outputValue /= divisor;
        if(outputValue>1 || outputValue<-1)
            qDebug() << "output value >  < 1 : " << outputValue;
    }
    else
        qDebug() << "Coefficient count not matching inputs.";
}

void Neuron::reset()
{
    outputValue = 0;
}

void Neuron::setCoefficients(std::vector<float> dna)
{
    coefficients.clear();
    coefficients.resize(dna.size());
    coefficients = dna;
}
