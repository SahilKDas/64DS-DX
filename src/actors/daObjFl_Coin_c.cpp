//cpp
/* Lethal Lava Land coin puzzle, ov064 0x02118b50..0x0211929c.
 *
 * Two classes, one translation unit: their methods are interleaved in the
 * ROM, so one file has to emit both. daObjFl_Coin_c is the manager
 * (FL_COIN, 0xd8). daObjFl_Puzzle_c is a piece (FL_PUZZLE, 0x33c).
 * Typeinfo: _ZTI14daObjFl_Coin_c at 0x0211bf98, _ZTI16daObjFl_Puzzle_c at
 * 0x0211bfa4. The factories and both g_profile rows sit past 0x0211929c
 * and stay outside this file.
 *
 * Source order is the reverse of the ROM. mwccarm 2004/b56 emits one .text
 * section per function in reverse source order, so the highest address is
 * written first. Do not reorder. The destructor pairs are the exception:
 * each class's destructor is inline, and the two forcing calls at the
 * bottom of this file pull D1 out before D0, which is the cartridge order.
 *
 * Leftover: the eleven piece helpers still walk the object through char*
 * offsets (link id 0x320, state info 0x324, state index 0x328, bob 0x330,
 * flags 0x336/0x338/0x339/0x33a). Coin Behavior still reaches DistToCPlayer
 * through the mangled free function. Model::LoadFile and
 * dBgW_KcMbg::SetFile stay ABI-exact free declarations; SetFile's by-value
 * Fix12<int> grows the call when spelled as the real method. The two
 * daWater_Hakidasi_c slots past this span are not piece methods.
 */
#include "daObjFl_Puzzle_c.h"
#include "daObjFl_Coin_c.h"
#include "common.h"
#include "decl_common.h"
#include "types.h"
#include "SharedFilePtr.h"
#include "dBgW.h"

/* State table at 0x0211c904. Each entry is a pointer-to-member that Behavior
 * calls. The targets are the piece methods below. */
typedef void (daObjFl_Puzzle_c::*PMF)();
struct Entry { PMF pmf; };

/* InitResources' legacy file completed CLPS_Block as one word. No header
 * defines the body; the call passes it by reference. */
struct CLPS_Block { int x; };

/* func_ov064_02118fa4's legacy file invented a local dActor_c so it could
 * name the position words at 0x5c. The real class is already in scope. */
struct Ov064Fa4Actor {
    char pad[0x5c];
    s32 f5c;
    s32 f60;
    s32 f64;
};

extern "C" {
extern char* _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int a, unsigned int b, const struct Vector3* pos, const struct Vector3_16* rot, int e, int f);
extern char* _ZN8dActor_c10FindWithIDEj(unsigned int id);
extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned int n, const struct Vector3 *v);
extern void Matrix4x3_FromTranslation(struct Matrix4x3* m, int x, int y, int z);
extern Entry data_ov064_0211c904[];
extern int _ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(void* c, Fix12i a, Fix12i b);
extern int _ZN8dActor_c13DistToCPlayerEv(void *self);
BMD_File* _ZN5Model8LoadFileER13SharedFilePtr(SharedFilePtr&);
void _ZN9ModelBase7SetFileEP8BMD_Fileii(void* thiz, BMD_File*, int, int);
KCL_File* _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(SharedFilePtr&);
void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(void* thiz, KCL_File*, const Matrix4x3&, int fix, short s, CLPS_Block&);
void func_020393c4(int* p, int v);
extern SharedFilePtr *data_ov064_0211adc8[];
extern SharedFilePtr data_ov064_0211c800;
extern CLPS_Block data_ov064_0211baac;
extern Matrix4x3 data_020a0e68;
void _ZN10dBgW_KcMbg9TransformERK9Matrix4x3s(void* self, const Matrix4x3&, s16);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- _ZN14daObjFl_Coin_c13InitResourcesEv, 0x02119284, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN14daObjFl_Coin_c13InitResourcesEv
/* recovered: renamed to Class_Method, RTTI class fields named */
// recovered name: daObjFl_Coin_c_InitResources
/* recovered: renamed to Class_Method */
/* daObjFl_Coin_c::InitResources - recovered from vtable slot identity */
s32 daObjFl_Coin_c::InitResources() {
    unk_0d5 = 0;
    unk_0d4 = 0;
    unk_0d6 = 0;
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- _ZN16daObjFl_Puzzle_c13InitResourcesEv, 0x021191a8, size 0xdc */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daObjFl_Puzzle_c::InitResources()
{
    mType = param1 & 0xf;
    _ZN9ModelBase7SetFileEP8BMD_Fileii(&mModel,
        _ZN5Model8LoadFileER13SharedFilePtr(*data_ov064_0211adc8[mType]), 1, -1);
    func_ov064_02119010();
    func_ov064_02118fa4();
    _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
        &mMeshCollider,
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov064_0211c800),
        mClsnMat, 0x1000, mAngleY, data_ov064_0211baac);
    func_020393c4((int*)((char*)&mMeshCollider), (int)&func_ov064_021192bc);
    mStateInfo = data_ov064_0211c198[mType];
    mStateIndex = 0;
    unk_32c = 0;
    mMoveTimer = 0;
    mHadClsn = 0;
    mFreezeState = 1;
    mState = 0;
    mCanSpawnCoin = 1;
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- _ZN14daObjFl_Coin_c8BehaviorEv, 0x0211915c, size 0x4c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN14daObjFl_Coin_c8BehaviorEv
// recovered name: daObjFl_Coin_c_Behavior
/* recovered: renamed to Class_Method */
/* daObjFl_Coin_c::Behavior - recovered from vtable slot identity */
s32 daObjFl_Coin_c::Behavior() {
    char * a = (char *)this;
    switch (*(u8 *)(a + 0xd5)) {
    case 0:
        if (*(u8 *)(a + 0xd4) == 3) {
            if (_ZN8dActor_c13DistToCPlayerEv(a) < 0x3e8000) {
                (*(u8 *)(((int)a + 0xd5)))++;
            }
        }
        break;
    case 1:
        break;
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- _ZN16daObjFl_Puzzle_c8BehaviorEv, 0x021190b0, size 0xac */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c8BehaviorEv
int daObjFl_Puzzle_c::Behavior() {
    func_ov064_02118ee4();
    (this->*data_ov064_0211c904[mState].pmf)();
    char* cc = (char*)this;
    char* p = 0;
    unsigned int id = mOtherPieceId;
    if (id != 0)
        p = _ZN8dActor_c10FindWithIDEj(id);
    if (p == 0 || *(unsigned char*)(p + 0xd6) == 0) {
        u16* ctr = &mMoveTimer;
        *ctr = *ctr + 1;
    }
    func_ov064_02119010();
    if (_ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(cc, 0, 0) != 0)
        func_ov064_02118fa4();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- _ZN16daObjFl_Puzzle_c6RenderEv, 0x02119088, size 0x28 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c6RenderEv
/* recovered: named members + shared header, real C++ method */
int daObjFl_Puzzle_c::Render()
{
    mModel.Render(0);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- _ZN16daObjFl_Puzzle_c16CleanupResourcesEv, 0x0211904c, size 0x3c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c16CleanupResourcesEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daObjFl_Puzzle_c::CleanupResources()
{
    unsigned char idx;
    ((dBgW *)((char *)&mMeshCollider))->Disable();
    idx = *(unsigned char *)((char *)&mType);
    ((SharedFilePtr *)(data_ov064_0211adc8[idx]))->Release();
    ((SharedFilePtr *)(&data_ov064_0211c800))->Release();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov064_02119010, 0x02119010, size 0x3c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02119010Ev
void daObjFl_Puzzle_c::func_ov064_02119010() {
    char* c = (char*)this;
    int x = *(int*)((char*)c + 0x5c) >> 3;
    int y = (*(int*)((char*)c + 0x60) + *(int*)((char*)c + 0x330)) >> 3;
    int z = *(int*)((char*)c + 0x64) >> 3;
    Matrix4x3_FromTranslation((struct Matrix4x3*)((char*)c + 0xf0), x, y, z);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov064_02118fa4, 0x02118fa4, size 0x6c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118fa4Ev
void daObjFl_Puzzle_c::func_ov064_02118fa4() {
    struct Ov064Fa4Actor* c = (struct Ov064Fa4Actor*)this;
    Matrix4x3_FromTranslation(&data_020a0e68, c->f5c, c->f60 + *(s32*)((char*)c+0x330), c->f64);
    *(Matrix4x3*)((char*)c+0x2ec) = data_020a0e68;
    _ZN10dBgW_KcMbg9TransformERK9Matrix4x3s((char*)c+0x124, *(Matrix4x3*)((char*)c+0x2ec), *(s16*)((char*)c+0x8e));
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov064_02118ee4, 0x02118ee4, size 0xc0 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118ee4Ev
void daObjFl_Puzzle_c::func_ov064_02118ee4()
{
  char *c = (char *)this;
  int p_addr;
  if (*(u8 *)(c + 0x338)) {
    if (*(u32 *)(c + 0x320)) {
      char *a = (char *)_ZN8dActor_c10FindWithIDEj(*(u32 *)(c + 0x320));
      if (a)
        *(u8 *)(a + 0xd4) = 1;
    }
  }
  if (*(u8 *)(c + 0x339))
    return;
  p_addr = (u32)(c + 0x328);
  {
    u8 idx = *(u8 *)(c + 0x328);
    s8 *tab = *(s8 **)(c + 0x324);
    u8 *p = (u8 *)((u64)p_addr);
    int m1 = ~0;
    *(s8 *)(c + 0x336) = tab[idx];
    *p = *p + 1;
    if ((*(s8 **)(c + 0x324))[*(u8 *)(c + 0x328)] == (s8)m1) {
      *(u8 *)(c + 0x328) = 0;
      if (*(u32 *)(c + 0x320)) {
        char *a = (char *)_ZN8dActor_c10FindWithIDEj(*(u32 *)(c + 0x320));
        if (a) {
          u8 *f = (u8 *)(a + 0xd4);
          *f |= 2;
        }
      }
    }
  }
  *(u8 *)(c + 0x339) = 1;
  {
    char *b = (char *)(c + 0x300);
    *(unsigned short *)(b + 0x34) = 0;
  }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov064_02118e24, 0x02118e24, size 0xc0 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118e24Eiii
void daObjFl_Puzzle_c::func_ov064_02118e24(int a1, int a2, int a3)
{
    unsigned char *c = (unsigned char *)this;
    unsigned int st = *(unsigned short *)((c + 0x300) + 0x34);

    if (st < 0x14) {
        if (st & 1) {
            *(int *)(c + 0x330) = -0x6000;
        } else {
            *(int *)(c + 0x330) = 0;
        }
        return;
    }

    if (st == 0x14) {
        *(int *)(c + 0x330) = 0;
        _ZN5Sound9PlayBank3EjRK7Vector3(0xe7, (const struct Vector3 *)(c + 0x74));
    }

    if ((int)*(unsigned short *)((c + 0x300) + 0x34) >= a3 + 0x14) {
        *(unsigned char *)(c + 0x336) = 1;
        *(unsigned char *)(c + 0x339) = 0;
        return;
    }

    {
        int *px = (int *)(((int)c + 0x5c));
        int *pz = (int *)(((int)c + 0x64));
        *px += a1;
        *pz += a2;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov064_02118da0, 0x02118da0, size 0x84 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118da0Ev
typedef struct { int x, y, z; } Vec;
extern "C" {
char* _ZN8dActor_c15FindWithActorIDEjPS_(unsigned int, void*);
void _ZN7fBase_c18MarkForDestructionEv(void*);
}
void daObjFl_Puzzle_c::func_ov064_02118da0(){
  char* c = (char*)this;
  volatile int pad[4];
  char* a = _ZN8dActor_c15FindWithActorIDEjPS_(0x4f, 0);
  (void)&pad;
  if(a!=0){
    Vec* p = (Vec*)(((int)a + 0x5c));
    *(int*)(c+0x320) = *(int*)(a+4);
    int z = p->z;
    int x = p->x;
    int y = *(int*)(c+0x60);
    *(int*)(a+0x5c) = x;
    *(int*)(a+0x60) = y;
    *(int*)(a+0x64) = z;
    *(unsigned char*)(c+0x339) = 0;
    *(unsigned char*)(((int)c + 0x336)) = *(unsigned char*)(((int)c + 0x336)) + 1;
    return;
  }
  _ZN7fBase_c18MarkForDestructionEv(c);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov064_02118d3c, 0x02118d3c, size 0x64 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118d3cEv
void daObjFl_Puzzle_c::func_ov064_02118d3c(){
  char *c = (char *)this;
  if (*(unsigned char*)(c+0x33a) != 0) {
    unsigned int id = *(unsigned int*)(c+0x320);
    if (id != 0) {
      char *a = _ZN8dActor_c10FindWithIDEj(id);
      if (a != 0 && *(unsigned char*)(a+0xd5) == 1) {
        *(char*)(c+0x33a) = 0;
        func_ov064_02118c48();
      }
    }
  }
  if (*(unsigned short*)(c+0x334) >= 0x18)
    *(char*)(c+0x339) = 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov064_02118d20, 0x02118d20, size 0x1c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118d20Ev
void daObjFl_Puzzle_c::func_ov064_02118d20()
{
    func_ov064_02118e24(-0x78000, 0, 4);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov064_02118d08, 0x02118d08, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118d08Ev
void daObjFl_Puzzle_c::func_ov064_02118d08() {
    func_ov064_02118e24(0x78000, 0, 4);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov064_02118cec, 0x02118cec, size 0x1c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118cecEv
void daObjFl_Puzzle_c::func_ov064_02118cec()
{
    func_ov064_02118e24(0, -0x78000, 4);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov064_02118cd4, 0x02118cd4, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118cd4Ev
void daObjFl_Puzzle_c::func_ov064_02118cd4() {
    func_ov064_02118e24(0, 0x78000, 4);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov064_02118c48, 0x02118c48, size 0x8c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_c19func_ov064_02118c48Ev
/* Spawns actor 0x120 (param 2) at this piece's position with the signed byte
 * param at +0xcc; if this piece stores a link ID at +0x320, finds that actor,
 * copies its unique ID (+4) into the spawned actor's +0xd4, and bumps the
 * found actor's byte refcount at +0xd6. */
void daObjFl_Puzzle_c::func_ov064_02118c48()
{
    char* r5 = (char*)this;
    char* spawned = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x120, 2, (struct Vector3*)(r5 + 0x5c), 0, *(signed char*)(r5 + 0xcc), -1);
    char* found;
    if (spawned == 0)
        return;
    if (*(int*)(r5 + 0x320) == 0)
        return;
    found = _ZN8dActor_c10FindWithIDEj(*(int*)(r5 + 0x320));
    if (found == 0)
        return;
    *(int*)(spawned + 0xd4) = *(int*)(found + 4);
    (*(unsigned char*)(found + 0xd6)) += 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 1 -- _ZN16daObjFl_Puzzle_cD0Ev, 0x02118b94, size 0x58 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_cD0Ev
/* A delete expression forces the compiler-spelled deleting destructor. */
#ifdef _MSC_VER
/* MSVC needs this flat D0 entry. Call the actual class-body destructor
 * qualified so dispatch is direct, then use the class-specific deallocator.
 * The inline body includes member/base teardown; no separate flat D1 provider
 * is supplied by this branch. The mwccarm definition below is unchanged. */
extern "C" daObjFl_Puzzle_c *_ZN16daObjFl_Puzzle_cD0Ev(daObjFl_Puzzle_c *thiz)
{
    thiz->daObjFl_Puzzle_c::~daObjFl_Puzzle_c();          /* direct member/base teardown */
    daObjFl_Puzzle_c::operator delete(thiz);  /* the class-specific delete D0 ends with */
    return thiz;
}
#else
void daObjFl_Puzzle_c_EmitDeletingDestructor(daObjFl_Puzzle_c *piece)
{
    delete piece;
}
#endif

/* -------------------------------------------------------------------------- */
/* ROM ordinal 0 -- _ZN16daObjFl_Puzzle_cD1Ev, 0x02118b50, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN16daObjFl_Puzzle_cD1Ev
/* Force mwccarm to emit the class-body destructor as a genuine C++ D1. */
void daObjFl_Puzzle_c_EmitDestructor(daObjFl_Puzzle_c *piece)
{
    piece->~daObjFl_Puzzle_c();
}

/* Not called. The coin destructor is inline in the header; these two calls
 * are what ask mwccarm for the out-of-line D1 and D0. */
void daObjFl_Coin_c_EmitDeletingDestructor(daObjFl_Coin_c *coin)
{
    delete coin;
}

void daObjFl_Coin_c_EmitDestructor(daObjFl_Coin_c *coin)
{
    coin->~daObjFl_Coin_c();
}
