#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionExport_triggered();

    void on_actionCut_triggered();

    void on_actionAbout_Tintero_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

private:
    Ui::client *ui;
};

#endif // CLIENT_H
