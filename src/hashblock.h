// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_HASHBLOCK_H
#define BITCOIN_HASHBLOCK_H

#include "uint256.h"
//#include "serialize.h"
#include "hefty1.h"
#include "sph_keccak.h"
#include "sph_blake.h"
#include "sph_groestl.h"

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <vector>
#include "bignum.h"
//#include "main.h" //LOST

#include <stdio.h>


class CBlock;
typedef struct
{
    unsigned int version;
    uint256 prev_block;
    uint256 merkle_root;
    unsigned int timestamp;
    unsigned int bits;
    unsigned int nonce;

} block_header;

uint256 hefty1_hash(const char* pbegin, const char* pend);

unsigned int scanhash_hefty1(block_header *pdata, uint32_t max_nonce, uint32_t &hash_count,
                             uint256 &result,CBigNum bnPOWLimit);

unsigned int roughscanhash_hefty1(CBlock *pBlockSolo, const uint256 &hashTarget, unsigned int &nHashesDone,uint256 &hashBlock);


#endif
