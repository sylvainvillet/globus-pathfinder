#include "mapview.h"
#include "tileitem.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

constexpr int TILE_SIZE = 24;

MapView::MapView(const GameMap& map, QWidget* parent)
    : QGraphicsView(parent),
    m_map(map),
    m_scene(QGraphicsScene(this)) {
    setScene(&m_scene);
    setRenderHint(QPainter::Antialiasing);
}

void MapView::draw() {
    // Removes and deletes all items from the scene
    m_scene.clear();

    // Create new map
    for (int y = 0; y < m_map.height(); ++y) {
        for (int x = 0; x < m_map.width(); ++x) {
            const Tile& tile = m_map.tiles()[y][x];
            TileItem* item = new TileItem(QPoint(x, y), tile, TILE_SIZE);

            connect(item, &TileItem::clicked, this, &MapView::tileClicked);
            m_scene.addItem(item);
        }
    }

    drawPaths();
}

void MapView::drawPaths() {
    const QVector<BattleUnit>& units = m_map.m_units;
    if (units.isEmpty()) return;

    QPen pen(Qt::red);
    pen.setWidth(2);

    for (const BattleUnit& unit : units) {
        const QVector<QPoint>& path = unit.m_path;

        // Draw lines connecting the center of the tiles
        for (int i = 0; i < path.size() - 1; ++i) {
            QPoint from = path[i];
            QPoint to = path[i + 1];

            qreal x1 = from.x() * TILE_SIZE + TILE_SIZE / 2.0;
            qreal y1 = from.y() * TILE_SIZE + TILE_SIZE / 2.0;
            qreal x2 = to.x() * TILE_SIZE + TILE_SIZE / 2.0;
            qreal y2 = to.y() * TILE_SIZE + TILE_SIZE / 2.0;

            m_scene.addLine(x1, y1, x2, y2, pen);
        }
    }
}

void MapView::resizeEvent(QResizeEvent* event) {
    fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}
