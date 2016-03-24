#include "hashblock.h"
#include "main.h";
#include "scrypt.h"



/* Combine stop 64-bits from each hash into a single hash */
inline uint256 CombineHashes(uint256 hash1, uint512 hash2, uint512 hash3, uint512 hash4)
{
    uint256 mask = uint256("0x8000000000000000000000000000000000000000000000000000000000000000");
    uint256 hash[4] = { hash1, hash2.trim256(), hash3.trim256(), hash4.trim256() };

    /* Transpose first 64 bits of each hash into final */
    uint256 final = 0;
    for (unsigned int i = 0; i < 64; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            final <<= 1;
            if ((hash[j] & mask) != 0)
                final |= 1;
        }
        mask >>= 1;
    }

    return final;
}

/* Combine stop 64-bits from each hash into a single hash */
inline uint256 CombineHashes_p2(uint512 hash1, uint512 hash2)
{
    uint256 mask = uint256("0x8000000000000000000000000000000000000000000000000000000000000000");
    uint256 hash[2] = { hash1.trim256(), hash2.trim256()};

    /* Transpose first 64 bits of each hash into final */
    uint256 final = 0;
    for (unsigned int i = 0; i < 128; i++) {
        for (unsigned int j = 0; j < 2; j++) {
            final <<= 1;
            if ((hash[j] & mask) != 0)
                final |= 1;
        }
        mask >>= 1;
    }

    return final;
}

uint256 hefty1_hash(const char* pbegin, const char* pend)
{
    static unsigned char pblank[1];
    uint256 hash1;
    HEFTY1((pbegin == pend ? pblank : (unsigned char*)&pbegin[0]),
           (pend - pbegin) * sizeof(pbegin[0]), (unsigned char*)&hash1);

    /* HEFTY1 is new, so take an extra security measure to eliminate
     * the possiblity of collisions:
     *
     *     Hash(x) = SHA256(x + HEFTY1(x))
     *
     * N.B. '+' is concatenation.
     */
    uint256 hash2;
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx,
                  (pbegin == pend ? pblank : (unsigned char *)&pbegin[0]),
                  (pend - pbegin) * sizeof(pbegin[0]));
    SHA256_Update(&ctx, (unsigned char *)&hash1, sizeof(hash1));
    SHA256_Final((unsigned char *)&hash2, &ctx);

    /* Additional security: Do not rely on a single cryptographic hash
     * function.  Instead, combine the outputs of 4 of the most secure
     * cryptographic hash functions-- SHA256, KECCAK512, GROESTL512
     * and BLAKE512.
     */

   // uint256 thash;
   // scrypt_1024_1_1_256(pbegin, BEGIN(thash));

    uint512 hash3;
    sph_keccak512_context keccakCtx;
    sph_keccak512_init(&keccakCtx);
    sph_keccak512(&keccakCtx,(pbegin == pend ? pblank : (unsigned char*)&pbegin[0]),
                  (pend - pbegin) * sizeof(pbegin[0]));
   // sph_keccak512(&keccakCtx, (unsigned char *)&hash1, sizeof(hash1));
    sph_keccak512_close(&keccakCtx, (void *)&hash3);

    uint512 hash4;
    sph_groestl512_context groestlCtx;
    sph_groestl512_init(&groestlCtx);
    sph_groestl512(&groestlCtx,(pbegin == pend ? pblank : (unsigned char*)&pbegin[0]),
                   (pend - pbegin) * sizeof(pbegin[0]));
   // sph_groestl512(&groestlCtx, (unsigned char *)&hash1, sizeof(hash1));
    sph_groestl512_close(&groestlCtx, (void *)&hash4);

    uint512 hash5;
    sph_blake512_context blakeCtx;
    sph_blake512_init(&blakeCtx);
    sph_blake512(&blakeCtx,(pbegin == pend ? pblank : (unsigned char*)&pbegin[0]),
                  (pend - pbegin) * sizeof(pbegin[0]));
   // sph_blake512(&blakeCtx, (unsigned char *)&hash1, sizeof(hash1));
    sph_blake512_close(&blakeCtx, (void *)&hash5);

    uint256 final = CombineHashes_p2(hash3, hash4);

    return final;
}

//LOST
unsigned int scanhash_hefty1(block_header *pdata, uint32_t max_nonce, uint32_t &hash_count,
                             uint256 &result,CBigNum bnPOWLimit)
{
    hash_count = 0;
    block_header data = *pdata;
    //uint32_t hash[8];
    //unsigned char *hashc = (unsigned char *) &hash;
    uint32_t n = 0;

    while (true) {

        data.nonce = n++;
        uint256 hash = hefty1_hash((char *)pdata,(char *)pdata+sizeof(block_header));
        hash_count += 1;

        if(hash < bnPOWLimit.getuint256())
        {
            printf("\n\nscanhash_hefty1: Hash  == 0x%s\n", hash.ToString().c_str());
            printf("\n\nscanhash_hefty1: Limit == 0x%s\n", bnPOWLimit.getuint256().ToString().c_str());
            result = hash;
            return data.nonce;
        }
        if (n >= max_nonce) {
            hash_count = max_nonce + 1;
            break;
        }

    }

    return (unsigned int) -1;
}

unsigned int roughscanhash_hefty1(CBlock *pBlockSolo, const uint256 &hashTarget, unsigned int &nHashesDone,uint256 &hashBlock)
{
  //  CBlock* pBlockSolo = pblock.get();

    //uint256 hashFind = pBlockSolo.GetHash();
    nHashesDone = 0;

    while (pBlockSolo->GetHash() > hashTarget)
    {
        pBlockSolo->nNonce++;
        nHashesDone++;
        if((pBlockSolo->nNonce  & 0xffff) == 0 )
        {
            break;
        }

        //printf("nonce=%u hashFind=%s target=%s\n",block.nNonce, hashFind.ToString().c_str(),bnProofOfWorkLimit.getuint256().ToString().c_str());
    }
     if(pBlockSolo->GetHash() <= hashTarget)
     {
         hashBlock = pBlockSolo->GetHash();
         return pBlockSolo->nNonce;
     }

     return (unsigned int) -1;

}
