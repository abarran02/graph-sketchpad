#include "vertex.h"

void Vertex::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (highlighted) {
		painter.setBrush(Qt::yellow);
	}
	else {
		painter.setBrush(color);
	}

	// Ensure the entire circle's bounding area is updated, preventing clipping
	painter.drawEllipse(circleRect.adjusted(-1, -1, 1, 1));

	// degree label
	QString str = QString::number(degree);
	dLabel->setText(str);
	dLabel->move(circleRect.center());
}
