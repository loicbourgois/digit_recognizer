#pragma once

#include <vector>
#include <QMainWindow>
#include <QTimer>
#include "brain.hpp"
#include "digit.hpp"

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
    Ui::MainWindow *ui;
    std::vector<Brain*> brains;
    std::vector<Digit*> digits;

    int brainCount;
    int neuronPerBrain;
    int digitCount;

    QTimer * timerAdvance;
    QTimer * timerRefresh;
    int digitId;
};
