#pragma once

#include <QtGlobal>
#include <QColor>

class Tile {
public:
    enum class Type {
        Start     = 0,
        Target    = 8,
        Elevated  = 3,
        Reachable = -1
    };

    Tile(Type type = Type::Reachable) : m_type(type) {}
    Tile(double value);

    Type type() const { return m_type; }
    void setType(Type type) { m_type = type; }

    bool isStart() const      { return m_type == Type::Start; }
    bool isTarget() const     { return m_type == Type::Target; }
    bool isElevated() const   { return m_type == Type::Elevated; }
    bool isReachable() const  { return m_type == Type::Reachable; }

    QColor color() const;

private:
    Type m_type;
};
