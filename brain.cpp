#include "brain.hpp"

int Brain::roundToMutation = 1000;
float Brain::averageRatio = 0.0f;
Brain * Brain::olderBrain = nullptr;
int Brain::ageOlder = 0;
Brain * Brain::bestBrain = nullptr;
float Brain::bestRatio = 0;

Brain::Brain(int neuronCount, int externalInputPerNeuronCount) :
    neurons(),
    dna(),
    neuronCount(neuronCount),
    inputPerNeuronCount(0),
    externalInputPerNeuronCount(0),
    dnaSize(0),
    age(0),
    winsCount(0),
    loseCount(0),
    ratio(1.0f),
    choiceCount(10)
{
    this->neuronCount = neuronCount;
    this->externalInputPerNeuronCount = externalInputPerNeuronCount;
    this->inputPerNeuronCount = neuronCount + externalInputPerNeuronCount;
    this->dnaSize = neuronCount * inputPerNeuronCount;
    setRandomDna();
    buildPhenotype();
}

void Brain::setAverageRatio(float ratio)
{
    Brain::averageRatio = ratio;
}

float Brain::getAverageRatio()
{
    return Brain::averageRatio;
}

int Brain::getRoundToMutaion()
{
    return Brain::roundToMutation;
}

void Brain::train(float loop, std::vector<float> inputs, int wantedValue)
{
    resetOutputs();
    for(int l = 0 ; l < loop ; l++)
    {
        compute(inputs);
    }
    if(getDecision() == wantedValue)
    {
        addWin();
    }
    else
    {
        addLose();
    }

    age++;
    if(age > ageOlder)
    {
        Brain::ageOlder = age;
        Brain::olderBrain = this;
    }
    if(ratio > bestRatio)
    {
        Brain::bestRatio = ratio;
        Brain::bestBrain = this;
    }
}

void Brain::mutateDna(float r1, float r2, float r3)
{
    for(unsigned int i = 0 ; i < dnaSize ; i++)
    {
        if(rand() < RAND_MAX * r1) // ratio apparition mutation
        {
            float rand = std::rand() * 2 - RAND_MAX;
            rand *= r2; // ratio importance de la mutation
            rand /=  RAND_MAX;
            dna[i] += rand;
            if(dna[i] > 1.0f)
                dna[i] = 1.0f;
            if(dna[i] < -1.0f)
                dna[i] = -1.0f;
        }
    }
    //  Mutation totale
    if(rand() < RAND_MAX * r3) // ratio mutation totale
        setRandomDna();
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

int Brain::getAge()
{
    return age;
}

const std::vector<float> & Brain::getDna()
{
    return dna;
}

void Brain::addLose()
{
    loseCount++;
    ratio = (float)(winsCount) / (float)(winsCount + loseCount);
}

void Brain::addWin()
{
    winsCount++;
    ratio = (float)(winsCount) / (float)(winsCount + loseCount);
}

float Brain::getRatio()
{
    return ratio;
}

int Brain::getWinCount()
{
    return winsCount;
}

int Brain::getLoseCount()
{
    return loseCount;
}

void Brain::resetOutputs()
{
    for(unsigned int i = 0 ; i < neurons.size() ; i++)
    {
        neurons[i]->resetOutput();
    }
}

void Brain::addAge()
{
    age ++;
}

void Brain::mutateMaybe()
{
    //winsCount = 0;
    //loseCount = 0;
    if(ratio < Brain::averageRatio)
    {
        if(olderBrain == this)
            ageOlder = 0;
        age = 0;
        ratio = 0;
        winsCount = 0;
        loseCount = 0;
        setDna(Brain::olderBrain->getDna());
        mutateDna(0.1, 0.1, 0.1);
        buildPhenotype();
    }
}


Brain::~Brain()
{

}

void Brain::setDna(std::vector<float> dna)
{
    if(dna.size() != this->dnaSize)
    {
        qDebug() << "difference in size between dna and new dna";
    }
    else
    {
        this->dna.clear();
        this->dna.resize(dnaSize);
        for(unsigned int i = 0 ; i < dnaSize ; i++)
        {
            this->dna[i] = dna[i];
        }
        //buildPhenotype();
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
    // Delete
    for(int i =  0 ; i < neurons.size() ; i++)
    {
        delete neurons[i];
    }
    neurons.clear();
    // Create
    neurons.resize(neuronCount, nullptr);
    for(int i =  0 ; i < neuronCount ; i++)
    {
        std::vector<float> neuronDna(&dna[i * inputPerNeuronCount], &dna[(i+1) * inputPerNeuronCount]);
        neurons[i] = new Neuron(neuronDna, inputPerNeuronCount);
    }
}

