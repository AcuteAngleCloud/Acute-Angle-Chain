/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#pragma once

#include "consumer_core.h"

#include <aacio/chain/block_state.hpp>

namespace aacio {

class block_storage : public consumer_core<chain::block_state_ptr>
{
public:
    void consume(const std::vector<chain::block_state_ptr>& blocks) override;
};

} // namespace
