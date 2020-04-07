#ifndef DOCTITLEDIALOG_H
#define DOCTITLEDIALOG_H

#include <QDialog>

namespace Ui {
class DocTitleDialog;
}

class DocTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocTitleDialog(QWidget *parent = nullptr);
    ~DocTitleDialog();

    QString getDocumentTitle();

private:
    Ui::DocTitleDialog *ui;
};

#endif // DOCTITLEDIALOG_H
