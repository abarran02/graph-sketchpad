#ifndef CANVAS_H
#define CANVAS_H

#include "vertex.h"
#include "mainwindow.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class Canvas : public QWidget {
	Q_OBJECT

public:
	Canvas(QWidget* parent, std::vector<Vertex*>& vertices, std::vector<std::vector<int>>& adjacencyMatrix)
		: QWidget(parent), vertices(vertices), adjacencyMatrix(adjacencyMatrix) {
	}

	bool edgeMode = false;

signals:
	void newVertex(const QPoint& pos);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	//void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	int findVertexUnderMouse(const QPoint& pos);

	void setCurrentVertex(Vertex* v, int idx);
	void setLastVertex(Vertex* v, int idx);

	QPoint dragStartPos;
	Vertex* currentVertex = nullptr;
	int currentVertexIdx = -1;
	Vertex* lastVertex = nullptr;
	int lastVertexIdx = -1;

	std::vector<Vertex*>& vertices;  // this is awful and I am sorry
	std::vector<std::vector<int>>& adjacencyMatrix;
};

#endif
