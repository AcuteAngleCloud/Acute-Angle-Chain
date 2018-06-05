#include <aacio/chain/wasm_aacio_constraints.hpp>
#include <aacio/chain/wasm_aacio_injection.hpp>
#include <aacio/chain/wasm_aacio_binary_ops.hpp>
#include <fc/exception/exception.hpp>
#include <aacio/chain/exceptions.hpp>
#include "IR/Module.h"
#include "IR/Operators.h"
#include "WASM/WASM.h"

namespace aacio { namespace chain { namespace wasm_injections {
using namespace IR;
using namespace aacio::chain::wasm_constraints;

std::map<std::vector<uint16_t>, uint32_t> injector_utils::type_slots;
std::map<std::string, uint32_t>           injector_utils::registered_injected;
std::map<uint32_t, uint32_t>              injector_utils::injected_index_mapping;
uint32_t                                  injector_utils::next_injected_index;


void noop_injection_visitor::inject( Module& m ) { /* just pass */ }
void noop_injection_visitor::initializer() { /* just pass */ }

void memories_injection_visitor::inject( Module& m ) {
}
void memories_injection_visitor::initializer() {
}

void data_segments_injection_visitor::inject( Module& m ) {
}
void data_segments_injection_visitor::initializer() {
}
void max_memory_injection_visitor::inject( Module& m ) {
   if(m.memories.defs.size() && m.memories.defs[0].type.size.max > maximum_linear_memory/wasm_page_size)
      m.memories.defs[0].type.size.max = maximum_linear_memory/wasm_page_size;
}
void max_memory_injection_visitor::initializer() {}

uint32_t instruction_counter::icnt = 0;
int32_t  checktime_injector::checktime_idx = -1;

}}} // namespace aacio, chain, injectors
