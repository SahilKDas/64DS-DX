//cpp
/* dMgState_c -- the minigame scene's little state machine, and the rest of its
 * translation unit: 52 functions, ov004 .text 0x020b67e8..0x020b8c18.
 *
 * dScMgBase_c keeps one of these by value and drives it once a frame: Behavior
 * counts the state timer down and calls the current state's per-frame handler,
 * Render calls the current state's draw handler, and SetState swaps both out.
 * Every handler is a pointer to a dMgState_c member, so the table below is a
 * table of member pointers rather than of plain function addresses.
 *
 * The four methods sit in the middle of the unit. Below them, 0x020b67e8 to
 * 0x020b8714, are the 47 state handlers the table points at (the pointer-to-
 * member constants at 0x020bc92c..0x020bca44 name 33 of them as targets) and
 * the helpers they share. Above them, func_ov004_020b8a8c at 0x020b8a8c fills
 * in the screen-position words at 0x020bc850..0x020bc8b8 that the handlers read.
 * The handlers still take the machine as a char* and are written C-style under
 * their ROM names (extern "C"), so the link names stay the ROM's; turning them
 * into dMgState_c members is the next step for this unit.
 *
 * Edges. 0x020b67e8 is the first function after func_ov004_020b67e4, the last
 * destructor helper __sinit_ov004_020b9ad0 registers for the preceding unit
 * (the 0x14-entry array at data_ov004_020bfa34). 0x020b8c18 is the start of
 * dMgPsOpt_c. The unit's .data window, 0x020bc850..0x020bca44, is read only
 * by this unit's functions and ends before dMgPsOpt_c's at 0x020bca58; the
 * words at data_ov004_020bc7d4 and data_ov004_020bfa24 belong to the preceding
 * unit, which this unit only writes. The unit emits no static initializer of
 * its own, so none of the four __sinit_ov004_* records belongs to it.
 *
 * mwccarm emits one .text section per function and lays them out in the reverse
 * of source order, so the functions are written here highest-ROM-address first.
 *
 * The cartridge proves the 0x28-byte layout, the twenty-entry enter table, the
 * construction order and the three callback roles. The class and member names
 * are inferred from the dMg* minigame family; see include/dMgState_c.h.
 *
 * Several handlers were matched in C and keep that spelling; the vtable-call
 * handlers were matched as C++ and carry local Base and Obj views of the
 * objects they call through. The two-word copies go through W2 (an int[2]
 * wrapper), which keeps the block-move shape under C++.
 *
 * Retired one-function sources (ROM address order):
 *   0x020b67e8  func_ov004_020b67e8
 *   0x020b67f8  func_ov004_020b67f8
 *   0x020b6808  func_ov004_020b6808
 *   0x020b682c  func_ov004_020b682c
 *   0x020b68e8  func_ov004_020b68e8
 *   0x020b6948  func_ov004_020b6948
 *   0x020b6ad8  func_ov004_020b6ad8
 *   0x020b6b40  func_ov004_020b6b40
 *   0x020b6c10  func_ov004_020b6c10
 *   0x020b6c9c  func_ov004_020b6c9c
 *   0x020b6d6c  func_ov004_020b6d6c
 *   0x020b6ddc  func_ov004_020b6ddc
 *   0x020b6f14  func_ov004_020b6f14
 *   0x020b6f88  func_ov004_020b6f88
 *   0x020b7020  func_ov004_020b7020
 *   0x020b70b4  func_ov004_020b70b4
 *   0x020b7124  func_ov004_020b7124
 *   0x020b724c  func_ov004_020b724c
 *   0x020b72d4  func_ov004_020b72d4
 *   0x020b743c  func_ov004_020b743c
 *   0x020b7460  func_ov004_020b7460
 *   0x020b746c  func_ov004_020b746c
 *   0x020b7594  func_ov004_020b7594
 *   0x020b75e4  func_ov004_020b75e4
 *   0x020b7744  func_ov004_020b7744
 *   0x020b77b4  func_ov004_020b77b4
 *   0x020b7854  func_ov004_020b7854
 *   0x020b78f4  func_ov004_020b78f4
 *   0x020b798c  func_ov004_020b798c
 *   0x020b79b0  func_ov004_020b79b0
 *   0x020b7a18  func_ov004_020b7a18
 *   0x020b7b20  func_ov004_020b7b20
 *   0x020b7b90  func_ov004_020b7b90
 *   0x020b7c04  func_ov004_020b7c04
 *   0x020b7cd0  func_ov004_020b7cd0
 *   0x020b7e38  func_ov004_020b7e38
 *   0x020b7eac  func_ov004_020b7eac
 *   0x020b7f5c  func_ov004_020b7f5c
 *   0x020b7fec  func_ov004_020b7fec
 *   0x020b8098  func_ov004_020b8098
 *   0x020b81f8  func_ov004_020b81f8
 *   0x020b8284  func_ov004_020b8284
 *   0x020b83ac  func_ov004_020b83ac
 *   0x020b841c  func_ov004_020b841c
 *   0x020b853c  func_ov004_020b853c
 *   0x020b8560  func_ov004_020b8560
 *   0x020b8688  func_ov004_020b8688
 *   0x020b8a8c  func_ov004_020b8a8c
 */

#include "dMgState_c.h"

extern int ApproachLinear(s32 &value, s32 target, s32 step);

struct W2 { int w[2]; };
struct Pair { int a; int b; };

struct Base {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual int m_4c(void* arg);
    char pad[0xa4];
    int a8;
};
struct Obj {
    virtual void v0(); virtual void v1(); virtual void v2();
    virtual void m_c(int a, int b);
    virtual void m_10(int a, int b);
    virtual void v5();
    virtual int m_18();
    virtual int m_1c();
};
struct StateObj { char pad[0x10]; W2 p10; int f18; int f1c; };

extern "C" {
extern void FreeGfxSlotsById(int arg);
extern int GetGameLanguage(void);
extern unsigned int _ZN5Sound12PlayBank2_2DEj(unsigned int);
extern void _ZN8dScene_c14StartSceneFadeEjjt(unsigned int a, unsigned int b, unsigned short c);
extern void _ZN8dScene_c9SetFadersEP15FaderBrightness(void* fb);
extern unsigned int func_02012790(unsigned int x);
extern void func_02012dd0(void* c);
extern int func_0202ec9c(void* thiz, int arg1);
extern void func_ov004_020ad90c(void);
extern int func_ov004_020adbc0(void);
extern int func_ov004_020adbe0(void);
extern void func_ov004_020ae20c(void);
extern void func_ov004_020ae274(void* c);
extern void func_ov004_020ae2c8(void);
extern void func_ov004_020af948(void *a, int b, int c, void *d);
extern int func_ov004_020b04c0(void);
extern int func_ov004_020b0b1c(int arg);
extern void func_ov004_020b0cac(int c, int a1, int a2, int a3, int arg5, short arg6);
extern void func_ov004_020b0d8c(void *, int, int);
extern void func_ov004_020b14f0(void *);
extern void func_ov004_020b2444(int a, int b, int c, int d, int e, int f, int g);
extern void func_ov004_020b29a0(void* c, void* arg);
extern void func_ov004_020b67e8(int);
extern void func_ov004_020b6f14(char *c);
extern void func_ov004_020b7020(char *c);
extern void func_ov004_020b743c(char *p);
extern void func_ov004_020b7460(void *c);
}

extern W2 data_02086b58;
extern Obj* data_0209f5bc;
extern char data_0209f61c[];
extern unsigned char data_020a0e40;
extern unsigned char data_020a0de8[];
extern unsigned char data_020a0de9[];
extern Base* data_ov004_020beb68;
extern int data_ov004_020beb6c;
extern int data_ov004_020bc150;
extern int data_ov004_020bc7d4;
extern int data_ov004_020bfa24;

extern int data_ov004_020bc850;
extern int data_ov004_020bc854;
extern int data_ov004_020bc858;
extern int data_ov004_020bc85c;
extern int data_ov004_020bc860;
extern int data_ov004_020bc864;
extern int data_ov004_020bc868;
extern int data_ov004_020bc86c;
extern int data_ov004_020bc870;
extern int data_ov004_020bc874;
extern int data_ov004_020bc878;
extern int data_ov004_020bc87c;
extern int data_ov004_020bc880;
extern int data_ov004_020bc884;
extern int data_ov004_020bc888;
extern int data_ov004_020bc88c;
extern int data_ov004_020bc890;
extern int data_ov004_020bc894;
extern int data_ov004_020bc898;
extern int data_ov004_020bc89c;
extern int data_ov004_020bc8a0;
extern int data_ov004_020bc8a4;
extern int data_ov004_020bc8a8;
extern int data_ov004_020bc8ac;
extern int data_ov004_020bc8b0;
extern int data_ov004_020bc8b4;
extern int data_ov004_020bc8b8;
extern unsigned char *data_ov004_020bca44[];

extern W2 data_ov004_020bc8bc;
extern W2 data_ov004_020bc8c4;
extern W2 data_ov004_020bc8cc;
extern W2 data_ov004_020bc8d4;
extern W2 data_ov004_020bc8dc;
extern W2 data_ov004_020bc8e4;
extern W2 data_ov004_020bc8ec;
extern W2 data_ov004_020bc8f4;
extern W2 data_ov004_020bc8fc;
extern Pair data_ov004_020bc904;
extern W2 data_ov004_020bc90c;
extern Pair data_ov004_020bc914;
extern W2 data_ov004_020bc91c;
extern W2 data_ov004_020bc924;
extern W2 data_ov004_020bc93c;
extern W2 data_ov004_020bc98c;
extern W2 data_ov004_020bc994;
extern W2 data_ov004_020bc9ac;
extern W2 data_ov004_020bc9b4;
extern W2 data_ov004_020bc9c4;
extern W2 data_ov004_020bc9cc;
extern W2 data_ov004_020bc9e4;
extern W2 data_ov004_020bc9fc;
extern W2 data_ov004_020bca04;
extern W2 data_ov004_020bca14;
extern W2 data_ov004_020bca1c;
extern W2 data_ov004_020bca24;
extern W2 data_ov004_020bca2c;
extern W2 data_ov004_020bca34;

/* The per-state enter handlers, in state order. These are pointer-to-member
 * constants that mwccarm laid down in ov004's .data; the states they belong to
 * are not yet named, so they are still carried by address. */
extern dMgState_c::Callback data_ov004_020bc974;
extern dMgState_c::Callback data_ov004_020bc96c;
extern dMgState_c::Callback data_ov004_020bc964;
extern dMgState_c::Callback data_ov004_020bc95c;
extern dMgState_c::Callback data_ov004_020bc954;
extern dMgState_c::Callback data_ov004_020bc94c;
extern dMgState_c::Callback data_ov004_020bc944;
extern dMgState_c::Callback data_ov004_020bc92c;
extern dMgState_c::Callback data_ov004_020bc934;
extern dMgState_c::Callback data_ov004_020bc97c;
extern dMgState_c::Callback data_ov004_020bc984;
extern dMgState_c::Callback data_ov004_020bc99c;
extern dMgState_c::Callback data_ov004_020bc9a4;
extern dMgState_c::Callback data_ov004_020bca3c;
extern dMgState_c::Callback data_ov004_020bc9bc;
extern dMgState_c::Callback data_ov004_020bc9d4;
extern dMgState_c::Callback data_ov004_020bc9dc;
extern dMgState_c::Callback data_ov004_020bc9ec;
extern dMgState_c::Callback data_ov004_020bc9f4;
extern dMgState_c::Callback data_ov004_020bca0c;

// @symbol func_ov004_020b8a8c
extern "C" void func_ov004_020b8a8c(void) {
    data_ov004_020bc880 = 0x80;
    data_ov004_020bc854 = 0x80;
    data_ov004_020bc884 = 0x60;
    data_ov004_020bc888 = 0x80;
    data_ov004_020bc8b8 = 0x80;
    data_ov004_020bc87c = 0x60;
    data_ov004_020bc88c = 0x80;
    data_ov004_020bc860 = 0x60;
    data_ov004_020bc878 = 0x80;
    data_ov004_020bc890 = 0x60;
    data_ov004_020bc864 = -0x60;
    data_ov004_020bc8b4 = 0x40;
    data_ov004_020bc858 = 0x58;
    data_ov004_020bc874 = 0x28;
    data_ov004_020bc898 = 0x6c;
    data_ov004_020bc8a4 = 0x9c;
    data_ov004_020bc8a0 = 0x1d;
    data_ov004_020bc150 = 1;
    data_ov004_020bc868 = 0x80;
    data_ov004_020bc8b0 = 0x80;
    data_ov004_020bc8ac = 0x60;
    data_ov004_020bc894 = 0x80;
    data_ov004_020bc8a8 = 0x80;
    data_ov004_020bc86c = 0x80;
    data_ov004_020bc850 = 0x80;
    data_ov004_020bc89c = 0x60;
    data_ov004_020bc85c = 0x80;
    data_ov004_020bc870 = 0x80;
}

// @symbol _ZN10dMgState_cC1Ev
dMgState_c::dMgState_c()
    : mState(-1), mTimer(0), unk_020(0), unk_024(0)
{
}

// @symbol _ZN10dMgState_c8SetStateEi
void dMgState_c::SetState(s32 state)
{
    static Callback sEnterTable[20] = {
        data_ov004_020bc974,
        data_ov004_020bc96c,
        data_ov004_020bc964,
        data_ov004_020bc95c,
        data_ov004_020bc954,
        data_ov004_020bc94c,
        data_ov004_020bc944,
        data_ov004_020bc92c,
        data_ov004_020bc934,
        data_ov004_020bc97c,
        data_ov004_020bc984,
        data_ov004_020bc99c,
        data_ov004_020bc9a4,
        data_ov004_020bca3c,
        data_ov004_020bc9bc,
        data_ov004_020bc9d4,
        data_ov004_020bc9dc,
        data_ov004_020bc9ec,
        data_ov004_020bc9f4,
        data_ov004_020bca0c,
    };

    mState = state;
    mEnter = sEnterTable[mState];
    mRender = 0;
    if (mEnter)
        (this->*mEnter)();
}

// @symbol _ZN10dMgState_c8BehaviorEv
void dMgState_c::Behavior()
{
    if (mState == -1)
        return;
    ApproachLinear(mTimer, 0, 1);
    if (mBehavior == 0)
        return;
    (this->*mBehavior)();
}

// @symbol _ZN10dMgState_c6RenderEv
void dMgState_c::Render()
{
    if (mState == -1)
        return;
    if (mRender == 0)
        return;
    (this->*mRender)();
}

// @symbol func_ov004_020b8688
extern "C" void func_ov004_020b8688(char* r4){
  int r3, c;
  *(int*)(r4 + 0x1c) = 0xb4;
  r3 = data_ov004_020bc8a0;
  c = 3;
  if (r3 != 0x1d) { data_ov004_020bc8a0 = 0x1d; c = r3; }
  func_ov004_020b0cac(c, data_ov004_020bc880, data_ov004_020bc884, -1, -1, 0xd);
  func_ov004_020ae274((void*)4);
  *(W2*)(r4 + 8) = data_ov004_020bca1c;
}

// @symbol func_ov004_020b8560
extern "C" void func_ov004_020b8560(char* c){
  Base* r4 = data_ov004_020beb68;
  if (data_0209f5bc->m_18()) {
    data_0209f5bc->m_c(0x1e, 0);
    FreeGfxSlotsById(0x1d);
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(r4, *(void**)(c + 0x18));
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  } else {
    if (*(int*)(c + 0x1c) != 0)
      return;
    if (data_0209f5bc->m_1c())
      return;
    if (r4 == 0)
      return;
    if (r4->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    data_0209f5bc->m_10(0x1e, 0);
  }
}

// @symbol func_ov004_020b853c
extern "C" void func_ov004_020b853c(char *p) { *(int *)(p + 0x1c) = 15; *(W2 *)(p + 0x8) = data_ov004_020bc8bc; }

// @symbol func_ov004_020b841c
extern "C" void func_ov004_020b841c(char* c) {
  if (data_0209f5bc->m_18() != 0) {
    Base* r5 = data_ov004_020beb68;
    if (r5 == 0) return;
    if (r5->m_4c(*(void**)(c + 0x18)) == 0) return;
    data_0209f5bc->m_c(0x1e, 0);
    FreeGfxSlotsById(0x1d);
    func_ov004_020b29a0(r5, *(void**)(c + 0x18));
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  } else {
    if (*(int*)(c + 0x1c) != 0) return;
    if (data_0209f5bc->m_1c() != 0) return;
    data_0209f5bc->m_10(0x1e, 0);
  }
}

// @symbol func_ov004_020b83ac
extern "C" void func_ov004_020b83ac(char* c){
  *(int*)(c+0x1c) = 0xb4;
  func_ov004_020b0cac(5, data_ov004_020bc854, data_ov004_020bc87c, -1, -1, 0xd);
  func_ov004_020ae274((void*)1);
  *(W2*)(c+8) = data_ov004_020bc8c4;
}

// @symbol func_ov004_020b8284
extern "C" void func_ov004_020b8284(char* c){
  if (data_0209f5bc->m_18()) {
    Base* r5 = data_ov004_020beb68;
    if (r5 == 0)
      return;
    if (r5->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    data_0209f5bc->m_c(0x1e, 0);
    FreeGfxSlotsById(0x1d);
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(r5, *(void**)(c + 0x18));
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  } else {
    if (*(int*)(c + 0x1c) != 0)
      return;
    if (data_0209f5bc->m_1c() == 0)
      data_0209f5bc->m_10(0x1e, 0);
  }
}

// @symbol func_ov004_020b81f8
extern "C" void func_ov004_020b81f8(char* r4){
  int r3, c;
  *(int*)(r4 + 0x1c) = 0x3c;
  r3 = data_ov004_020bc8a0;
  c = 8;
  if (r3 != 0x1d) { data_ov004_020bc8a0 = 0x1d; c = r3; }
  func_ov004_020b0cac(c, data_ov004_020bc8b8, data_ov004_020bc8b4, -1, -1, 0xd);
  func_ov004_020ae274((void*)3);
  *(W2*)(r4 + 8) = data_ov004_020bc8d4;
}

// @symbol func_ov004_020b8098
extern "C" void func_ov004_020b8098(char* r4){
  Obj* o;
  if (func_ov004_020b0b1c(0)) {
    o = data_0209f5bc;
    o->m_10(0x1e, 0);
    *(W2*)(r4 + 8) = data_ov004_020bc8dc;
    return;
  }
  if (func_ov004_020b0b1c(2) || func_ov004_020b0b1c(1)) {
    o = data_0209f5bc;
    if (o->m_1c())
      return;
    _ZN8dScene_c9SetFadersEP15FaderBrightness(data_0209f61c);
    func_0202ec9c(data_0209f61c, 2);
    _ZN8dScene_c14StartSceneFadeEjjt(5, 0, 0);
    func_02012dd0((void*)0x3c);
    return;
  }
  if (*(int*)(r4 + 0x1c) != 0)
    return;
  func_ov004_020b0cac(0, data_ov004_020bc8a8, data_ov004_020bc898, -1, -1, 0xd);
  func_ov004_020b0cac(2, data_ov004_020bc86c, data_ov004_020bc8a4, -1, -1, 0xd);
}

// @symbol func_ov004_020b7fec
extern "C" void func_ov004_020b7fec(char* c){
  if (data_0209f5bc->m_18() == 0)
    return;
  void* r5 = data_ov004_020beb68;
  if (r5 == 0)
    return;
  data_0209f5bc->m_c(0x1e, 0);
  FreeGfxSlotsById(0x1d);
  func_ov004_020ae20c();
  func_ov004_020ae2c8();
  func_ov004_020b29a0(r5, *(void**)(c + 0x18));
  *(int*)(c + 0x1c) = 0;
  *(int*)(c + 0x18) = -1;
  *(int*)(c + 0x20) = 0;
  *(int*)(c + 0x24) = 0;
}

// @symbol func_ov004_020b7f5c
extern "C" void func_ov004_020b7f5c(char* r4){
  int r3, c;
  *(int*)(r4 + 0x1c) = 0x78;
  r3 = data_ov004_020bc8a0;
  c = 9;
  if (r3 != 0x1d) { data_ov004_020bc8a0 = 0x1d; c = r3; }
  func_ov004_020b0cac(c, data_ov004_020bc88c, data_ov004_020bc860, -1, -1, 0xd);
  _ZN5Sound12PlayBank2_2DEj(0x136);
  *(W2*)(r4 + 8) = data_ov004_020bc8ec;
}

// @symbol func_ov004_020b7eac
extern "C" void func_ov004_020b7eac(char* c){
  if (*(int*)(c + 0x1c) > 0x64)
    return;
  if (*(int*)(c + 0x1c) != 0)
    return;
  Base* o = data_ov004_020beb68;
  if (o == 0)
    return;
  if (o->m_4c(*(void**)(c + 0x18)) == 0)
    return;
  FreeGfxSlotsById(0x1d);
  func_ov004_020ae20c();
  func_ov004_020ae2c8();
  func_ov004_020b29a0(o, *(void**)(c + 0x18));
  *(int*)(c + 0x1c) = 0;
  *(int*)(c + 0x18) = -1;
  *(int*)(c + 0x20) = 0;
  *(int*)(c + 0x24) = 0;
}

// @symbol func_ov004_020b7e38
extern "C" void func_ov004_020b7e38(char* c) {
    *(int*)(c + 0x1c) = 0x78;
    func_ov004_020b0cac(0xa, data_ov004_020bc878, data_ov004_020bc890, -1, -1, 0xd);
    _ZN5Sound12PlayBank2_2DEj(0x137);
    *(W2*)(c + 8) = data_ov004_020bc8f4;
}

// @symbol func_ov004_020b7cd0
extern "C" void func_ov004_020b7cd0(char* c){
  int state = *(int*)(c + 0x1c);
  if (state > 0x64)
    return;
  Base* p = data_ov004_020beb68;
  int a8 = (p != 0) ? *(int*)((char*)p + 0xa8) : 0;
  if (a8 == 0) {
    if (state != 0)
      return;
    Base* q = data_ov004_020beb68;
    if (q == 0)
      return;
    if (q->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    data_ov004_020bc7d4 = 0;
    data_ov004_020bfa24 = 0;
    func_ov004_020ae20c();
    FreeGfxSlotsById(0x1d);
    *(int*)(c + 0x1c) = 0xb4;
    {
        int a = data_ov004_020bc904.a;
        int b = data_ov004_020bc904.b;
        *(int*)(c + 8) = b ? a : a;
        *(int*)(c + 0xc) = b;
    }
  } else {
    if (state != 0)
      return;
    Base* r5 = data_ov004_020beb68;
    if (r5 == 0)
      return;
    if (r5->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    FreeGfxSlotsById(0x1d);
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(r5, *(void**)(c + 0x18));
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  }
}

// @symbol func_ov004_020b7c04
extern "C" void func_ov004_020b7c04(char* c)
{
    Obj* o;
    int v = *(int*)(c + 0x1c);
    if (v == 0xb3) {
        func_ov004_020b0cac(8, data_ov004_020bc8b8, data_ov004_020bc8b4, -1, -1, 0xd);
        func_ov004_020ae274((void*)3);
        return;
    }
    if (v != 0) return;
    o = *(Obj**)&data_0209f5bc;
    if (o->m_1c() != 0) return;
    _ZN8dScene_c9SetFadersEP15FaderBrightness(data_0209f61c);
    func_0202ec9c(data_0209f61c, 2);
    _ZN8dScene_c14StartSceneFadeEjjt(5, 0, 0);
    func_02012dd0((void*)0x3c);
}

// @symbol func_ov004_020b7b90
extern "C" void func_ov004_020b7b90(char* c) {
    *(int*)(c + 0x1c) = 0x78;
    func_ov004_020b0cac(0xa, data_ov004_020bc878, data_ov004_020bc890, -1, -1, 0xd);
    _ZN5Sound12PlayBank2_2DEj(0x137);
    *(W2*)(c + 8) = data_ov004_020bc90c;
}

// @symbol func_ov004_020b7b20
extern "C" void func_ov004_020b7b20(char* c){
  int v = *(int*)(c+0x1c);
  if(v > 0x64) return;
  if(v != 0) return;
  FreeGfxSlotsById(0x1d);
  *(W2*)(c+8) = data_ov004_020bc93c;
  *(int*)(c+0x24) = 1;
  data_ov004_020bc7d4 = 0;
  data_ov004_020bfa24 = 0;
}

// @symbol func_ov004_020b7a18
extern "C" void func_ov004_020b7a18(char *c)
{
    Base *g = data_ov004_020beb68;
    int cond = g ? g->a8 : 0;
    if (cond == 0) {
        g = data_ov004_020beb68;
        if (!g) return;
        if (!g->m_4c(*(void**)(c + 0x18))) return;
        func_ov004_020ae20c();
        *(int *)(c + 0x1c) = 0xb4;
        *(W2 *)(c + 8) = data_ov004_020bc98c;
        return;
    }
    g = data_ov004_020beb68;
    if (!g) return;
    if (!g->m_4c(*(void**)(c + 0x18))) return;
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(g, *(void**)(c + 0x18));
    *(int *)(c + 0x1c) = 0;
    *(int *)(c + 0x18) = -1;
    *(int *)(c + 0x20) = 0;
    *(int *)(c + 0x24) = 0;
}

// @symbol func_ov004_020b79b0
extern "C" void func_ov004_020b79b0(char *c) {
    *(int *)(c + 0x1c) = 0x78;
    func_ov004_020b0cac(0xb, data_ov004_020bc868, data_ov004_020bc858, -1, -1, 0xd);
    *(W2 *)(c + 8) = data_ov004_020bc9ac;
}

// @symbol func_ov004_020b798c
extern "C" void func_ov004_020b798c(char *p) { *(int *)(p + 0x1c) = 0; *(W2 *)(p + 0x8) = data_ov004_020bc9b4; }

// @symbol func_ov004_020b78f4
extern "C" void func_ov004_020b78f4(char* c) {
  Base* r4;
  if (*(int*)(c + 0x1c) != 0) return;
  r4 = data_ov004_020beb68;
  if (r4 == 0) return;
  if (r4->m_4c(*(void**)(c + 0x18)) == 0) return;
  FreeGfxSlotsById(0x1d);
  func_ov004_020b29a0(r4, *(void**)(c + 0x18));
  *(int*)(c + 0x1c) = 0;
  *(int*)(c + 0x18) = -1;
  *(int*)(c + 0x20) = 0;
  *(int*)(c + 0x24) = 0;
}

// @symbol func_ov004_020b7854
extern "C" void func_ov004_020b7854(char* c){
    int arg0;
    *(int*)(c+0x1c) = 0xb4;
    func_ov004_020ae274((void*)4);
    arg0 = 3;
    if (data_ov004_020bc8a0 != 0x1d) { arg0 = data_ov004_020bc8a0; data_ov004_020bc8a0 = 0x1d; }
    func_ov004_020b0cac(arg0, data_ov004_020bc8b0, data_ov004_020bc8ac - (data_ov004_020beb6c + 0xc0), -1, -1, 0xa);
    *(W2*)(c+8) = data_ov004_020bc9e4;
}

// @symbol func_ov004_020b77b4
extern "C" void func_ov004_020b77b4(char* c){
  if (*(int*)(c + 0x1c) != 0)
    return;
  Base* o = data_ov004_020beb68;
  if (o == 0)
    return;
  if (o->m_4c(*(void**)(c + 0x18)) == 0)
    return;
  FreeGfxSlotsById(0x1d);
  func_ov004_020ae20c();
  func_ov004_020ae2c8();
  func_ov004_020b29a0(o, *(void**)(c + 0x18));
  *(int*)(c + 0x1c) = 0;
  *(int*)(c + 0x18) = -1;
  *(int*)(c + 0x20) = 0;
  *(int*)(c + 0x24) = 0;
}

// @symbol func_ov004_020b7744
extern "C" void func_ov004_020b7744(char* c){
  *(int*)(c+0x1c) = 0xb4;
  func_ov004_020ae274((void*)3);
  func_ov004_020b0cac(8, data_ov004_020bc894, data_ov004_020bc874, -1, -1, 0xb);
  *(W2*)(c+8) = data_ov004_020bc9fc;
}

// @symbol func_ov004_020b75e4
extern "C" void func_ov004_020b75e4(char* r4){
  Obj* o;
  if (func_ov004_020b0b1c(0)) {
    o = data_0209f5bc;
    o->m_10(0x1e, 0);
    *(W2*)(r4 + 8) = data_ov004_020bca14;
    return;
  }
  if (func_ov004_020b0b1c(2) || func_ov004_020b0b1c(1)) {
    o = data_0209f5bc;
    if (o->m_1c())
      return;
    _ZN8dScene_c9SetFadersEP15FaderBrightness(data_0209f61c);
    func_0202ec9c(data_0209f61c, 2);
    _ZN8dScene_c14StartSceneFadeEjjt(5, 0, 0);
    func_02012dd0((void*)0x3c);
    return;
  }
  if (*(int*)(r4 + 0x1c) != 0)
    return;
  func_ov004_020b0cac(0, data_ov004_020bc8a8, data_ov004_020bc898, -1, -1, 0xd);
  func_ov004_020b0cac(1, data_ov004_020bc86c, data_ov004_020bc8a4, -1, -1, 0xd);
}

// @symbol func_ov004_020b7594
extern "C" void func_ov004_020b7594(char* c){
  *(int*)(c+0x1c)=0xb4;
  func_ov004_020ae274((void*)1);
  *(W2*)(c+0x8)=data_ov004_020bca24;
  *(W2*)(c+0x10)=data_ov004_020bc8cc;
}

// @symbol func_ov004_020b746c
extern "C" void func_ov004_020b746c(char* c){
  if (data_0209f5bc->m_18()) {
    Base* r5 = data_ov004_020beb68;
    if (r5 == 0)
      return;
    if (r5->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    data_0209f5bc->m_c(0x1e, 0);
    FreeGfxSlotsById(0x1d);
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(r5, *(void**)(c + 0x18));
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  } else {
    if (*(int*)(c + 0x1c) != 0)
      return;
    if (data_0209f5bc->m_1c() == 0)
      data_0209f5bc->m_10(0x1e, 0);
  }
}

// @symbol func_ov004_020b7460
extern "C" void func_ov004_020b7460(void *c) {
    func_ov004_020b743c((char *)c);
}

// @symbol func_ov004_020b743c
extern "C" void func_ov004_020b743c(char *p) { *(int *)(p + 0x1c) = 10; *(W2 *)(p + 0x8) = data_ov004_020bc8fc; }

// @symbol func_ov004_020b72d4
extern "C" void func_ov004_020b72d4(char* c){
  if (*(int*)(c + 0x1c) == 0) {
    if (func_ov004_020b0b1c(0) != 0) {
      data_0209f5bc->m_10(0x1e, 0);
      int a = data_ov004_020bc914.a;
      int b = data_ov004_020bc914.b;
      *(int*)(c + 8) = b ? a : a;
      *(int*)(c + 0xc) = b;
      return;
    }
    if (func_ov004_020b0b1c(2) == 0) {
      if (func_ov004_020b0b1c(1) == 0)
        return;
    }
    if (data_0209f5bc->m_1c() != 0)
      return;
    _ZN8dScene_c9SetFadersEP15FaderBrightness(data_0209f61c);
    func_0202ec9c(data_0209f61c, 2);
    _ZN8dScene_c14StartSceneFadeEjjt(5, 0, 0);
    func_02012dd0((void*)0x3c);
    return;
  }
  func_ov004_020b0cac(0, data_ov004_020bc8a8, data_ov004_020bc898, -1, -1, 0xd);
  func_ov004_020b0cac(2, data_ov004_020bc86c, data_ov004_020bc8a4, -1, -1, 0xd);
  *(int*)(c + 0x1c) = 0;
}

// @symbol func_ov004_020b724c
extern "C" void func_ov004_020b724c(char* c){
  *(int*)(c+0x1c) = 0xb4;
  func_ov004_020b0cac(5, data_ov004_020bc854, data_ov004_020bc87c, -1, -1, 0xd);
  func_ov004_020ae274((void*)1);
  *(W2*)(c+8) = data_ov004_020bc994;
  *(W2*)(c+0x10) = data_ov004_020bc9c4;
}

// @symbol func_ov004_020b7124
extern "C" void func_ov004_020b7124(char* c){
  if (data_0209f5bc->m_18()) {
    Base* r5 = data_ov004_020beb68;
    if (r5 == 0)
      return;
    if (r5->m_4c(*(void**)(c + 0x18)) == 0)
      return;
    data_0209f5bc->m_c(0x1e, 0);
    func_ov004_020ae20c();
    func_ov004_020ae2c8();
    func_ov004_020b29a0(r5, *(void**)(c + 0x18));
    FreeGfxSlotsById(0x1d);
    *(int*)(c + 0x1c) = 0;
    *(int*)(c + 0x18) = -1;
    *(int*)(c + 0x20) = 0;
    *(int*)(c + 0x24) = 0;
  } else {
    if (*(int*)(c + 0x1c) != 0)
      return;
    if (data_0209f5bc->m_1c() == 0)
      data_0209f5bc->m_10(0x1e, 0);
  }
}

// @symbol func_ov004_020b70b4
extern "C" void func_ov004_020b70b4(char* c){
  *(int*)(c+0x1c) = 0x3c;
  func_ov004_020b0cac(0xe, data_ov004_020bc8b8, data_ov004_020bc8b4, -1, -1, 0xd);
  func_ov004_020ae274((void*)3);
  *(W2*)(c+8) = data_ov004_020bca04;
}

// @symbol func_ov004_020b7020
extern "C" void func_ov004_020b7020(char* c) {
  if (func_ov004_020adbe0()) {
    func_ov004_020b6f14((char *)c);
    return;
  }
  func_ov004_020b0cac(7, 0x80, 0x14, -1, -1, 0xd);
  *(int*)(c+0x1c) = 0x78;
  *(W2*)(c+8) = data_ov004_020bc8e4;
  *(W2*)(c+0x10) = data_ov004_020bc91c;
  func_ov004_020ad90c();
}

// @symbol func_ov004_020b6f88
extern "C" void func_ov004_020b6f88(char* c) {
  int ok;
  unsigned char idx;
  int i;
  if (*(int*)(c + 0x1c) != 0) return;
  idx = data_020a0e40;
  ok = 0;
  i = idx * 4;
  if (data_020a0de8[i] != 0) {
    if (data_020a0de9[i] != 0) ok = 1;
  }
  if (ok == 0) return;
  func_02012790(0x62);
  FreeGfxSlotsById(7);
  *(W2*)(c + 0x10) = data_02086b58;
  func_ov004_020b7460(c);
}

// @symbol func_ov004_020b6f14
extern "C" void func_ov004_020b6f14(char* c) {
  func_ov004_020b0cac(7, 0x80, 0x14, -1, -1, 0xd);
  *(int*)(c+0x1c) = 0x12c;
  *(W2*)(c+8) = data_ov004_020bc9cc;
  *(W2*)(c+0x10) = data_ov004_020bca2c;
  func_ov004_020ad90c();
}

// @symbol func_ov004_020b6ddc
extern "C" void func_ov004_020b6ddc(char *c)
{
    int idx;
    int flag;
    void *o;
    int v;
    int s;
    int r4;

    if (*(int *)(c + 0x1c) == 0) {
        idx = data_020a0e40;
        flag = 0;
        if (data_020a0de8[idx * 4] != 0) {
            if (data_020a0de9[idx * 4] != 0)
                flag = 1;
        }
        if (flag != 0) {
            FreeGfxSlotsById(7);
            func_02012790(0x62);
            *(W2 *)(c + 0x10) = data_02086b58;
            func_ov004_020b7460(c);
        }
    }

    if (*(int *)(c + 0x1c) != 0xb4)
        return;

    o = data_ov004_020beb68;
    if (o) {
        void (*m50)(void *) = *(void (**)(void *))(*(unsigned int *)o + 0x50);
        m50(o);
    }

    r4 = 5;
    v = data_ov004_020bc8a0;
    if (v != 0x1d) {
        data_ov004_020bc8a0 = 0x1d;
        r4 = v;
    }

    s = data_ov004_020bc864;
    if (s < 0)
        s -= func_ov004_020b04c0();

    func_ov004_020b0cac(r4, data_ov004_020bc888, s, -1, -1, 0xd);
    func_ov004_020ae274((void*)1);
}

// @symbol func_ov004_020b6d6c
extern "C" void func_ov004_020b6d6c(char* c){
  *(int*)(c+0x1c) = 0xb4;
  func_ov004_020b0cac(8, data_ov004_020bc8b8, data_ov004_020bc8b4, -1, -1, 0xd);
  func_ov004_020ae274((void*)3);
  *(W2*)(c+8) = data_ov004_020bc924;
}

// @symbol func_ov004_020b6c9c
extern "C" void func_ov004_020b6c9c(StateObj *c)
{
    int x = c->f1c;
    void *g = data_ov004_020beb68;
    if (x != 0) {
        unsigned char idx = data_020a0e40;
        int off = idx * 4;
        int ok = 0;
        if (data_020a0de8[off]) {
            if (data_020a0de9[off]) ok = 1;
        }
        if (ok == 0)
            return;
    }
    {
        int (*fn)(void *, int) = (int (*)(void *, int)) (*(void ***)g)[0x13];
        if (fn(g, c->f18) == 0)
            return;
    }
    c->p10 = data_02086b58;
    FreeGfxSlotsById(0x1d);
    if (func_ov004_020adbe0() != 0) {
        func_ov004_020b6f14((char *)c);
        return;
    }
    func_ov004_020b7020((char *)c);
}

// @symbol func_ov004_020b6c10
extern "C" void func_ov004_020b6c10(char* r4){
  int r3, c;
  *(int*)(r4 + 0x1c) = 0xb4;
  r3 = data_ov004_020bc8a0;
  c = 3;
  if (r3 != 0x1d) { data_ov004_020bc8a0 = 0x1d; c = r3; }
  func_ov004_020b0cac(c, data_ov004_020bc880, data_ov004_020bc884, -1, -1, 0xd);
  func_ov004_020ae274((void*)4);
  *(W2*)(r4 + 8) = data_ov004_020bca34;
}

// @symbol func_ov004_020b6b40
extern "C" void func_ov004_020b6b40(StateObj *c)
{
    int x = c->f1c;
    void *g = data_ov004_020beb68;
    if (x != 0) {
        unsigned char idx = data_020a0e40;
        int off = idx * 4;
        int ok = 0;
        if (data_020a0de8[off]) {
            if (data_020a0de9[off]) ok = 1;
        }
        if (ok == 0)
            return;
    }
    {
        int (*fn)(void *, int) = (int (*)(void *, int)) (*(void ***)g)[0x13];
        if (fn(g, c->f18) == 0)
            return;
    }
    c->p10 = data_02086b58;
    FreeGfxSlotsById(0x1d);
    if (func_ov004_020adbe0() != 0) {
        func_ov004_020b6f14((char *)c);
        return;
    }
    func_ov004_020b7020((char *)c);
}

// @symbol func_ov004_020b6ad8
extern "C" void func_ov004_020b6ad8(void) {
    int r;
    if (data_ov004_020beb68 == 0) return;
    r = func_ov004_020adbc0();
    func_ov004_020b2444(data_ov004_020bc850, data_ov004_020bc89c, r, -1, -1, 0, 0);
}

// @symbol func_ov004_020b6948
extern "C" void func_ov004_020b6948(void) {
    unsigned int r;
    int idx;

    if (data_ov004_020beb68 == 0) return;

    r = func_ov004_020adbc0();
    func_ov004_020b2444(data_ov004_020bc85c + 4, data_ov004_020bc870, r / 100, -1, -1, 1, 0);

    r = func_ov004_020adbc0();
    func_ov004_020b2444(data_ov004_020bc85c + 0x1c, data_ov004_020bc870, r % 100 / 10, -1, -1, 1, 0);

    r = func_ov004_020adbc0();
    func_ov004_020b2444(data_ov004_020bc85c + 0x2c, data_ov004_020bc870, r % 10, -1, -1, 1, 0);

    idx = GetGameLanguage();
    func_ov004_020af948((void *)*(int *)(data_ov004_020bca44[idx] + 0xc), data_ov004_020bc85c - 0x24, data_ov004_020bc870, 0);

    idx = GetGameLanguage();
    func_ov004_020af948((void *)*(int *)(data_ov004_020bca44[idx] + 0x1c), data_ov004_020bc85c + 0x10, data_ov004_020bc870, 0);
}

// @symbol func_ov004_020b68e8
extern "C" void func_ov004_020b68e8(int *c) {
    void *g = data_ov004_020beb68;
    if (g == 0) return;
    func_ov004_020b14f0(g);
    if (c[7] > 0) return;
    func_ov004_020b0d8c(g, 0xe0, 0xa0);
}

// @symbol func_ov004_020b682c
extern "C" void func_ov004_020b682c(void)
{
    int v1 = -0x1c - func_ov004_020b04c0();
    data_ov004_020bc88c = 0x80;
    data_ov004_020bc860 = v1;
    {
        int v2 = -0x1c - func_ov004_020b04c0();
        data_ov004_020bc878 = 0x80;
        data_ov004_020bc890 = v2;
    }
    {
        int v3 = -0x1c - func_ov004_020b04c0();
        data_ov004_020bc868 = 0x80;
        data_ov004_020bc858 = v3;
        data_ov004_020bc888 = 0x80;
        data_ov004_020bc864 = -0x1c;
        data_ov004_020bc8b8 = 0x80;
        data_ov004_020bc8b4 = 0x60;
    }
}

// @symbol func_ov004_020b6808
extern "C" void func_ov004_020b6808(void)
{
    data_ov004_020bc888 = 0x80;
    data_ov004_020bc864 = -0x1c;
}

// @symbol func_ov004_020b67f8
extern "C" void func_ov004_020b67f8(void) {
    func_ov004_020b67e8(6);
}

// @symbol func_ov004_020b67e8
extern "C" void func_ov004_020b67e8(int v) { data_ov004_020bc8a0 = v; }
