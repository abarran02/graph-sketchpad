#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), adjacencyMatrix(MAX_VERTICES, std::vector<int>(MAX_VERTICES, 0))
{
	// Create central widget
	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	// Create canvas
	canvas = new Canvas(this, vertices, adjacencyMatrix);
	canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// button column layout
	QVBoxLayout* buttonLayout = new QVBoxLayout();
	// edge toggle button
	QPushButton* edgeButton = new QPushButton("Edge Mode", this);
	edgeButton->setCheckable(true);
	connect(edgeButton, &QPushButton::toggled, this, &MainWindow::edgeToggle);
	buttonLayout->addWidget(edgeButton);

	// delete toggle button
	QPushButton* deleteButton = new QPushButton("Delete Mode", this);
	deleteButton->setCheckable(true);
	connect(deleteButton, &QPushButton::toggled, this, &MainWindow::deleteToggle);
	buttonLayout->addWidget(deleteButton);

	// push buttons to top
	buttonLayout->addStretch();

	// button column, max width 200px
	QWidget* buttonColumn = new QWidget(this);
	buttonColumn->setLayout(buttonLayout);
	buttonColumn->setFixedWidth(200);

	QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
	mainLayout->addWidget(buttonColumn);
	mainLayout->addWidget(canvas);

	resize(1280, 720);
	connect(canvas, &Canvas::newVertex, this, &MainWindow::handleNewVertex);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QSize newSize = canvas->size();

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

void MainWindow::edgeToggle(bool checked)
{
	edgeMode = checked;
	canvas->edgeMode = checked;
}

void MainWindow::deleteToggle(bool checked)
{
	deleteMode = checked;
	canvas->deleteMode = checked;
}
