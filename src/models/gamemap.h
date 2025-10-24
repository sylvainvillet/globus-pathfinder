/*
 * gamemap.h
 *
 * Represents the game map: grid of Tile objects, start/target positions,
 * loading/saving via QJson, and utility helpers.
 *
 * Author: Sylvain Villet
 */
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QString>
#include <QPoint>
#include "tile.h"

class GameMap : public QObject {
    Q_OBJECT

public:
    explicit GameMap(QObject* parent = nullptr);

    bool loadDefault();
    bool loadFromFile(const QString& filePath);
    bool loadFromJson(const QJsonObject& json);
    QJsonObject saveToJson() const;

    int width() const { return m_width; }
    int height() const { return m_height; }
    const QVector<QVector<Tile>>& tiles() const { return m_tiles; }

    QPoint startPosition() const;
    QPoint targetPosition() const;

    bool isPointInMap(QPoint point) const;
    bool areCoordinatesInMap(int row, int col) const;

public slots:
    void setStart(int row, int col);
    void setTarget(int row, int col);

signals:
    void mapLoaded();

private:
    int m_width = 0;
    int m_height = 0;
    QVector<QVector<Tile>> m_tiles;
};
