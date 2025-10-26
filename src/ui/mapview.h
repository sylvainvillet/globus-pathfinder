/*
 * mapview.h
 *
 * QGraphicsView-based widget to render a GameMap and optional path.
 * Provides signals for left/right tile clicks.
 *
 * Author: Sylvain Villet
 */
#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QPoint>
#include "../models/gamemap.h"
#include "../models/tile.h"

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView(QWidget* parent = nullptr);

    void draw(const GameMap& map, const QVector<QPoint>& path);        // Provide the map to draw

signals:
    void tileClicked(int row, int col);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawMap();
    void drawPath(const QVector<QPoint>& path);

    QGraphicsScene* m_scene;
    GameMap* m_map;
    QVector<QGraphicsRectItem*> m_tileItems;   // Keep track of tiles
};
