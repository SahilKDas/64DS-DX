//cpp
/* Production translation unit for ov081/daSnowball_c.
 * 13 function(s), .text 0x02125f14..0x02126504. The SNOWBALL actor.
 *
 * NAME: _ZTS12daSnowball_c is "12daSnowball_c" at ov081 0x02128a88; _ZTI at
 * 0x02128a7c reads [__si_class_type_info, that string, _ZTI12dEnemyBase_c],
 * and the word before the _ZTV12daSnowball_c address point (0x02128abc)
 * points at that _ZTI. The tree previously called the class Snowball
 * (coined; that spelling is not in the cartridge).
 *
 * This is the whole unit: the destructor pair, the five state and collision
 * helpers that sit between it and the virtuals, the five virtuals, then the
 * registry factory daSnowball_c_classInit (0x021264b4). The
 * out-of-line destructor is the key function, so this TU emits
 * _ZTV/_ZTI/_ZTS12daSnowball_c; the manifest's compiler_only_output rows
 * route those (and the homeless D2) to the cartridge's own copies.
 *
 * decl_common.h is first so common.h's Matrix4x3 wins over math/Matrix.h.
 * Under `#pragma defer_codegen off` .text is laid down in source order, so
 * this file is ROM-ascending.
 *
 * Leftover: the five helpers keep their func_ov081_* names and their
 * offset-arithmetic bodies, with C linkage, exactly as the loose files had
 * them. None has a recovered name, and InitResources hands
 * func_ov081_021261d4 a state record by address, so they are not yet
 * methods.
 */

#pragma defer_codegen off

#include "decl_common.h"
#include "daSnowball_c.h"
#include "Player.h"
#include "SharedFilePtr.h"

extern SharedFilePtr data_ov081_02128d90;

extern "C" {
unsigned short DecIfAbove0_Short(unsigned short *p);
void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
    void *thiz, void *actor, int radius, int height, unsigned int flags, unsigned int vuln);
void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
    void *thiz, void *actor, int radius, int height, void *a, int b);
}

extern "C" {
void *_ZN8dActor_c10FindWithIDEj(unsigned int id);
void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
void _ZN7fBase_c18MarkForDestructionEv(void *thiz);
void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(
    void *player, const Vector3 *pos, unsigned int a, int b, unsigned int c, unsigned int d, unsigned int e);
void func_02012694(int a0, void *a1);
int _ZNK10dBgCh_Actr10IsOnGroundEv(void *c);
void Vec3_Asr(void *dst, void *src, int n);
void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
void Matrix4x3_ApplyInPlaceToRotationXYZExt(void *m, short rx, short ry, short rz);
}

extern Matrix4x3 data_020a0e68;

// @symbol _ZN12daSnowball_cD1Ev
// @symbol _ZN12daSnowball_cD0Ev
/* The key function. D1 stores the vtable, destroys the members in reverse
 * declaration order, then runs dEnemyBase_c::~dEnemyBase_c; D0 does the
 * same and hands the object to dEnemyBase_c's inline operator delete. The
 * base-object D2 mwcc also emits has no home in the cartridge. */
daSnowball_c::~daSnowball_c()
{
}

enum SnowballBool { SNOWBALL_FALSE, SNOWBALL_TRUE };

// @symbol func_ov081_02125fb8
/* Contact handler: something hit the snowball (flags +0x130, other actor's
 * id +0x134). A player takes 1 damage unless it is invincible; either way
 * the snowball bursts. */
extern "C" void func_ov081_02125fb8(void *thisp)
{
    char *self = (char *)thisp;
    void *other;
    int flags;
    unsigned int id = *(unsigned int *)(self + 0x134);
    if (id == 0) return;
    other = _ZN8dActor_c10FindWithIDEj(id);
    if (other == 0) return;
    flags = *(int *)(self + 0x130);
    if ((flags & 0x10) || (flags & 0x40000)) {
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x11c, *(int *)(self + 0x5c), *(int *)(self + 0x60), *(int *)(self + 0x64));
        _ZN7fBase_c18MarkForDestructionEv(self);
        return;
    }
    {
        SnowballBool eq = (SnowballBool)(*(unsigned short *)((char *)other + 0xc) == 0xbf);
        if (eq != SNOWBALL_FALSE) {
            if (*(unsigned char *)((char *)other + 0x6fb) != 0) return;
            if (*(unsigned char *)((char *)other + 0x6f9) == 1) {
                _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x11c, *(int *)(self + 0x5c), *(int *)(self + 0x60), *(int *)(self + 0x64));
                _ZN7fBase_c18MarkForDestructionEv(self);
                return;
            }
            {
                Vector3 pos;
                pos.x = *(int *)(self + 0x5c);
                pos.y = *(int *)(self + 0x60);
                pos.z = *(int *)(self + 0x64);
                _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(other, &pos, 1, 0xc000, 1, 0, 1);
                func_02012694(0x3c, self + 0x74);
                _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x11c, *(int *)(self + 0x5c), *(int *)(self + 0x60), *(int *)(self + 0x64));
                _ZN7fBase_c18MarkForDestructionEv(self);
                return;
            }
        }
    }
}

// @symbol func_ov081_021260fc
/* State update, presumably the record's second function (Behavior calls it).
 * Once the snowball has dropped below its spawn height at terminal speed,
 * stop horizontal motion; burst on landing or when the timer runs out, and
 * hand any contact to func_ov081_02125fb8. */
extern "C" int func_ov081_021260fc(char *thiz)
{
    if (*(int *)(thiz + 0x388) == 0 && *(int *)(thiz + 0xa0) == -0x3c000) {
        if (*(int *)(thiz + 0x380) > *(int *)(thiz + 0x60)) {
            *(int *)(thiz + 0xa8) = 0;
            *(int *)(thiz + 0x9c) = 0;
            *(int *)(thiz + 0x388) = 1;
        }
    }
    if (*(unsigned short *)(thiz + 0x100) == 0 ||
        _ZNK10dBgCh_Actr10IsOnGroundEv(thiz + 0x144) != 0) {
        func_02012694(0x3c, thiz + 0x74);
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(
            0x11c, *(int *)(thiz + 0x5c), *(int *)(thiz + 0x60), *(int *)(thiz + 0x64));
        _ZN7fBase_c18MarkForDestructionEv(thiz);
    }
    if (*(int *)(thiz + 0xa4) != 0 || *(int *)(thiz + 0xac) != 0) {
        func_ov081_02125fb8(thiz);
    }
    return 1;
}

// @symbol func_ov081_021261b8
/* State entry, presumably the record's first function (func_ov081_021261d4
 * runs it): clear the stopped flag and arm a 200-frame timer. */
extern "C" int func_ov081_021261b8(char *p)
{
    *(int *)(p + 0x388) = 0;
    *(short *)(p + 0x100) = 200;
    return 1;
}

struct SnowballStateOwner;
typedef int (SnowballStateOwner::*SnowballStatePMF)();
struct SnowballStateOwner { char pad[0x378]; SnowballStatePMF *pp; };

// @symbol func_ov081_021261d4
/* Installs a state record in unk_378 and runs its entry function. */
extern "C" int func_ov081_021261d4(SnowballStateOwner *c, SnowballStatePMF *p)
{
    c->pp = p;
    SnowballStatePMF *q = c->pp;
    if (*q == 0) return 1;
    return (c->**q)();
}

// @symbol func_ov081_02126224
/* Builds the model matrix from position and angle into +0x31c. */
extern "C" void func_ov081_02126224(char *c)
{
    int v[3];
    Vec3_Asr(v, c + 0x5c, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, v[0], v[1], v[2]);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68,
        *(short *)(c + 0x8c), *(short *)(c + 0x8e), *(short *)(c + 0x90));
    *(Matrix4x3 *)(c + 0x31c) = data_020a0e68;
}

/* Behavior's state call. The pointer lives in unk_378; the member
 * function sits 8 bytes into that record. */
struct SnowballState;
typedef void (SnowballState::*SnowballStateFn)();
struct SnowballStateRec { char pad[8]; SnowballStateFn fn; };

// @symbol _ZN12daSnowball_c16CleanupResourcesEv
/* Releases the one shared file InitResources claimed. Touches no field: the
 * ROM body never reads `this`, and as a method it receives one and ignores
 * it, which measured byte-free. */
int daSnowball_c::CleanupResources()
{
    data_ov081_02128d90.Release();
    return 1;
}

// @symbol _ZN12daSnowball_c16OnPendingDestroyEv
/* fBase_c slot 12. Empty in the ROM: four bytes, `bx lr`. */
void daSnowball_c::OnPendingDestroy()
{
}

// @symbol _ZN12daSnowball_c6RenderEv
int daSnowball_c::Render()
{
    mModel.Render(0);
    return 1;
}

// @symbol _ZN12daSnowball_c8BehaviorEv
int daSnowball_c::Behavior()
{
    DecIfAbove0_Short((unsigned short *)&mStateTimer);
    SnowballStateRec *rec = *(SnowballStateRec **)&unk_378;
    if (rec->fn != 0)
        (((SnowballState *)(char *)this)->*(rec->fn))();
    int v = mVertSpeed + mVertAccel;
    int hi = mTerminalVelocity;
    if (v >= hi)
        hi = v;
    int tmp = unk_0ac;
    mVertSpeed = hi;
    unk_0ac = tmp;
    UpdatePosWithOnlySpeed((dCc_c *)&mdCcAc_c);
    UpdateWMClsn(mWithMeshClsn, 0);
    mAngleY = mPrevAngleY;
    func_ov081_02126224((char *)this);
    mdCcAc_c.Clear();
    Player *player = ClosestPlayer();
    if (player != 0 && player->mIsVanish == 0)
        mdCcAc_c.Update();
    return 1;
}

// @symbol _ZN12daSnowball_c13InitResourcesEv
int daSnowball_c::InitResources()
{
    void *file = Model::LoadFile(data_ov081_02128d90);
    if (mModel.SetFile((BMD_File *)file, 1, -1) == 0)
        return 0;

    mShadowModel.InitCylinder();

    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;
    /* Leftover: both Init calls below stay on their mangled names.
       dCcAc_c::Init takes Fix12<int> by value, and passing the radii that way
       spills them into .rodata, so the method call no longer matches.
       dBgCh_Actr::Init is declared with Fix12i, a plain s32 that mangles as
       `i`, so the method call would name a symbol other than the ROM's
       5Fix12IiE one. */
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
        &mdCcAc_c, this, 0x1e000, 0x1e000, 0x200004, 0x40010);

    unk_37c = mPosX;
    unk_380 = mPosY;
    unk_384 = mPosZ;
    mPosY += 0x32000;
    mAngleY = mPrevAngleY;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        &mWithMeshClsn, this, 0x14000, 0x14000, 0, 0);

    func_ov081_021261d4((SnowballStateOwner *)this, (SnowballStatePMF *)&data_ov081_02128eb4);
    return 1;
}

// @symbol daSnowball_c_classInit
extern "C" daSnowball_c *daSnowball_c_classInit()
{
    return new daSnowball_c();
}
