#include "canvas.h"

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));

	// iterate over vertices to find edges to draw
	for (int vertex = 0; vertex < MAX_VERTICES; vertex++) {
		for (int edge = 0; edge < MAX_VERTICES; edge++) {
			if (adjacencyMatrix[vertex][edge]) {
				Vertex* from = vertices[vertex];
				Vertex* to = vertices[edge];

				painter.drawLine(from->circleRect.center(), to->circleRect.center());
			}
		}
	}
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	int vertexIdx = findVertexUnderMouse(event->pos());

	if (vertexIdx != -1) {
		if (deleteMode) {
			handleDeleteVertex(vertexIdx);
		}
		else {
			handleVertexAction(vertexIdx, event->pos());
		}
	}
	else if (!edgeMode) {
		// Create a new vertex if it's not in edge mode
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

void Canvas::setCurrentVertex(Vertex* v, int idx)
{
	currentVertex = v;
	currentVertexIdx = idx;
}

void Canvas::setLastVertex(Vertex* v, int idx)
{
	lastVertex = v;
	lastVertexIdx = idx;
}

void Canvas::handleDeleteVertex(int vertexIdx) {
	delete vertices[vertexIdx];  // calls the destructor of Vertex to clear paint
	vertices.erase(vertices.begin() + vertexIdx);  // remove from vertices vector
	std::fill(adjacencyMatrix[vertexIdx].begin(), adjacencyMatrix[vertexIdx].end(), 0);  // remove edge row

	// remove edge column
	for (auto& row : adjacencyMatrix) {
		row[vertexIdx] = 0;
	}
}

// Handle actions when clicking on a vertex
void Canvas::handleVertexAction(int vertexIdx, const QPoint& pos) {
	setLastVertex(currentVertex, currentVertexIdx);
	setCurrentVertex(vertices[vertexIdx], vertexIdx);

	if (edgeMode && lastVertex) {
		adjacencyMatrix[lastVertexIdx][currentVertexIdx] = 1;
		adjacencyMatrix[currentVertexIdx][lastVertexIdx] = 1;
		setLastVertex(nullptr, -1);
		setCurrentVertex(nullptr, -1);
		this->update();
	}
	else {
		dragStartPos = pos - currentVertex->circleRect.topLeft();
	}
}
