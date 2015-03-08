#include "main_window.hpp"
#include "ui_main_window.h"

#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    brainForms(),
    timerAdvance(new QTimer(this)),
    timerRefresh(new QTimer(this)),
    brains(),
    images(),
    imageCount(42000),
    brainCount(10),
    neuronPerBrain(20),
    loopPerCompute(5),
    steps(0),
    imageId(0)

{
    ui->setupUi(this);
    ui->spinBoxImageId->setMinimum(-1);
    ui->spinBoxImageId->setMaximum(imageCount);


    QSplitterHandle *handle = ui->splitter->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    layout->addWidget(line);

    //  Images
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
    //  Brains
    for(int i = 0 ; i < brainCount ; i++)
    {
        brains.push_back(new Brain(neuronPerBrain, 28*28));
        brainForms.push_back(new BrainForm(this));
        ui->widgetBrains->layout()->addWidget(brainForms[i]);
    }
    // Go !
    QObject::connect(timerAdvance, SIGNAL(timeout()), this, SLOT(onTimerAdvance()));
    QObject::connect(timerRefresh, SIGNAL(timeout()), this, SLOT(onTimerRefresh()));
    timerRefresh->start(1000/10);
    timerAdvance->start(0);
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
    for(int i = 0 ; i < brainCount ; i++)
    {
        brains[i]->train(loopPerCompute, inputs, images[imageId]->getValue());
        averageRatio += brains[i]->getRatio() * brains[i]->getAge();
        totalAge += brains[i]->getAge();
    }
    averageRatio /= (float)totalAge;
    Brain::setAverageRatio(averageRatio);
    //  Mutate ?
    if(!(steps %= Brain::getRoundToMutaion()))
    {
        for(int i = 0 ; i < brainCount ; i++)
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
    //qDebug() << "**************************************";
    for(int i = 0 ; i < brainCount ; i++)
    {
        brainForms[i]->ui->labelDecision->setText(QString::number(brains[i]->getDecision()));
        brainForms[i]->ui->labelAge->setText(QString::number(brains[i]->getAge()));
        brainForms[i]->ui->labelRatio->setText(QString::number(brains[i]->getRatio(), 'f', 4));
        brainForms[i]->ui->labelWin->setText(QString::number(brains[i]->getWinCount()));
        brainForms[i]->ui->labelLose->setText(QString::number(brains[i]->getLoseCount()));
    }
    ui->labelAverage->setText(QString::number(Brain::getAverageRatio(), 'f', 4));
}

void MainWindow::on_pushButtonGo_clicked()
{
    onTimerAdvance();
}
