#ifndef VERTEX_H
#define VERTEX_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QSize>


class Vertex : public QWidget {
    Q_OBJECT

public:
    Vertex(QWidget* parent, QRect rect) : QWidget(parent), circleRect(rect)
    {
        setMinimumSize(parent->width(), parent->height());
    }

    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QRect circleRect;
    bool isDragging = false;

    QPoint dragStartPos;
};

#endif
