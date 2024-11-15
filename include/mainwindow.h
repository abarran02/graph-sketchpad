#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vertex.h"
#include "canvas.h"

#include <vector>
#include <QMainWindow>
#include <QSize>
#include <QPoint>

class Canvas;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void handleNewVertex(const QPoint& pos);

private:
    Canvas* canvas;
    std::vector<Vertex*> vertices;
};

#endif // MAINWINDOW_H
