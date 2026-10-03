//cpp
/**
 * Arrow sign (YAJIRUSI_R / YAJIRUSI_L).
 *
 * Signpost with mesh collision. The two registry variants select the model
 * and collision columns through mVariant (actor id 0x12b/0x12c); Kill and
 * the Mega handlers break it with a particle poof.
 *
 * daObjYajirusi_c_classInit_YAJIRUSI_R/_L are reconstructed (RTTI
 * daObjYajirusi_c at 0x0213c36c, YAJIRUSI_R/L registry). Retail does not
 * store those spellings. Historical aliases ArrowSignRight_Spawn /
 * ArrowSignLeft_Spawn.
 *
 * The two registry factories are the same body (alloc 896, dBgActor_c C2,
 * same vtable, ShadowModel C1 at +0x320) and are contiguous at
 * 0x02138008..0x02138040, so this TU spans 0x02137be0..0x02138040 with no
 * hole: 12 functions. The synthesized constructor reproduces the ROM init
 * sequence, and the inline destructor chain also emits a homeless
 * _ZN10dBgActor_cD2Ev, a licensed deadstrip in the manifest
 * (ov012/daObjC0Water_c precedent).
 *
 * Function order is the reverse of the ROM's (highest address first): mwccarm
 * 2004/b56 emits one .text section per function in reverse source order. Do
 * not reorder. The D1/D0 pair comes out in ROM order because the destructor is
 * inline-first in daObjYajirusi_c.h and the factories are real new-expressions.
 * The "// address (size)" line above each definition is its ROM location.
 *
 * Known limits:
 * - func_ov098_02137c8c keeps its ROM-unnamed spelling; it builds the
 *   collision/model matrix at +0x348/0x36c from yaw and pos>>3 for
 *   InitResources. No replacement name is coined.
 * - The OnAttacked1 / OnHitByMegaChar reference spellings are guesses (ref vs
 *   pointer is indistinguishable in ARM); the method names come from vtable
 *   slots, and ownership, bodies and relocations are proven. See
 *   daObjYajirusi_c.h and symbols/actor_renames.tsv.
 * - Particle::System::NewSimple, dBgActor_c::UpdateKillByMegaChar,
 *   dBgActor_c::IsClsnInRangeOnScreen, dActor_c::DropShadowScaleXYZ and
 *   dBgW_KcMbg::SetFile keep mangled extern-C spellings (Fix12<int> by value).
 * - g_profile_YAJIRUSI_R/L stay outside the TU.
 */

#include "daObjYajirusi_c.h"
#include "Sound.h"
#include "SharedFilePtr.h"
#include "dBgW.h"
#include "Player.h"
#include "decl_ActorBase.h"
#include "decl_Platform.h"
#include "decl_ShadowModel.h"

/* One row of the ov098 resource table at 0x0213c380 (model, KCL, CLPS). */
struct ArrowSignFileColumn {
    void *value;
    void *nextColumn1;
    void *nextColumn2;
};

extern "C" {
extern void Matrix4x3_FromRotationY(void *, int);
void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(u32 id, Fix12i x, Fix12i y, Fix12i z);
int func_02012694(int, void*);
int _ZN10dBgActor_c20UpdateKillByMegaCharEsss5Fix12IiE(void* c, short a, short b, short d, int e);
void func_02039394(int* p, int v);
void func_020393a4(int* p, int v);
void _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(void* c, void* sm, void* mtx, int s, int x, int y, unsigned int j);
int _ZN10dBgActor_c21IsClsnInRangeOnScreenE5Fix12IiES1_(void* c, int a, int b);
void func_ov098_02137c8c(char *self);
void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
void *self, void *kcl, void *mtx, int scale, short angle, void *clps);
/* Each row is model/KCL/CLPS, but the ROM gives each column its own symbol.
* These three overlapping stride-0xc views keep those relocation destinations
* distinct while still indexing the table as rows. */
extern ArrowSignFileColumn data_ov098_0213c380[];
extern ArrowSignFileColumn data_ov098_0213c384[];
extern ArrowSignFileColumn data_ov098_0213c388[];
}

// 0x02138008 (0x38)
// @symbol daObjYajirusi_c_classInit_YAJIRUSI_L
/* Second registry factory for the same class (YAJIRUSI_L profile); body twin
 * of _R below. Contiguous at 0x02138008..0x02138040, so the genuine TU spans
 * 0x02137be0..0x02138040 with no hole. Historical alias: ArrowSignLeft_Spawn. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int *daObjYajirusi_c_classInit_YAJIRUSI_L(void)
{
    return (int *)new daObjYajirusi_c;
}
}

// 0x02137fd0 (0x38)
// @symbol daObjYajirusi_c_classInit_YAJIRUSI_R
extern "C" {  /* .c-derived member: C linkage for the whole block */
int *daObjYajirusi_c_classInit_YAJIRUSI_R(void)
{
    return (int *)new daObjYajirusi_c;
}
}

// 0x02137eec (0xe4)
int daObjYajirusi_c::InitResources()
{
    u16 id = actorID;
    if (id != 0x12b) {
        if (id == 0x12c)
            mVariant = 1;
    } else {
        mVariant = 0;
    }

    u32 modelIndex = mVariant;
    void *model = Model::LoadFile(*(SharedFilePtr *)data_ov098_0213c380[modelIndex].value);
    mModel.ModelBase::SetFile((BMD_File *)model, 1, -1);
    mShadowModel.InitCuboid();
    UpdateModelPosAndRotY();
    UpdateClsnPosAndRot();
    func_ov098_02137c8c((char *)this);

    u32 collisionIndex = mVariant;
    void *kcl = dBgW_Kc::LoadFile(*(SharedFilePtr *)data_ov098_0213c384[collisionIndex].value);
    _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
        &mMeshCollider, kcl, &mClsnMat, 0x199, mAngleY,
        data_ov098_0213c388[collisionIndex].value);
    return 1;
}

// 0x02137e48 (0xa4)
// @symbol _ZN15daObjYajirusi_c8BehaviorEv
int daObjYajirusi_c::Behavior()
{
    if (_ZN10dBgActor_c20UpdateKillByMegaCharEsss5Fix12IiE(((char*)this), -0x2000, 0, 0, 0x96000))
        return 1;
    func_02039394((int*)((char*)&(*(u8 *)&mMeshCollider)), 0xc0000);
    func_020393a4((int*)((char*)&(*(u8 *)&mMeshCollider)), 0xe0000);
    _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
        ((char*)this), (void*)((char*)&mShadowModel), (void*)((char*)&mShadowMat), 0x10e000, 0x64000, 0x46000, 0xf);
    _ZN10dBgActor_c21IsClsnInRangeOnScreenE5Fix12IiES1_(((char*)this), 0x600000, 0);
    return 1;
}

// 0x02137e20 (0x28)
// @symbol _ZN15daObjYajirusi_c6RenderEv
int daObjYajirusi_c::Render()
{
    mModel.Render(0);
    return 1;
}

// 0x02137dbc (0x64)
// @symbol _ZN15daObjYajirusi_c16CleanupResourcesEv
int daObjYajirusi_c::CleanupResources()
{
    if (mMeshCollider.IsEnabled())
        mMeshCollider.Disable();
    ((SharedFilePtr *)data_ov098_0213c380[mVariant].value)->Release();
    ((SharedFilePtr *)data_ov098_0213c384[mVariant].value)->Release();
    return 1;
}

// 0x02137d80 (0x3c)
// @symbol _ZN15daObjYajirusi_c15OnHitByMegaCharER6Player
void daObjYajirusi_c::OnHitByMegaChar(Player &player)
{
    player.IncMegaKillCount();
    func_02012694(0x1e, &mCamSpacePosX);
    KillByMegaChar(player);
}

// 0x02137d40 (0x40)
// @symbol _ZN15daObjYajirusi_c11OnAttacked1ER8dActor_c
/* Vtable slot identity: Kill() is slot 31. The other actor's actorID (offset
 * 0xc) is read raw, as found. */
int daObjYajirusi_c::OnAttacked1(dActor_c &other)
{
    unsigned r = (*(unsigned short*)((char*)&other + 0xc) == 0xce) ? 1u : 0u;
    if (r == 0) return;
    Kill();
}

// 0x02137ccc (0x74)
// @symbol _ZN15daObjYajirusi_c4KillEv
/* Kill is vtable slot 31: _ZTV15daObjYajirusi_c (0x0213c3d8) relocates +0x7c to
 * 0x02137ccc where _ZTV10dBgActor_c carries dBgActor_c::Kill, so this is the
 * class's own override. Slot 30 is the same main-module function in both
 * tables, which makes 31 the first slot where they differ.
 *
 * Same shape as dBgActor_c::Kill with three differences the ROM dictates:
 * particle 0xe instead of 0xa, spawned 0x28000 (forty 20.12 units) above the
 * sign instead of a hundred, and DisappearPoofDustAt (particles 0x127/0x128)
 * instead of PoofDustAt.
 *
 * The second Vector3 is copied memberwise on purpose: Vector3 declares a
 * destructor (types.h), so a whole-object assignment compiles to an ldm/stm
 * pair, four instructions where the ROM has six. NewSimple keeps its mangled
 * name because its Fix12<int> parameters are by value and declaring the true
 * types changes how the caller passes them; dBgActor_c.cpp argues both. */
void daObjYajirusi_c::Kill()
{
    Vector3 pos;
    Vector3 dustPos;
    Fix12i x = mPosX;
    Fix12i y = mPosY + 0x28000;
    Fix12i z = mPosZ;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xe, pos.x, pos.y, pos.z);
    dustPos.x = pos.x;
    dustPos.y = pos.y;
    dustPos.z = pos.z;
    DisappearPoofDustAt(dustPos);
    Sound::PlayBank3(0x41, *(Vector3 *)&mCamSpacePosX);
    MarkForDestruction();
}

// 0x02137c8c (0x40)
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov098_02137c8c(char *t)
{
    Matrix4x3_FromRotationY(t + 0x348, *(short *)(t + 0x8e));
    *(int *)(t + 0x36c) = *(int *)(t + 0x5c) >> 3;
    *(int *)(t + 0x370) = *(int *)(t + 0x60) >> 3;
    *(int *)(t + 0x374) = *(int *)(t + 0x64) >> 3;
}
}

// 0x02137c2c (0x60), 0x02137be0 (0x4c)
// @symbol _ZN15daObjYajirusi_cD0Ev
// @symbol _ZN15daObjYajirusi_cD1Ev
/* The destructor is defined inline-first in daObjYajirusi_c.h, which is what
 * makes mwccarm emit the retail D1-then-D0 pair in ROM order with the vtable
 * homed in this TU and no leaf D2 (class-form skill). Its body is the empty
 * braces plus the implicit member/base destruction the ROM's own D1/D0 show:
 * this class adds no member with a destructor of its own; D0 additionally
 * destroys through the base and returns the object to its heap via an inline
 * operator delete. */
