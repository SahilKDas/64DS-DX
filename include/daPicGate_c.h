#ifndef DAPICGATE_C_H
#define DAPICGATE_C_H

#include "types.h"
#include "dActor_c.h"

/* TWO WITNESSES, and they close on each other:
 *
 *   daPicGate_c_classInit  fBase_c::operator new(444 = 0x1bc), dActor_c::dActor_c(), stores _ZTV11daPicGate_c,
 *                 then the members below in this order.
 *   ~daPicGate_c   the same members destroyed in reverse, then ~dActor_c.
 *
 * SIZE 0x1bc is the factory's own literal, and the last member closes exactly on it.
 *
 * THE VTABLE was diffed slot by slot against _ZTV8dActor_c. Only the slots declared
 * below differ; every other slot holds the base's own word and is inherited, so it
 * is deliberately not redeclared here.
 *
 * SM64DS RTTI names the implementation daPicGate_c. The reconstructed
 * factory daPicGate_c_classInit (historical alias
 * Painting_Spawn) constructs it for the PICTURE_GATE
 * registry profile.
 */
struct daPicGate_c : dActor_c {
    /* One point of the picture's mesh. The grid in mCells is mCols x mRows of
       these; mCorners holds the four corners of the flat frame. */
    struct Vertex {
        s32 x, y, z;             /* 0x00 -- z is the ripple height */
        s32 dist;                /* 0x0c -- ripple phase input */
        u32 color;               /* 0x10 -- written straight to GXCOLOR */
        u32 texCoord;            /* 0x14 -- written straight to GXTEXCOORD */
    };

    /* One row of data_ov080_021277a8, 0x14 bytes. */
    struct WaveParams {
        s32 unk_00;
        s32 phaseStep;           /* 0x04 -- added to mWavePhase every frame */
        s32 unk_08;
        s32 unk_0c;
        s32 duration;            /* 0x10 -- reloads mWaveTimer */
    };

    /* The three per-state handlers, one table row per param1 bits 13-14. */
    typedef void (daPicGate_c::*StateFunc)();
    struct State {
        StateFunc init;          /* 0x00 */
        StateFunc behavior;      /* 0x08 */
        StateFunc render;        /* 0x10 */
    };

    u8  pad_0d0[4];              /* 0x0d0 */
    s32 mMtx[12];                /* 0x0d4 -- model-to-world, Matrix4x3 */
    s32 mInvMtx[12];             /* 0x104 -- its inverse */
    s32 unk_134[3];              /* 0x134 */
    Vertex mCorners[4];          /* 0x140 */
    Vertex *mCells;              /* 0x1a0 -- mNumCells, Memory::operator_new2'd */
    State *mState;               /* 0x1a4 */
    s32 mTexRecord;              /* 0x1a8 -- from func_ov080_02125630 */
    const WaveParams *mWaveParams; /* 0x1ac */
    s32 mClosedPosX;             /* 0x1b0 -- mPosX as spawned */
    s16 mWavePhase;              /* 0x1b4 */
    u16 mWaveTimer;              /* 0x1b6 */
    u16 mNumCells;               /* 0x1b8 */
    u8  mCols;                   /* 0x1ba */
    u8  mRows;                   /* 0x1bb */

    virtual ~daPicGate_c();            /* slots 16 (D1), 17 (D0) */

    virtual s32   InitResources();         /* slot  0 */
    virtual s32   CleanupResources();      /* slot  3 */
    virtual s32   Behavior();              /* slot  6 */
    virtual s32   Render();                /* slot  9 */
    virtual void  OnPendingDestroy();      /* slot 12 */
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char daPicGate_Vertex_size_must_be_0x18[
    sizeof(daPicGate_c::Vertex) == 0x18 ? 1 : -1];
typedef char daPicGate_c_size_must_be_0x1bc[sizeof(daPicGate_c) == 0x1bc ? 1 : -1];
#endif

#endif /* DAPICGATE_C_H */
