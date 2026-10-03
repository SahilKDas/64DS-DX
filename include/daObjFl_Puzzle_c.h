/* AUTO-GENERATED from matched-function evidence by tools/gen_header.py
 * class daObjFl_Puzzle_c: 5 matched functions, 14 evidenced fields.
 * Offsets/widths are observed, not guessed. Gaps are explicit padding.
 * Field NAMES are placeholders - renaming cannot change codegen. */
#ifndef DAOBJFL_PUZZLE_C_H
#define DAOBJFL_PUZZLE_C_H
#include "dBgActor_c.h"

/* Lethal Lava Land puzzle piece (FL_PUZZLE).
 *
 * _ZTI16daObjFl_Puzzle_c is at ov064 0x0211bfa4 and the name bytes at
 * 0x0211bfc4 say daObjFl_Puzzle_c. The vtable at 0x0211c25c stores that
 * typeinfo. The sole base is dBgActor_c: the destructor restores
 * _ZTV10dBgActor_c, destroys the moving-mesh member at 0x124 and Model at
 * 0x0d4, then chains to dActor_c::~dActor_c.
 */
struct daObjFl_Puzzle_c : dBgActor_c {
    u32 mOtherPieceId;       /* 0x320 */
    s32 mStateInfo;            /* 0x324 */
    u8  mStateIndex;            /* 0x328 */
    u8  pad_329[0x3];
    s32 unk_32c;            /* 0x32c */
    u8  pad_330[0x4];
    u16 mMoveTimer;            /* 0x334 */
    u8  mState;            /* 0x336 */
    u8  mType;            /* 0x337 */
    u8  mHadClsn;            /* 0x338 */
    u8  mFreezeState;            /* 0x339 */
    u8  mCanSpawnCoin;            /* 0x33a */
    /* Inline is load-bearing. The forcing calls in src/actors/daObjFl_Coin_c.cpp
     * emit D1 then D0. An out-of-line body emits D2, D0, D1. */
    virtual ~daObjFl_Puzzle_c() {}

    /* Overrides of fBase_c's resource/behavior/render slots. */
    int InitResources();
    int CleanupResources();
    int Behavior();
    int Render();

    /* State-table targets and the helpers they call. The address stays in the
     * name; symbols.txt carries the mangled spelling. Not virtual: the table
     * at 0x0211c904 is a pointer-to-member array, not extra vtable slots. */
    void func_ov064_02118c48();
    void func_ov064_02118cd4();
    void func_ov064_02118cec();
    void func_ov064_02118d08();
    void func_ov064_02118d20();
    void func_ov064_02118d3c();
    void func_ov064_02118da0();
    void func_ov064_02118e24(int a1, int a2, int a3);
    void func_ov064_02118ee4();
    void func_ov064_02118fa4();
    void func_ov064_02119010();
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char daObjFl_Puzzle_c_size_must_be_0x33c[
    sizeof(struct daObjFl_Puzzle_c) == 0x33c ? 1 : -1];
#endif

#endif
