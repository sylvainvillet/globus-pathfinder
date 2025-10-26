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
