//cpp
/* ov020/daBook_c+daBookGen_c -- the haunted books of Big Boo's Haunt
 * (daBook_c) and the generator that fires them (daBookGen_c). 31 functions,
 * .text 0x021111a0..0x02112938: the first function of ov020 .text up to
 * daChair_c (src/actors/daChair_c.cpp), which begins at 0x02112938.
 *
 * Class identity comes from the ROM RTTI:
 *   daBook_c     _ZTS 0x0211482c, _ZTI 0x02114844, _ZTV 0x0211495c
 *                (__si_class_type_info, parent dEnemyBase_c)
 *   daBookGen_c  _ZTI 0x02114838, _ZTS 0x02114850, _ZTV 0x021148d8
 *                (__si_class_type_info, parent dActor_c)
 * The coined names BookShot and BookShotSpawner survive only as symbols.txt
 * aliases of the two vtables.
 *
 * SOURCE ORDER IS ROM ORDER. `defer_codegen off` makes mwccarm emit each
 * function as it is parsed. Each empty out-of-line destructor emits D1 then
 * D0; its D2 has no ROM home and is deadstripped (policy rows in the TU
 * manifest). Defining the destructors here makes this file the key-function
 * home of both vtables and their RTTI.
 *
 * The four registry factories come last, in ROM order, and abut each other
 * with no gap: daBook_c_classInit_BOOK_SWITCH (0x021127f4),
 * daBookGen_c_classInit (0x02112850), daBook_c_classInit_KILLER_BOOK
 * (0x02112880) and daBook_c_classInit_SHOOT_BOOK (0x021128dc). Three
 * profiles (BOOK_SWITCH, KILLER_BOOK, SHOOT_BOOK) build the same daBook_c and
 * InitResources tells them apart by actorID; BOOK_GENERATOR builds
 * daBookGen_c. Neither class declares a constructor, so each factory is a
 * plain `return new`. The classInit spellings are reconstructed from later
 * EAD lineage; the historical aliases were func_ov020_021127f4,
 * BookShotSpawner_Spawn, Bookend_Spawn and BookShot_Spawn.
 *
 * common.h comes first, and that is load-bearing: it and math/Matrix.h
 * define Matrix4x3 under one guard, and only common.h's flat s32 m[12]
 * spelling makes InitResources' mShadowMat = IDENTITY_MATRIX4X3 the ROM's
 * twelve-word block copy.
 *
 * Leftover (still to deslop; each change needs a rematch):
 *  - the fourteen func_ov020_* helpers take a raw `char *` and reach fields
 *    by offset; they are state/member bodies whose real names and member
 *    form are not yet recovered. The animation calls go through the
 *    Animation base at +0x160: calling through ModelAnim (+0x110) adds a
 *    base adjustment and does not match;
 *  - calls that pass Fix12<int> by value (ModelAnim::SetAnim,
 *    dBgCh_Actr::Init, dCcAcPos_c::Init, Player::Hurt, Player::Bounce,
 *    DropShadowRadHeight, cstd::atan2) stay mangled extern "C" calls: the
 *    headers declare those parameters as Fix12i (a plain s32), so a member
 *    call would mangle to a symbol the ROM does not have;
 *  - GetWallResult has no dBgCh_Actr method, and the ROM calls
 *    dBgCh_Actr_UpdateContinuous_Veneer, not UpdateContinuous;
 *  - data_ov020_02114aa0/aa8/ab0/ab8 are the four SharedFilePtr model and
 *    animation slots. SharedFilePtr's layout is not recovered, so they are
 *    typed `int[]` and [1] is the loaded file pointer;
 *  - the s16 triple at 0x444 (aim pitch/yaw/roll toward the closest player)
 *    is still padding in the header.
 */

#include "common.h"
#include "daBook_c.h"
#include "daBookGen_c.h"
#include "Player.h"
#include "SharedFilePtr.h"
#include "SurfaceInfo.h"
#include "dCc_c.h"
#include "Sound.h"

int ApproachLinear(int &value, int target, int step);
int ApproachLinear(short &value, short target, short step);

/* A destructor-free xyz view; func_ov020_021115ac's copy through a Vector3
 * local does not match. */
typedef struct { int x, y, z; } Vec3;

#define A8C (*(u16 *)((char *)c + 0x8c))
#define A8E (*(u16 *)((char *)c + 0x8e))

extern "C" {
void func_ov020_021112b0(char *c);
void func_ov020_02111340(char *c);
int  func_ov020_02111418(char *c);
int  func_ov020_021115ac(char *c);
void func_ov020_0211174c(char *c);
void func_ov020_021119dc(char *c);
void func_ov020_02111aa8(char *c);
void func_ov020_02111b28(char *c);
void func_ov020_02111c30(char *c);
void func_ov020_02111ee0(char *c);
void func_ov020_02111fc4(char *c);
void func_ov020_02112080(char *c);
void func_ov020_02112110(char *c);
void func_ov020_0211216c(char *c);

short _ZN4cstd5atan2E5Fix12IiES1_(int, int);
void Vec3_Sub(Vector3 *d, Vector3 *a, Vector3 *b);
int  Vec3_HorzLen(const Vector3 *);
Fix12i Vec3_Dist(const Vector3 *a, const Vector3 *b);
s16  Vec3_HorzAngle(const Vector3 *a, const Vector3 *b);
int  Vec3_HorzDist(const Vector3 *a, const Vector3 *b);
void AddVec3(void *a, void *b, void *c_);
int  RandomIntInternal(void *p);
int  _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(Player *self, const Vector3 *pos, unsigned int a, int fix, u8 b, u8 d, u8 e);
void _ZN6Player6BounceE5Fix12IiE(Player *self, int fix);
void func_ov063_0211cae8(void *found, unsigned int mask);
void func_0203568c(int *p, int v);
void func_0201267c(unsigned int id, const Vector3 *pos);
void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(ModelAnim *, BCA_File *, int, int, u16);
void dBgCh_Actr_UpdateContinuous_Veneer(void *c_);
void *_ZNK10dBgCh_Actr13GetWallResultEv(void *self);
void Matrix4x3_FromRotationZXYExt(void *m, int x, int y, int z);
void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(dActor_c *thiz, ShadowModel *sm, Matrix4x3 *mtx, int radius, int depth, u8 opacity);
void UnloadBlueCoinModel(void *);
void LoadBlueCoinModel(void *c);
void func_0200f760(void *a, void *b);
void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
    dBgCh_Actr *self, dActor_c *actor, int radius, int height,
    Vector3_16 *first, Vector3_16 *second);
void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
    dCcAcPos_c *self, dActor_c *actor, const Vector3 *offset,
    int radius, int height, u32 flags, u32 vulnFlags);
}

extern int data_0209e650[];
extern s16 data_02082214[];
extern u8 data_ov020_02114828[];
extern int data_ov020_02114aa0[];
extern int data_ov020_02114aa8[];
extern int data_ov020_02114ab0[];
extern int data_ov020_02114ab8[];
extern Matrix4x3 IDENTITY_MATRIX4X3;

#pragma defer_codegen off

// @symbol _ZN8daBook_cD1Ev
// @symbol _ZN8daBook_cD0Ev
/* One native destructor definition emits both ROM variants. D1 destroys the
 * class-typed members in reverse declaration order, then dEnemyBase_c; D0 is
 * the same destruction followed by the inherited actor-heap operator delete. */
daBook_c::~daBook_c()
{
}

#ifdef _MSC_VER
/* The host uses the flat ROM D0 name, which MSVC never emits: it folds the
 * Itanium destructor variants into the one ~daBook_c() above. This arm
 * spells out what the deleting destructor does -- the D1 body, called
 * qualified so it is a direct call, then the class-specific operator delete.
 * Nothing here reaches mwccarm. */
extern "C" daBook_c *_ZN8daBook_cD0Ev(daBook_c *thiz)
{
    thiz->daBook_c::~daBook_c();
    daBook_c::operator delete(thiz);
    return thiz;
}
#endif

// @symbol _ZN11daBookGen_cD1Ev
// @symbol _ZN11daBookGen_cD0Ev
daBookGen_c::~daBookGen_c()
{
}

#ifdef _MSC_VER
/* Host D0, as for daBook_c above. */
extern "C" daBookGen_c *_ZN11daBookGen_cD0Ev(daBookGen_c *thiz)
{
    thiz->daBookGen_c::~daBookGen_c();
    daBookGen_c::operator delete(thiz);
    return thiz;
}
#endif

// @symbol func_ov020_021112b0
extern "C" void func_ov020_021112b0(char *c)
{
  char *p = (char*)((dActor_c*)c)->ClosestPlayer();
  if (!p)
    return;
  struct Vector3 *ps = (struct Vector3 *)(p + 0x5c);
  struct Vector3 tmp;
  tmp.x = ps->x;
  tmp.y = ps->y;
  tmp.z = ps->z;
  struct Vector3 d;
  Vec3_Sub(&d, &tmp, (struct Vector3 *)(c + 0x5c));
  *((short *)(c + 0x446)) = _ZN4cstd5atan2E5Fix12IiES1_(d.x, d.z);
  *((short *)(c + 0x444)) = _ZN4cstd5atan2E5Fix12IiES1_(d.y, Vec3_HorzLen(&d));
  *((short *)(c + 0x448)) = 0x4000;
}

// @symbol func_ov020_02111340
extern "C" void func_ov020_02111340(char *c)
{
    int bit = ((unsigned int)RandomIntInternal(data_0209e650) >> 16) & 1;
    if (*(int *)(c + 0x41c) == 0)
        return;
    {
        char *found = (char *)dActor_c::FindWithID(*(unsigned int *)(c + 0x418));
        if (found == 0)
            return;
        {
            struct Vector3 pos;
            struct Vector3_16 rot;
            int *fp = (int *)(found + 0x5c);
            int mul = 0x1f4 * bit;
            int x = fp[0];
            int dx = mul - 0xfa;
            pos.x = x;
            {
                int y = fp[1];
                int nx = x + (dx << 12);
                pos.y = y;
                {
                    int z = fp[2];
                    int ny = y + 0x41000;
                    pos.z = z;
                    pos.x = nx;
                    pos.y = ny;
                }
            }
            pos.z = *(int *)(*(char **)(c + 0x41c) + 0x64);
            rot.y = (short)((bit << 15) + 0x4000);
            rot.x = 0;
            rot.z = 0;
            dActor_c::Spawn(0x145, 0, pos, &rot, *(signed char *)(c + 0xcc), -1);
        }
    }
}

// @symbol func_ov020_02111418
extern "C" int func_ov020_02111418(char *c)
{
    int r = func_ov020_021115ac(c);
    if (r == 1) { func_ov020_02112110(c); return 1; }
    if (r == 2) {
        Vector3 v;
        v.x = *(int*)(c+0x5c); v.y = *(int*)(c+0x60); v.z = *(int*)(c+0x64);
        _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(*(Player**)(c+0x41c), &v, 0, 0xc000, 1, 0, 1);
        func_ov020_02112110(c);
        return 1;
    }
    if (r == -1) {
        int eq = (*(unsigned short*)(c+0xc) == 0x147);
        if (eq) {
            Vector3 v;
            v.x = *(int*)(c+0x5c); v.y = *(int*)(c+0x60); v.z = *(int*)(c+0x64);
            _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(*(Player**)(c+0x41c), &v, 2, 0xc000, 1, 0, 1);
        } else {
            Vector3 v;
            v.x = *(int*)(c+0x5c); v.y = *(int*)(c+0x60); v.z = *(int*)(c+0x64);
            _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(*(Player**)(c+0x41c), &v, 1, 0xc000, 1, 0, 1);
        }
        *(char*)(c+0x108) = 0;
        func_ov020_02112110(c);
        return 1;
    }
    if (r != -2) return 0;
    _ZN6Player6BounceE5Fix12IiE(*(Player**)(c+0x41c), 0x28000);
    func_ov020_02112110(c);
    return 1;
}

// @symbol func_ov020_021115ac
/* The ROM reserves a 12-byte frame it never touches; two address-taken
 * volatile locals reserve it with no emitted code. */
extern "C" int func_ov020_021115ac(char *c)
{
    u32 id;
    char *found;
    int t;
    volatile int t1, t2;
    (void)&t1; (void)&t2;

    id = *(u32 *)(c + 0x240);
    if (id == 0)
        return 0;
    found = (char *)dActor_c::FindWithID(id);
    if (found == 0)
        return 0;

    t = (int)(*(u16 *)(found + 0xc) == 0xbf);
    if (t == 0)
        return 0;

    if ((*(u32 *)(c + 0x23c) & 0x8000) != 0)
        return 0;

    if (*(unsigned char *)(found + 0x6fb) != 0) {
        t = (int)(*(u16 *)(c + 0xc) == 0x145);
        if (t != 0)
            return 0;
    }

    *(void **)(c + 0x41c) = found;

    if ((*(u32 *)(c + 0x23c) & 0x26fe0) != 0)
        return 1;

    if (((dActor_c *)c)->BumpedUnderneathByPlayer(*(Player *)found) != 0) {
        Vec3 *p = (Vec3 *)(found + 0xa4);
        Vec3 v;
        v.z = p->z;
        v.x = p->x;
        v.y = 0;
        *(int *)(found + 0xa4) = v.x;
        *(int *)(found + 0xa8) = v.y;
        *(int *)(found + 0xac) = v.z;
        *(void **)(c + 0x41c) = found;
        return 1;
    }

    if (((dActor_c *)c)->JumpedOnByPlayer(*(dCc_c *)(c + 0x21c), *(Player *)found) != 0) {
        *(void **)(c + 0x41c) = found;
        return ~1;
    }

    if (*(unsigned char *)(found + 0x6f9) != 0)
        return 2;

    if (*(unsigned char *)(found + 0x6fb) != 0) {
        t = (int)(*(u16 *)(c + 0xc) == 0x147);
        if (t != 0)
            return 0;
    }
    return ~0;
}

// @symbol func_ov020_0211174c
extern "C" void func_ov020_0211174c(char *c)
{
    char *found;

    dActor_c::FindWithID(*(unsigned int *)(c + 0x418));
    found = (char *)dActor_c::FindWithID(*(unsigned int *)(c + 0x418));
    if (found == 0)
        return;

    switch (*(s32 *)(c + 0x424)) {
    case 6:
        if ((*(u8 *)(found + 0x157) & 8) == 0)
            return;
        *(s32 *)(c + 0x424) = 7;
        *(u16 *)(c + 0x100) = 0;
        return;
    case 7:
    {
        u16 *ctr = (u16 *)(c + 0x100);
        s32 *z;
        s32 *fl;
        if (*ctr != 0) {
            *ctr -= 1;
            return;
        }
        z = (s32 *)(c + 0x64);
        *z -= 0x4000;
        if (*(s32 *)(c + 0x434) - *(s32 *)(c + 0x64) < 0x28000)
            return;
        *(s32 *)(c + 0x64) = *(s32 *)(c + 0x434) - 0x28000;
        fl = (s32 *)(c + 0x234);
        *(s32 *)(c + 0x424) = 8;
        *fl &= ~1;
        return;
    }
    case 8:
    {
        s32 *fl;
        if (func_ov020_021115ac(c) <= 0)
            return;
        fl = (s32 *)(c + 0x234);
        *(s32 *)(c + 0x424) = 9;
        *fl |= 1;
        Sound::PlayBank0(0xb5, *(Vector3 *)(c + 0x74));
        return;
    }
    case 9:
    {
        s32 *z = (s32 *)(c + 0x64);
        *z += 0xa000;
        if (*(s32 *)(c + 0x64) < *(s32 *)(c + 0x434))
            return;
        {
            int idx = *(s32 *)(c + 8);
            if ((*(u8 *)(found + 0x157) & 7) != data_ov020_02114828[idx])
                func_ov020_02111340(c);
        }
        *(s32 *)(c + 0x41c) = 0;
        func_ov063_0211cae8(found, (1u << *(s32 *)(c + 8)) & 0xff);
        *(s32 *)(c + 0x64) = *(s32 *)(c + 0x434);
        *(s32 *)(c + 0x424) = 0xa;
        return;
    }
    case 10:
    {
        s32 *z;
        if ((*(u8 *)(found + 0x157) & (1 << *(s32 *)(c + 8))) == 0) {
            *(s32 *)(c + 0x424) = 7;
            *(u16 *)(c + 0x100) = 0xa;
            return;
        }
        if (*(u8 *)(found + 0x150) == 2) {
            z = (s32 *)(c + 0x5c);
            *z += 0x5000;
            if (*(s32 *)(c + 0x5c) < (s32)0xffa24000)
                return;
            ((dActor_c *)c)->MarkForDestruction();
            return;
        }
        if (*(u8 *)(found + 0x150) != 3)
            return;
        ((dActor_c *)c)->MarkForDestruction();
        return;
    }
    default:
        return;
    }
}

// @symbol func_ov020_021119dc
extern "C" void func_ov020_021119dc(char *c)
{
    func_0203568c((int*)(c + 0x25c), 0x64000);
    if (*(unsigned short*)(c + 0x104) != 0) {
        unsigned short* p = (unsigned short*)(c + 0x104);
        *p = (unsigned short)(*p - 1);
        if (*(unsigned short*)(c + 0x104) != 0) {
            int* q = (int*)(c + 0x234);
            *q = *q | 1;
        } else {
            int* q = (int*)(c + 0x234);
            *q = *q & ~1;
        }
    }
    ApproachLinear(*(int*)(c + 0xa8), 0, 0x800);
    ApproachLinear(*(int*)(c + 0x98), 0, 0x800);
    if (*(int*)(c + 0xa8) == 0 && *(int*)(c + 0x98) == 0) {
        func_0203568c((int*)(c + 0x25c), 0x32000);
        *(int*)(c + 0x424) = *(int*)(c + 0x428);
        if (*(int*)(c + 0x424) == 3)
            *(int*)(c + 0x424) = 2;
    }
    ((dActor_c*)c)->UpdatePos(0);
}

// @symbol func_ov020_02111aa8
extern "C" void func_ov020_02111aa8(char *c)
{
    *(unsigned char *)(c + 0x108) = 0;
    (*(unsigned short *)(c + 0x100))++;
    if (*(unsigned short *)(c + 0x100) >= 3) {
        int *bf;
        int t;
        int sid;
        *(int *)(c + 0x424) = 3;
        *(int *)(c + 0x98) = 0x32000;
        bf = (int *)(c + 0x234);
        t = *bf;
        sid = 0x166;
        *bf = t & ~1;
        func_0201267c(sid, (const Vector3*)(c + 0x74));
    }
    ApproachLinear(*(int *)(c + 0x98), 0x32000, 0x1000);
    ((dActor_c*)c)->UpdatePos(0);
}

// @symbol func_ov020_02111b28
extern "C" void func_ov020_02111b28(char *c)
{
  if (*(unsigned char*)(c+0x450) != 0) {
    ((Animation *)(c + 0x160))->Advance();
  }
  AddVec3(c+0x5c, c+0xa4, c+0x5c);
  if (func_ov020_02111418(c) != 0) return;
  dBgCh_Actr_UpdateContinuous_Veneer(c+0x25c);
  if (((dBgCh_Actr *)(c + 0x25c))->IsOnGround() != 0) {
    *(unsigned char*)(c+0x108) = 0;
    func_ov020_02112110(c);
    func_0201267c(0xc5, (const Vector3*)(c+0x74));
    return;
  }
  if (((dBgCh_Actr *)(c + 0x25c))->IsOnWall() == 0) return;
  {
    Vector3 normal;
    void* w = _ZNK10dBgCh_Actr13GetWallResultEv(c+0x25c);
    ((SurfaceInfo *)((char *)w + 4))->CopyNormalTo(normal);
    short angle = _ZN4cstd5atan2E5Fix12IiES1_(normal.x, normal.z);
    if (((dActor_c*)c)->GetSubtraction(*(short*)(c+0x94), angle) <= 0x4000) return;
  }
  *(unsigned char*)(c+0x108) = 0;
  func_ov020_02112110(c);
  func_0201267c(0xc5, (const Vector3*)(c+0x74));
}

// @symbol func_ov020_02111c30
extern "C" void func_ov020_02111c30(char *c)
{
    if (func_ov020_02111418(c))
        return;

    ((Animation *)(c + 0x160))->Advance();

    {
        if (ApproachLinear(*(s32 *)(c + 0x98), 0xa000, 0xa00) == 0)
            return;
    }

    if (((Animation *)(c + 0x160))->Finished()) {
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj((ModelAnim*)(c + 0x110), (BCA_File*)data_ov020_02114ab0[1], 0, 0x1000, 0);
        *(s32 *)(c + 0x424) = 3;
        *(s32 *)(c + 0x98) = 0;
        func_ov020_021112b0(c);

        *(s16 *)(c + 0x92) = -*(s16 *)(c + 0x444);
        *(s16 *)(c + 0x8c) = *(s16 *)(c + 0x92);
        *(s16 *)(c + 0x94) = *(s16 *)(c + 0x446);
        *(s16 *)(c + 0x8e) = *(s16 *)(c + 0x94);

        {
            s32 r = (s32)(((s64)data_02082214[(A8C >> 4) * 2 + 1] * 0x32000 + 0x800) >> 12);
            *(s32 *)(c + 0xa4) = (s32)(((s64)r * data_02082214[(A8E >> 4) * 2] + 0x800) >> 12);
            *(s32 *)(c + 0xa8) = (s32)(((s64)data_02082214[(A8C >> 4) * 2] * -0x32000 + 0x800) >> 12);
            *(s32 *)(c + 0xac) = (s32)(((s64)r * data_02082214[(A8E >> 4) * 2 + 1] + 0x800) >> 12);
        }
        return;
    }

    (*(u16 *)(c + 0x100))++;
    if (*(u16 *)(c + 0x100) < 5)
        return;

    func_ov020_021112b0(c);
    ApproachLinear(*(s16 *)(c + 0x8e), *(s16 *)(c + 0x446), 0x7d0);
    ApproachLinear(*(s16 *)(c + 0x8c), -*(s16 *)(c + 0x444), 0x7d0);

    if (*(u16 *)(c + 0x100) < 9)
        return;

    ApproachLinear(*(s16 *)(c + 0x90), *(s16 *)(c + 0x448), 0x7d0);

    if (*(u16 *)(c + 0x100) < 0x13)
        return;

    ApproachLinear(*(s32 *)(c + 0x44c), 0x1800, 0x19a);
    *(s32 *)(c + 0x220) = *(s32 *)(c + 0x44c) * 0x32;
    *(s32 *)(c + 0x224) = *(s32 *)(c + 0x44c) * 0x64;
    *(s32 *)(c + 0x43c) = *(s32 *)(c + 0x44c) * -0x32;
    {
        s32 v = *(s32 *)(c + 0x44c);
        *(s32 *)(c + 0x80) = v;
        *(s32 *)(c + 0x84) = v;
        *(s32 *)(c + 0x88) = v;
    }
}

// @symbol func_ov020_02111ee0
extern "C" void func_ov020_02111ee0(char *c)
{
  int r = func_ov020_02111418(c);
  if(r) return;
  if(ApproachLinear(*(s16*)(c+0x8c), -0x2000, 0x200)){
    int s;
    *(int*)(c+0x98) = 0;
    s = ((ModelBase*)(c+0x110))->SetFile((BMD_File*)data_ov020_02114aa0[1], 1, -1);
    if(s == 0) return;
    *(int*)(c+0x424) = 2;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj((ModelAnim*)(c+0x110), (BCA_File*)data_ov020_02114aa8[1], 0x40000000, 0x1000, 0);
    *(unsigned char*)(c+0x450) = 1;
    *(short*)(c+0x100) = 0;
    {
      int* p60 = (int*)(c + 0x60);
      *p60 = *p60 + 0x32000;
    }
    *(int*)(c+0x43c) = -0x19000;
  }
  ((dActor_c*)c)->UpdatePos(0);
}

// @symbol func_ov020_02111fc4
extern "C" void func_ov020_02111fc4(char *c)
{
    Vector3 v;
    char* p = (char*)((dActor_c*)c)->ClosestPlayer();
    {
        int* s = (int*)(p + 0x5c);
        v.x = s[0];
        v.y = s[1];
        v.z = s[2];
    }
    if (Vec3_Dist((Vector3*)(c + 0x5c), &v) >= 0x190000) return;
    {
        short ang = Vec3_HorzAngle((Vector3*)(c + 0x5c), &v);
        if (((dActor_c*)c)->GetSubtraction(*(short*)(c + 0x94), ang) >= 0x3000) return;
    }
    *(int*)(c + 0x424) = 1;
    *(int*)(c + 0x98) = 0x5000;
    *(short*)(c + 0x100) = 0;
    {
        int* p234 = (int*)(c + 0x234);
        *p234 = *p234 & ~1;
    }
    func_0201267c(0x166, (const Vector3*)(c + 0x74));
}

// @symbol func_ov020_02112080
extern "C" void func_ov020_02112080(char *c)
{
    switch (*(int *)(c + 0x424)) {
    case 0: func_ov020_02111fc4(c); break;
    case 1: func_ov020_02111ee0(c); break;
    case 2: func_ov020_02111c30(c); break;
    case 3: func_ov020_02111b28(c); break;
    case 4: func_ov020_02111aa8(c); break;
    case 5: func_ov020_021119dc(c); break;
    }
}

// @symbol func_ov020_02112110
extern "C" void func_ov020_02112110(char *c)
{
  if (*(unsigned char*)(c+0x108)) {
    int param = *(signed char*)(c+0xcc);
    dActor_c::Spawn(0x122, 2, *(const Vector3*)(c+0x5c), 0, param, -1);
  }
  ((dActor_c*)c)->PoofDust();
  ((dActor_c*)c)->MarkForDestruction();
}

// @symbol func_ov020_0211216c
extern "C" void func_ov020_0211216c(char *c)
{
    char* m = (*(unsigned char*)(c + 0x450) != 0) ? (c + 0x12c) : (c + 0x190);
    Matrix4x3_FromRotationZXYExt(m, *(short*)(c + 0x8c), *(short*)(c + 0x8e), *(short*)(c + 0x90));
    *(int*)(m + 0x24) = *(int*)(c + 0x5c) >> 3;
    *(int*)(m + 0x28) = *(int*)(c + 0x60) >> 3;
    *(int*)(m + 0x2c) = *(int*)(c + 0x64) >> 3;
    int b = (*(unsigned short*)(c + 0xc) == 0x147);
    if (b == 0) return;
    *(int*)(c + 0x210) = *(int*)(c + 0x5c) >> 3;
    *(int*)(c + 0x214) = *(int*)(c + 0x430) >> 3;
    *(int*)(c + 0x218) = *(int*)(c + 0x64) >> 3;
    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(
        (dActor_c*)c, (ShadowModel*)(c + 0x1c4), (Matrix4x3*)(c + 0x1ec), *(int*)(c + 0x80) * 0x64, 0x12c000, 0xf);
}

// @symbol _ZN8daBook_c16CleanupResourcesEv
int daBook_c::CleanupResources()
{
    ((SharedFilePtr *)(&data_ov020_02114aa0))->Release();
    ((SharedFilePtr *)(&data_ov020_02114ab8))->Release();
    ((SharedFilePtr *)(&data_ov020_02114aa8))->Release();
    ((SharedFilePtr *)(&data_ov020_02114ab0))->Release();
    UnloadBlueCoinModel(this);
    return 1;
}

// @symbol _ZN11daBookGen_c16CleanupResourcesEv
int daBookGen_c::CleanupResources()
{
    ((SharedFilePtr *)&data_ov020_02114aa0)->Release();
    ((SharedFilePtr *)&data_ov020_02114ab8)->Release();
    UnloadBlueCoinModel(this);
    return 1;
}

// @symbol _ZN8daBook_c6RenderEv
int daBook_c::Render()
{
    bool isHidden = mFlags & 0x40000;
    if (isHidden != 0)
        return 1;

    if (unk_450 != 0)
        mModelAnim.Render((Vector3 *)&mScaleX);
    else
        mModel.Render((Vector3 *)&mScaleX);

    return 1;
}

// @symbol _ZN8daBook_c8BehaviorEv
int daBook_c::Behavior()
{
    func_0200f760(this, &mdCcAcPos_c);
    if (UpdateYoshiEat(mWithMeshClsn) != 0) {
        if (mEatenByYoshi != 0 && unk_104 == 5) {
            unk_428 = mState;
            mState = 5;
            mEatenByYoshi = 0;
            mVertSpeed = 0;
            mHorzSpeed = 0x8000;
        }
        func_ov020_0211216c((char *)this);
        return 1;
    }
    switch (unk_420) {
    case 0:
        func_ov020_02112080((char *)this);
        break;
    case 1:
        func_ov020_0211174c((char *)this);
        break;
    }
    func_ov020_0211216c((char *)this);
    mdCcAcPos_c.Clear();
    mdCcAcPos_c.SetPosRelativeToActor(mClsnOffset);
    mdCcAcPos_c.Update();
    return 1;
}

// @symbol _ZN11daBookGen_c8BehaviorEv
int daBookGen_c::Behavior()
{
    if (mSpawnTimer > 0x28) {
        Player *player = ClosestPlayer();
        if (player != 0) {

            Vector3 tmp;
            Vector3 *ps = (Vector3 *)&player->mPosX;
            tmp = *ps;

            if (Vec3_HorzDist((Vector3 *)&mPosX, &tmp) < 0x258000) {
                short angle = Vec3_HorzAngle((Vector3 *)&mPosX, &tmp);
                if (GetSubtraction(mAngleY, angle) < 0x2000) {
                    signed char sc = mAreaId;
                    dActor_c::Spawn(
                        0x145, 0, *(Vector3 *)&mPosX, (Vector3_16 *)&mPrevAngleX,
                        sc, -1);
                    mSpawnTimer = 0;
                }
            }
        }
    } else {
        u16 *timer = &mSpawnTimer;
        *timer = *timer + 1;
    }
    return 1;
}

// @symbol _ZN8daBook_c13InitResourcesEv
int daBook_c::InitResources()
{
    Model::LoadFile(*(SharedFilePtr *)&data_ov020_02114aa0);
    Model::LoadFile(*(SharedFilePtr *)&data_ov020_02114ab8);
    Animation::LoadFile(*(SharedFilePtr *)&data_ov020_02114aa8);
    Animation::LoadFile(*(SharedFilePtr *)&data_ov020_02114ab0);
    LoadBlueCoinModel(this);

    if (mShadowModel.InitCylinder() == 0)
        return 0;

    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(&mWithMeshClsn, this, 0x32000, 0x32000, 0, 0);

    mClsnOffset.x = 0;
    mClsnOffset.y = 0;
    mClsnOffset.z = 0;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(&mdCcAcPos_c, this, &mClsnOffset, 0x19000, 0x32000, 0x200001, 0);

    unk_418 = 0;
    unk_41c = 0;
    mScaleX = 0x800;
    mScaleY = 0x800;
    mScaleZ = 0x800;
    unk_42c = mPosX;
    unk_430 = mPosY;
    unk_434 = mPosZ;

    if (mModel.SetFile((BMD_File*)data_ov020_02114ab8[1], 1, -1) == 0)
        return 0;

    mShadowMat = IDENTITY_MATRIX4X3;
    unk_450 = 0;
    mAngleY += 0x8000;
    unk_44c = 0x800;

    {
        unsigned short id = actorID;
        unsigned int match1 = (id == 0x145);

        if (match1)
        {
            unk_420 = 0;
            mState = 4;
            unk_108 = 0;
            mdCcAcPos_c.vulnFlags |= 0x26fe0;
            goto success;
        }
        {
            unsigned int match2 = (id == 0x147);
            if (match2)
            {
                unk_420 = 0;
                mState = 0;
                unk_108 = 3;
                mdCcAcPos_c.vulnFlags |= 0x2efe0;
                goto success;
            }
        }
        {
            unsigned int match3 = (id == 0xd5);
            if (match3)
            {
                unk_420 = 1;
                mState = 6;
                unk_108 = 0;
                mdCcAcPos_c.flags |= 4;
                mdCcAcPos_c.vulnFlags |= 0x3c0;
                mScaleX = 0x1000;
                mScaleY = 0x800;
                mScaleZ = 0x800;
                goto success;
            }
        }
    }
    return 0;

success:
    return 1;
}

// @symbol _ZN11daBookGen_c13InitResourcesEv
int daBookGen_c::InitResources()
{
    mSpawnTimer = 0;
    Model::LoadFile(*(SharedFilePtr *)data_ov020_02114aa0);
    Model::LoadFile(*(SharedFilePtr *)data_ov020_02114ab8);
    LoadBlueCoinModel(this);
    return 1;
}

// @symbol _ZN8daBook_c16OnAimedAtWithEggEv
s32 daBook_c::OnAimedAtWithEgg()
{
    char* c = (char*)this;
    int eq = (*(unsigned short*)(c+0xc)==0x147);
    if(eq) return 0;
    return 0x19000;
}

// @symbol _ZN8daBook_c13OnYoshiTryEatEv
s32 daBook_c::OnYoshiTryEat()
{
    char* c = (char*)this;
    unsigned int b = *(unsigned short*)(c+0xc)==0x147; return b ? 2 : 0;
}

// @symbol daBook_c_classInit_BOOK_SWITCH
extern "C" daBook_c *daBook_c_classInit_BOOK_SWITCH()
{
    return new daBook_c;
}

// @symbol daBookGen_c_classInit
extern "C" daBookGen_c *daBookGen_c_classInit()
{
    return new daBookGen_c;
}

// @symbol daBook_c_classInit_KILLER_BOOK
extern "C" daBook_c *daBook_c_classInit_KILLER_BOOK()
{
    return new daBook_c;
}

// @symbol daBook_c_classInit_SHOOT_BOOK
extern "C" daBook_c *daBook_c_classInit_SHOOT_BOOK()
{
    return new daBook_c;
}
