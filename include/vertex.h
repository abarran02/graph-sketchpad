#ifndef VERTEX_H
#define VERTEX_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Vertex : public QWidget {
	Q_OBJECT

public:
	Vertex(QWidget* parent, QRect rect) : QWidget(parent), circleRect(rect)
	{
		setMinimumSize(parent->width(), parent->height());
	}

	void paintEvent(QPaintEvent* event) override;

	QRect circleRect;

	bool highlighted = false;
	Qt::GlobalColor color = Qt::gray;
};

#endif
