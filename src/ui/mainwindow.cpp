#include "mainwindow.h"
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

MainWindow::MainWindow(GameState &state, QWidget* parent)
    : QMainWindow(parent),
    m_state(state),
    m_map_view(MapView(state.map(), this)) {
    setCentralWidget(&m_map_view);

    // Use queued connection to ensure the click event is finished before redrawing
    connect(&m_map_view, &MapView::tileClicked, &m_state, &GameState::setTarget, Qt::QueuedConnection);
    connect(&m_state.map(), &GameMap::mapChanged, &m_map_view, &MapView::draw);

    // Set tooltip text
    statusBar()->showMessage("Click on a tile to move");

    createMenus();

    if (!m_state.map().loadDefault()) {
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

    fileMenu->addSeparator();

    QAction* quitAction = fileMenu->addAction("Quit");
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::importJsonMap() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "Open Map JSON", QString(), "JSON Files (*.json)");
    if (filePath.isEmpty())
        return;

    if (!m_state.map().loadFromFile(filePath)) {
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
    QJsonObject mapJson = m_state.map().saveToJson();
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
