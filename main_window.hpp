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
    void onTimerAutosave();

    void on_pushButtonGo_clicked();
    void on_pushButtonPlay_clicked();
    void on_pushButtonPause_clicked();
    void on_doubleSpinBoxMutationFrequency_valueChanged(double arg1);
    void on_doubleSpinBoxMutationIntensity_valueChanged(double arg1);
    void on_pushButtonReset_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonLoad_clicked();
    void on_spinBoxBrainCount_valueChanged(int arg1);
    void on_spinBoxNeuronPerBrain_valueChanged(int arg1);
    void on_spinBoxLoopPerCompute_valueChanged(int arg1);

private:
    void setSpinboxes();
    void save(QString path);
    void load(QString path);

    Ui::MainWindow * ui;
    std::vector<BrainForm *> brainForms;
    QTimer * timerAdvance;
    QTimer * timerRefresh;
    QTimer * timerAutoSave;

    std::vector<Brain*> brains;
    std::vector<Digit*> images;

    int imageCount;
    int brainCount;
    int neuronPerBrain;
    int loopPerCompute;
    int steps;
    int imageId;
};
