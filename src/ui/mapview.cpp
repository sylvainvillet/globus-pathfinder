#include "mapview.h"
#include "tileitem.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

constexpr int TILE_SIZE = 24;

MapView::MapView(QWidget* parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)), m_map(nullptr) {
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
}

void MapView::draw(const GameMap& map, const QVector<QPoint> &path) {
    // Removes and deletes all items from the scene
    m_scene->clear();

    // Create new map
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            const Tile& tile = map.tiles()[y][x];
            TileItem* item = new TileItem(y, x, tile, TILE_SIZE);

            connect(item, &TileItem::leftClicked, this, &MapView::tileLeftClicked);
            connect(item, &TileItem::rightClicked, this, &MapView::tileRightClicked);
            m_scene->addItem(item);
        }
    }

    drawPath(path);
}

void MapView::drawPath(const QVector<QPoint>& path) {
    if (path.isEmpty()) return;

    QPen pen(Qt::red);
    pen.setWidth(2);

    // Draw lines connecting the center of the tiles
    for (int i = 0; i < path.size() - 1; ++i) {
        QPoint from = path[i];
        QPoint to = path[i + 1];

        qreal x1 = from.x() * TILE_SIZE + TILE_SIZE / 2.0;
        qreal y1 = from.y() * TILE_SIZE + TILE_SIZE / 2.0;
        qreal x2 = to.x() * TILE_SIZE + TILE_SIZE / 2.0;
        qreal y2 = to.y() * TILE_SIZE + TILE_SIZE / 2.0;

        m_scene->addLine(x1, y1, x2, y2, pen);
    }
}

void MapView::resizeEvent(QResizeEvent* event) {
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}
