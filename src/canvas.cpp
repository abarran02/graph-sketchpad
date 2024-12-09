#include "canvas.h"

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	for (Edge* e : edges)
		drawEdge(e, painter);

	// update vertex degrees
	for (int i = 0; i < vertices.size(); i++)
		vertices[i]->degree = degreeMatrix(i, i);
}

void Canvas::updateAdjMatLabel() {
	std::stringstream ss;
	int size = vertices.size();
	if (size == 0) {
		adjMatLabel->setText("");
		return;
	}

	Eigen::MatrixXd subMatrix = adjacencyMatrix.topLeftCorner(size, size);

	for (int i = 0; i < subMatrix.rows(); ++i) {
		for (int j = 0; j < subMatrix.cols(); ++j) {
			ss << subMatrix(i, j) << " ";
		}
		ss << "\n";
	}

	adjMatLabel->setText(QString::fromStdString(ss.str()));
	adjMatLabel->adjustSize();
}

QPoint Canvas::getCenter(const QPoint& p1, const QPoint& p2) {
	return QPoint((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
}

int Canvas::getEdgeCount() {
	int count = 0;

	for (auto it = edges.begin(); it != edges.end(); ++it) {
		count += (*it)->multiplicity;
	}

	return count;
}

int Canvas::getComponentCount(Eigen::MatrixXd& adjMat, Eigen::MatrixXd& degMat) {
	int size = vertices.size();
	if (size == 0)
		return 0;

	Eigen::MatrixXd laplacian = adjMat - degMat;
	Eigen::MatrixXd subMatrix = laplacian.topLeftCorner(size, size);  // otherwise thinks it is 128x128
	Eigen::FullPivLU<Eigen::MatrixXd> lu(subMatrix);
	Eigen::MatrixXd nullSpace = lu.kernel();

	return nullSpace.cols();
}

void Canvas::drawEdge(const Edge* e, QPainter& painter) {
	painter.setPen(QPen(e->bridge ? Qt::blue : Qt::red, LINE_WIDTH, Qt::SolidLine, Qt::RoundCap));

	if (e->from == e->to) {
		// loop
		QPoint center(e->to->circleRect.center().x() - HALF_RADIUS, e->to->circleRect.center().y() - HALF_RADIUS);
		painter.drawEllipse(center, HALF_RADIUS, HALF_RADIUS);
	}
	else {
		painter.drawLine(e->from->circleRect.center(), e->to->circleRect.center());

		if (e->multiplicity > 1) {
			QPoint center = getCenter(e->from->circleRect.center(), e->to->circleRect.center());
			QString str = QString::number(e->multiplicity);

			e->mLabel->setText(str);
			e->mLabel->move(center);
			e->mLabel->show();
		}
		else {
			e->mLabel->hide();
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
		// create new vertex if not edge mode
		setCurrentVertex(nullptr, -1);
		emit newVertex(event->pos());
	}
	else if (currentMode == del) {
		removeClickedEdge(event->pos());
		this->update();
	}
	else if (currentMode == edge) {
		setCurrentVertex(nullptr, -1);
	}

	// determine whether each edge is a bridge
	for (Edge* e : edges) {
		int fromIdx = getVertexIdx(e->from);
		int toIdx = getVertexIdx(e->to);

		// copy matrices to prevent modifying original
		Eigen::MatrixXd bridgeAdjCheck = adjacencyMatrix;
		Eigen::MatrixXd bridgeDegCheck = degreeMatrix;

		// remove edge from copies to check if removal would increase components
		bridgeAdjCheck(fromIdx, toIdx) = 0;
		bridgeAdjCheck(toIdx, fromIdx) = 0;
		bridgeDegCheck(fromIdx, fromIdx) -= e->multiplicity;
		bridgeDegCheck(toIdx, toIdx) -= e->multiplicity;

		// compare component counts
		e->bridge = getComponentCount(bridgeAdjCheck, bridgeDegCheck) != getComponentCount(adjacencyMatrix, degreeMatrix);
	}

	updateAdjMatLabel();

	emit updateStats();
}

bool Canvas::removeClickedEdge(const QPoint& pos) {
	// iterate over edges
	for (auto it = edges.begin(); it != edges.end();) {
		// discover edge containing click
		if ((*it)->contains(pos, LINE_WIDTH)) {
			int fromIdx = getVertexIdx((*it)->from);
			int toIdx = getVertexIdx((*it)->to);

			// remove edge from adjacency matrix, degree matrix, and edge vector
			adjacencyMatrix(fromIdx, toIdx) = 0;
			adjacencyMatrix(toIdx, fromIdx) = 0;

			degreeMatrix(fromIdx, fromIdx)--;
			degreeMatrix(toIdx, toIdx)--;

			delete (*it);
			it = edges.erase(it);

			return true;
		}
		else {
			++it;
		}
	}

	// no matching edge found
	return false;
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
	if (currentVertex) {
		currentVertex->highlighted = false;
		currentVertex->update();
	}

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

void Canvas::setColor(QColor color) {
	currentColor = color;
}

void Canvas::removeMatrixRowCol(Eigen::MatrixXd& matrix, int removeIdx) {
	int rows = matrix.rows();
	int cols = matrix.cols();

	// shift rows up
	if (removeIdx < rows - 1) {
		matrix.block(removeIdx, 0, rows - removeIdx - 1, cols) =
			matrix.block(removeIdx + 1, 0, rows - removeIdx - 1, cols);
	}

	// set the last row to zero
	matrix.row(rows - 1).setZero();

	// shift columns left
	if (removeIdx < cols - 1) {
		matrix.block(0, removeIdx, rows, cols - removeIdx - 1) =
			matrix.block(0, removeIdx + 1, rows, cols - removeIdx - 1);
	}

	// set the last column to zero
	matrix.col(cols - 1).setZero();
}

void Canvas::handleDeleteVertex(int vertexIdx)
{
	// remove edge objects
	for (auto it = edges.begin(); it != edges.end();) {
		if ((*it)->from == vertices[vertexIdx]) {
			int adjIdx = getVertexIdx((*it)->to);
			degreeMatrix(adjIdx, adjIdx)--;  // decrement degrees of adjacent edges
			delete (*it);
			it = edges.erase(it);
		}
		else if ((*it)->to == vertices[vertexIdx]) {
			int adjIdx = getVertexIdx((*it)->from);
			degreeMatrix(adjIdx, adjIdx)--;
			delete (*it);
			it = edges.erase(it);
		}
		else {
			++it;
		}
	}

	// remove vertex from adjacency and degree matrices
	removeMatrixRowCol(adjacencyMatrix, vertexIdx);
	removeMatrixRowCol(degreeMatrix, vertexIdx);

	// delete vertex and remove from vertices vector
	if (vertices[vertexIdx]->highlighted)  // was the current vertex
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
		if (adjacencyMatrix(lastVertexIdx, currentVertexIdx) == 1) {
			e = findMatchingEdge(currentVertex, lastVertex);
			e->multiplicity++;
		}
		// new edge
		else {
			adjacencyMatrix(lastVertexIdx, currentVertexIdx) = 1;
			adjacencyMatrix(currentVertexIdx, lastVertexIdx) = 1;

			e = new Edge(this, lastVertex, currentVertex);
			edges.push_back(e);
		}

		// increment degree of both vertices
		degreeMatrix(currentVertexIdx, currentVertexIdx)++;
		if (currentVertexIdx != lastVertexIdx)  // prevent incrementing by 2 for loops
			degreeMatrix(lastVertexIdx, lastVertexIdx)++;

		setLastVertex(nullptr, -1);
		setCurrentVertex(nullptr, -1);
		this->update();
	}
	else if (currentMode == color) {
		currentVertex->color = currentColor;
		setCurrentVertex(nullptr, -1);
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
