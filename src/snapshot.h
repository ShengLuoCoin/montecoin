#ifndef BITCOIN_SNAPSHOT_H
#define BITCOIN_SNAPSHOT_H
#include "main.h"

//The address number of snapshot
#define SNAPSHOT_ADDR_COUNT 1

//
#define SNAPSHOT_COIN_AMOUNT (629999999.99999297 *COIN)

extern int64 nChainStartTime;

void CreateSnapshotTxArray(CTransaction *txNew);
void CreateSnapshotTxArrayV2(CTransaction *txNew);

#endif // BITCOIN_SNAPSHOT_H
