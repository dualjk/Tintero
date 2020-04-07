#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include <QtWidgets>

#include <doctitledialog.h>

namespace Ui {
class page;
}

class page : public QMainWindow
{
    Q_OBJECT

public:
    explicit page(QWidget *parent = nullptr);
    ~page();

    void setAvatar(QPixmap p);
    void setUsernameLabel(QString a);
    void setGridLayout();
    void newDocumentSetup();
    void newDocumentCreate();

private:
    Ui::page *ui;
};

#endif // PAGE_H
