#include "mainwindow.h"
#include "../logic/pathfinder.h"
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
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), 
    m_map(new GameMap(this)), 
    m_map_view(new MapView(this)),
    m_travel_timer(new QTimer(this)){
    setCentralWidget(m_map_view);

    m_travel_timer->setInterval(200);
    m_travel_timer->setSingleShot(false);
    connect(m_travel_timer, &QTimer::timeout, this, &MainWindow::travel);

    connect(m_map_view, &MapView::tileClicked, m_map, &GameMap::setTarget);

    // Use queued connections to avoid redrawing the map before the click handling is finished
    connect(m_map_view, &MapView::tileClicked, this, &MainWindow::refresh, Qt::QueuedConnection);

    // Set tooltip text
    statusBar()->showMessage("Click on a tile to move");

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

    QAction* exportPathAction = fileMenu->addAction("Export Last Path...");
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
    for (const QPoint& p : m_last_path) {
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
    m_map_view->draw(*m_map, m_path);
    if (!m_travel_timer->isActive())
        m_travel_timer->start();
}

void MainWindow::findPath() {
    PathFinder pf(*m_map);
    m_path = pf.findPath();
    // Save a copy for export
    m_last_path = m_path;
}

void MainWindow::travel() {
    // Nothing to do if path is empty
    if (m_path.isEmpty())
        return;

    // Path starts with the current position, so pop it
    m_path.pop_front();

    // If path was only containing the current position, then stop
    if (m_path.isEmpty())
        return;

    // Get the destination
    QPoint destination = m_path.first();

    m_map->setStart(destination.y(), destination.x());
    m_map_view->draw(*m_map, m_path);
}
