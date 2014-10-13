#ifndef FORM_LOGTEXT_H
#define FORM_LOGTEXT_H

#include <QWidget>

namespace Ui {
class Form_LogText;
}

class Form_LogText : public QWidget
{
    Q_OBJECT

public:
    explicit Form_LogText(QWidget *parent = 0);
    ~Form_LogText();

    void append_log(QString log);

    void save_log();
    void clearn_log();

private:
    Ui::Form_LogText *ui;
};

#endif // FORM_LOGTEXT_H
