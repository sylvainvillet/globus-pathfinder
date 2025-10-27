/*
 * pathfinder.h
 *
 * A* pathfinding for GameMap: 
 * computes shortest path between two points on the game map.
 *
 * Author: Sylvain Villet
 */
#pragma once

#include <QPoint>
#include <QVector>
#include "../models/gamemap.h"

class PathFinder {
public:
    explicit PathFinder(const GameMap& map);

    // Find the shortest path from start to target using A* algorithm
    // Returns a list of positions from start to target (empty if no path)
    void findPath(BattleUnit& unit) const;

private:
    const GameMap& m_map;
    const QPoint m_neighbors_deltas[4] = {
        QPoint(1, 0),
        QPoint(-1, 0),
        QPoint(0, 1),
        QPoint(0, -1),
    };

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
