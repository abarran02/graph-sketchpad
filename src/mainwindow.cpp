#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), adjacencyMatrix(MAX_VERTICES, std::vector<int>(MAX_VERTICES, 0))
{
	// create central widget
	canvas = new Canvas(this, vertices, adjacencyMatrix);
	setCentralWidget(canvas);

	resize(1280, 720);
	connect(canvas, &Canvas::newVertex, this, &MainWindow::handleNewVertex);

	QPushButton* toggleButton = new QPushButton("Edge Mode", this);
	toggleButton->setCheckable(true);
	connect(toggleButton, &QPushButton::toggled, this, &MainWindow::handleToggle);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QSize newSize = event->size();

	for (Vertex* v : vertices) {
		v->setMinimumSize(newSize);
	}

	QWidget::resizeEvent(event);
}

void MainWindow::handleNewVertex(const QPoint& pos)
{
	if (!edgeMode) {
		Vertex* v = new Vertex(canvas, QRect(pos.x() - 25, pos.y() - 25, 50, 50));
		v->show();

		vertices.push_back(v);
	}
}

void MainWindow::handleToggle(bool checked)
{
	edgeMode = checked;
	canvas->edgeMode = checked;
}
