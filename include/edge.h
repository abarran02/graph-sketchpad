#ifndef EDGE_H
#define EDGE_H

#include "mode.h"
#include "vertex.h"

#include <cmath>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Edge : public QWidget {
	Q_OBJECT

public:
	Edge(QWidget* parent, Vertex* from, Vertex* to) : QWidget(parent), from(from), to(to) {}

	bool contains(const QPoint& point, double width);

	Vertex* from;
	Vertex* to;

	int multiplicity = 1;

private:
	bool checkContainedLine(const QPoint& point, const double width);
	bool checkContainedLoop(const QPoint& point, const double width);
};

#endif
