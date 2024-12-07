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
	void updateStats();

private:
	void addModeButton(const QString& text, Mode mode, QButtonGroup* modeGroup, QVBoxLayout* buttonLayout);
	void addColors();

	int getComponentCount();

	Canvas* canvas;
	QComboBox* colorBox;
	QLabel* stats;

	std::vector<Vertex*> vertices;

	Eigen::MatrixXd adjacencyMatrix;
	Eigen::MatrixXd degreeMatrix;

	Mode currentMode = basic;
};

#endif // MAINWINDOW_H
