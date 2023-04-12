#ifndef ALTAIR_RAYS_HPP
#define ALTAIR_RAYS_HPP

#include "defs.hpp"

namespace Rays
{
    void init();
    BitBoard get(Direction direction, Square square);
}

#endif // ALTAIR_RAYS_HPP