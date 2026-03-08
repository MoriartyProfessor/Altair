#ifndef GWAIHIR_RAYS_HPP
#define GWAIHIR_RAYS_HPP

#include "defs.hpp"

namespace Rays
{
    void init();
    BitBoard get(Direction direction, Square square);
}

#endif // GWAIHIR_RAYS_HPP