#ifndef CANVAS_H
#define CANVAS_H

#include "vertex.h"
#include "mainwindow.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class Canvas : public QWidget {
    Q_OBJECT

public:
    Canvas(QWidget* parent = nullptr) : QWidget(parent) {}

signals:
    void newVertex(const QPoint& pos);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif