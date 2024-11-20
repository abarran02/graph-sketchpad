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

	QVBoxLayout* buttonLayout = new QVBoxLayout();
	QButtonGroup* modeGroup = new QButtonGroup(this);

	addModeButton("Basic Mode", basic, modeGroup, buttonLayout);
	addModeButton("Edge Mode", edge, modeGroup, buttonLayout);
	addModeButton("Delete Mode", del, modeGroup, buttonLayout);

	// Push buttons to top
	buttonLayout->addStretch();

	// Button column, max width 200px
	QWidget* buttonColumn = new QWidget(this);
	buttonColumn->setLayout(buttonLayout);
	buttonColumn->setFixedWidth(200);

	// Main layout
	QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
	mainLayout->addWidget(buttonColumn);
	mainLayout->addWidget(canvas);

	// Connect button group to mode toggle handler
	connect(modeGroup, &QButtonGroup::idClicked, this, &MainWindow::changeMode);

	// Set initial mode
	modeGroup->button(Mode::basic)->setChecked(true);  // Assuming basic mode is default

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
	if (currentMode == basic && vertices.size() < MAX_VERTICES) {
		Vertex* v = new Vertex(canvas, QRect(pos.x() - 25, pos.y() - 25, 50, 50));
		v->show();

		vertices.push_back(v);
	}
}

void MainWindow::changeMode(int mode) {
	currentMode = static_cast<Mode>(mode);
	canvas->setMode(currentMode);
}

void MainWindow::addModeButton(const QString& text, Mode mode, QButtonGroup* modeGroup, QVBoxLayout* buttonLayout) {
	QPushButton* button = new QPushButton(text, this);
	button->setCheckable(true);
	modeGroup->addButton(button, mode);
	buttonLayout->addWidget(button);
}