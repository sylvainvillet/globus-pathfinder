/*
 * mapview.h
 *
 * QGraphicsView-based widget to render a GameMap and optional path.
 * Provides signal for tile clicks.
 *
 * Author: Sylvain Villet
 */
#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QPoint>
#include "../models/gamemap.h"

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView(const GameMap& map, QWidget* parent = nullptr);

    void draw();

signals:
    void tileClicked(QPoint position);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawPaths();

    const GameMap& m_map;

    QGraphicsScene m_scene;
    QVector<QGraphicsRectItem*> m_tileItems;   // Keep track of tiles
};
