#include "main_window.hpp"
#include "ui_main_window.h"

#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    brains(),
    brainCount(10),
    neuronPerBrain(20),
    digitCount(42),
    timerAdvance(new QTimer(this)),
    timerRefresh(new QTimer(this)),
    digitId(0)
{
    ui->setupUi(this);
    ui->spinBoxImageId->setMinimum(-1);
    ui->spinBoxImageId->setMaximum(digitCount);


    QSplitterHandle *handle = ui->splitter->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    layout->addWidget(line);

    //  Images
    QFile file("./digits/train.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "impossible d'ouvrir le fichier";
    }
    QTextStream in(&file);
    in.readLine();
    int a = 0;
    while (!in.atEnd() && a < digitCount ) {
        a++;
        QString line = in.readLine();
        digits.push_back(new Digit(line));
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
    digitId = i;
    digitId = (digitId + digitCount) % digitCount;
    ui->spinBoxImageId->setValue(digitId);
}

void MainWindow::onTimerAdvance()
{
    digitId++;
    digitId %= digitCount;
    std::vector<float> inputs = digits[digitId]->getPixels();
    for(int i = 0 ; i < brainCount ; i++)
    {
        brains[i]->compute(inputs);
        qDebug() << digits[digitId]->getValue() << brains[i]->getDecision();
    }
}

void MainWindow::onTimerRefresh()
{
    ui->spinBoxImageId->setValue(digitId);
    ui->labelValue->setText(QString::number(digits[digitId]->getValue()));
    ui->labelImage->setPixmap(QPixmap::fromImage(digits[digitId]->getImage()));
}

void MainWindow::on_pushButtonGo_clicked()
{
    onTimerAdvance();


}
