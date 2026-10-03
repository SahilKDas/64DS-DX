//cpp
/* ov080/daChoro_Rock_c+daChoropu_c -- the Monty Mole (daChoropu_c) and the rock it throws
 * (daChoro_Rock_c). 26 functions, .text 0x02123740..0x02124a20: tu_map's
 * 24-function run 0x02123740..0x02124998 plus the two registry factories
 * that abut it, daChoro_Rock_c_classInit and daChoropu_c_classInit.
 *
 * Class identity comes from the ROM RTTI:
 *   daChoropu_c     _ZTI 0x02127fbc, _ZTS 0x02127fc8, _ZTV 0x021280b0
 *   daChoro_Rock_c  _ZTI 0x02127fb0, _ZTS 0x02127fd8, _ZTV 0x0212802c
 *                   (__si_class_type_info, parent dEnemyBase_c)
 *
 * SOURCE ORDER IS ROM ORDER. `defer_codegen off` makes mwccarm emit each
 * function as it is parsed, so the definitions below follow the ROM layout.
 * Each empty out-of-line destructor emits D1 then D0; its D2 has no ROM home
 * and is deadstripped (policy rows in the TU manifest). The two classes'
 * members are interleaved in the ROM, and so they are here.
 *
 * daChoropu_c::Behavior dispatches through the state table at
 * data_ov080_02128438 -- one pointer-to-member per state, indexed by mState.
 * The table is .bss, filled outside this span; the state bodies are among
 * the raw-offset helpers below.
 *
 * Leftover (still to deslop; each change needs a rematch):
 *  - the eleven func_ov080_* helpers take a raw `char *` and reach fields by
 *    offset; they are state/member bodies whose real names and member form
 *    are not yet recovered (same shape as the daBttBk_c TU);
 *  - calls that pass Fix12<int> by value (ModelAnim::SetAnim, dCcAc_c::Init,
 *    dBgCh_Actr::Init, Player::Hurt, Particle::System::New/NewSimple) are
 *    still spelled as mangled extern "C" calls: the headers declare those
 *    parameters as Fix12i (a plain s32), so a member call would mangle to a
 *    symbol the ROM does not have;
 *  - LAUND() keeps three field addresses opaque in func_ov080_02124088;
 *  - data_ov080_021283d0..021283e8 are still typed `int[]` (they are
 *    SharedFilePtr animation slots -- data_ov080_0212766c points at them).
 *
 * The registry factories daChoro_Rock_c_classInit (0x02124998) and
 * daChoropu_c_classInit (0x021249e0) come last, in ROM order. Neither class
 * declares a constructor, so each is a plain `return new`.
 */

#include "decl_common.h"
#include "daChoropu_c.h"
#include "daChoro_Rock_c.h"
#include "decl_Animation.h"
#include "decl_Player.h"
#include "SharedFilePtr.h"
#include "dCc_c.h"
#include "Player.h"

/* daChoropu_c's state table: one pointer-to-member per state, indexed by
 * the state number at +0x17c. */
typedef void (daChoropu_c::*ChoropuStateFn)();
struct ChoropuStateRow { ChoropuStateFn fn[1]; };

#define LAUND(p) ((void*)((((long long)(int)(p)))))

extern "C" {
void  _ZN9Animation7AdvanceEv(void *anim);
int   _ZN9Animation8FinishedEv(void *anim);
void  _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *self, void *file, int a, int b, unsigned int c);
void  _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
unsigned int _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
    unsigned int a, unsigned int b, int x, int y, int z, void *vel, void *cb);
void *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int id, unsigned int param,
    const Vector3 &pos, const Vector3_16 *ang, int area, int unk);
void *_ZN8dActor_c10FindWithIDEj(unsigned int id);
dActor_c *_ZN8dActor_c15FindWithActorIDEjPS_(unsigned int id, dActor_c *after);
char *_ZN8dActor_c13ClosestPlayerEv(void *self);
int   _ZN8dActor_c13DistToCPlayerEv(void *self);
void  _ZN8dActor_c10PoofDustAtERK7Vector3(void *self, const Vector3 &v);
int   _ZN8dActor_c16JumpedOnByPlayerER5dCc_cR6Player(void *self, void *clsn, void *player);
void  _ZN6Player16IncMegaKillCountEv(void *player);
void  _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *player, void *pos, unsigned int a, int b,
    unsigned int d, unsigned int e, unsigned int f);
void  _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *self, void *actor, int r, int h,
    unsigned int e, unsigned int g);
int   _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void *self, void *actor,
    int radius, int height, Vector3_16 *a, Vector3_16 *b);
int   RandomIntInternal(int *seed);
short Vec3_HorzAngle(const Vector3 *a, const Vector3 *b);
int   Vec3_HorzDist(const Vector3 *a, const Vector3 *b);
void  Matrix4x3_FromRotationY(void *m, int ang);
void  func_0201267c(unsigned int id, const void *pos);
void  func_02012694(int a, void *p);

void *func_ov080_02124360(char *c);
void  func_ov080_02124088(char *c);
void  func_ov080_02124208(char *c);
void  func_ov080_021243d8(char *t);
void  func_ov080_02124418(char *t);
}

extern int data_0209e650;
extern s16 data_02082214[];
extern int data_ov080_0212767c[];
extern int data_ov080_021283d0[];
extern int data_ov080_021283e0[];
extern int data_ov080_021283e8[];
extern SharedFilePtr data_ov002_0210d9d8;
extern SharedFilePtr data_ov080_021283c0;
extern SharedFilePtr data_ov080_021283c8;
extern SharedFilePtr *data_ov080_0212766c[];
extern ChoropuStateRow data_ov080_02128438[];

#pragma defer_codegen off

// @symbol _ZN11daChoropu_cD1Ev
// @symbol _ZN11daChoropu_cD0Ev
daChoropu_c::~daChoropu_c()
{
}

// @symbol _ZN14daChoro_Rock_cD1Ev
// @symbol _ZN14daChoro_Rock_cD0Ev
daChoro_Rock_c::~daChoro_Rock_c()
{
}

// @symbol _ZN11daChoropu_c16OnAimedAtWithEggEv
s32 daChoropu_c::OnAimedAtWithEgg()
{
    return 0x28000;
}

// @symbol func_ov080_02123860
extern "C" void func_ov080_02123860(char *self)
{
    unsigned int idx;

    _ZN9Animation7AdvanceEv(self + 0x124);
    idx = (unsigned int)(*(int *)(self + 0x12c) << 4) >> 0x10;
    if (idx >= 0xf) {
        *(int *)((int)(self + 0x150)) |= 1;
        *(int *)((int)(self + 0xb0)) &= ~0x10000000;
        if (idx == 0xf) {
            _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x2a, *(int *)(self + 0x5c), *(int *)(self + 0x60), *(int *)(self + 0x64));
            _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x2b, *(int *)(self + 0x5c), *(int *)(self + 0x60) + 0x1e000, *(int *)(self + 0x64));
        }
    }
    {
        int raw = *(int *)(self + 0x12c);
        int *tbl = (int *)(int)(data_ov080_0212767c);
        idx = (unsigned int)(raw << 4) >> 0x10;
        *(int *)(self + 0x140) = tbl[idx] << 0xc;
    }
    if (_ZN9Animation8FinishedEv(self + 0x124) == 0)
        return;
    *(int *)(self + 0x17c) = 1;
    func_ov080_02124360(self);
}

// @symbol func_ov080_02123924
extern "C" void func_ov080_02123924(char *c)
{
    _ZN9Animation7AdvanceEv(c + 0x124);
    if (_ZN9Animation8FinishedEv(c + 0x124)) {
        *(int *)(c + 0x17c) = 5;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e8[1], 0x40000000, 0x1000, 0);
        return;
    }
    {
        char *p = _ZN8dActor_c13ClosestPlayerEv(c);
        Vector3 v;
        Fix12i *q;
        if (p == 0) return;
        q = (Fix12i *)(((int)p + 0x5c));
        v.x = q[0];
        v.y = q[1];
        v.z = q[2];
        if ((short)AngleDiff(*(short *)(c + 0x8e), Vec3_HorzAngle((Vector3 *)(c + 0x5c), &v)) >= 0x4000) return;
        if (Vec3_HorzDist((Vector3 *)(c + 0x5c), &v) >= 0x1f4000) return;
        *(int *)(c + 0x17c) = 5;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e8[1], 0x40000000, 0x1000, 0);
    }
}

// @symbol func_ov080_02123a34
extern "C" void func_ov080_02123a34(char *c)
{
    _ZN9Animation7AdvanceEv(c + 0x124);
    if (_ZNK9Animation12WillHitFrameEi(c + 0x124, 0xa)) {
        Vector3_16 v16;
        Vector3 pos;
        unsigned short ax, ay;
        void *a;
        int d;
        short s;
        int idx;
        int px, py, pz;
        Vector3_16 *pAng = &v16;

        ax = *(unsigned short *)(c + 0x8c);
        ay = *(unsigned short *)(c + 0x8e);
        *(volatile short *)&v16.y = (short)ay;
        *(volatile short *)&v16.x = (short)ax;
        {
            unsigned short z = *(unsigned short *)(c + 0x90);
            pAng->z = z;
            short yv = (short)v16.y;
            px = *(int *)(c + 0x5c);
            pos.x = px;
            yv = (short)(yv + 0x400);
            py = *(int *)(c + 0x60);
            pos.y = py;
            pz = *(int *)(c + 0x64);
            pAng->y = yv;
            pos.z = pz;
            pos.y = py + 0xa000;
        }

        idx = (unsigned short)(short)(*(short *)(c + 0x8e) - 0x4000) >> 4;
        s = data_02082214[idx << 1];
        pos.x = s * (short)0x50 + px;

        idx = (unsigned short)(short)(*(short *)(c + 0x8e) - 0x4000) >> 4;
        s = data_02082214[(idx << 1) + 1];
        pos.z = s * (short)0x50 + pz;

        a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
            py ? 0x137u : 0x137u, 0, pos, pAng, *(signed char *)(c + 0xcc), -1);
        d = _ZN8dActor_c13DistToCPlayerEv(c);
        if (d >= 0x258000)
            d = 0x258000;
        *(int *)((char *)a + 0x98) = 0x1e000;
        *(int *)((char *)a + 0xa4) = 0;
        *(int *)((char *)a + 0xa8) = (d << 2) / 100 + 0x4000;
        *(int *)((char *)a + 0xac) = 0;
        func_0201267c(0xd2, c + 0x74);
    }

    if (_ZN9Animation8FinishedEv(c + 0x124) == 0)
        return;

    if (_ZN8dActor_c13DistToCPlayerEv(c) < 0x3e8000) {
        *(int *)(c + 0x17c) = 5;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e8[1], 0x40000000, 0x1000, 0);
    } else {
        *(int *)(c + 0x17c) = 4;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283d0[1], 0x40000000, 0x1000, 0);
    }
}

// @symbol func_ov080_02123c24
extern "C" void func_ov080_02123c24(char *c)
{
    int amt;
    unsigned int state;
    int raw;

    _ZN9Animation7AdvanceEv(c + 0x124);
    raw = *(int *)(c + 0x12c);
    amt = 0;
    state = ((unsigned int)raw << 4) >> 16;

    if (state == 6) {
        int *p150 = (int *)(((int)c + 0x150));
        int *pb0 = (int *)(((int)c + 0xb0));
        *p150 = *p150 & ~1;
        *pb0 = *pb0 | 0x10000000;
    }

    if (state >= 6) {
        if (state >= 0x1a) {
            amt = 0x50000;
        } else {
            amt = data_ov080_021276c4[state - 6] << 12;
            *(unsigned int *)(c + 0x188) = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                *(unsigned int *)(c + 0x188), 0x29,
                *(int *)(c + 0x5c), *(int *)(c + 0x60), *(int *)(c + 0x64),
                0, 0);
        }
    }

    *(int *)(c + 0x140) = amt;

    if (_ZN9Animation8FinishedEv(c + 0x124) == 0)
        return;

    *(unsigned int *)(c + 0x188) = 0;

    if (*(unsigned char *)(c + 0x180) == 2) {
        unsigned int rv = (unsigned int)RandomIntInternal(&data_0209e650) >> 8;
        unsigned int rem = (rv % 3) & 0xff;
        if (rem == 0) {
            *(int *)(c + 0x17c) = 3;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e0[1], 0x40000000, 0x1000, 0);
            return;
        }
        if (rem == 1) {
            *(int *)(c + 0x17c) = 4;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283d0[1], 0x40000000, 0x1000, 0);
            return;
        }
        *(int *)(c + 0x17c) = 5;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e8[1], 0x40000000, 0x1000, 0);
        return;
    }

    {
        char *player = _ZN8dActor_c13ClosestPlayerEv(c);
        if (player == 0) {
            *(int *)(c + 0x17c) = 3;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e0[1], 0x40000000, 0x1000, 0);
            return;
        }
        {
            Vector3 pos;
            short horz;
            int diff;
            int *pb = (int *)(((int)player + 0x5c));
            pos.x = pb[0];
            pos.y = pb[1];
            pos.z = pb[2];
            horz = Vec3_HorzAngle((Vector3 *)(c + 0x5c), &pos);
            diff = (short)AngleDiff(*(short *)(c + 0x8e), horz);
            if (diff < 0x4000) {
                *(int *)(c + 0x17c) = 3;
                _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283e0[1], 0x40000000, 0x1000, 0);
            } else {
                *(int *)(c + 0x17c) = 4;
                _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283d0[1], 0x40000000, 0x1000, 0);
            }
        }
    }
}

// @symbol func_ov080_02123ecc
extern "C" void func_ov080_02123ecc(dActor_c *self)
{
    char *s = (char *)self;
    char *p = (char *)self->ClosestPlayer();
    int dist;
    if (p == 0) dist = 0x5dc000;
    else dist = Vec3_HorzDist((Vector3 *)(s + 0x5c), (Vector3 *)(p + 0x5c));
    if (dist >= 0x5dc000) return;
    if (*(signed char *)(s + 0x181) != 1) return;
    if (dist >= 0xfa000) {
        *(int *)(s + 0x17c) = 2;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(s + 0xd4, (void *)data_ov080_021283d8[1], 0x40000000, 0x1000, 0);
        char *p2 = (char *)self->ClosestPlayer();
        *(short *)(s + 0x8e) = Vec3_HorzAngle((Vector3 *)(s + 0x5c), (Vector3 *)(p2 + 0x5c));
        {
            int *a = (int *)((int)s + 0x150);
            int *b = (int *)((int)s + 0xb0);
            *(short *)(s + 0x94) = *(short *)(s + 0x8e);
            *a = *a & ~1;
            *b = *b | 0x10000000;
            func_0201267c(0x116, s + 0x74);
        }
        return;
    }
    func_ov080_02124360(s);
}

// @symbol func_ov080_02123fcc
extern "C" void func_ov080_02123fcc(char *thiz)
{
    char *c = thiz;
    {
        int *p150 = (int *)(((int)c + 0x150));
        int *pb0 = (int *)(((int)c + 0xb0));
        *p150 = *p150 | 1;
        *pb0 = *pb0 & ~0x10000000;
    }
    if (*(unsigned char *)(c + 0x180) != 0) {
        dActor_c *a = 0;
        while (1) {
            a = _ZN8dActor_c15FindWithActorIDEjPS_(0x136, a);
            if (a == 0) break;
            if (a != (dActor_c *)c) {
                if (*(unsigned char *)(c + 0x182) == *(unsigned char *)((char *)a + 0x182)) {
                    *(int *)(c + *(unsigned char *)(c + 0x183) * 4 + 0x16c) = *(int *)((char *)a + 4);
                    (*(unsigned char *)(c + 0x183))++;
                    if (*(unsigned char *)(c + 0x183) == 4) break;
                }
            }
        }
        *(int *)(c + 0x17c) = 1;
        return;
    }
    *(int *)(c + 0x17c) = 1;
}

// @symbol func_ov080_02124088
extern "C" void func_ov080_02124088(char *c)
{
    Vector3 v1;
    Vector3 v3;
    Vector3 v2;
    u8 acc;
    int i;
    void *a;

    *(int *)(c + 0x17c) = 1;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)data_ov080_021283d8[1], 0x40000000, 0x1000, 0);

    *(int *)(c + 0x12c) = 0;
    {
        u32 *p150 = (u32 *)LAUND(c + 0x150);
        *p150 |= 1;
    }
    {
        u32 *pb0 = (u32 *)LAUND(c + 0xb0);
        *pb0 &= ~0x10000000;
    }

    func_0201267c(0xd5, c + 0x74);

    v1.x = *(int *)(c + 0x5c);
    {
        int y1 = *(int *)(c + 0x60);
        v1.y = y1;
        v1.z = *(int *)(c + 0x64);
        v1.y = y1 + (*(int *)(c + 0x140) - 0x50000);
    }
    ((int *)&v2)[0] = ((int *)&v1)[0];
    ((int *)&v2)[1] = ((int *)&v1)[1];
    ((int *)&v2)[2] = ((int *)&v1)[2];
    _ZN8dActor_c10PoofDustAtERK7Vector3(c, v2);

    func_ov080_02124360(c);

    acc = *(u8 *)(c + 0x184);
    i = 0;
    while (i < *(u8 *)(c + 0x183)) {
        a = _ZN8dActor_c10FindWithIDEj(((u32 *)(c + 0x16c))[i]);
        i = i + 1;
        if (a != 0) {
            acc = (u8)(acc + *(u8 *)((char *)a + 0x184));
        }
    }

    if (acc < 7) goto tail_inc;
    if (acc != 7) return;

    v3.x = *(int *)(c + 0x5c);
    {
        int y3 = *(int *)(c + 0x60);
        v3.y = y3;
        v3.z = *(int *)(c + 0x64);
        v3.y = y3 + 0x64000;
    }
    _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x114, 0, v3, 0, (int)*(signed char *)(c + 0xcc), -1);

    *(u8 *)(c + 0x184) = 8;
    return;

tail_inc:
    {
        u8 *p184 = (u8 *)LAUND(c + 0x184);
        (*p184)++;
    }
}

// @symbol func_ov080_02124208
extern "C" void func_ov080_02124208(char *c)
{
    char *p;
    unsigned int id = *(unsigned int *)(c + 0x15c);

    if (id == 0)
        return;

    p = (char *)_ZN8dActor_c10FindWithIDEj(id);
    if (p == 0)
        return;

    if ((*(int *)(c + 0x158) & 0x66fe0) != 0) {
        func_ov080_02124088(c);
        return;
    }

    if ((*(int *)(c + 0x158) & 0x400000) == 0)
        return;

    if (_ZN8dActor_c16JumpedOnByPlayerER5dCc_cR6Player(c, c + 0x138, p)) {
        func_ov080_02124088(c);
        _ZN6Player6BounceE5Fix12IiE(p, 0x28000);
        return;
    }

    if (*(unsigned char *)(p + 0x6fb) != 0)
        return;

    if (*(unsigned char *)(p + 0x6f9) != 0) {
        func_ov080_02124088(c);
        return;
    }

    if ((*(int *)(c + 0x158) & 0x10) != 0) {
        func_02012694(0x1d, c + 0x74);
        _ZN6Player16IncMegaKillCountEv(p);
        func_ov080_02124088(c);
        return;
    }

    {
        Vector3 pos;
        pos.x = *(int *)(c + 0x5c);
        pos.y = *(int *)(c + 0x60);
        pos.z = *(int *)(c + 0x64);
        _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(p, &pos, 2, 0xc000, 1, 0, 1);
    }
}

// @symbol func_ov080_02124360
extern "C" void *func_ov080_02124360(char *c)
{
    unsigned char n = *(unsigned char *)(c + 0x180);
    void *obj;
    if (n == 0)
        return (void *)(unsigned int)n;
    *(unsigned char *)(c + 0x181) = 0;
    for (;;) {
        unsigned int r = ((unsigned int)RandomIntInternal(&data_0209e650)) >> 8;
        unsigned int cnt = *(unsigned char *)(c + 0x183);
        unsigned int idx = r % cnt;
        unsigned int id = *(unsigned int *)(c + 0x16c + (idx * 4));
        obj = _ZN8dActor_c10FindWithIDEj(id);
        if (obj == 0)
            continue;
        if (*(unsigned char *)((char *)obj + 0x181) != 0)
            continue;
        *(unsigned char *)((char *)obj + 0x181) = 1;
        return obj;
    }
}

// @symbol func_ov080_021243d8
extern "C" void func_ov080_021243d8(char *t)
{
    Matrix4x3_FromRotationY(t + 0xf0, *(short *)(t + 0x8e));
    *(int *)(t + 0x114) = *(int *)(t + 0x5c) >> 3;
    *(int *)(t + 0x118) = *(int *)(t + 0x60) >> 3;
    *(int *)(t + 0x11c) = *(int *)(t + 0x64) >> 3;
}

// @symbol func_ov080_02124418
extern "C" void func_ov080_02124418(char *t)
{
    Matrix4x3_FromRotationY(t + 0x12c, *(short *)(t + 0x8e));
    *(int *)(t + 0x150) = *(int *)(t + 0x5c) >> 3;
    *(int *)(t + 0x154) = *(int *)(t + 0x60) >> 3;
    *(int *)(t + 0x158) = *(int *)(t + 0x64) >> 3;
}

// @symbol _ZN11daChoropu_c16CleanupResourcesEv
/* Releases the shared model and animation files; never touches `this`. */
s32 daChoropu_c::CleanupResources()
{
    data_ov002_0210d9d8.Release();
    data_ov080_021283c0.Release();
    data_ov080_021283c8.Release();
    int i = 0;
    do {
        data_ov080_0212766c[i]->Release();
        i++;
    } while (i < 4);
    return 1;
}

// @symbol _ZN14daChoro_Rock_c16CleanupResourcesEv
s32 daChoro_Rock_c::CleanupResources()
{
    data_ov080_021283c8.Release();
    return 1;
}

// @symbol _ZN11daChoropu_c6RenderEv
s32 daChoropu_c::Render()
{
    Model *m = &mModelAnim;
    m->Render(0);
    return 1;
}

// @symbol _ZN14daChoro_Rock_c6RenderEv
s32 daChoro_Rock_c::Render()
{
    Model *m = &mModel;
    m->Render((const Vector3 *)&mScaleX);
    return 1;
}

// @symbol _ZN11daChoropu_c8BehaviorEv
s32 daChoropu_c::Behavior()
{
    MakeVanishLuigiWork(mdCcAc_c);
    (this->*data_ov080_02128438[mState].fn[0])();
    func_ov080_02124208((char *)this);
    func_ov080_021243d8((char *)this);
    mdCcAc_c.Clear();
    mdCcAc_c.Update();
    return 1;
}

// @symbol _ZN14daChoro_Rock_c8BehaviorEv
s32 daChoro_Rock_c::Behavior()
{
    MakeVanishLuigiWork(mdCcAc_c);

    u32 id = mdCcAc_c.otherOwner;
    if (id != 0) {
        Player *player = (Player *)FindWithID(id);
        if (player != 0 && (mdCcAc_c.hitFlags & 0x400000)) {
            if (player->mIsMetal) {
                Vector3 pos;
                pos.x = mPosX;
                pos.y = mPosY;
                pos.z = mPosZ;
                _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(player, &pos, 1, 0xc000, 1, 0, 1);
            } else if (!player->mIsVanish) {
                Vector3 pos;
                pos.x = mPosX;
                pos.y = mPosY;
                pos.z = mPosZ;
                _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(player, &pos, 1, 0xc000, 1, 0, 1);
            }
        }
    }

    if (mWithMeshClsn.IsOnGround()) {
        if (!mIsSmall) {
            /* A big rock breaks into two small ones on landing. */
            dActor_c *s;
            int r;

            s = dActor_c::Spawn(0x137, 1, *(Vector3 *)&mPosX, 0, mAreaId, -1);
            r = RandomIntInternal(&data_0209e650);
            s->mPrevAngleX = 0;
            s->mPrevAngleY = (short)((unsigned int)r >> 8);
            s->mPrevAngleZ = 0;
            s->mHorzSpeed = mHorzSpeed >> 1;
            s->unk_0a4 = 0;
            s->mVertSpeed = 0x5000;
            s->unk_0ac = 0;

            s = dActor_c::Spawn(0x137, 1, *(Vector3 *)&mPosX, 0, mAreaId, -1);
            r = RandomIntInternal(&data_0209e650);
            s->mPrevAngleX = 0;
            s->mPrevAngleY = (short)((unsigned int)r >> 8);
            s->mPrevAngleZ = 0;
            s->mHorzSpeed = mHorzSpeed >> 1;
            s->unk_0a4 = 0;
            s->mVertSpeed = 0x5000;
            s->unk_0ac = 0;
        }
        MarkForDestruction();
    }

    UpdatePos(0);
    UpdateWMClsn(mWithMeshClsn, 0);
    func_ov080_02124418((char *)this);
    mdCcAc_c.Clear();
    mdCcAc_c.Update();
    return 1;
}

// @symbol _ZN11daChoropu_c13InitResourcesEv
s32 daChoropu_c::InitResources()
{
    int i;
    for (i = 0; i < 4; i++) Animation::LoadFile(*data_ov080_0212766c[i]);
    Model::LoadFile(data_ov002_0210d9d8);
    Model::LoadFile(data_ov080_021283c8);
    mModelAnim.SetFile((BMD_File *)Model::LoadFile(data_ov080_021283c0), 1, -1);
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, (void *)data_ov080_021283d8[1], 0, 0x1000, 0);
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, this, 0x50000, 0x64000, 0x200000, 0x66fe0);
    unk_180 = param1 & 0xf;
    mState = 0;
    if (unk_180 == 0) unk_181 = 1;
    else unk_181 = (param1 >> 8) & 1;
    unk_182 = (param1 >> 4) & 0xf;
    unk_184 = 0;
    mNumPartners = 0;
    for (i = 0; i < 4; i++) mPartnerIDs[i] = 0;
    unk_188 = 0;
    return 1;
}

// @symbol _ZN14daChoro_Rock_c13InitResourcesEv
s32 daChoro_Rock_c::InitResources()
{
    if (!mModel.SetFile((BMD_File *)Model::LoadFile(data_ov080_021283c8), 1, -1)) return 0;
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, this, 0x1e000, 0x1e000, 0x200004, 0);
    mIsSmall = param1 & 1;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(&mWithMeshClsn, this, 0x1e000, 0x1e000, 0, 0);
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;
    if (mIsSmall == 0) {
        mScaleX = 0x1000;
        mScaleY = 0x1000;
        mScaleZ = 0x1000;
    } else {
        mScaleX = 0x800;
        mScaleY = 0x800;
        mScaleZ = 0x800;
    }
    return 1;
}

// @symbol daChoro_Rock_c_classInit
extern "C" daChoro_Rock_c *daChoro_Rock_c_classInit()
{
    return new daChoro_Rock_c();
}

// @symbol daChoropu_c_classInit
extern "C" daChoropu_c *daChoropu_c_classInit()
{
    return new daChoropu_c();
}
