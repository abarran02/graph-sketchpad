#include "vertex.h"

void Vertex::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// Ensure the entire circle's bounding area is updated, preventing clipping
	painter.setBrush(Qt::gray);
	painter.drawEllipse(circleRect.adjusted(-1, -1, 1, 1));
}
