#ifndef CANVAS_H
#define CANVAS_H

#include "edge.h"
#include "mode.h"
#include "vertex.h"

#include <algorithm>
#include <QColor>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QLabel>

class Canvas : public QWidget
{
	Q_OBJECT

public:
	Canvas(QWidget* parent, std::vector<Vertex*>& vertices, std::vector<std::vector<int>>& adjacencyMatrix, std::vector<std::vector<int>>& degreeMatrix)
		: QWidget(parent), vertices(vertices), adjacencyMatrix(adjacencyMatrix), degreeMatrix(degreeMatrix) {}

	void paintEvent(QPaintEvent* event) override;
	void setMode(Mode mode);
	void setColor(QColor color);

signals:
	void newVertex(const QPoint& pos);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	QPoint getCenter(const QPoint& p1, const QPoint& p2);
	int findVertexUnderMouse(const QPoint& pos);

	void setCurrentVertex(Vertex* v, int idx);
	void setLastVertex(Vertex* v, int idx);

	bool removeClickedEdge(const QPoint& pos);
	Edge* findMatchingEdge(Vertex* v1, Vertex* v2);

	void removeVectorRowCol(std::vector<std::vector<int>>& matrix, int removeIdx);
	void handleDeleteVertex(int vertexIdx);
	void handleVertexAction(int vertexIdx, const QPoint& pos);

	int getVertexIdx(Vertex* vertex);

	QPoint dragStartPos;
	Vertex* currentVertex = nullptr;
	int currentVertexIdx = -1;
	Vertex* lastVertex = nullptr;
	int lastVertexIdx = -1;

	QColor currentColor = Qt::gray;
	Mode currentMode = basic;
	std::vector<Vertex*>& vertices;
	std::vector<Edge*> edges;  // tracks edges for mouse interaction
	std::vector<std::vector<int>>& adjacencyMatrix;
	std::vector<std::vector<int>>& degreeMatrix;
};

#endif
