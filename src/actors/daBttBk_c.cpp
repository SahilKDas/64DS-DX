//cpp
/* daBttBk_c -- the BATTA_BLOCK crate. InitResources drops it onto the ground
 * below its spawn point and enters state 0, where it falls and bounces;
 * landing on a carrier actor (mCarrier) moves it to state 1, where it rides
 * that actor's angles and matrix. A Mega Mario hit (func_ov080_02124acc), or
 * the flag test in state 1 (func_ov080_02124edc), breaks it into dust and
 * five coins.
 *
 * ov080 .text 0x02124a20..0x02125404, 21 functions: tu_map's 20-function run
 * 0x02124a20..0x021253b4 plus the abutting registry factory
 * daBttBk_c_classInit (0x021253b4; reconstructed name, historical alias
 * CrazedCrate_Spawn), written last.
 *
 * NAME: daBttBk_c is the cartridge's RTTI spelling -- _ZTS at ov080
 * 0x0212815c is the byte string "9daBttBk_c", and _ZTI at 0x02128168 reads
 * [__si_class_type_info vtable (0x0209a764), that string, _ZTI8dActor_c
 * (0x0208e390)]. The vtable's offset-to-top word (0x02128190) is 0 and its
 * RTTI word (0x02128194) is that _ZTI; the address point _ZTV9daBttBk_c is
 * 0x02128198 (slot 0, InitResources). The class was previously the coined
 * name CrazedCrate.
 *
 * THE DESTRUCTOR IS THE KEY FUNCTION, declared first in daBttBk_c.h and
 * defined first below, so this TU emits _ZTV9daBttBk_c and the RTTI chain
 * as vague linkage. Under `#pragma defer_codegen off` mwccarm emits each
 * function as it is parsed, so the file is written in ROM-ascending order
 * and the out-of-line destructor comes out D1 (0x02124a20), D0
 * (0x02124a68), then a D2 the cartridge has no home for (deadstripped;
 * vtable slots 16/17 hold D1 and D0 only). D0's deallocation is an inline
 * operator delete, which is why nothing below mentions a heap.
 *
 * THE STATE MACHINE. The eleven func_ov080_* functions of this run keep
 * their address names: the cartridge preserves no spelling for them. The
 * ROM ties each one to this class instead of a name --
 *   - the six state bodies are the pointer-to-member constants at ov080
 *     0x0212812c..0x02128158, the .data words directly before
 *     _ZTS9daBttBk_c. __sinit_ov080_02127a60 constructs this class's model
 *     file data_ov080_02128468 and copies those constants into the 3-row
 *     state table data_ov080_0212847c (.bss): state 0 = {0212509c enter,
 *     0212500c update}, state 1 = {02124fec, 02124edc}, state 2 =
 *     {02124eb0, 02124e60}. Each enter function stores its own index in
 *     mState;
 *   - 0212513c points mStateRow at a row and runs its enter function
 *     through 02125104; 021250c8 runs the row's update function from
 *     Behavior;
 *   - 02124acc (collision reaction, from state 0's update) and 02124c3c
 *     (matrices and drop shadow, from Behavior and InitResources) have no
 *     callers outside this run.
 * All eleven lie between OnYoshiTryEat (0x02124ac4) and CleanupResources
 * (0x02125158) with no gap, and nothing outside this run and its PMF
 * constants references any of them.
 *
 * Known limits:
 * - func_ov080_02124eb0's old one-function source called it "MontyMole_Kill"
 *   (daChoropu_c::Kill); the state table makes it this class's state-2 enter
 *   function, not a daChoropu_c method.
 * - Particle::System::NewSimple, dActor_c::SpawnCoins and
 *   dActor_c::DropShadowScaleXYZ stay mangled extern "C" calls -- each takes
 *   Fix12<int> by value, and a member call homes the argument and changes
 *   the ROM ABI.
 * - func_ov080_02124acc, 02124edc, 021250c8 and 0212513c are declared with
 *   `char *` in decl_common.h, so those keep a `char *` parameter.
 * - The carrier's +0xc8 pointer (a Matrix4x3) is read raw; dActor_c has no
 *   member there yet.
 */

#include "decl_common.h"
#include "daBttBk_c.h"
#include "dBgCh_Gnd.h"
#include "SharedFilePtr.h"
#include "Sound.h"
#include "Player.h"

struct Mtx43 { int a[12]; };
typedef char Mtx43_size_must_be_0x30[sizeof(Mtx43) == 0x30 ? 1 : -1];

/* The state table's rows hold pointers to member functions of this
   pretend class; only the row pointer at 0x36c is ever read through it. */
struct C;
typedef void (C::*PMF)();
struct C { char pad[0x36c]; PMF *pp; };
typedef char C_size_must_be_0x370[sizeof(C) == 0x370 ? 1 : -1];

extern "C" {
void *func_02010304(void *a, void *b);
void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
void _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(void *self, const Vector3 &v, unsigned int n, int vel, short unk);
void Matrix4x3_ApplyInPlaceToTranslation(Mtx43 *m, int x, int y, int z);
void Vec3_LslInPlace(Vector3 *v, int sh);
void Matrix4x3_FromRotationY(void *m, int angle);
void _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
    void *self, void *sm, void *mtx, int r, int t5, int t6, unsigned int u);
void dBgCh_Actr_UpdateDiscreteNoLava_veneer(dBgCh_Actr *w);
void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *self, void *actor, int a, int b, unsigned int c, unsigned int d);
void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void *self, void *actor, int a, int b, void *v, int c);
extern void func_ov080_02124c3c(daBttBk_c *self);
}

extern Mtx43 data_020a0e68;
extern s16 data_02082214[];
extern char data_ov080_0212847c[];

/* InitResources and func_ov080_02124e60 leave r1 holding the 0 they just
 * stored. A two-arg call would write r1 again. */
typedef void (*Fn2513c1)(char *);

#pragma defer_codegen off

// @symbol _ZN9daBttBk_cD1Ev
// @symbol _ZN9daBttBk_cD0Ev
/* Empty on purpose. mwccarm destroys the members, then ~dActor_c, and
 * emits retail D1 followed by D0. */
daBttBk_c::~daBttBk_c()
{
}

// @symbol _ZN9daBttBk_c13OnYoshiTryEatEv
s32 daBttBk_c::OnYoshiTryEat()
{
    return 1;
}

/* Collision reaction, run from state 0's update: a carrier found by
 * func_02010304 is stored in mCarrier and enters state 1; otherwise, against
 * actor id 0xbf, mFlags bit 17 enters state 2 and a hitFlags bit 4 contact
 * breaks the crate (Player::IncMegaKillCount, dust, five coins). */
// @symbol func_ov080_02124acc
extern "C" void func_ov080_02124acc(char *c)
{
    daBttBk_c *self = (daBttBk_c *)c;
    if (self->mdCcAc_c.otherOwner == 0) return;
    void *p = func_02010304(self, &self->mdCcAc_c);
    if (p != 0) { self->mCarrier = (dActor_c *)p; func_ov080_0212513c((char *)self, 1); return; }
    dActor_c *other = dActor_c::FindWithID(self->mdCcAc_c.otherOwner);
    if (other == 0) return;
    int isId0xbf = (int)(other->actorID == 0xbf);
    if (isId0xbf == 0) return;
    int inYoshiMouthA = (int)((self->mFlags & 0x20000) != 0);
    if (inYoshiMouthA) { func_ov080_0212513c((char *)self, 2); return; }
    if ((self->mdCcAc_c.hitFlags & 0x10) == 0) return;
    ((Player *)other)->IncMegaKillCount();
    Vector3 v; Vector3 v2; Vector3 v3;
    int y0 = self->mPosY;
    int z = self->mPosZ;
    int x = self->mPosX;
    int y = y0 + 0x32000;
    v.x = x; v.y = y; v.z = z;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(6, v.x, v.y, v.z);
    v2.x = v.x;
    v2.y = v.y;
    v2.z = v.z;
    self->PoofDustAt(v2);
    int t = self->mPosY + 0x64000;
    v3.x = v.x;
    v.y = t;
    v3.y = t;
    v3.z = v.z;
    _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(self, v3, 5, 0xf000, 0);
    Sound::PlayBank3(0x41, *(Vector3 *)&self->mCamSpacePosX);
    self->MarkForDestruction();
}

/* Model and shadow matrices: follows the carrier's matrix while carried,
 * then rebuilds the model matrix (mModel.mat4x3) and the drop shadow
 * matrix (mShadowMtx). */
// @symbol func_ov080_02124c3c
extern "C" void func_ov080_02124c3c(daBttBk_c *self)
{
    Vector3 t;
    Vector3 pos;
    int flags = self->mFlags;
    int inYoshiMouthB = (flags & 0x40000) != 0;
    if (inYoshiMouthB != false) return;
    dActor_c *carrier = self->mCarrier;
    if (carrier != 0) {
        int hasBit14 = (flags & 0x4000) != 0;
        if (hasBit14 != false) {
            /* +0xc8 of the carrier is a Matrix4x3 pointer. */
            if (*(int *)((char *)carrier + 0xc8) != 0) {

                int tx = 0xc000, ty = 0x2000, tz = 0;
                t.x = tx; t.y = ty; t.z = tz;
                carrier = self->mCarrier;
                data_020a0e68 = *(Mtx43 *)(*(void **)((char *)carrier + 0xc8));
                Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, t.x, t.y, t.z);

                self->mPosX = data_020a0e68.a[9];
                self->mPosY = data_020a0e68.a[10];
                self->mPosZ = data_020a0e68.a[11];
                Vec3_LslInPlace((Vector3 *)&self->mPosX, 3);
            }
        }
    }
    Matrix4x3_FromRotationXYZExt(&self->mModel.mat4x3, self->mAngleX, self->mAngleY, self->mAngleZ);
    self->mModel.mat4x3.m[9] = self->mPosX >> 3;
    self->mModel.mat4x3.m[10] = self->mPosY >> 3;
    self->mModel.mat4x3.m[11] = self->mPosZ >> 3;
    pos.x = self->mPosX;
    pos.y = self->mPosY;
    pos.z = self->mPosZ;
    pos.y = pos.y + 0x14000;
    dBgCh_Gnd rg;
    rg.SetObjAndPos(pos, 0);
    int groundY = pos.y;
    if (rg.DetectClsn()) {
        groundY = rg.clsnY;
    }
    Matrix4x3_FromRotationY(self->mShadowMtx, self->mAngleY);
    ((Matrix4x3 *)self->mShadowMtx)->m[9] = self->mPosX >> 3;
    ((Matrix4x3 *)self->mShadowMtx)->m[10] = groundY >> 3;
    ((Matrix4x3 *)self->mShadowMtx)->m[11] = self->mPosZ >> 3;
    {
        s16 a = self->mAngleX;
        int sv = data_02082214[((unsigned short)(short)(a << 1) >> 4) * 2];
        if (sv < 0) sv = -sv;
        int result = (int)(((s64)sv * 0x28000 + 0x800) >> 12);
        _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
            self, &self->mShadowModel, self->mShadowMtx, 0x96000, 0x32000, result + 0x96000, 0xf);
    }
}

/* State 2 update: back to state 0 once neither flag bit 17 nor 18 is set.
 * The one-argument call selects state 0 through the r1 left at 0. */
// @symbol func_ov080_02124e60
extern "C" int func_ov080_02124e60(daBttBk_c *self)
{
    int flags = self->mFlags;
    int inYoshiMouthA = (flags & 0x20000) ? 1 : 0;
    if (inYoshiMouthA != 0) goto done;
    int inYoshiMouthB = (flags & 0x40000) ? 1 : 0;
    if (inYoshiMouthB != 0) goto done;
    *(int *)self->pad_0d0 = 0;
    ((Fn2513c1)func_ov080_0212513c)((char *)self);
done:
    return 1;
}

/* State 2 enter. */
// @symbol func_ov080_02124eb0
extern "C" int func_ov080_02124eb0(daBttBk_c *self)
{
    self->mHorzSpeed = 0;
    ((dCc_c *)&self->mdCcAc_c)->Clear();
    self->mState = 2;
    return 1;
}

/* State 1 update: copies the carrier's angles; breaks the crate unless
 * mFlags bit 8 is set and bit 13 clear. */
// @symbol func_ov080_02124edc
extern "C" int func_ov080_02124edc(char *c)
{
    daBttBk_c *self = (daBttBk_c *)c;
    self->mAngleX = self->mCarrier->mAngleX;
    self->mAngleY = self->mCarrier->mAngleY;
    self->mPrevAngleX = self->mAngleX;
    self->mPrevAngleY = self->mAngleY;
    {
        int flags = self->mFlags;
        int hasBit8 = (int)((flags & 0x100) != 0);
        if (hasBit8 != 0) {
            int hasBit13 = (int)((flags & 0x2000) != 0);
            if (hasBit13 == 0) goto clear;
        }
        {
            Vector3 vec;
            Vector3 vec2;
            int x = self->mPosX;
            int z = self->mPosZ;
            int y = self->mPosY + 0xb4000;
            vec.x = x;
            vec.y = y;
            vec.z = z;
            vec2.x = vec.x;
            vec2.y = vec.y;
            vec2.z = vec.z;
            self->PoofDustAt(vec2);
            _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(6, vec.x, vec.y, vec.z);

            {
                int ybase = self->mPosY;
                int xx = vec.x;
                int y2 = ybase + 0x64000;
                int zz = vec.z;
                Vector3 v3;
                v3.x = xx;
                v3.z = zz;
                vec.y = y2;
                v3.y = y2;
                _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(self, v3, 5, 0xf000, 0);
            }

            Sound::PlayBank3(0x41, *(Vector3 *)&self->mCamSpacePosX);
            self->MarkForDestruction();
        }
    }
clear:
    ((dCc_c *)&self->mdCcAc_c)->Clear();
    return 1;
}

/* State 1 enter. */
// @symbol func_ov080_02124fec
extern "C" int func_ov080_02124fec(daBttBk_c *self)
{
    u32 *flagsPtr = &self->mFlags;
    int one = 1;
    self->mState = one;
    *flagsPtr &= ~3;
    return one;
}

/* State 0 update: fall, bounce at 60% on landing, collide. */
// @symbol func_ov080_0212500c
extern "C" int func_ov080_0212500c(daBttBk_c *self)
{
    dBgCh_Actr_UpdateDiscreteNoLava_veneer(&self->mWithMeshClsn);
    if (self->mWithMeshClsn.JustHitGround()) {
        int v = self->mVertSpeed * -0x3c;
        self->mVertSpeed = v / 100;
    } else if (self->mWithMeshClsn.IsOnGround()) {
        self->mVertSpeed = 0xc000;
    }
    self->UpdatePos(&self->mdCcAc_c);
    func_ov080_02124acc((char *)self);
    ((dCc_c *)&self->mdCcAc_c)->Clear();
    ((dCc_c *)&self->mdCcAc_c)->Update();
    return 1;
}

/* State 0 enter. */
// @symbol func_ov080_0212509c
extern "C" int func_ov080_0212509c(daBttBk_c *self)
{
    self->mVertSpeed = 49152;
    self->mWithMeshClsn.SetLimMovFlag();
    self->mState = 0;
    return 1;
}

/* Runs the current state row's update function (second PMF). */
// @symbol func_ov080_021250c8
extern "C" void func_ov080_021250c8(char *raw)
{
    C *c = (C *)raw;
    PMF *p = c->pp + 1;
    (c->**p)();
}

/* Runs the current state row's enter function (first PMF). */
// @symbol func_ov080_02125104
extern "C" void func_ov080_02125104(C *c)
{
    PMF *p = c->pp;
    (c->**p)();
}

/* Changes state: points mStateRow at row i of the state table, then enters it. */
// @symbol func_ov080_0212513c
extern "C" void func_ov080_0212513c(char *c, int i)
{
    ((daBttBk_c *)c)->mStateRow = data_ov080_0212847c + (i << 4);
    func_ov080_02125104((C *)c);
}

/* Slot 3: one shared model file handle to give back. */
// @symbol _ZN9daBttBk_c16CleanupResourcesEv
s32 daBttBk_c::CleanupResources()
{
    ((SharedFilePtr *)&data_ov080_02128468)->Release();
    return 1;
}

/* Slot 12: empty override. */
// @symbol _ZN9daBttBk_c16OnPendingDestroyEv
void daBttBk_c::OnPendingDestroy()
{
}

// @symbol _ZN9daBttBk_c6RenderEv
int daBttBk_c::Render()
{
    int flags = mFlags;
    flags = flags & 0x40000;
    flags = flags ? 1 : 0;
    if (flags) return 1;
    mModel.Render(0);
    return 1;
}

// @symbol _ZN9daBttBk_c8BehaviorEv
int daBttBk_c::Behavior()
{
    func_ov080_021250c8((char *)this);
    func_ov080_02124c3c(this);
    return 1;
}

// @symbol _ZN9daBttBk_c13InitResourcesEv
int daBttBk_c::InitResources()
{
    Vector3 pos;
    void *file = Model::LoadFile(*(SharedFilePtr *)&data_ov080_02128468);
    mModel.SetFile((BMD_File *)file, 1, 1);
    mShadowModel.InitCuboid();
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, this, 0x64000, 0x78000, 0x800004, 0x9010);
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;
    {
        int spawnY;
        pos.x = mPosX;
        spawnY = mPosY;
        pos.y = spawnY;
        pos.z = mPosZ;
        pos.y = spawnY + 0xc8000;
    }
    dBgCh_Gnd ground;
    ground.SetObjAndPos(pos, 0);
    if (ground.DetectClsn())
        mPosY = ground.clsnY;
    else
        mPosY = pos.y;
    mScaleX = 0x1000;
    mScaleY = 0x1000;
    mScaleZ = 0x1000;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(&mWithMeshClsn, this, 0x32000, 0x32000, 0, 0);
    mCarrier = 0;
    ((Fn2513c1)func_ov080_0212513c)((char *)this);
    func_ov080_02124c3c(this);
    return 1;
}

// @symbol _ZN9daBttBk_c13OnTurnIntoEggER6Player
void daBttBk_c::OnTurnIntoEgg(Player &player)
{
    if (player.IsCollectingCap()) {
        GivePlayerCoins(player, 5, 0);
    }
    Vector3 vec;
    Vector3 vec2;
    int x = mPosX;
    int z = mPosZ;
    int y = mPosY + 0xb4000;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec2.x = vec.x;
    vec2.y = vec.y;
    vec2.z = vec.z;
    PoofDustAt(vec2);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(6, vec.x, vec.y, vec.z);
    Sound::PlayBank3(0x41, *(Vector3 *)&mCamSpacePosX);
    MarkForDestruction();
}

// @symbol daBttBk_c_classInit
extern "C" daBttBk_c *daBttBk_c_classInit()
{
    return new daBttBk_c();
}
