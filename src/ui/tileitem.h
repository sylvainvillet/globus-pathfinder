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
    TileItem(int row, int col, const Tile& tile, qreal size);

signals:
    void clicked(int row, int col);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_row;
    int m_col;
    Tile m_tile;

    QColor color(Tile tile) const;
};
