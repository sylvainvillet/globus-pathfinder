/*
 * tileitem.h
 *
 * QGraphics item representing a Tile in the map.
 *
 * Author: Sylvain Villet
 */
#pragma once

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "../models/tile.h"

class TileItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    TileItem(QPoint position, const Tile& tile, qreal size);

signals:
    void clicked(QPoint position);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPoint m_position;
    Tile m_tile;

    QColor color(Tile tile) const;
};
