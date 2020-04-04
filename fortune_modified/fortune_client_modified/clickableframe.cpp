#include "clickableframe.h"

ClickableFrame::ClickableFrame (QWidget* parent, Qt::WindowFlags f)
    : QFrame(parent)
{

}

ClickableFrame::~ClickableFrame() {}

void ClickableFrame::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}


