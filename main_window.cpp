#include "main_window.hpp"
#include "ui_main_window.h"

#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    brains(),
    brainCount(10),
    neuronPerBrain(20),
    imageCount(42),
    timerAdvance(new QTimer(this)),
    timerRefresh(new QTimer(this)),
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
    }
    // Go !
    QObject::connect(timerAdvance, SIGNAL(timeout()), this, SLOT(onTimerAdvance()));
    QObject::connect(timerRefresh, SIGNAL(timeout()), this, SLOT(onTimerRefresh()));
    timerRefresh->start(1000/10);
    //timerRefresh->start(1000/10);
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
    imageId++;
    imageId %= imageCount;
    std::vector<float> inputs = images[imageId]->getPixels();
    for(int i = 0 ; i < brainCount ; i++)
    {
        brains[i]->compute(inputs);
        qDebug() << images[imageId]->getValue() << brains[i]->getDecision();
    }
}

void MainWindow::onTimerRefresh()
{
    ui->spinBoxImageId->setValue(imageId);
    ui->labelValue->setText(QString::number(images[imageId]->getValue()));
    ui->labelImage->setPixmap(QPixmap::fromImage(images[imageId]->getImage()));
}

void MainWindow::on_pushButtonGo_clicked()
{
    onTimerAdvance();


}
