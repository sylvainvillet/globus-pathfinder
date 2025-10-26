#include "tileitem.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>

TileItem::TileItem(int row, int col, const Tile& tile, qreal size)
    : QGraphicsRectItem(col * size, row * size, size, size),
    m_row(row), m_col(col), m_tile(tile) {
    setBrush(QBrush(color(tile)));
    setPen(QPen(Qt::black));
    setAcceptHoverEvents(true);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsRectItem::mousePressEvent(event);
    clicked(m_row, m_col);
}

QColor TileItem::color(Tile tile) const {
    switch (tile.type()) {
    case Tile::Type::Start:     return Qt::red;
    case Tile::Type::Target:    return Qt::blue;
    case Tile::Type::Elevated:  return Qt::green;
    case Tile::Type::Reachable: return Qt::white;
    }
}
