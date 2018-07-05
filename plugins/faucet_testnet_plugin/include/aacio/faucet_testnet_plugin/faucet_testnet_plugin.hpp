/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once
#include <appbase/application.hpp>
#include <aacio/http_plugin/http_plugin.hpp>

namespace aacio {

using namespace appbase;

class faucet_testnet_plugin : public appbase::plugin<faucet_testnet_plugin> {
public:
   faucet_testnet_plugin();
   ~faucet_testnet_plugin();

   APPBASE_PLUGIN_REQUIRES((http_plugin))
   virtual void set_program_options(options_description&, options_description& cfg) override;
 
   void plugin_initialize(const variables_map& options);
   void plugin_startup();
   void plugin_shutdown();

private:
   std::unique_ptr<struct faucet_testnet_plugin_impl> my;
};

}
