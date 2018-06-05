#pragma once
#include <aaciolib/aacio.hpp>
#include <aaciolib/transaction.hpp>

namespace aacio {

   class multisig : public contract {
      public:
         multisig( account_name self ):contract(self){}

         void propose();
         void approve( account_name proposer, name proposal_name, permission_level level );
         void unapprove( account_name proposer, name proposal_name, permission_level level );
         void cancel( account_name proposer, name proposal_name, account_name canceler );
         void exec( account_name proposer, name proposal_name, account_name executer );

      private:
         struct proposal {
            name                       proposal_name;
            vector<permission_level>   requested_approvals;
            vector<permission_level>   provided_approvals;
            vector<char>               packed_transaction;

            auto primary_key()const { return proposal_name.value; }
         };
         typedef aacio::multi_index<N(proposal),proposal> proposals;
   };

} /// namespace aacio
