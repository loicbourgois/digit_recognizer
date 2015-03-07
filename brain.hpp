#pragma once

#include <vector>
#include <QDebug>

#include "neuron.hpp"
#include "util.hpp"

class Brain
{
public:
    Brain(int neuronCount, int externalInputPerNeuronCount);
    ~Brain();
    void compute(std::vector<float> inputs);
    int getDecision();
    int getAge();
    const std::vector<float> & getDna();
    void addLose();
    void addWin();
    void train(float loop, std::vector<float> inputs, int wantedValue);

    void mutateDna(float r1, float r2, float r3);


    void resetOutputs();
    void addAge();
    void mutateMaybe();

    static void setAverageRatio(float ratio);
    static float getAverageRatio();
    static int getRoundToMutaion();

    float getRatio();
    int getWinCount();
    int getLoseCount();

private:
    void setRandomDna();
    void setDna(std::vector<float> dna);
    void buildPhenotype();

    static int roundToMutation;
    static float averageRatio;
    static Brain * olderBrain;
    static int ageOlder;
    static Brain *  bestBrain;
    static float bestRatio;


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
