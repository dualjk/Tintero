#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void setValue(int n);
    int getValue();

    ~ClickableLabel();

signals:
    void clicked();



protected:
    void mousePressEvent(QMouseEvent* event);
    int value;



};

#endif // CLICKABLELABEL_H
