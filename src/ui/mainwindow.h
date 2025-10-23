#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
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
    void drawMap();
    void drawPath();

private:
    const int tileSize = 24;
    void createMenus();

    GameMap m_map;
    QVector<QPoint> path;

    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
};
