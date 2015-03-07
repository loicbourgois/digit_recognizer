#ifndef BRAIN_FORM_HPP
#define BRAIN_FORM_HPP

#include <QWidget>

namespace Ui {
class BrainForm;
}

class BrainForm : public QWidget
{
    Q_OBJECT

public:
    explicit BrainForm(QWidget *parent = 0);
    ~BrainForm();
    Ui::BrainForm *ui;
};

#endif // BRAIN_FORM_HPP
