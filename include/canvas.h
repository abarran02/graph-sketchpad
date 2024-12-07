#ifndef CANVAS_H
#define CANVAS_H

#include "edge.h"
#include "mode.h"
#include "vertex.h"

#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <QColor>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QLabel>

class Canvas : public QWidget
{
	Q_OBJECT

public:
	Canvas(QWidget* parent, std::vector<Vertex*>& vertices, Eigen::MatrixXd& adjacencyMatrix, Eigen::MatrixXd& degreeMatrix)
		: QWidget(parent), vertices(vertices), adjacencyMatrix(adjacencyMatrix), degreeMatrix(degreeMatrix)
	{
	}

	void paintEvent(QPaintEvent* event) override;
	void setMode(Mode mode);
	void setColor(QColor color);

	int getEdgeCount();

signals:
	void newVertex(const QPoint& pos);
	void updateStats();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	QPoint getCenter(const QPoint& p1, const QPoint& p2);
	void drawEdge(const Edge* e, QPainter& painter);

	void setCurrentVertex(Vertex* v, int idx);
	void setLastVertex(Vertex* v, int idx);

	int findVertexUnderMouse(const QPoint& pos);
	bool removeClickedEdge(const QPoint& pos);
	Edge* findMatchingEdge(Vertex* v1, Vertex* v2);

	void removeMatrixRowCol(Eigen::MatrixXd& matrix, int removeIdx);
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
	Eigen::MatrixXd& adjacencyMatrix;
	Eigen::MatrixXd& degreeMatrix;
};

#endif
