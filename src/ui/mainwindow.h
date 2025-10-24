/*
 * mainwindow.h
 *
 * Main application window for the Globus Pathfinder UI.
 * Hosts the map view and exposes actions to import/export maps
 * and to compute paths on the current GameMap.
 *
 * Author: Sylvain Villet
 */
#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mapview.h"
#include "../models/gamemap.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void importJsonMap();
    void exportJsonMap();
    void exportJsonPath();
    void refresh();
    void findPath();

private:
    void createMenus();

    GameMap* m_map = nullptr;
    QVector<QPoint> path;

    MapView* m_map_view = nullptr;
};
