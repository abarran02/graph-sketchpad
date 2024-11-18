#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vertex.h"
#include "canvas.h"

#include <vector>
#include <QMainWindow>
#include <QSize>
#include <QPoint>
#include <QPushButton>

// forward declarations
class Vertex;
class Canvas;

#include <QVBoxLayout>

const int MAX_VERTICES = 128;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

protected:
	void resizeEvent(QResizeEvent* event);
	void edgeToggle(bool checked);
	void deleteToggle(bool checked);

private slots:
	void handleNewVertex(const QPoint& pos);

private:
	Canvas* canvas;

	std::vector<Vertex*> vertices;
	std::vector<std::vector<int>> adjacencyMatrix;

	bool edgeMode = false;
	bool deleteMode = false;
};

#endif // MAINWINDOW_H
