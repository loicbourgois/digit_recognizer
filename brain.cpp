#include "brain.hpp"

#include "constants.hpp"

int Brain::roundToMutation = 1000;
float Brain::averageRatio = 0.0f;
Brain * Brain::olderBrain = nullptr;
int Brain::ageOlder = 0;
Brain * Brain::bestBrain = nullptr;
float Brain::bestRatio = 0;

float Brain::mutationFrequency = 0.1f;
float Brain::mutationIntensity = 0.1f;

Brain::Brain(int neuronCount, int externalInputPerNeuronCount) :
    neurons(),
    dna(),
    neuronCount(neuronCount),
    inputPerNeuronCount(0),
    externalInputPerNeuronCount(0),
    dnaSize(0),
    age(0),
    winCount(0),
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

Brain::Brain(int neuronCount, int externalInputPerNeuronCount, QXmlStreamReader &xml) :
    neurons(),
    dna(),
    neuronCount(neuronCount),
    inputPerNeuronCount(0),
    externalInputPerNeuronCount(0),
    dnaSize(0),
    age(0),
    winCount(0),
    loseCount(0),
    ratio(1.0f),
    choiceCount(10)
{
    this->neuronCount = neuronCount;
    this->externalInputPerNeuronCount = externalInputPerNeuronCount;
    this->inputPerNeuronCount = neuronCount + externalInputPerNeuronCount;
    this->dnaSize = neuronCount * inputPerNeuronCount;
    load(xml);
    buildPhenotype();
}

Brain::~Brain()
{
    if(this == Brain::olderBrain)
    {
        Brain::olderBrain = nullptr;
        Brain::ageOlder = 0;
    }
    for(int i = 0 ; i < neurons.size(); i++)
    {
        delete neurons[i];
    }
}

void Brain::save(QXmlStreamWriter &xml)
{
    xml.writeStartElement(ConstantStrings::brain); // Attributs x 5
    {
        xml.writeTextElement(ConstantStrings::age, QString::number(age));
        xml.writeTextElement(ConstantStrings::winCount, QString::number(winCount));
        xml.writeTextElement(ConstantStrings::loseCount, QString::number(loseCount));
        xml.writeTextElement(ConstantStrings::dnaSize, QString::number(dna.size()));
        QString dnastr;
        for(unsigned int i = 0 ; i < dna.size() ; i++)
        {
            dnastr += QString::number(dna[i]) + ";";
        }
        dnastr.remove(dnastr.size()-1, 1);
        xml.writeTextElement(ConstantStrings::dna, dnastr);
    }
    xml.writeEndElement();
}

void Brain::load(QXmlStreamReader & xml)
{
    // Attributs x 5
    int age = 0;
    int winCount = 0;
    int loseCount = 0;
    int dnaSize = 0;
    QString dnastr;
    //
    while(!xml.atEnd()
          && !(xml.name() == ConstantStrings::brain && xml.tokenType() == QXmlStreamReader::EndElement))
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement) // Attributs x 5
        {
            if(xml.name() == ConstantStrings::age)
                age = xml.readElementText().toFloat();
            if(xml.name() == ConstantStrings::winCount)
                winCount = xml.readElementText().toFloat();
            if(xml.name() == ConstantStrings::loseCount)
                loseCount = xml.readElementText().toFloat();
            if(xml.name() == ConstantStrings::dnaSize)
                dnaSize = xml.readElementText().toInt();
            if(xml.name() == ConstantStrings::dna)
                dnastr = xml.readElementText();

        }
    }
    //
    this->age = age;
    this->winCount = winCount;
    this->loseCount = loseCount;
    this->dnaSize = dnaSize;
    QStringList list = dnastr.split(';');
    if(list.size() == this->dnaSize)
    {
        dna.clear();
        for (int i = 0; i < list.size(); ++i)
        {
            dna.push_back(list[i].toFloat());
        }
    }
    else
    {
        qWarning() << "DNA size doesn't match";
    }
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

void Brain::mutateDna()
{
    for(unsigned int i = 0 ; i < dnaSize ; i++)
    {
        if(rand() < RAND_MAX * Brain::mutationFrequency) // ratio apparition mutation
        {
            float rand = std::rand() * 2 - RAND_MAX;
            rand *= Brain::mutationIntensity; // ratio importance de la mutation
            rand /=  RAND_MAX;
            dna[i] += rand;
            if(dna[i] > 1.0f)
                dna[i] = 1.0f;
            if(dna[i] < -1.0f)
                dna[i] = -1.0f;
        }
    }
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
    ratio = (float)(winCount) / (float)(winCount + loseCount);
}

void Brain::addWin()
{
    winCount++;
    ratio = (float)(winCount) / (float)(winCount + loseCount);
}

float Brain::getRatio()
{
    return ratio;
}

int Brain::getWinCount()
{
    return winCount;
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
        winCount = 0;
        loseCount = 0;
        setDna(Brain::olderBrain->getDna());
        mutateDna();
        buildPhenotype();
    }
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
    for(unsigned int i =  0 ; i < neurons.size() ; i++)
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

