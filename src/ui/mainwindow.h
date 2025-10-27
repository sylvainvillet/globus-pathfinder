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
#include "../logic/gamestate.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(GameState &state, QWidget* parent = nullptr);

private slots:
    void importJsonMap();
    void exportJsonMap();

private:
    void createMenus();

    GameState &m_state;
    MapView m_map_view;
};
