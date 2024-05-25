#ifndef APP_HPP
#define APP_HPP

#include "components/component.hpp"
#include "components/graphics.hpp"

static auto app =
    ComponentGroup(
        (Graphics()));

#endif // APP_HPP