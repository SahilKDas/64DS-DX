#ifndef DAOBJFL_COIN_C_H
#define DAOBJFL_COIN_C_H

#include "types.h"
#include "dActor_c.h"

/* Lethal Lava Land coin-puzzle manager (FL_COIN).
 *
 * _ZTI14daObjFl_Coin_c is at ov064 0x0211bf98 and the name bytes at
 * 0x0211bfb0 say daObjFl_Coin_c. The vtable at 0x0211c1d8 stores that
 * typeinfo, and daObjFl_Coin_c_classInit writes the same vtable after
 * operator new(0xd8). The three state bytes sit at 0xd4.
 */
struct daObjFl_Coin_c : dActor_c {
    u8 pad_0d0[4];
    s8 unk_0d4;
    s8 unk_0d5;
    s8 unk_0d6;
    u8 pad_0d7;

    /* Inline so this TU emits D1 then D0. An out-of-line body emits D2, D0, D1,
     * and the cartridge's run is D1 at 0x02118bec then D0 at 0x02118c10. */
    virtual ~daObjFl_Coin_c() {}

    virtual s32 InitResources(); /* slot 0 */
    virtual s32 Behavior();      /* slot 6 */
};

#ifndef SM64DS_PLATFORM_PC
typedef char daObjFl_Coin_c_size_must_be_0xd8[sizeof(daObjFl_Coin_c) == 0xd8 ? 1 : -1];
#endif

#endif /* DAOBJFL_COIN_C_H */
