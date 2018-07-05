/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#pragma once

#include <vector>

namespace aacio {

template<typename T>
class consumer_core {
public:
    virtual ~consumer_core() {}
    virtual void consume(const std::vector<T>& elements) = 0;
};

} // namespace


