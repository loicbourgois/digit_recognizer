#include "brain.hpp"

int Brain::loopToComputeCount = 10;
int Brain::ageMinimumToDie = 1000;

Brain::Brain(int neuronCount, int externalInputPerNeuronCount) :
    neuronCount(neuronCount),
    dnaSize(0),
    age(0),
    winsCount(0),
    loseCount(0),
    ratio(0.0f),
    choiceCount(10)
{
    this->neuronCount = neuronCount;
    this->externalInputPerNeuronCount = externalInputPerNeuronCount;
    this->inputPerNeuronCount = neuronCount + externalInputPerNeuronCount;
    this->dnaSize = neuronCount * inputPerNeuronCount;
    setRandomDna();
    buildPhenotype();
}


void Brain::compute(std::vector<float> inputs)
{
    for(unsigned int i = 0 ; i < neurons.size() ; i++)
    {
        inputs.push_back(neurons[i]->getOutput());
    }

    for(unsigned int i = 0 ; i < neurons.size() ; i++)
    {
        neurons[i]->compute(inputs);
    }
}

int Brain::getDecision()
{
    int decision = -1;
    float decisionRatio = -1.0f;

    for(int i = 0 ; i < choiceCount ; i++)
    {
        float r = 0.0f;
        for(int j = i*(neuronCount / choiceCount)
            ; j < (i*(neuronCount / choiceCount)) + (neuronCount / choiceCount)
            ; j++)
        {
            r += neurons[j]->getOutput();
        }
        r /= (neuronCount / choiceCount);
        if(r > decisionRatio)
        {
            decisionRatio = r;
            decision = i;
        }
    }
    return decision;
}


Brain::~Brain()
{

}

void Brain::setDna(const std::vector<float> & dna)
{
    if(dna.size() != this->dnaSize)
    {
        qDebug() << "difference in size between dna and new dna";
    }
    else
    {
        this->dna = dna;
        buildPhenotype();
    }
}

void Brain::setRandomDna()
{
    dna.clear();
    dna.resize(dnaSize);
    for(unsigned int i = 0 ; i < dnaSize ; i++)
    {
        dna[i] = Util::getRandomFloat(-1.0f, 1.0f);
    }
}

void Brain::buildPhenotype()
{
    neurons.clear();
    neurons.resize(neuronCount);
    for(int i =  0 ; i < neuronCount ; i++)
    {
        std::vector<float> neuronDna(&dna[i * inputPerNeuronCount], &dna[(i+1) * inputPerNeuronCount]);
        neurons[i] = new Neuron(neuronDna, inputPerNeuronCount);
    }
}

