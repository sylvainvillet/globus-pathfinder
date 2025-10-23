#include "tileitem.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>

TileItem::TileItem(int row, int col, const Tile& tile, qreal size)
    : QGraphicsRectItem(col * size, row * size, size, size),
    m_row(row), m_col(col), m_tile(tile) {
    setBrush(QBrush(tile.color()));
    setPen(QPen(Qt::black));
    setAcceptHoverEvents(true);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsRectItem::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(m_row, m_col);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    }
    // no actions on middle button
}
