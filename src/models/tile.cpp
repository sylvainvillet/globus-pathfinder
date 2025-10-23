#include "tile.h"

Tile::Tile(double value) {
    if (value == -1.0) {
        m_type = Type::Reachable;
    } else if (value == 0.0) {
        m_type = Type::Start;
    } else if (value >= 8.0) {
        m_type = Type::Target;
    } else {
        // Consider unknown values as elevated
        m_type = Type::Elevated;
    }
}

QColor Tile::color() const {
    switch (m_type) {
    case Type::Start:     return Qt::red;
    case Type::Target:    return Qt::blue;
    case Type::Elevated:  return Qt::green;
    case Type::Reachable: return Qt::white;
    }
}
