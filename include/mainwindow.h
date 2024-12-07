#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vertex.h"
#include "canvas.h"
#include "mode.h"

#include <Eigen/Dense>
#include <vector>
#include <QColor>
#include <QComboBox>
#include <QLabel>
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
	void addColors();
	Canvas* canvas;
	QComboBox* colorBox;

	std::vector<Vertex*> vertices;
	std::vector<std::vector<int>> adjacencyMatrix;
	std::vector<std::vector<int>> degreeMatrix;

	Mode currentMode = basic;
};

#endif // MAINWINDOW_H
