#pragma once

#include <vector>
#include <QMainWindow>
#include <QTimer>
#include "brain.hpp"
#include "image.hpp"
#include "brain_form.hpp"
#include "ui_brain_form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_spinBoxImageId_valueChanged(int value);
    void onTimerAdvance();
    void onTimerRefresh();

    void on_pushButtonGo_clicked();

private:
    Ui::MainWindow * ui;
    std::vector<BrainForm *> brainForms;

    std::vector<Brain*> brains;
    std::vector<Digit*> images;


    int imageCount;

    int brainCount;
    int neuronPerBrain;
    int loopPerCompute;

    int steps;

    QTimer * timerAdvance;
    QTimer * timerRefresh;
    int imageId;
};
