#ifndef VERTEX_H
#define VERTEX_H

#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Vertex : public QWidget {
	Q_OBJECT

public:
	Vertex(QWidget* parent, QRect rect) : QWidget(parent), circleRect(rect)
	{
		setMinimumSize(parent->width(), parent->height());
		dLabel = new QLabel(parent);
		dLabel->show();
	}

	~Vertex()
	{
		delete dLabel;
	}

	void paintEvent(QPaintEvent* event) override;

	QRect circleRect;

	int degree = 0;
	bool highlighted = false;
	QColor color = Qt::gray;
	QLabel* dLabel;
};

#endif
