#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // create central widget
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    Vertex* v = new Vertex(canvas, QRect(10, 10, 50, 50));
    vertices.push_back(v);

    resize(1280, 720);
    connect(canvas, &Canvas::newVertex, this, &MainWindow::handleNewVertex);
}

MainWindow::~MainWindow() {

}

void MainWindow::resizeEvent(QResizeEvent* event)  {
    QSize newSize = event->size();

    for (Vertex* v : vertices) {
        v->setMinimumSize(newSize);
    }

    QWidget::resizeEvent(event);
}

void MainWindow::handleNewVertex(const QPoint& pos) {
    Vertex* v = new Vertex(canvas, QRect(pos.x()-25, pos.y()-25, 50, 50));
    v->show();

    vertices.push_back(v);
}