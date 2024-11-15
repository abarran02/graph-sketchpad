#include "canvas.h"


void Canvas::mousePressEvent(QMouseEvent* event)
{
    emit newVertex(event->pos());
}
