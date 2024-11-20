#include "canvas.h"

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// iterate over vertices to find edges to draw
	for (int vertex = 0; vertex < MAX_VERTICES; vertex++) {
		for (int edge = 0; edge < MAX_VERTICES; edge++) {
			if (adjacencyMatrix[vertex][edge]) {
				Vertex* from = vertices[vertex];
				Vertex* to = vertices[edge];
				Edge* e = findMatchingEdge(from, to);

				painter.setPen(QPen(Qt::red, LINE_WIDTH * e->multiplicity, Qt::SolidLine, Qt::RoundCap));

				if (from == to) {
					// loop
					painter.drawEllipse(to->circleRect.center().x() - 50, to->circleRect.center().y() - 50, 50, 50);
				}
				else {
					painter.drawLine(from->circleRect.center(), to->circleRect.center());
				}
			}
		}
	}
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	int vertexIdx = findVertexUnderMouse(event->pos());

	if (vertexIdx != -1) {
		if (currentMode == del) {
			handleDeleteVertex(vertexIdx);
		}
		else {
			handleVertexAction(vertexIdx, event->pos());
		}
	}
	else if (currentMode == basic && vertices.size() < MAX_VERTICES) {
		// Create a new vertex if it's not in edge mode
		setCurrentVertex(nullptr, -1);
		emit newVertex(event->pos());
	}
	else if (currentMode == del) {
		for (auto it = edges.begin(); it != edges.end();) {
			if ((*it)->contains(event->pos(), LINE_WIDTH * (*it)->multiplicity)) {
				int fromIdx = getVertexIdx((*it)->from);
				int toIdx = getVertexIdx((*it)->to);

				adjacencyMatrix[fromIdx][toIdx] = 0;
				adjacencyMatrix[toIdx][fromIdx] = 0;

				it = edges.erase(it);
				this->update();
			}
			else {
				++it;
			}
		}
	}
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (currentMode == basic && currentVertex) {
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
	// reset old vertex highlight
	if (currentVertex)
		currentVertex->highlighted = false;

	currentVertex = v;
	currentVertexIdx = idx;

	// set new vertex highlight
	if (currentVertex) {
		currentVertex->highlighted = true;
		currentVertex->update();
	}
}

void Canvas::setLastVertex(Vertex* v, int idx)
{
	lastVertex = v;
	lastVertexIdx = idx;
}

void Canvas::setMode(Mode mode) {
	currentMode = mode;
}

void Canvas::handleDeleteVertex(int vertexIdx)
{
	// shift rows up
	for (int i = vertexIdx; i < adjacencyMatrix.size() - 1; ++i) {
		adjacencyMatrix[i] = adjacencyMatrix[i + 1];
	}

	// maintain matrix size at MAX_VERTICES
	adjacencyMatrix.back() = std::vector<int>(adjacencyMatrix.size(), 0);

	// shift columns left
	for (auto& row : adjacencyMatrix) {
		for (int j = vertexIdx; j < row.size() - 1; ++j) {
			row[j] = row[j + 1];
		}
		row.back() = 0;  // set the last column to zero
	}

	// remove edge objects
	for (auto it = edges.begin(); it != edges.end();) {
		if ((*it)->from == vertices[vertexIdx] || (*it)->to == vertices[vertexIdx]) {
			it = edges.erase(it);
		}
		else {
			++it;
		}
	}

	// delete vertex and remove from vertices vector
	if (vertices[vertexIdx]->highlighted)
		setCurrentVertex(nullptr, -1);
	delete vertices[vertexIdx];
	vertices.erase(vertices.begin() + vertexIdx);
}

Edge* Canvas::findMatchingEdge(Vertex* v1, Vertex* v2)
{
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		if (((*it)->from == v1 && (*it)->to == v2)
			|| ((*it)->to == v1 && (*it)->from == v2)) {
			return (*it);
		}
	}
}

void Canvas::handleVertexAction(int vertexIdx, const QPoint& pos)
{
	setLastVertex(currentVertex, currentVertexIdx);
	setCurrentVertex(vertices[vertexIdx], vertexIdx);

	if (currentMode == edge && lastVertex) {
		Edge* e;

		// parallel edge
		if (adjacencyMatrix[lastVertexIdx][currentVertexIdx] == 1) {
			e = findMatchingEdge(currentVertex, lastVertex);
			e->multiplicity++;
		}
		// new edge
		else {
			adjacencyMatrix[lastVertexIdx][currentVertexIdx] = 1;
			adjacencyMatrix[currentVertexIdx][lastVertexIdx] = 1;

			e = new Edge(this, lastVertex, currentVertex);
			edges.push_back(e);
		}

		setLastVertex(nullptr, -1);
		setCurrentVertex(nullptr, -1);
		this->update();
	}
	else {
		dragStartPos = pos - currentVertex->circleRect.topLeft();
	}
}

int Canvas::getVertexIdx(Vertex* vertex)
{
	auto it = std::find(vertices.begin(), vertices.end(), vertex);
	if (it != vertices.end()) {
		return std::distance(vertices.begin(), it);
	}
	else {
		return -1;
	}
}
