#include "pathfinder.h"
#include <QSet>
#include <QMap>
#include <algorithm>

PathFinder::PathFinder(const GameMap& map)
    : m_map(map) {}

// Manhattan distance
int PathFinder::heuristic(const QPoint& a, const QPoint& b) const {
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}

// Returns all the possible neighbors coordinates
QVector<QPoint> PathFinder::neighbors(const QPoint& pos) const {
    QVector<QPoint> result;

    // Check all 4 neighbours
    for (QPoint delta : m_neighbors_deltas) {
        QPoint neighbor = pos + delta;
        if (!m_map.isPointInMap(neighbor))
            continue;

        const Tile& tile = m_map.tiles()[neighbor.y()][neighbor.x()];
        if (tile.isElevated()) // can't go there
            continue;

        result.append(neighbor);
    }
    return result;
}

// Rebuilds a vector of points corresponding to the path
QVector<QPoint> PathFinder::reconstructPath(Node* endNode) const {
    QVector<QPoint> path;
    Node* current = endNode;
    while (current) {
        path.prepend(current->pos);
        current = current->parent;
    }
    return path;
}

void PathFinder::findPath(BattleUnit& unit) const {
    QPoint startPos = unit.m_position;
    QPoint targetPos = unit.m_target;

    // Check start and target validity
    if (!m_map.isPointInMap(startPos) || !m_map.isPointInMap(targetPos)) {
        unit.m_path = {};
        return;
    }

    QHash<QPoint, Node*> allNodes;
    auto cmp = [](Node* a, Node* b){ return a->fCost() > b->fCost(); };

    QVector<Node*> openList;
    QSet<QPoint> closedSet;

    // Setup starting point with manhattan distance to target
    Node* startNode = new Node{startPos, 0, heuristic(startPos, targetPos), nullptr};
    openList.append(startNode);
    allNodes[startPos] = startNode;

    while (!openList.isEmpty()) {
        // Pop node with lowest fCost
        std::sort(openList.begin(), openList.end(), cmp);
        Node* current = openList.takeLast();

        // Check if we're at the target
        if (current->pos == targetPos) {
            QVector<QPoint> path = reconstructPath(current);
            qDeleteAll(allNodes); // free memory
            unit.m_path = path;
            return;
        }

        closedSet.insert(current->pos);

        QVector<QPoint> currentNeighbors = neighbors(current->pos);
        for (QPoint newPos : currentNeighbors) {
            // Ignore closed neighbors
            if (closedSet.contains(newPos))
                continue;

            // uniform cost of 1 for all steps
            int tentativeG = current->gCost + 1;

            // Check if the node is already in the list
            Node* neighbor = allNodes.value(newPos, nullptr);
            if (!neighbor) {
                // Not in the list, add it
                neighbor = new Node{newPos, tentativeG, heuristic(newPos, targetPos), current};
                allNodes[newPos] = neighbor;
                openList.append(neighbor);
            } else if (tentativeG < neighbor->gCost) {
                // Already in the list, and new cost is lower
                neighbor->gCost = tentativeG;
                neighbor->parent = current;
            }
        }
    }

    qDeleteAll(allNodes); // no path found
    unit.m_path = {};
}
