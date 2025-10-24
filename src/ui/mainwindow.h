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

    GameMap m_map;
    QVector<QPoint> path;

    MapView* m_map_view = nullptr;
};
