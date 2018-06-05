// TEMP:
const bn                = require('bignumber.js')
const Balance           = require('./Balance')
const util              = require('../utilities')

class Wallet {

  constructor( address, config ){
    this.address              = address
    this.aac_key              = null
    this.balance              = new Balance()
    this.registered           = false
    this.registered_fallback  = false
    this.accepted             = false
    this.last_activity_block  = null

    this.fallback             = false

    this.register_error       = null
    this.fallback_error       = null

    this.buys                 = new Array(CS_NUMBER_OF_PERIODS).fill(new bn(0))
    this.claims               = new Array(CS_NUMBER_OF_PERIODS).fill(false)
    this.reclaimables         = []
    this.transfers            = []

    this.config = config
  }

  json (){
    return {
      address:              this.address
      ,aac_key:             this.aac_key
      ,balance_wallet:      this.balance.wallet
      ,balance_unclaimed:   this.balance.unclaimed
      ,balance_reclaimed:   this.balance.reclaimed
      ,balance_total:       this.balance.total
      ,registered:          this.registered
      ,register_error:      this.register_error
      ,fallback_error:      this.fallback_error
      ,fallback:            this.fallback
      ,valid:               this.accepted
    }
  }

  reject () {
    this.accepted = false
  }

  accept ( callback ) {
    this.accepted = true
  }

  set_index ( index ) {
    this.index = index
  }

  maybe_fix_key( ) {
    //remove whitespace
    let aac_key = null

    if( this.aac_key ) {
      aac_key = this.aac_key.trim()

      //Might be hex, try to convert it.
      if( aac_key.length == 106 ){ //TODO use web3.isHex()
        let aac_key_from_hex = util.misc.hex_to_ascii( "0x"+aac_key )
        if ( util.misc.is_aac_public_key(aac_key_from_hex) ) {
          aac_key = aac_key_from_hex
        }
      }
      //Might be user error
      else if(aac_key.startsWith('key')){
        let aac_key_mod = aac_key.substring(3)
        if( aac_key_mod.startsWith('AAC') ) {
          aac_key = aac_key_mod
        }
      }
      //Convert something that looks like a key to AAC key (STM, BTS, ETC)
      else if(!aac_key.startsWith('AAC') && !/[^a-zA-Z0-9]/.test(aac_key)) {
        let aac_key_test = `AAC${aac_key.slice(3, aac_key.length)}`
        aac_key = util.misc.is_aac_public_key(aac_key_test) ? aac_key_test : aac_key
      }
    }

    // this.aac_key = aac_key ? (this.registered=true, aac_key) : (this.registered=false, null)
    this.aac_key = aac_key
  }

  set_registration_status() {
    if( util.misc.is_aac_public_key(this.aac_key) ) {
      this.registered = true
    }
  }

  valid() {
    //Set registration status of any entry with valid key regardless of balance validation.
    this.set_registration_status()
    this.register_error = this.validate()
    return !this.register_error ? true : false
  }

  validate(){
    let error = null

    //Because we haven't toFormat(4) yet, there will be some balances that may be wrongly attributed this error. We'll test against decimal precision 4.
    //Consider toFixed() before validation!
    if( parseFloat(this.balance.total.toFixed(4)) < parseFloat(1.0000) ) {
      error = 'balance_insufficient'
    }

    else if (this.aac_key===null) {
      error = 'not_registered'
    }

    else {
      error = this.validate_key()
    }

    return error;
  }

  validate_key(){
    let error = null

    if(!this.registered) {

      //Key is blank
      if(this.aac_key.length == 0) {
        error = 'key_is_empty'
      }

      //It may be an AAC private key
      else if(this.aac_key.startsWith('5')) {
        error = 'key_is_private'
      }

      // It almost looks like an AAC key
      else if(this.aac_key.startsWith('AAC')) {
        error = 'key_is_malformed'
      }

      // ETH address
      else if(this.aac_key.startsWith('0x')) {
        error = 'key_is_eth'
      }

      //Reject everything else with junk label
      else {
        error = 'key_is_junk'
      }
    }

    return error
  }

}

module.exports = Wallet
