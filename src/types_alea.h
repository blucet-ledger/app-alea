#pragma once

// #include <stddef.h>  // size_t
// #include <stdint.h>  // uint*_t

// #include "bip32.h"

// #include "constants.h"
// #include "transaction/types.h"

/**
 * TODO comment
*/
typedef enum {
    DRAW_READY,
    DRAW_IN_PROGRESS,
    DRAW_FINISHED
} draw_state_e;

// /**
//  * Enumeration with expected INS of APDU commands.
//  */
// typedef enum {
//     GET_VERSION = 0x03,     /// version of the application
//     GET_APP_NAME = 0x04,    /// name of the application
//     GET_PUBLIC_KEY = 0x05,  /// public key of corresponding BIP32 path
//     SIGN_TX = 0x06          /// sign transaction with BIP32 path
// } command_e;

// /**
//  * Structure for transaction information context.
//  */
// typedef struct {
//     uint8_t raw_tx[MAX_TRANSACTION_LEN];  /// raw transaction serialized
//     size_t raw_tx_len;                    /// length of raw transaction
//     transaction_t transaction;            /// structured transaction
//     uint8_t m_hash[32];                   /// message hash digest
//     uint8_t signature[MAX_DER_SIG_LEN];   /// transaction signature encoded in DER
//     uint8_t signature_len;                /// length of transaction signature
//     uint8_t v;                            /// parity of y-coordinate of R in ECDSA signature
// } transaction_ctx_t;