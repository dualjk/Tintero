#ifndef URIDIALOG_H
#define URIDIALOG_H

#include <QDialog>

namespace Ui {
class UriDialog;
}

class UriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UriDialog(QWidget *parent = nullptr);
    void setUri(QString s);
    void setLabel(QString s);
    QString getUri();
    ~UriDialog();

private:
    Ui::UriDialog *ui;
};

#endif // URIDIALOG_H
