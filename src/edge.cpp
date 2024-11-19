#include "edge.h"

bool Edge::contains(const QPoint& point, double width) {
    QPoint p1 = from->circleRect.center();
    QPoint p2 = to->circleRect.center();

    // Calculate the squared length of the line segment
    double lineLengthSquared = std::pow(p2.x() - p1.x(), 2) + std::pow(p2.y() - p1.y(), 2);
    if (lineLengthSquared == 0.0)
        return false; // p1 and p2 are the same point

    // Project the point onto the line segment and find the closest point
    double t = ((point.x() - p1.x()) * (p2.x() - p1.x()) + (point.y() - p1.y()) * (p2.y() - p1.y())) / lineLengthSquared;
    t = std::max(0.0, std::min(1.0, t));

    QPoint closestPoint = QPoint(p1.x() + t * (p2.x() - p1.x()), p1.y() + t * (p2.y() - p1.y()));

    // Calculate the distance from the point to the closest point
    double distanceSquared = std::pow(point.x() - closestPoint.x(), 2) + std::pow(point.y() - closestPoint.y(), 2);

    // Check if the distance is within the line's width
    return distanceSquared <= std::pow(width / 2.0, 2);
}
