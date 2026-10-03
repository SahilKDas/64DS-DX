#ifndef DABOOK_C_H
#define DABOOK_C_H

#include "types.h"

/* Derives from dEnemyBase_c, and TWO INDEPENDENT WITNESSES agree on the layout: the
 * class's own destructor `_ZN8daBook_cD1Ev` destroys each member, and
 * `daBook_c_classInit_SHOOT_BOOK` constructs the same types at the same offsets after
 * storing `_ZTV8daBook_c`. Everything this header used to restate below 0x110
 * belongs to dEnemyBase_c and dActor_c and is inherited now.
 *
 * The members close on each other, which is what makes the layout a reading
 * rather than a guess:
 *
 *     0x110 ModelAnim                  0x64    -> 0x174
 *     0x174 Model                      0x50    -> 0x1c4
 *     0x1c4 ShadowModel                0x28    -> 0x1ec
 *     0x1ec Matrix4x3                  0x30    -> 0x21c
 *     0x21c dCcAcPos_c  0x40    -> 0x25c
 *     0x25c dBgCh_Actr               0x1bc   -> 0x418
 *
 * SIZE IS THE ROM'S OWN: `daBook_c_classInit_SHOOT_BOOK` calls `fBase_c::operator new(1108)`
 * -- 0x454 -- and stores this class's vtable, so that literal IS this
 * class's sizeof.
 *
 * NAME. The cartridge's RTTI names this class daBook_c: _ZTS8daBook_c at
 * ov020 0x0211482c, _ZTI8daBook_c at 0x02114844, _ZTV8daBook_c at 0x0211495c.
 * It was carried here under the coined name BookShot until that was replaced
 * by the ROM's. The reconstructed factory
 * daBook_c_classInit_SHOOT_BOOK (historical alias BookShot_Spawn) installs this class's
 * cartridge vtable for the SHOOT_BOOK registry profile.
 */

#include "dEnemyBase_c.h"
#include "Model.h"
#include "ModelAnim.h"
#include "dCcAcPos_c.h"
#include "ShadowModel.h"
#include "dBgCh_Actr.h"

struct daBook_c : dEnemyBase_c {
    ModelAnim                    mModelAnim;            /* 0x110 */
    Model                        mModel;                /* 0x174 */
    ShadowModel                  mShadowModel;          /* 0x1c4 */
    /* Drop-shadow matrix: InitResources copies IDENTITY_MATRIX4X3 here and
       func_ov020_0211216c writes the position, scaled by 8, into its
       translation row before handing it to DropShadowRadHeight. */
    Matrix4x3                    mShadowMat;            /* 0x1ec */
    dCcAcPos_c    mdCcAcPos_c; /* 0x21c */
    dBgCh_Actr                 mWithMeshClsn;         /* 0x25c */
    s32                          unk_418;               /* 0x418 */
    s32                          unk_41c;               /* 0x41c */
    s32                          unk_420;               /* 0x420 */
    s32                          mState;                /* 0x424 */
    s32                          unk_428;               /* 0x428 */
    s32                          unk_42c;               /* 0x42c */
    s32                          unk_430;               /* 0x430 */
    s32                          unk_434;               /* 0x434 */
    /* Cylinder offset handed to dCcAcPos_c::Init and SetPosRelativeToActor. */
    Vector3                      mClsnOffset;           /* 0x438 */
    u8  pad_444[0x8];
    s32                          unk_44c;               /* 0x44c */
    u8                           unk_450;               /* 0x450 */
    u8  pad_451[0x3];

    /* --- vtable --- */
    virtual ~daBook_c();

    virtual s32   OnYoshiTryEat();         /* slot 18 */
    virtual s32   OnAimedAtWithEgg();      /* slot 29 */

    /* --- non-virtual --- */
    int Behavior();
    int CleanupResources();
    int InitResources();
    int Render();
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char daBook_c_size_must_be_0x454[sizeof(daBook_c) == 0x454 ? 1 : -1];
#endif

#endif /* DABOOK_C_H */
