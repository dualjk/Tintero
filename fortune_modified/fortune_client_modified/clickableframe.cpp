#include "clickableframe.h"

ClickableFrame::ClickableFrame (QWidget* parent, Qt::WindowFlags f)
    : QFrame(parent)
{

}

ClickableFrame::~ClickableFrame() {}

void ClickableFrame::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

void ClickableFrame::setRandomTitle(QString t){
    this->rndTitle = t;
}

QString ClickableFrame::getRandomTitle(){
    return rndTitle;
}



