#include "brain_form.hpp"
#include "ui_brain_form.h"

BrainForm::BrainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrainForm)
{
    ui->setupUi(this);
}

BrainForm::~BrainForm()
{
    delete ui;
}
