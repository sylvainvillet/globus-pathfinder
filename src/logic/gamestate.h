/*
 * gamestate.h
 *
 * State management of the game, including the main clock to move the battle units
 *
 * Author: Sylvain Villet
 */
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QTimer>
#include "../models/gamemap.h"
#include "pathfinder.h"

class GameState : public QObject
{
    Q_OBJECT
public:
    explicit GameState(QObject *parent = nullptr);

    GameMap& map() { return m_map; }

public slots:
    void setTarget(QPoint position);

signals:
    void redraw();

private slots:
    void tick();
    void findPath();
    void travel();

private:
    QTimer m_clock;

    GameMap m_map;
    PathFinder m_path_finder;
    QVector<QPoint> m_path;
    QVector<QPoint> m_last_path;

};

#endif // GAMESTATE_H
