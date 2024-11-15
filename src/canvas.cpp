#include "canvas.h"

void Canvas::mousePressEvent(QMouseEvent* event)
{
	int vertexIdx = findVertexUnderMouse(event->pos());

	if (vertexIdx != -1) {
		setLastVertex(currentVertex, currentVertexIdx);
		setCurrentVertex(vertices[vertexIdx], vertexIdx);

		if (edgeMode && lastVertex) {
			adjacencyMatrix[lastVertexIdx][currentVertexIdx] = 1;
			adjacencyMatrix[currentVertexIdx][lastVertexIdx] = 1;
		}
		else {
			dragStartPos = event->pos() - currentVertex->circleRect.topLeft();
		}
	}
	else {
		setCurrentVertex(nullptr, -1);
		emit newVertex(event->pos());
	}
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (!edgeMode && currentVertex) {
		QPoint newTopLeft = event->pos() - dragStartPos;
		currentVertex->circleRect.moveTopLeft(newTopLeft);
		currentVertex->update();
	}
}

int Canvas::findVertexUnderMouse(const QPoint& pos)
{
	for (int i = 0; i < vertices.size(); ++i) {
		if (vertices[i]->circleRect.contains(pos))
			return i;
	}
	return -1;
}

void Canvas::setCurrentVertex(Vertex* v, int idx) {
	currentVertex = v;
	currentVertexIdx = idx;
}

void Canvas::setLastVertex(Vertex* v, int idx) {
	lastVertex = v;
	lastVertexIdx = idx;
}
