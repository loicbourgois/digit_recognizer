#pragma once

#include <vector>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "neuron.hpp"
#include "util.hpp"

class Brain
{
public:
    Brain(int neuronCount, int externalInputPerNeuronCount);
    Brain(int neuronCount, int externalInputPerNeuronCount, QXmlStreamReader & xml);
    ~Brain();

    void save(QXmlStreamWriter &xml);
    void load(QXmlStreamReader &xml);

    void compute(std::vector<float> inputs);
    int getDecision();
    int getAge();
    const std::vector<float> & getDna();
    void addLose();
    void addWin();
    void train(float loop, std::vector<float> inputs, int wantedValue);
    void mutateMaybe();

    static float getMutationFrequency(){return Brain::mutationFrequency;}
    static void setMutationFrequency(float f){Brain::mutationFrequency = f;}
    static float getMutationIntensity(){return Brain::mutationIntensity;}
    static void setMutationIntensity(float f){Brain::mutationIntensity = f;}
    static float getAverageRatio(){return Brain::averageRatio;}
    static void setAverageRatio(float f){Brain::averageRatio = f;}

    void resetOutputs();
    void addAge();
    static int getRoundToMutaion();
    float getRatio();
    int getWinCount();
    int getLoseCount();

private:
    void setRandomDna();
    void setDna(std::vector<float> dna);
    void mutateDna();
    void buildPhenotype();

    static int roundToMutation;
    static float averageRatio;
    static Brain * olderBrain;
    static int ageOlder;
    static Brain *  bestBrain;
    static float bestRatio;
    static float mutationFrequency;
    static float mutationIntensity;

    std::vector<Neuron*> neurons;
    std::vector<float> dna;
    int neuronCount;
    int inputPerNeuronCount;
    int externalInputPerNeuronCount;
    unsigned int dnaSize;

    int age;
    int winCount;
    int loseCount;
    float ratio;

    int choiceCount;


};
