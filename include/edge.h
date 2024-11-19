#ifndef EDGE_H
#define EDGE_H

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
};

#endif
