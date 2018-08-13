/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once

#include <aacio/chain/types.hpp>
#include <aacio/chain/exceptions.hpp>

namespace aacio { namespace chain {

   struct permission_level {
      account_name    actor;
      permission_name permission;
   };

   inline bool operator== (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) == std::tie(rhs.actor, rhs.permission);
   }

   inline bool operator!= (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) != std::tie(rhs.actor, rhs.permission);
   }

   inline bool operator< (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) < std::tie(rhs.actor, rhs.permission);
   }

   inline bool operator<= (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) <= std::tie(rhs.actor, rhs.permission);
   }

   inline bool operator> (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) > std::tie(rhs.actor, rhs.permission);
   }

   inline bool operator>= (const permission_level& lhs, const permission_level& rhs) {
      return std::tie(lhs.actor, lhs.permission) >= std::tie(rhs.actor, rhs.permission);
   }

   /**
    *  An action is performed by an actor, aka an account. It may
    *  be created explicitly and authorized by signatures or might be
    *  generated implicitly by executing application code.
    *
    *  This follows the design pattern of React Flux where actions are
    *  named and then dispatched to one or more action handlers (aka stores).
    *  In the context of aacio, every action is dispatched to the handler defined
    *  by account 'scope' and function 'name', but the default handler may also
    *  forward the action to any number of additional handlers. Any application
    *  can write a handler for "scope::name" that will get executed if and only if
    *  this action is forwarded to that application.
    *
    *  Each action may require the permission of specific actors. Actors can define
    *  any number of permission levels. The actors and their respective permission
    *  levels are declared on the action and validated independently of the executing
    *  application code. An application code will check to see if the required authorization
    *  were properly declared when it executes.
    */
   /**
    * 动作由演员（又称帐户）执行。 它可以由签名显式创建并授权，也可以通过执行应用程序代码隐式生成。
    * 这遵循React Flux的设计模式，其中操作被命名，然后被分派到一个或多个动作处理程序（也称为商店）。 
    * 在aacio的上下文中，每个操作都被分派到由帐户“scope”和函数“name”定义的处理程序，但是默认处理
    * 程序也可以将操作转发给任意数量的其他处理程序。 任何应用程序都可以为“scope :: name”编写一个
    * 处理程序，当且仅当此操作被转发到该应用程序时才会执行该处理程序。
    * 每个动作都可能需要特定演员的许可。 参与者可以定义任意数量的权限级别。 在操作上声明actor及其各
    * 自的权限级别，并独立于正在执行的应用程序代码进行验证。 应用程序代码将检查执行时是否正确声明了所
    * 需的授权。
    */
   struct action {
      account_name               account;
      action_name                name;
      vector<permission_level>   authorization;
      bytes                      data;

      action(){}

      template<typename T, std::enable_if_t<std::is_base_of<bytes, T>::value, int> = 1>
      action( vector<permission_level> auth, const T& value ) {
         account     = T::get_account();
         name        = T::get_name();
         authorization = move(auth);
         data.assign(value.data(), value.data() + value.size());
      }

      template<typename T, std::enable_if_t<!std::is_base_of<bytes, T>::value, int> = 1>
      action( vector<permission_level> auth, const T& value ) {
         account     = T::get_account();
         name        = T::get_name();
         authorization = move(auth);
         data        = fc::raw::pack(value);
      }

      action( vector<permission_level> auth, account_name account, action_name name, const bytes& data )
            : account(account), name(name), authorization(move(auth)), data(data) {
      }

      template<typename T>
      T data_as()const {
         AAC_ASSERT( account == T::get_account(), action_type_exception, "account is not consistent with action struct" );
         AAC_ASSERT( name == T::get_name(), action_type_exception, "action name is not consistent with action struct"  );
         return fc::raw::unpack<T>(data);
      }
   };

   struct action_notice : public action {
      account_name receiver;
   };

} } /// namespace aacio::chain

FC_REFLECT( aacio::chain::permission_level, (actor)(permission) )
FC_REFLECT( aacio::chain::action, (account)(name)(authorization)(data) )
