#ifndef ClickableFrame_H
#define ClickableFrame_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class ClickableFrame : public QFrame
{
    Q_OBJECT

public:
    ClickableFrame(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setIndex(int i);
    int getIndex();
    ~ClickableFrame();

private:
    int index;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);


};

#endif // ClickableFrame_H
