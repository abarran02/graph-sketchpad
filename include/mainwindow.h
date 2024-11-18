#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vertex.h"
#include "canvas.h"
#include "mode.h"

#include <vector>
#include <QMainWindow>
#include <QSize>
#include <QPoint>
#include <QPushButton>
#include <QButtonGroup>
#include <QOverload>

// forward declarations
class Vertex;
class Canvas;

#include <QVBoxLayout>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

protected:
	void resizeEvent(QResizeEvent* event);
	void changeMode(int mode);

private slots:
	void handleNewVertex(const QPoint& pos);

private:
	void addModeButton(const QString& text, Mode mode, QButtonGroup* modeGroup, QVBoxLayout* buttonLayout);
	Canvas* canvas;

	std::vector<Vertex*> vertices;
	std::vector<std::vector<int>> adjacencyMatrix;

	Mode currentMode = basic;
};

#endif // MAINWINDOW_H
