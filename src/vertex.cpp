#include "vertex.h"


void Vertex::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Ensure the entire circle's bounding area is updated, preventing clipping
    painter.setBrush(Qt::gray);
    painter.drawEllipse(circleRect.adjusted(-1, -1, 1, 1));
}

void Vertex::mousePressEvent(QMouseEvent* event)
{
    if (circleRect.contains(event->pos())) {
        isDragging = true;
        dragStartPos = event->pos() - circleRect.topLeft();
    }
    else {
        QWidget::mousePressEvent(event);
    }
}

void Vertex::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        QPoint newTopLeft = event->pos() - dragStartPos;
        circleRect.moveTopLeft(newTopLeft);
        update();
    }
}

void Vertex::mouseReleaseEvent(QMouseEvent* event)
{
    isDragging = false;
}
