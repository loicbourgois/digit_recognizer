#include "main_window.hpp"
#include "ui_main_window.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include "constants.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    brainForms(),
    timerAdvance(new QTimer(this)),
    timerRefresh(new QTimer(this)),
    timerAutoSave(new QTimer(this)),
    brains(),
    images(),
    imageCount(42000),
    brainCount(10),
    neuronPerBrain(20),
    loopPerCompute(4),
    steps(0),
    imageId(0)
{
    // Initial values
    steps = 0;
    Brain::setAverageRatio(0.0f);
    // UI
    ui->setupUi(this);
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonPlay->setDisabled(true);
    ui->pushButtonReset->setDisabled(true);
    ui->spinBoxImageId->setMinimum(-1);
    ui->spinBoxImageId->setMaximum(imageCount);
    setSpinboxes();
    QSplitterHandle *handle = ui->splitter->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);
    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    layout->addWidget(line);
    //  Load images
    QFile file("C:/Users/Loic/coding/digits/train.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "impossible d'ouvrir le fichier";
    }
    QTextStream in(&file);
    in.readLine();
    int a = 0;
    while (!in.atEnd() && a < imageCount ) {
        a++;
        QString line = in.readLine();
        images.push_back(new Digit(line));
    }
    // Timers
    QObject::connect(timerAdvance, SIGNAL(timeout()), this, SLOT(onTimerAdvance()));
    QObject::connect(timerRefresh, SIGNAL(timeout()), this, SLOT(onTimerRefresh()));
    QObject::connect(timerAutoSave, SIGNAL(timeout()), this, SLOT(onTimerAutosave()));
    timerRefresh->start(1000/10);
    timerAutoSave->start(1000 * 60 * 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBoxImageId_valueChanged(int i)
{
    imageId = i;
    imageId = (imageId + imageCount) % imageCount;
    ui->spinBoxImageId->setValue(imageId);
}

void MainWindow::onTimerAdvance()
{
    // Select image
    imageId++;
    imageId %= imageCount;
    std::vector<float> inputs = images[imageId]->getPixels();
    // Compute
    float averageRatio = 0.0f;
    int totalAge = 0;

    for(unsigned int i = 0 ; i < brains.size() ; i++)
    {
        brains[i]->train(loopPerCompute, inputs, images[imageId]->getValue());
        averageRatio += brains[i]->getRatio() * brains[i]->getAge();
        totalAge += brains[i]->getAge();
    }
    averageRatio /= (float)totalAge;
    Brain::setAverageRatio(averageRatio);
    //  Mutate ?
    if(!(steps % Brain::getRoundToMutaion()) && steps)
    {
        for(unsigned int i = 0 ; i < brains.size() ; i++)
        {
            brains[i]->mutateMaybe();
        }
    }
    steps++;
}

void MainWindow::onTimerRefresh()
{
    // Image ?
    ui->spinBoxImageId->setValue(imageId);
    ui->labelValue->setText(QString::number(images[imageId]->getValue()));
    ui->labelImage->setPixmap(QPixmap::fromImage(images[imageId]->getImage()));
    //
    for(unsigned int i = 0 ; i < brainForms.size() && i < brains.size() ; i++)
    {
        brainForms[i]->ui->labelDecision->setText(QString::number(brains[i]->getDecision()));
        brainForms[i]->ui->labelAge->setText(QString::number(brains[i]->getAge()));
        brainForms[i]->ui->labelRatio->setText(QString::number(brains[i]->getRatio(), 'f', 4));
        brainForms[i]->ui->labelWin->setText(QString::number(brains[i]->getWinCount()));
        brainForms[i]->ui->labelLose->setText(QString::number(brains[i]->getLoseCount()));
    }
    ui->labelAverage->setText(QString::number(Brain::getAverageRatio(), 'f', 4));
    ui->labelSteps->setText(QString::number(steps));
}

void MainWindow::onTimerAutosave()
{
    int i = 0;
    QDir dir("C:/Users/Loic/coding/digit_recognizer_saves/autosaves/");
    if (!dir.exists()) {
        dir.mkpath("C:/Users/Loic/coding/digit_recognizer_saves/autosaves/");
    }
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();
    if(list.size())
    {
        QString str = list[0].fileName();
        QRegExp rx("(\\d+)");
        std::vector<int> result;
        int pos = 0;
        while ((pos = rx.indexIn(str, pos)) != -1)
        {
            QString theNumber = rx.cap(1);
            result.push_back(theNumber.toInt());
            pos += rx.matchedLength();
        }
        if(result.size() != 1)
        {
            qDebug() << "void MainWindow::onTimerAutosave() : problem";
        }
        else
        {
            i = result[0]+1;
        }
    }
    //
    QString path = "C:/Users/Loic/coding/digit_recognizer_saves/autosaves/autosave" + QString::number(i) + ".digit";
    save(path);
}

void MainWindow::on_pushButtonGo_clicked()
{
    ui->pushButtonGo->setDisabled(true);
    ui->pushButtonReset->setDisabled(false);
    ui->pushButtonGo->unsetCursor();
    ui->spinBoxLoopPerCompute->setDisabled(true);
    ui->spinBoxBrainCount->setDisabled(true);
    ui->spinBoxNeuronPerBrain->setDisabled(true);
    //  Brains
    brains.clear();
    brainForms.clear();
    for(int i = 0 ; i < brainCount ; i++)
    {
        brains.push_back(new Brain(neuronPerBrain, 28*28));
        brainForms.push_back(new BrainForm(this));
        ui->widgetBrains->layout()->addWidget(brainForms[i]);
    }
    //setSpinboxes();
    on_pushButtonPlay_clicked();
}

void MainWindow::on_pushButtonPlay_clicked()
{
    ui->pushButtonPause->setDisabled(false);
    ui->pushButtonPlay->setDisabled(true);
    timerAdvance->start(0);
}

void MainWindow::on_pushButtonPause_clicked()
{
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonPlay->setDisabled(false);
    timerAdvance->stop();
}



void MainWindow::on_pushButtonReset_clicked()
{
    on_pushButtonPause_clicked();
    ui->pushButtonGo->setDisabled(false);
    ui->spinBoxLoopPerCompute->setDisabled(false);
    ui->spinBoxBrainCount->setDisabled(false);
    ui->spinBoxNeuronPerBrain->setDisabled(false);
    ui->pushButtonReset->setDisabled(true);
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonPlay->setDisabled(true);
    for(int i = 0 ; i < brains.size() ; i++)
    {
        delete brains[i];
    }
    brains.clear();
    for(int i = 0 ; i < brainForms.size() ; i++)
    {
        delete brainForms[i];
    }
    brainForms.clear();
    imageId = 0;
    steps = 0;
    Brain::setAverageRatio(0.0f);
}
void MainWindow::on_pushButtonSave_clicked()
{
    on_pushButtonPause_clicked();
    QDir dir("C:/Users/Loic/coding/digit_recognizer_saves/");
    if (!dir.exists()) {
        dir.mkpath("C:/Users/Loic/coding/digit_recognizer_saves/");
    }
    QString aa = QDateTime::currentDateTime().toString();
    QString path = QFileDialog::getSaveFileName(this, "Save", "C:/Users/Loic/coding/digit_recognizer_saves/" + aa, "Save (*.digit)");
    if(path.size())
    {
        save(path);
    }
    on_pushButtonPlay_clicked();
}

void MainWindow::on_pushButtonLoad_clicked()
{
    on_pushButtonPause_clicked();
    QDir dir("C:/Users/Loic/coding/digit_recognizer_saves/");
    if (!dir.exists()) {
        dir.mkpath("C:/Users/Loic/coding/digit_recognizer_saves/");
    }

    QString path = QFileDialog::getOpenFileName(this, "Open", "C:/Users/Loic/coding/digit_recognizer_saves/", "Save (*.digit)");
    if(path.size())
    {
        load(path);
    }
    on_pushButtonPlay_clicked();
}


void MainWindow::save(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "can't open";
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("digit_recogniser_save");
    {
        xml.writeTextElement("title", "Title");
        xml.writeStartElement("options");
        {
            //  Options x 6
            xml.writeTextElement(ConstantStrings::averageRatio,  QString::number(Brain::getAverageRatio()));
            xml.writeTextElement(ConstantStrings::brainCount,  QString::number(brainCount));
            xml.writeTextElement(ConstantStrings::neuronPerBrain,  QString::number(neuronPerBrain));
            xml.writeTextElement(ConstantStrings::loopPerCompute,  QString::number(loopPerCompute));
            xml.writeTextElement(ConstantStrings::mutationFrequency,  QString::number(Brain::getMutationFrequency()));
            xml.writeTextElement(ConstantStrings::mutationIntensity,  QString::number(Brain::getMutationIntensity()));
        }
        xml.writeEndElement();
        xml.writeStartElement("infos");
        {
            //  Infos x
            xml.writeTextElement(ConstantStrings::imageId,  QString::number(imageId));
            xml.writeTextElement(ConstantStrings::steps,  QString::number(steps));
        }
        xml.writeEndElement();
        xml.writeStartElement(ConstantStrings::brains);
        {
            //  Brain x 1
            for(int i = 0 ; i < brains.size() ; i++)
            {
                brains[i]->save(xml);
            }
        }
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void MainWindow::load(QString path)
{
    // Reset
    on_pushButtonReset_clicked();
    ui->pushButtonGo->setDisabled(true);
    ui->pushButtonReset->setDisabled(false);
    ui->pushButtonGo->unsetCursor();
    ui->spinBoxLoopPerCompute->setDisabled(true);
    ui->spinBoxBrainCount->setDisabled(true);
    ui->spinBoxNeuronPerBrain->setDisabled(true);
    // load
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            { // Options x 6
                if(xml.name() == ConstantStrings::averageRatio)
                    Brain::setAverageRatio(xml.readElementText().toFloat());
                if(xml.name() == ConstantStrings::brainCount)
                    brainCount = (xml.readElementText().toFloat());
                if(xml.name() == ConstantStrings::neuronPerBrain)
                    neuronPerBrain = (xml.readElementText().toFloat());
                if(xml.name() == ConstantStrings::loopPerCompute)
                    loopPerCompute = (xml.readElementText().toFloat());
                if(xml.name() == ConstantStrings::mutationFrequency)
                    Brain::setMutationFrequency(xml.readElementText().toFloat());
                if(xml.name() == ConstantStrings::mutationIntensity)
                    Brain::setMutationIntensity(xml.readElementText().toFloat());
            }
            {// Infos x 2
                if(xml.name() == ConstantStrings::imageId)
                    imageId = (xml.readElementText().toInt());
                if(xml.name() == ConstantStrings::steps)
                    steps = (xml.readElementText().toInt());
            }
            {// Brain x 1
                if(xml.name() == ConstantStrings::brain)
                {
                    brains.push_back(new Brain(neuronPerBrain, 28*28, xml));
                    brainForms.push_back(new BrainForm(this));
                    ui->widgetBrains->layout()->addWidget(brainForms[brainForms.size()-1]);
                }

            }
        }
        if (xml.hasError())
            qDebug() << xml.errorString();
    }
    file.close();
    setSpinboxes();
}

void MainWindow::setSpinboxes()
{
    ui->spinBoxBrainCount->setValue(brainCount);
    ui->spinBoxImageId->setValue(imageId);
    ui->spinBoxLoopPerCompute->setValue(loopPerCompute);
    ui->spinBoxNeuronPerBrain->setValue(neuronPerBrain);
    ui->doubleSpinBoxMutationFrequency->setValue(Brain::getMutationFrequency());
    ui->doubleSpinBoxMutationIntensity->setValue(Brain::getMutationIntensity());
}
void MainWindow::on_spinBoxBrainCount_valueChanged(int i)
{
    brainCount = i;
}
void MainWindow::on_spinBoxNeuronPerBrain_valueChanged(int i)
{
    neuronPerBrain = i;
}
void MainWindow::on_spinBoxLoopPerCompute_valueChanged(int i)
{
    loopPerCompute = i;
}
void MainWindow::on_doubleSpinBoxMutationFrequency_valueChanged(double d)
{
    Brain::setMutationFrequency(d);
}
void MainWindow::on_doubleSpinBoxMutationIntensity_valueChanged(double d)
{
    Brain::setMutationIntensity(d);
}
