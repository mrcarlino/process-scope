#include "ClickableCard.h"

ClickableCard::ClickableCard(QWidget *parent) : QFrame(parent) 
{
    // Automatically set the mouse cursor to a pointing hand
    setCursor(Qt::PointingHandCursor);
}

void ClickableCard::mouseReleaseEvent(QMouseEvent *event) 
{
    // Only trigger if it was a left click
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    
    // Always pass the event back to the base class
    QFrame::mouseReleaseEvent(event);
}