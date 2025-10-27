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
#include "battleunit.h"
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
    QVector<BattleUnit> m_units;

    bool isPointInMap(QPoint position) const;
    bool isTileReachable(QPoint position) const;

public slots:
    void moveUnit(QPoint fromPosition, QPoint toPosition);
    void setTarget(QPoint position);

signals:
    void mapLoaded();
    void mapChanged();

private:
    Tile* tileAt(QPoint point);

    int m_width = 0;
    int m_height = 0;
    QVector<QVector<Tile>> m_tiles;
};
