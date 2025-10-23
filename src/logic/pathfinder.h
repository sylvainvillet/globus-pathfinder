#pragma once

#include <QPoint>
#include <QVector>
#include "../models/gamemap.h"


class PathFinder {
public:
    explicit PathFinder(const GameMap& map);

    // Find the shortest path from start to target using A* algorithm
    // Returns a list of positions from start to target (empty if no path)
    QVector<QPoint> findPath() const;

private:
    const GameMap& m_map;

    struct Node {
        QPoint pos;
        int gCost = 0; // cost from start
        int hCost = 0; // heuristic cost to target
        int fCost() const { return gCost + hCost; }
        Node* parent = nullptr;
    };

    int heuristic(const QPoint& a, const QPoint& b) const;
    QVector<QPoint> reconstructPath(Node* endNode) const;
    QVector<QPoint> neighbors(const QPoint& pos) const;
};
