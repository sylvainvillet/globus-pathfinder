#include "mainwindow.h"
#include "../logic/pathfinder.h"
#include "tileitem.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);
    setCentralWidget(m_view);

    // Set tooltip text
    statusBar()->showMessage("Left click: Start | Right click: Target");

    createMenus();

    connect(&m_map, &GameMap::mapLoaded, this, &MainWindow::refresh);

    if (!m_map.loadDefault()) {
        QMessageBox::warning(this, "Error", "Failed to load default map");
    }

    setWindowTitle("Globus Pathfinder");
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* importAction = fileMenu->addAction("Import Map...");
    connect(importAction, &QAction::triggered, this, &MainWindow::importJsonMap);

    QAction* exportMapAction = fileMenu->addAction("Export Map...");
    connect(exportMapAction, &QAction::triggered, this, &MainWindow::exportJsonMap);

    QAction* exportPathAction = fileMenu->addAction("Export Path...");
    connect(exportPathAction, &QAction::triggered, this, &MainWindow::exportJsonPath);

    fileMenu->addSeparator();

    QAction* quitAction = fileMenu->addAction("Quit");
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::importJsonMap() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "Open Map JSON", QString(), "JSON Files (*.json)");
    if (filePath.isEmpty())
        return;

    if (!m_map.loadFromFile(filePath)) {
        QMessageBox::warning(this, "Error", "Failed to load JSON map.");
        return;
    }

    statusBar()->showMessage("Map loaded: " + filePath);
}

void MainWindow::exportJsonMap() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Save JSON Map", QString(), "JSON Files (*.json)");
    if (filePath.isEmpty())
        return;

    // Build JSON object
    QJsonObject mapJson = m_map.saveToJson();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot save file.");
        return;
    }

    QJsonDocument doc(mapJson);
    file.write(doc.toJson());
    file.close();

    statusBar()->showMessage("Map saved: " + filePath);
}

void MainWindow::exportJsonPath() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Save JSON Path", QString(), "JSON Files (*.json)");
    if (filePath.isEmpty())
        return;

    // Build JSON object
    QJsonObject root;
    QJsonArray pathArray;
    for (const QPoint& p : path) {
        QJsonObject pointObj;
        pointObj["x"] = p.x();
        pointObj["y"] = p.y();
        pathArray.append(pointObj);
    }
    root["path"] = pathArray;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot save file.");
        return;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    statusBar()->showMessage("Path saved: " + filePath);
}

void MainWindow::refresh() {
    findPath();
    drawMap();
    drawPath();
}

void MainWindow::findPath() {
    PathFinder pf(m_map);
    path = pf.findPath();
}

void MainWindow::drawMap() {

    m_scene->clear();

    for (int y = 0; y < m_map.height(); ++y) {
        for (int x = 0; x < m_map.width(); ++x) {
            const auto& tile = m_map.tiles()[y][x];
            TileItem* item = new TileItem(y, x, tile, tileSize);

            connect(item, &TileItem::leftClicked, &m_map, &GameMap::setStart);
            connect(item, &TileItem::rightClicked, &m_map, &GameMap::setTarget);

            // Use queued connections to avoid redrawing the map before the click handling is finished
            connect(item, &TileItem::leftClicked, this, &MainWindow::refresh, Qt::QueuedConnection);
            connect(item, &TileItem::rightClicked, this, &MainWindow::refresh, Qt::QueuedConnection);

            m_scene->addItem(item);
        }
    }

    const int width = m_map.width() * tileSize;
    const int height = m_map.height() * tileSize;
    m_scene->setSceneRect(0, 0, width, height);
    resize(width, height + statusBar()->geometry().height());
}

void MainWindow::drawPath() {
    if (path.isEmpty())
        return;

    QPen pen(Qt::red);
    pen.setWidth(2);

    // Draw a line connecting centers of each tile
    for (int i = 0; i < path.size() - 1; ++i) {
        QPoint from = path[i];
        QPoint to = path[i + 1];

        qreal x1 = from.x() * tileSize + tileSize / 2.0;
        qreal y1 = from.y() * tileSize + tileSize / 2.0;
        qreal x2 = to.x() * tileSize + tileSize / 2.0;
        qreal y2 = to.y() * tileSize + tileSize / 2.0;

        m_scene->addLine(x1, y1, x2, y2, pen);
    }
}
