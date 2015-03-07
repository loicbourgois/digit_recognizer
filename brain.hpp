#pragma once

#include <vector>
#include <QDebug>

#include "neuron.hpp"
#include "util.hpp"

class Brain
{
public:
    Brain(int neuronCount, int externalInputPerNeuronCount);
    void compute(std::vector<float> inputs);
    int getDecision();
    ~Brain();
private:
    void setRandomDna();
    void setDna(const std::vector<float> & dna);
    void buildPhenotype();

    static int loopToComputeCount;
    static int ageMinimumToDie;

    int neuronCount;
    int inputPerNeuronCount;
    int externalInputPerNeuronCount;
    unsigned int dnaSize;

    std::vector<Neuron*> neurons;
    std::vector<float> dna;
    int age;
    int winsCount;
    int loseCount;
    float ratio;

    int choiceCount;


};
