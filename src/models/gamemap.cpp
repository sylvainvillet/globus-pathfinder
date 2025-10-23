#include "gamemap.h"

#include <QFile>
#include <QPoint>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>
#include <QRandomGenerator>

GameMap::GameMap(QObject* parent)
    : QObject(parent)
{
}

bool GameMap::loadDefault() {
    QFile file(":/map/assets/sample_map.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open default map resource!");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject())
        return false;

    return loadFromJson(doc.object());
}

bool GameMap::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open map file:" << filePath;
        return false;
    }

    const QByteArray data = file.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in map file.";
        return false;
    }

    return loadFromJson(doc.object());
}

bool GameMap::loadFromJson(const QJsonObject& json) {
    if (!json.contains("layers") || !json["layers"].isArray()) {
        qWarning() << "Missing or invalid 'layers' in JSON.";
        return false;
    }

    if (!json.contains("tilesets") || !json["tilesets"].isArray()) {
        qWarning() << "Missing or invalid 'tilesets' in JSON.";
        return false;
    }

    QJsonArray layers = json["layers"].toArray();
    if (layers.isEmpty()) {
        qWarning() << "No layers found in map.";
        return false;
    }

    QJsonObject firstLayer = layers[0].toObject();
    if (!firstLayer.contains("data")) {
        qWarning() << "Missing data in layer.";
        return false;
    }

    QJsonArray tileSets = json["tilesets"].toArray();
    if (tileSets.isEmpty()) {
        qWarning() << "No tilesets found in map.";
        return false;
    }

    QJsonObject firstTileSets = tileSets[0].toObject();
    if (!firstTileSets.contains("tilewidth") || !firstTileSets.contains("tileheight")) {
        qWarning() << "Missing width and height in tilesets.";
        return false;
    }

    QJsonArray dataArray = firstLayer["data"].toArray();
    m_width = firstTileSets["tilewidth"].toInt();
    m_height = firstTileSets["tileheight"].toInt();

    if (dataArray.size() != m_width * m_height) {
        qWarning() << "Data array size: " << dataArray.size() << " does not match width*height: " << m_width * m_height;
        return false;
    }

    m_tiles.resize(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_tiles[y].resize(m_width);
        for (int x = 0; x < m_width; ++x) {
            const int idx = y * m_width + x;
            const double value = dataArray[idx].toDouble();
            Tile tile(value);
            m_tiles[y][x] = tile;
        }
    }

    emit mapLoaded();
    return true;
}

QJsonObject GameMap::saveToJson() const {
    QJsonObject layer;
    QJsonArray data;
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            data.append(static_cast<int>(tiles()[y][x].type()));
    layer["data"] = data;

    QJsonArray layers;
    layers.append(layer);

    QJsonArray tileSets;
    QJsonObject tileSet;

    tileSet["tilewidth"] = m_width;
    tileSet["tileheight"] = m_height;
    tileSets.append(tileSet);

    QJsonObject root;
    root["layers"] = layers;
    root["tilesets"] = tileSets;

    return root;
}

QPoint GameMap::startPosition() const {
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            if (m_tiles[y][x].isStart())
                return QPoint(x, y);
    return QPoint(-1, -1);
}

QPoint GameMap::targetPosition() const {
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            if (m_tiles[y][x].isTarget())
                return QPoint(x, y);
    return QPoint(-1, -1);
}

bool GameMap::isPointInMap(QPoint point) const {
    return areCoordinatesInMap(point.y(), point.x());
}

bool GameMap::areCoordinatesInMap(int row, int col) const {
    return col >= 0 && col < m_width && row >= 0 && row < m_height;
}

void GameMap::setStart(int row, int col) {
    if (areCoordinatesInMap(row, col) && m_tiles[row][col].isReachable()) {
        do {
            QPoint currentStart = startPosition();
            if (isPointInMap(currentStart)) {
                m_tiles[currentStart.y()][currentStart.x()].setType(Tile::Type::Reachable);
            }
        } while(isPointInMap(startPosition()));

        m_tiles[row][col].setType(Tile::Type::Start);
    }
}

void GameMap::setTarget(int row, int col) {
    if (areCoordinatesInMap(row, col) && m_tiles[row][col].type() == Tile::Type::Reachable) {
        do {
            QPoint currentTarget = targetPosition();
            if (isPointInMap(currentTarget)) {
                m_tiles[currentTarget.y()][currentTarget.x()].setType(Tile::Type::Reachable);
            }
        } while(isPointInMap(targetPosition()));

        m_tiles[row][col].setType(Tile::Type::Target);
    }
}
