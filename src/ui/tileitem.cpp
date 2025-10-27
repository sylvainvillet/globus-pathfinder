#include "tileitem.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>

TileItem::TileItem(QPoint position, const Tile& tile, qreal size)
    : QGraphicsRectItem(position.x() * size, position.y() * size, size, size),
    m_position(position),
    m_tile(tile) {
    setBrush(QBrush(color(tile)));
    setPen(QPen(Qt::black));
    setAcceptHoverEvents(true);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsRectItem::mousePressEvent(event);
    clicked(m_position);
}

QColor TileItem::color(Tile tile) const {
    switch (tile.type()) {
    case Tile::Type::Start:     return Qt::red;
    case Tile::Type::Target:    return Qt::blue;
    case Tile::Type::Elevated:  return Qt::green;
    case Tile::Type::Reachable: return Qt::white;
    }
}
