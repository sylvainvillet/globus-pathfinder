#include "gamestate.h"
#include "pathfinder.h"

GameState::GameState(QObject *parent)
    : QObject{parent},
    m_clock(QTimer(this)),
    m_map(GameMap(this)),
    m_path_finder(PathFinder(m_map)) {

    m_clock.setInterval(200);
    m_clock.setSingleShot(false);
    m_clock.start();
    connect(&m_clock, &QTimer::timeout, this, &GameState::tick);

    connect(&m_map, &GameMap::mapLoaded, this, &GameState::findPath);
}

void GameState::tick() {
    // Other actions could be added here like shooting or taking damage
    travel();
}

void GameState::travel() {
    for(BattleUnit& unit : m_map.m_units) {
        // Nothing to do if path is empty
        if (unit.m_path.isEmpty())
            return;

        // Path starts with the current position, so pop it
        unit.m_path.pop_front();

        // If path was only containing the current position, then stop
        if (unit.m_path.isEmpty())
            return;

        // Get the destination
        QPoint destination = unit.m_path.first();

        m_map.moveUnit(unit.m_position, destination);
        unit.m_position = destination;
    }
    emit m_map.mapChanged();
}

void GameState::setTarget(QPoint position) {
    if (!m_map.isTileReachable(position))
        return;

    for(BattleUnit& unit : m_map.m_units) {
        unit.m_target = position;
    }
    m_map.setTarget(position);
    findPath();
}

void GameState::findPath() {
    for(BattleUnit& unit : m_map.m_units) {
        m_path_finder.findPath(unit);
    }
    emit m_map.mapChanged();
}
