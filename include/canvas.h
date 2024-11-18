#ifndef CANVAS_H
#define CANVAS_H

#include "vertex.h"
#include "mode.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class Canvas : public QWidget
{
	Q_OBJECT

public:
	Canvas(QWidget* parent, std::vector<Vertex*>& vertices, std::vector<std::vector<int>>& adjacencyMatrix)
		: QWidget(parent), vertices(vertices), adjacencyMatrix(adjacencyMatrix) {
	}

	void paintEvent(QPaintEvent* event) override;
	void setMode(Mode mode);

signals:
	void newVertex(const QPoint& pos);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	int findVertexUnderMouse(const QPoint& pos);

	void setCurrentVertex(Vertex* v, int idx);
	void setLastVertex(Vertex* v, int idx);

	void handleDeleteVertex(int vertexIdx);
	void handleVertexAction(int vertexIdx, const QPoint& pos);

	QPoint dragStartPos;
	Vertex* currentVertex = nullptr;
	int currentVertexIdx = -1;
	Vertex* lastVertex = nullptr;
	int lastVertexIdx = -1;

	Mode currentMode = basic;
	std::vector<Vertex*>& vertices;  // this is awful and I am sorry
	std::vector<std::vector<int>>& adjacencyMatrix;
};

#endif
