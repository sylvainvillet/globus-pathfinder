/*
 * battleunit.h
 *
 * Represents a single battle unit
 *
 * Author: Sylvain Villet
 */
#ifndef BATTLEUNIT_H
#define BATTLEUNIT_H

#include <QObject>
#include <QPoint>

struct BattleUnit
{
    QVector<QPoint> m_path;
    QPoint m_position;
    QPoint m_target = QPoint(-1, -1);
};

#endif // BATTLEUNIT_H
