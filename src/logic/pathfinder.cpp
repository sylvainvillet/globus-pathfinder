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
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    // Check all 4 neighbours
    for (int i = 0; i < 4; ++i) {
        int nx = pos.x() + dx[i];
        int ny = pos.y() + dy[i];
        if (!m_map.areCoordinatesInMap(ny, nx))
            continue;

        const Tile& tile = m_map.tiles()[ny][nx];
        if (tile.isElevated()) // can't go there
            continue;

        result.append(QPoint(nx, ny));
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

QVector<QPoint> PathFinder::findPath() const {
    QPoint startPos = m_map.startPosition();
    QPoint targetPos = m_map.targetPosition();

    // Check start and target validity
    if (!m_map.isPointInMap(startPos) || !m_map.isPointInMap(targetPos))
        return {};

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
            return path;
        }

        closedSet.insert(current->pos);

        for (QPoint newPos : neighbors(current->pos)) {
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
    return {};
}
