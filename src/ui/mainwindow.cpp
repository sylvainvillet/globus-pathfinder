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
    : QMainWindow(parent), 
    m_map(new GameMap(this)), 
    m_map_view(new MapView(this)) {
    setCentralWidget(m_map_view);

    connect(m_map_view, &MapView::tileLeftClicked, m_map, &GameMap::setStart);
    connect(m_map_view, &MapView::tileRightClicked, m_map, &GameMap::setTarget);

    // Use queued connections to avoid redrawing the map before the click handling is finished
    connect(m_map_view, &MapView::tileLeftClicked, this, &MainWindow::refresh, Qt::QueuedConnection);
    connect(m_map_view, &MapView::tileRightClicked, this, &MainWindow::refresh, Qt::QueuedConnection);

    // Set tooltip text
    statusBar()->showMessage("Left click: Start | Right click: Target");

    createMenus();

    connect(m_map, &GameMap::mapLoaded, this, &MainWindow::refresh);

    if (!m_map->loadDefault()) {
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

    if (!m_map->loadFromFile(filePath)) {
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
    QJsonObject mapJson = m_map->saveToJson();
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
    m_map_view->draw(*m_map, path);
}

void MainWindow::findPath() {
    PathFinder pf(*m_map);
    path = pf.findPath();
}
