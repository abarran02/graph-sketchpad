#ifndef EDGE_H
#define EDGE_H

#include "mode.h"
#include "vertex.h"

#include <cmath>
#include <QLabel>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Edge : public QWidget {
	Q_OBJECT

public:
	Edge(QWidget* parent, Vertex* from, Vertex* to) : QWidget(parent), from(from), to(to)
	{
		mLabel = new QLabel(parent);
		mLabel->hide();
	}

	~Edge() {
		delete mLabel;
	}

	bool contains(const QPoint& point, double width);

	Vertex* from;
	Vertex* to;

	int multiplicity = 1;
	QLabel* mLabel;

private:
	bool checkContainedLine(const QPoint& point, const double width);
	bool checkContainedLoop(const QPoint& point, const double width);
};

#endif
