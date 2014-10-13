#ifndef FORM_TESTINFO_H
#define FORM_TESTINFO_H

#include <QWidget>

namespace Ui {
class Form_TestInfo;
}

class Form_TestInfo : public QWidget
{
    Q_OBJECT

public:
    explicit Form_TestInfo(QWidget *parent = 0);
    ~Form_TestInfo();

private:
    Ui::Form_TestInfo *ui;
};

#endif // FORM_TESTINFO_H
