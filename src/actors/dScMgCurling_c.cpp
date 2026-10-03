//cpp
/* One-player curling scene: the whole ov006 unit 0x020e0638..0x020e3820,
 * 46 functions in ROM order, from the destructor to InitResources. The
 * destructor is out of line and comes first, which makes this file the key
 * function's home, so the vtable and RTTI are emitted here too.
 * dScMgCurling_c_classInit, just above at 0x020e3820, is still its own
 * file (src/d_s_mg_curling.cpp).
 *
 * Functions run in ROM order under `#pragma defer_codegen off`; do not
 * reorder.
 *
 * The nineteen functions from func_ov006_020e20bc to InitResources came
 * from one-function files, func_ov006_020e20bc last (it was the hole that
 * split the unit until it matched). Their bodies are kept as they matched
 * there; only the declarations were merged: cstd::atan2 keeps the int
 * spelling the lower half already needed, func_ov004_020adc1c takes no
 * argument, LoadFile returns void *, and func_ov006_020e3388 takes the
 * scene as char * rather than int. The two state tables read by
 * func_ov006_020e3078 and Behavior dispatch through the same incomplete
 * receiver C as the four below.
 *
 * Blocked: dScMgCurling_c.h types only the stones (mStone) and a few
 * fields; the falling bits at 0x478c, the score popups at 0x473c and the
 * dragged stone at 0x4eb0 are padding there, so the helpers take the scene
 * as raw bytes. The helpers are unnamed in symbols.txt.
 */
#pragma defer_codegen off

#include "types.h"
#include "dScMgCurling_c.h"

/* `C` is the receiver of the pointer-to-member state tables. It must stay
 * incomplete: mwccarm picks the pointer-to-member layout from whether the
 * class is complete, and completing it changes all four dispatchers. */
struct C;
typedef void (C::*PMF)(int);
struct Entry { PMF pmf; };
typedef void (C::*PMF0)();

/* The five score popups at 0x473c, as func_ov006_020e1554 reads them. The
 * other functions reach the same bytes by offset, which is what the ROM
 * shows. p0 covers the u16 countdown at +0xa and the active flag at +0xc. */
typedef struct {
    int x;
    int y;
    unsigned short points;
    unsigned char p0[3];
    unsigned char shown;
    unsigned char p1[2];
} ScorePopup;

typedef struct {
    unsigned char _pad[0x473c];
    ScorePopup popups[5];
    unsigned char _pad2[0x4ee8 - 0x473c - 5*16];
    unsigned char unk_4ee8;
} ScoreView;

/* The dragged stone, as func_ov006_020e26f8 reads it: the stone row plus
 * the aim point at 0x4eb0 past the five rows. */
struct DragStone {
    int x;
    int y;
    int pad[3];
    int grabX;
    int grabY;
    int pad2[3];
    unsigned char dragging;
    unsigned char pad3[3];
};

struct DragView {
    unsigned char pad[0x4660];
    DragStone stone[48];
    unsigned char pad2[0x10];
    int aimX;
    int aimY;
};

/* data_020a0dea and data_020a0deb are four-byte touch records. The drag
 * handler reads them through a `u8 *` cast; a plain `u8 []` costs eighteen
 * words there. */
struct B4 { unsigned char v; unsigned char pad[3]; };

/* One of the 0x32 falling bits at 0x478c (0x24 bytes each): position,
 * velocity, a wait and a timer, and state bytes. */
#define BIT_X(b,i)       (*(int*)  ((char*)(b) + 0x478c + (i)*0x24))
#define BIT_Y(b,i)       (*(int*)  ((char*)(b) + 0x4790 + (i)*0x24))
#define BIT_VX(b,i)      (*(int*)  ((char*)(b) + 0x4794 + (i)*0x24))
#define BIT_VY(b,i)      (*(int*)  ((char*)(b) + 0x4798 + (i)*0x24))
#define BIT_WAIT(b,i)    (*(unsigned short*)((char*)(b) + 0x47a0 + (i)*0x24))
#define BIT_WAIT_S(b,i)  (*(short*)((char*)(b) + 0x47a0 + (i)*0x24))
#define BIT_TIMER(b,i)   (*(unsigned short*)((char*)(b) + 0x47a2 + (i)*0x24))
#define BIT_TIMER_S(b,i) (*(short*)((char*)(b) + 0x47a2 + (i)*0x24))
#define BIT_STATE(b,i)   (*(unsigned char*)((char*)(b) + 0x47aa + (i)*0x24))
/* Launder: forces an address through an integer so it is not shared. */
#define M(p) ((int *)(int)(p))
/* FX_Mul: 12-bit fixed-point product, rounded. */
#define FMUL(a, b) ((int)(((long long)(a) * (b) + 0x800) >> 12))

/* Everything this file calls or reads. Class members cannot go inside an
 * `extern "C"` block, so the C-linkage names are collected here in one. */
extern "C" {

extern int  func_ov004_020adbc0(void);
extern void func_ov004_020af948(int a, int b, int c, int d);
extern int  func_ov004_020afdd0(int a, int b, int c, int d, int e);
extern void func_ov004_020b2220(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern void func_ov004_020b2444(int a1, int a2, int num, int a4, int a5, int sel, int idx);
extern void func_02012718(int a, int b);
extern void DrawOamSprite(int a, int b, int c, int d);
extern void RenderOamBothScreens(void* a0, int a1, int a2, int a3, int a4, void* a5);
extern int  RandomIntInternal(int *seed);
extern int  _ZN4cstd5atan2E5Fix12IiES1_(int y, int x);
extern int  func_020126e8(int a);
extern void func_020126ac(int a0, int a1, int a2, int a3, int s0);
extern int  func_02012468(int a, int b, int c, int d, int e, int f, int g, short h);
extern int  func_ov004_020adbe0(void);
extern void func_ov004_020b0a54(int state);
extern void func_ov004_020adb1c(int score);
extern int  func_ov004_020adc1c(void);
extern int  func_ov004_020b19f0(int self);
extern char *func_ov004_020adc74(void *p);
extern void func_ov004_020b04d0(int a);
extern void *_ZN2G213GetBG2CharPtrEv(void);
extern unsigned _ZN3G2S13GetBG2CharPtrEv(void);
extern void DecompressLZ16(const void *src, void *dst);
extern void *LoadFile(int handle);
extern void Deallocate(void *ptr);
extern void Ov004_Deallocate(void *p);
extern void func_020563d4(const void *src, u32 offset, u32 count);
extern void func_02056374(const void *src, u32 offset, u32 count);
extern void _ZN2GX10LoadBGPlttEPKvjj(const void *p, u32 a, u32 b);
extern void _ZN2GX11LoadOBJPlttEPKvjj(const void *p, u32 a, u32 b);
extern void _ZN3GXS10LoadBGPlttEPKvjj(const void *p, u32 a, u32 b);
extern void _ZN3GXS11LoadOBJPlttEPKvjj(const void *p, u32 a, u32 b);
extern void _ZN3G2x13SetBlendAlphaEPVttttj(volatile u16 *p, u16 a, u16 b, u16 c, u16 d);

extern int  data_0209d4b8;
extern s16  data_02082214[];
extern u8   data_020a0de8[];
extern u8   data_020a0de9[];
extern struct B4 data_020a0dea[];
extern struct B4 data_020a0deb[];
extern u8   data_020a0e40[];
extern unsigned char data_ov006_0212e450[];
extern unsigned char data_ov006_0212e454[];
extern u8   data_ov006_0212e458[];
extern int  data_ov006_0212e460[];
extern int  data_ov006_0212e468[];
extern int  data_ov006_0213a5e0[];
extern int  data_ov006_0213c264;
extern int  data_ov006_0213c2ac;
extern int  data_ov006_0213c2e4[];
extern unsigned char data_0209d45c;
extern unsigned char data_0209d454;
extern char data_ov006_0213c394;
extern char data_ov006_0213c3b4;
extern int  data_ov006_0212e478[];
extern int  data_ov006_0212e48c[];
extern int  data_ov006_0212e4a0[];
extern int  data_ov006_0212e4b4[];
extern int  data_ov006_0212e4c8[];
extern int  data_ov006_0212e4dc[];

/* The six state tables, filled at startup by the ov006 static init. */
extern PMF0  data_ov006_021418b0[];
extern PMF   data_ov006_021418c0[];
extern PMF   data_ov006_021418d8[];
extern Entry data_ov006_021418f0[];
extern PMF   data_ov006_02141910[];
extern PMF   data_ov006_02141930[];
extern PMF0  data_ov006_02141950[];

/* Defined below. */
extern void func_ov006_020e0694(char *c);
extern void func_ov006_020e071c(char *c, int i);
extern void func_ov006_020e07b0(char *o, int i);
extern void func_ov006_020e0884(char *c, int i);
extern void func_ov006_020e091c(char *base, int i);
extern void func_ov006_020e0a24(char *base, int idx);
extern void func_ov006_020e0b64(char *base, int index);
extern void func_ov006_020e0ca0(char *o, int i);
extern void func_ov006_020e0d84(char *c, int i);
extern void func_ov006_020e0e18(char *base, int idx);
extern void func_ov006_020e0edc(char *c, int idx);
extern void func_ov006_020e0ff0(void *base, int idx);
extern void func_ov006_020e1100(char *c, int idx);
extern void func_ov006_020e1214(char *base, int idx);
extern void func_ov006_020e1264(char *c, int idx);
extern void func_ov006_020e12d0(char *o);
extern void func_ov006_020e13a4(char *c);
extern void func_ov006_020e1554(ScoreView *o);
extern void func_ov006_020e1608(char *self);
extern void func_ov006_020e1680(char *o);
extern void func_ov006_020e17f8(char *self);
extern void func_ov006_020e1854(void *arg);
extern void func_ov006_020e1b54(char *c);
extern void func_ov006_020e1c68(char *a0);
extern void func_ov006_020e1dc8(dScMgCurling_c *self, int idx);
extern void func_ov006_020e20bc(dScMgCurling_c *self, int idx);
extern void func_ov006_020e269c(char *c, int i);
extern void func_ov006_020e26f8(DragView *w, int i);
extern void func_ov006_020e285c(dScMgCurling_c *self, int idx);
extern void func_ov006_020e2868(char *c, int idx);
extern void func_ov006_020e2c08(char *self, int idx);
extern void func_ov006_020e2dbc(char *c);
extern void func_ov006_020e2eb8(void);
extern void func_ov006_020e2ebc(char *thiz);
extern void func_ov006_020e2f78(char *c);
extern void func_ov006_020e3078(char *c);
extern void func_ov006_020e3210(char *c);
extern void func_ov006_020e3250(char *c);
extern void func_ov006_020e3378(char *p);
extern void func_ov006_020e3388(char *raw);

}  /* extern "C" */

namespace cstd { int sqrt(u64 value); }
namespace Sound { u32 PlayBank2_2D(u32 id); }

// @symbol _ZN14dScMgCurling_cD1Ev
// @symbol _ZN14dScMgCurling_cD0Ev
dScMgCurling_c::~dScMgCurling_c()
{
}
// @symbol func_ov006_020e0694
/* Draws the falling bits. The functions after this one are the bits'
 * states: each field is 0x24 * index past its base offset. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0694(char *bit)
{
    int i;
    for (i = 0; i < 0x32; i++) {
        if (*(unsigned char *)(bit + 0x47ac)) {
            int x = *(int *)(bit + 0x478c) >> 0xc;
            int y = *(int *)(bit + 0x4790) >> 0xc;
            func_ov004_020af948(data_ov006_0213a5e0[*(unsigned char *)(bit + 0x47ad)], x, y, 0);
            DrawOamSprite(data_ov006_0213a5e0[*(unsigned char *)(bit + 0x47ae)], x, y, 0);
        }
        bit += 0x24;
    }
}
}

// @symbol func_ov006_020e071c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e071c(char *raw, int index)
{
    int off = index * 0x24;
    if (*(unsigned short *)(raw + 0x47a4 + off) != 0) {
        short *p = (short *)(raw + 0x47a4 + off);
        *p = (short)(*(unsigned short *)p - 1);
        if (*p < 0)
            *p = 0;
    } else if (*(int *)(raw + 0x4798 + off) > 0x100) {
        int *q = (int *)(raw + 0x4798 + off);
        *q = *q - 0x10;
        if ((short)*q < 0x100)
            *q = 0x100;
    } else {
        *(unsigned char *)(raw + 0x4000 + off + 0x7ab) = 0;
    }
}
}

// @symbol func_ov006_020e07b0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e07b0(char *raw, int index)
{
    int off = index * 0x24;
    if (*(int *)(raw + 0x479c + off) > *(int *)(raw + 0x4798 + off)) {
        *(int *)(raw + 0x4798 + off) += 0x10;
        if (*(int *)(raw + 0x479c + off) > *(int *)(raw + 0x4798 + off))
            *(int *)(raw + 0x4798 + off) = *(int *)(raw + 0x479c + off);
    }
    if (*(unsigned short *)(raw + 0x47a4 + off) != 0) {
        *(unsigned short *)(raw + 0x47a4 + off) = *(unsigned short *)(raw + 0x47a4 + off) - 1;
        if (*(short *)(raw + 0x47a4 + off) < 0) *(short *)(raw + 0x47a4 + off) = 0;
    } else {
        *(unsigned char *)(raw + off + 0x47ab) = 2;
        *(short *)(raw + 0x47a4 + off) = (short)(unsigned char)((((0x20 * (((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff)) >> 0xf)) + 0x20);
    }
}
}

// @symbol func_ov006_020e0884
extern "C" {
void func_ov006_020e0884(char* raw, int index) {
  int off = index * 0x24;
  unsigned int roll;
  *(int*)(raw + 0x4798 + off) = 0;
  roll = ((unsigned)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
  *(int*)(raw + 0x479c + off) = (((roll << 4) >> 15) << 4) + 0x300;
  *(unsigned char*)(raw + 0x47ab + off) = 1;
  roll = ((unsigned)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
  roll = ((roll << 5) >> 15) + 0x20;
  *(short*)(raw + 0x47a4 + off) = (unsigned char)roll;
}
}

// @symbol func_ov006_020e091c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e091c(char *raw, int index)
{
    int off = index * 0x24;
    char *vxs = raw + 0x4794;
    char *xs = raw + 0x478c;
    char *ys = raw + 0x4790;
    *(int*)(xs + off) = *(int*)(xs + off) + *(int*)(vxs + off);
    *(int*)(ys + off) = *(int*)(ys + off) + *(int*)(raw + off + 0x4798);
    if (*(u16*)(raw + off + 0x47a0) != 0) {
        char *timers = raw + 0x47a2;
        *(u16*)(timers + off) = *(u16*)(timers + off) - 1;
        if (*(s16*)(timers + off) < 0) *(s16*)(timers + off) = 0;
        return;
    }
    if (*(int*)(vxs + off) > 0) {
        *(int*)(vxs + off) = *(int*)(vxs + off) - 8;
        if ((s16)*(int*)(vxs + off) < 0) *(int*)(vxs + off) = 0;
        return;
    }
    if (*(int*)(vxs + off) < 0) {
        *(int*)(vxs + off) = *(int*)(vxs + off) + 8;
        if (*(int*)(vxs + off) > 0) *(int*)(vxs + off) = 0;
        return;
    }
    *(u8*)(raw + off + 0x47aa) = 0;
}
}

// @symbol func_ov006_020e0a24
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0a24(char *raw, int index)
{
    unsigned short timer;

    BIT_X(raw, index) = BIT_X(raw, index) + BIT_VX(raw, index);
    BIT_Y(raw, index) = BIT_Y(raw, index) + BIT_VY(raw, index);

    if (BIT_WAIT(raw, index) != 0) {
        BIT_WAIT(raw, index) = BIT_WAIT(raw, index) - 1;
        if (BIT_WAIT_S(raw, index) < 0)
            BIT_WAIT(raw, index) = 0;
        return;
    }

    if (BIT_VX(raw, index) > -0x300) {
        BIT_VX(raw, index) -= 8;
        if (BIT_VX(raw, index) <= -0x300)
            BIT_VX(raw, index) = 0x300;
    }

    timer = BIT_TIMER(raw, index);
    if (timer != 0) {
        BIT_TIMER(raw, index) = timer - 1;
        if (BIT_TIMER_S(raw, index) < 0)
            BIT_TIMER(raw, index) = 0;
        return;
    }

    BIT_STATE(raw, index) = 3;
    BIT_TIMER(raw, index) = (unsigned char)(((((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) << 5 >> 0xf) + 0x20);
}
}

// @symbol func_ov006_020e0b64
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0b64(char *raw, int index)
{
    int off = index * 0x24;
    unsigned short timer;

    *(int *)(raw + 0x478c + off) += *(int *)(raw + 0x4794 + off);
    *(int *)(raw + 0x4790 + off) += *(int *)(raw + 0x4798 + off);

    timer = *(unsigned short *)(raw + 0x47a0 + off);
    if (timer != 0) {
        *(short *)(raw + 0x47a0 + off) = timer - 1;
        if (*(short *)(raw + 0x47a0 + off) < 0)
            *(short *)(raw + 0x47a0 + off) = 0;
        return;
    }

    if (*(int *)(raw + 0x4794 + off) < 0x300) {
        *(int *)(raw + 0x4794 + off) += 8;
        if (*(int *)(raw + 0x4794 + off) >= 0x300)
            *(int *)(raw + 0x4794 + off) = 0x300;
    }

    timer = *(unsigned short *)(raw + 0x47a2 + off);
    if (timer != 0) {
        *(short *)(raw + 0x47a2 + off) = timer - 1;
        if (*(short *)(raw + 0x47a2 + off) < 0)
            *(short *)(raw + 0x47a2 + off) = 0;
        return;
    }

    *(char *)(raw + 0x47aa + off) = 3;
    *(short *)(raw + 0x47a2 + off) = (((((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) << 5) >> 0xf) + 0x20 & 0xff;
}
}

// @symbol func_ov006_020e0ca0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0ca0(char *raw, int index)
{
    int off = index * 0x24;
    if (*(unsigned short *)(raw + 0x47a0 + off) != 0) {
        *(unsigned short *)(raw + 0x47a0 + off) = *(unsigned short *)(raw + 0x47a0 + off) - 1;
        if (*(short *)(raw + 0x47a0 + off) < 0) *(short *)(raw + 0x47a0 + off) = 0;
        return;
    }
    *(int *)(raw + 0x4794 + off) = 0;
    *(unsigned char *)(raw + 0x47aa + off) = data_ov006_0212e450[(((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) << 1 >> 15];
    *(unsigned short *)(raw + 0x47a0 + off) = (short)(unsigned char)((0x10 * (((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff)) >> 0xf);
    *(unsigned short *)(raw + 0x47a2 + off) = (short)(unsigned char)(((0x40 * (((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff)) >> 0xf) + 0x60);
}
}

// @symbol func_ov006_020e0d84
extern "C" void func_ov006_020e0d84(char *raw, int index)
{
    C *self = (C *)raw;
    int off = index * 0x24;
    unsigned char state = *(unsigned char *)(raw + off + 0x47aa);
    (self->*data_ov006_02141930[state])(index);
    unsigned char state2 = *(unsigned char *)((char *)self + off + 0x47ab);
    (self->*data_ov006_021418d8[state2])(index);
}

// @symbol func_ov006_020e0e18
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0e18(char *raw, int index) {
    int off = index * 0x24;
    int *x = (int *)(raw + 0x478c + off);
    int *vx = (int *)(raw + 0x4794 + off);
    int *y = (int *)(raw + 0x4790 + off);
    *x += *vx;
    *y += *(int *)(raw + off + 0x4798);
    if (*vx > 0) {
        *vx -= 0x20;
        if ((int)(short)*vx < 0) *vx = 0;
    } else if (*vx < 0) {
        *vx += 0x20;
        if (*vx > 0) *vx = 0;
    } else {
        *(unsigned char *)(raw + off + 0x47aa) = 0;
    }
}
}

// @symbol func_ov006_020e0edc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0edc(char *raw, int index)
{
    char *bits = raw;
    int off = index * 0x24;

    *(int *)(bits + 0x478c + off) += *(int *)(bits + 0x4794 + off);
    *(int *)(bits + 0x4790 + off) +=
        *(int *)((char *)bits + off + 0x4798);

    u16 *wait = (u16 *)(bits + 0x47a0 + off);
    if (*wait != 0) {
        *wait = *wait - 1;
        s16 left = *(s16 *)wait;
        if (left < 0) {
            *wait = 0;
        }
        return;
    }

    int *vx = (int *)(bits + 0x4794 + off);
    if (*vx > -0x400) {
        *vx -= 0x20;
        if (*vx <= -0x400) {
            *vx = 0x400;
        }
    }

    u16 *timer = (u16 *)(bits + 0x47a2 + off);
    if (*timer != 0) {
        *timer = *timer - 1;
        s16 left = *(s16 *)timer;
        if (left < 0) {
            *timer = 0;
        }
        return;
    }

    *(unsigned char *)(bits + off + 0x47aa) = 3;
}
}

#pragma push
#pragma inline_depth(0)
// @symbol func_ov006_020e0ff0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e0ff0(void* raw, int index) {
    char* bits = (char*)raw;
    int off = index * 0x24;

    *(int*)(bits + 0x478c + off) += *(int*)(bits + 0x4794 + off);
    *(int*)(bits + 0x4790 + off) += *(int*)((char*)raw + off + 0x4798);

    unsigned short* wait = (unsigned short*)(bits + 0x47a0 + off);
    if (*wait != 0) {
        *wait = *wait - 1;
        short left = *(short*)wait;
        if (left < 0) {
            *wait = 0;
        }
        return;
    }

    int* vx = (int*)(bits + 0x4794 + off);
    if (*vx < 0x400) {
        *vx = *vx + 0x20;
        if (*vx >= 0x400) {
            *vx = 0x400;
        }
    }

    unsigned short* timer = (unsigned short*)(bits + 0x47a2 + off);
    if (*timer != 0) {
        *timer = *timer - 1;
        short left = *(short*)timer;
        if (left < 0) {
            *timer = 0;
        }
        return;
    }

    *(char*)(bits + off + 0x47aa) = 3;
}
#pragma pop
}

// @symbol func_ov006_020e1100
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1100(char *raw, int idx)
{
    int off = idx * 0x24;
    unsigned short wait;
    short left;
    unsigned int roll;
    wait = *(unsigned short *)(raw + 0x47a0 + off);
    if (wait != 0) {
        left = (short)(wait - 1);
        *(short *)(raw + 0x47a0 + off) = left;
        if (*(short *)(raw + 0x47a0 + off) < 0) *(short *)(raw + 0x47a0 + off) = 0;
        return;
    }
    *(int *)(raw + 0x4794 + off) = 0;
    roll = ((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
    *(int *)(raw + 0x4798 + off) = (int)(((roll << 5) >> 15) << 4) + 0x600;
    roll = ((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
    *(unsigned char *)(raw + 0x47aa + off) = data_ov006_0212e454[(roll << 1) >> 15];
    roll = ((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
    *(short *)(raw + 0x47a0 + off) = (unsigned char)((roll << 4) >> 15);
    roll = ((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff;
    *(short *)(raw + 0x47a2 + off) = (unsigned char)(((roll * 0x30) >> 15) + 0x30);
}
}

// @symbol func_ov006_020e1214
extern "C" void func_ov006_020e1214(char *raw, int idx)
{
    unsigned char state = BIT_STATE(raw, idx);
    (((C*)raw)->*data_ov006_021418f0[state].pmf)(idx);
}

#pragma push
#pragma opt_propagation off
// @symbol func_ov006_020e1264
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1264(char *raw, int idx) {
    unsigned roll = (unsigned)RandomIntInternal(&data_0209d4b8);
    int k = 0;
    unsigned pick = ((roll >> 16) & 0x7fff) << 3 >> 0xf;
    if (pick == 5) k = 1;
    char *bit = raw + idx * 0x24;
    *(unsigned char *)(bit + 0x47a9) = data_ov006_0212e460[k];
    *(unsigned char *)(bit + 0x47aa) = 0;
}
}
#pragma pop

// @symbol func_ov006_020e12d0
extern "C" void func_ov006_020e12d0(char *raw)
{
    int i;
    char *bit = raw;
    for (i = 0; i < 0x32; i++) {
        if (*(unsigned char *)(bit + 0x47a8) != 0) {
            unsigned char kind = *(unsigned char *)(bit + 0x47a9);
            (((C *)raw)->*data_ov006_021418c0[kind])(i);
            if ((*(int *)(bit + 0x4790) >> 0xc) >= 0xc8) {
                *(int *)(bit + 0x478c) = (((unsigned int)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) << 5 >> 0xf << 0xf;
                *(int *)(bit + 0x4790) = -0x8000;
                *(unsigned char *)(bit + 0x47aa) = 0;
                *(unsigned char *)(bit + 0x47a9) = 0;
                *(unsigned char *)(bit + 0x47ab) = 0;
            }
        }
        bit += 0x24;
    }
}

// @symbol func_ov006_020e13a4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e13a4(char *raw)
{
    int i;
    char *bit;
    unsigned int roll;
    unsigned int v;
    int q;
    unsigned int m;

    i = 0;
    bit = raw;
    for (; i < 0x32; i++)
    {
        *(int *)(bit + 0x478c) = 0;
        *(int *)(bit + 0x4790) = 0;
        *(int *)(bit + 0x4794) = 0;
        *(int *)(bit + 0x4798) = 0;
        *(short *)(bit + 0x47a0) = 0;
        *(short *)(bit + 0x47a2) = 0;
        *(short *)(bit + 0x47a4) = 0;
        *(char *)(bit + 0x47a8) = 0;
        *(char *)(bit + 0x47a9) = 0;
        *(char *)(bit + 0x47aa) = 0;
        *(char *)(bit + 0x47ab) = 0;
        *(char *)(bit + 0x47ac) = 0;
        *(char *)(bit + 0x47ad) = 0;
        *(char *)(bit + 0x47ae) = 1;
        bit += 0x24;
    }

    i = 0;
    bit = raw;
    for (; i < 0x32; i++)
    {
        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        m = ((roll >> 16) & 0x7fff) << 5;
        *(int *)(bit + 0x478c) = (int)((m >> 0xf)) << 0xf;
        *(int *)(bit + 0x4790) = -0x8000;
        *(char *)(bit + 0x47a8) = 1;
        *(char *)(bit + 0x47ac) = 1;
        *(char *)(bit + 0x47a9) = 0;
        *(char *)(bit + 0x47aa) = 0;

        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        *(char *)(bit + 0x47ad) = (char)(((roll >> 16) & 0x7fff) * 5 >> 0xf);

        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        v = *(unsigned char *)(bit + 0x47ad) + ((((roll >> 16) & 0x7fff) << 2) >> 0xf) + 1;
        v = v & 0xff;
        if (v >= 5)
            v = (v - 5) & 0xff;
        *(char *)(bit + 0x47ae) = (char)v;

        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        *(short *)(bit + 0x47a0) = (short)(((i & 7) << 6) + (((roll >> 16) & 0x7fff) * 0x30 >> 0xf));

        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        m = ((roll >> 16) & 0x7fff) << 5;
        *(int *)(bit + 0x478c) = (int)((m >> 0xf)) << 0xf;

        roll = (unsigned int)RandomIntInternal(&data_0209d4b8);
        q = (((roll >> 16) & 0x7fff) * 0x1a) >> 0xf;
        *(int *)(bit + 0x4790) = (((q << 3) - 8)) << 0xc;
        *(short *)(bit + 0x47a0) = 0;
        bit += 0x24;
    }
}
}

// @symbol func_ov006_020e1554
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1554(ScoreView *view)
{
    int i;
    for (i = 0; i < 5; i++) {
        if (view->popups[i].shown != 0) {
            func_ov004_020b2444(view->popups[i].x >> 12, view->popups[i].y >> 12, view->popups[i].points, -1, -1, 0, 0);
        }
    }
    if (view->unk_4ee8 != 0) {
        int r = func_ov004_020adbc0();
        func_ov004_020b2220(0x80, 0x60, r, 1, 0, 0x800, 0);
    }
}
}

#pragma push
// @symbol func_ov006_020e1608
extern "C" {  /* .c-derived member: C linkage for the whole block */
#pragma opt_strength_reduction off
void func_ov006_020e1608(char *raw) {
    int i;
    for (i = 0; i < 5; i++) {
        char *popup = raw + (i << 4);
        if (*(unsigned char*)(popup + 0x4748) == 0) continue;
        if (*(unsigned short*)(popup + 0x4746) == 0) continue;
        {
            volatile unsigned short *timer = (volatile unsigned short*)((unsigned int)popup + 0x4746);
            *timer = *timer - 1;
            if (*timer != 0) continue;
        }
        *(unsigned char*)(popup + 0x4749) = 1;
        Sound::PlayBank2_2D(0x1bc);
    }
}
}
#pragma pop

#pragma push
#pragma opt_strength_reduction off
// @symbol func_ov006_020e1680
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1680(char *raw)
{
    int slot = 0;
    int i, j;
    char *stone;
    u8 *delay;

    for (j = 0; j < 5; j++) {
        if (*(u8 *)(raw + j * 16 + 0x4748) == 0) {
            slot = j;
            break;
        }
    }

    delay = &data_ov006_0212e458[slot];
    stone = raw;
    for (i = 0; i < 5; i++, stone += 0x2c) {
        int dx, dz, dist, top;

        if (*(u8 *)(stone + 0x4689) == 0)
            continue;
        dx = *(int *)(stone + 0x4660) - *(int *)(raw + 0x4e94);
        dz = *(int *)(stone + 0x4664) - *(int *)(raw + 0x4e98);
        {
            int ax = dx >> 12;
            int az = dz >> 12;
            dist = cstd::sqrt((u64)(s64)(ax * ax + az * az));
        }
        *(u8 *)(raw + slot * 16 + 0x4748) = 1;
        *(int *)(raw + slot * 16 + 0x473c) = *(int *)(stone + 0x4660);
        {
        int *popupY = M(raw + slot * 16 + 0x4740);
        *popupY = *(int *)(stone + 0x4664) + 0x1000;
        *(short *)(raw + slot * 16 + 0x4746) = *delay;
        if (dist <= 8)
            *(short *)(raw + slot * 16 + 0x4744) = 1000;
        else if (dist <= 0x18)
            *(short *)(raw + slot * 16 + 0x4744) = 500;
        else if (dist <= 0x28)
            *(short *)(raw + slot * 16 + 0x4744) = 300;
        else if (dist <= 0x38)
            *(short *)(raw + slot * 16 + 0x4744) = 100;
        else
            *(short *)(raw + slot * 16 + 0x4744) = 0;
        top = *popupY >> 12;
        if (top >= -32 && top <= 8)
            *popupY = -0x28000;
        }
        delay++;
        slot++;
    }
}
}
#pragma pop

// @symbol func_ov006_020e17f8
/* Draws a sprite at the aimed stone's position (0x4eb0) once the stylus
 * handler has set 0x4ee5. */
extern "C" void func_ov006_020e17f8(char *raw)
{
  if(*(unsigned char*)(raw+0x4ee5)==0) return;
  int x=*(int*)(raw+0x4eb0);
  int y=*(int*)(raw+0x4eb4);
  func_ov004_020afdd0((int)data_ov006_0213c2e4,(x>>12)-0x20,(y>>12)-8,-1,0);
}

#pragma push
#pragma opt_common_subs off
// @symbol func_ov006_020e1854
/* Stylus handler for the stone being aimed. While the stylus is down, the
 * touch point plus the grab offset becomes the new position, clamped to the
 * 0x20000..0xe0000 by 0x94000..0xb8000 box; a move under two units is undone.
 * Otherwise it updates the swing tracking (0x4eea), turns the move into an
 * angle at 0x4ede clamped to the lower half turn and averaged with the last
 * one, and folds the move length into the speed at 0x4ec8. With the stylus
 * up, two flags are reset.
 *
 * Shapes that must stay: the touch records are read through a `u8 *` cast;
 * dx is computed before dy with no temporaries; the tail's index `j` is
 * wider than a byte; and the pragma keeps the 0x4eb0 and 0x4eb4 re-reads
 * after the clamps. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1854(void *arg)
{
    u8 *raw = (u8 *)arg;
    u8 idx;
    int dx, oldx, oldy;
    int diff;
    u16 ang;

    idx = data_020a0e40[0];
    if (data_020a0de8[idx * 4] != 0) {
        int dy2, dy;

        int i4 = idx * 4;
        u8 *pa = (u8 *)data_020a0dea;
        u8 *pb = (u8 *)data_020a0deb;
        u8 bx = pa[i4];
        u8 by = pb[i4];

        oldx = *(int *)(raw + 0x4eb0);
        oldy = *(int *)(raw + 0x4eb4);
        *(int *)(raw + 0x4eb0) = (bx << 12) + *(int *)(raw + 0x4ec0);
        *(int *)(raw + 0x4eb4) = *(int *)(raw + 0x4ec4) + (by << 12);

        if (*(int *)(raw + 0x4eb4) <= 0x94000)
            *(int *)(raw + 0x4eb4) = 0x94000;
        if (*(int *)(raw + 0x4eb0) <= 0x20000)
            *(int *)(raw + 0x4eb0) = 0x20000;
        if (*(int *)(raw + 0x4eb0) >= 0xe0000)
            *(int *)(raw + 0x4eb0) = 0xe0000;
        if (*(int *)(raw + 0x4eb4) >= 0xb8000)
            *(int *)(raw + 0x4eb4) = 0xb8000;


        dx = (*(int *)(raw + 0x4eb0) - oldx) >> 12;
        dy = (*(int *)(raw + 0x4eb4) - oldy) >> 12;
        dy2 = dy * dy;

        if (cstd::sqrt((s64)(dx * dx + dy2)) <= 1) {
            *(int *)(raw + 0x4eb0) = oldx;
            *(int *)(raw + 0x4eb4) = oldy;
            return;
        }

        diff = (*(int *)(raw + 0x4eb4) - *(int *)(raw + 0x4ebc)) >> 12;
        if (*(u8 *)(raw + 0x4eea) == 0) {
            func_02012718(0x1d6, *(int *)(raw + 0x4eb0));
            *(u8 *)(raw + 0x4eea) = 2;
            *(int *)(raw + 0x4ed4) = (*(int *)(raw + 0x4eb4) - *(int *)(raw + 0x4ebc)) >> 12;
            *(int *)(raw + 0x4ebc) = *(int *)(raw + 0x4eb4);
        } else if (*(u8 *)(raw + 0x4eea) == 1) {
            if (*(int *)(raw + 0x4ed4) * diff > 0) {
                if (diff < 0)
                    diff = -diff;
                if (diff >= 0xa)
                    *(u8 *)(raw + 0x4eea) = 0;
            } else {
                *(int *)(raw + 0x4ed4) = diff;
                *(int *)(raw + 0x4ebc) = *(int *)(raw + 0x4eb4);
            }
        } else {
            if (*(int *)(raw + 0x4ed4) * diff < 0)
                *(u8 *)(raw + 0x4eea) = 1;
            *(int *)(raw + 0x4ed4) = (*(int *)(raw + 0x4eb4) - *(int *)(raw + 0x4ebc)) >> 12;
            *(int *)(raw + 0x4ebc) = *(int *)(raw + 0x4eb4);
        }

        ang = *(u16 *)(raw + 0x4ede);
        *(u16 *)(raw + 0x4ede) = _ZN4cstd5atan2E5Fix12IiES1_(dy, dx >> 1);
        {
            u16 a = *(u16 *)(raw + 0x4ede);
            if (a <= 0x8000) {
                if (a >= 0x4000) {
                    *(u16 *)(raw + 0x4ede) = 0x8000;
                    goto ang_done;
                }
            }
            if (a <= 0x4000)
                *(u16 *)(raw + 0x4ede) = 0;
        }
    ang_done:;

        {
            int mag;
            *(u16 *)(raw + 0x4ede) = (u16)((*(u16 *)(raw + 0x4ede) + ang) >> 1);
            mag = cstd::sqrt((s64)((dx >> 1) * (dx >> 1) + dy2)) * 9;
            mag = (mag << 12) >> 4;
            if (mag >= 0xc000)
                mag = 0xc000;
            if (mag > *(int *)(raw + 0x4ec8))
                *(int *)(raw + 0x4ec8) = mag;
            {
                int cur = *(int *)(raw + 0x4ec8);
                if (cur > mag) {
                    *(int *)(raw + 0x4ec8) = *(int *)(raw + 0x4ec8) - ((cur - mag) >> 1);
                }
            }
        }


        {
            int px = *(int *)(raw + 0x4eb0);
            int py = *(int *)(raw + 0x4eb4);
            int j = data_020a0e40[0];
            u8 jx = ((u8 *)data_020a0dea)[j * 4];
            int ax = (px >> 12) - jx;
            u8 jy = ((u8 *)data_020a0deb)[j * 4];
            int ay = (py >> 12) - jy;
            *(int *)(raw + 0x4ec0) = ax << 12;
            *(int *)(raw + 0x4ec4) = ay << 12;
        }

        return;
    }

    *(u8 *)(raw + 0x4ee4) = 0;
    *(u8 *)(raw + 0x4ee5) = 1;
}
}
#pragma pop

// @symbol func_ov006_020e1b54
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1b54(char *raw)
{
  int idx;
  int touching = 0;
  int x;
  int y;
  idx = data_020a0e40[0];
  if (data_020a0de8[idx * (4 & 0xFFFFFFFF)] != 0)
  {
    if (data_020a0de9[idx * 4] != 0)
    {
      touching = 1;
    }
  }
  if (touching == 0)
  {
    return;
  }
  x = ((*((int *) (raw + 0x4eb0))) >> 0xc) - ((u8 *)data_020a0dea)[idx * 4];
  y = ((*((int *) (raw + 0x4eb4))) >> 0xc) - ((u8 *)data_020a0deb)[idx * 4];
  *((int *) (raw + 0x4ec0)) = x << 0xc;
  *((int *) (raw + 0x4ec4)) = y << 0xc;
  *((u8 *) (raw + 0x4ee4)) = 1;
  *((u16 *) (raw + 0x4ede)) = 0xc000;
  if ((*((u8 *) (raw + 0x4ee9))) == 0)
  {
    func_02012718(0x1d2, *((int *) (raw + 0x4eb0)));
    *((u8 *) (raw + 0x4ee9)) = 6;
  }
  *((int *) (raw + 0x4ecc)) = 0;
  *((int *) (raw + 0x4ed0)) = 0;
  *((int *) (raw + 0x4eb8)) = (*((int *) (raw + 0x4eb0))) + (*((int *) (raw + 0x4ec0)));
  *((int *) (raw + 0x4ebc)) = (*((int *) (raw + 0x4eb4))) + (*((int *) (raw + 0x4ec4)));
  *((int *) (raw + 0x4ed4)) = 0xff;
  *((u8 *) (raw + 0x4eea)) = 0;
}
}

// @symbol func_ov006_020e1c68
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e1c68(char* raw) {
    int x, y;
    int i;
    char* stone;
    int remaining;
    int j;
    stone = raw;
    for (i = 0; i < 5; i++) {
        if (*(unsigned char*)(stone + 0x4689) == 0) goto next;
        if (*(unsigned char*)(stone + 0x468a) == 0) goto next;
        x = *(int*)(stone + 0x4660) >> 12;
        y = *(int*)(stone + 0x4664) >> 12;
        RenderOamBothScreens(&data_ov006_0213c264, x, y, -1, 1, 0);
        RenderOamBothScreens(&data_ov006_0213c2ac, x, y + 8, -1, 2, 0);
    next:
        stone += 0x2c;
    }
    remaining = 5 - *(unsigned char*)(raw + 0x4ee6);
    if (remaining < 0) remaining = 0;
    j = 0;
    if (remaining > 0) {
        for (; j < remaining; j++) {
            int slotX = data_ov006_0212e468[j];
            RenderOamBothScreens(&data_ov006_0213c264, slotX, 0xb0, -1, 1, 0);
            RenderOamBothScreens(&data_ov006_0213c2ac, slotX, 0xb8, -1, 2, 0);
        }
    }
}
}

// @symbol func_ov006_020e1dc8
/* Stone separation. Stone idx has just moved: the first other active stone
 * within 24 units is pushed out to 26 units along the line between them,
 * then that stone gets the same check once against the rest (with a bump
 * sound). Only one push each way per call.
 *
 * The rotated offsets must be named (vx, vy), and the inner scan needs its
 * own dx, dy, dist and ang; sharing the outer ones changes the registers. */
extern "C" void func_ov006_020e1dc8(dScMgCurling_c *self, int idx)
{
    int i;
    int j;
    int dx;
    int dy;
    int dist;
    u16 ang;
    int k;

    for (i = 0; i < 5; i++) {
        if (self->mStone[i].active == 0) continue;
        if (idx == i) continue;
        dx = (self->mStone[i].x - self->mStone[idx].x) >> 12;
        dy = (self->mStone[i].y - self->mStone[idx].y) >> 12;
        dist = cstd::sqrt((u64)(dx * dx + dy * dy));
        ang = _ZN4cstd5atan2E5Fix12IiES1_(dy, dx);
        if (dist > 0x18) continue;
        {
            int cs;
            int sn;
            int vx;
            int vy;

            k = (ang >> 4) * 2;
            cs = data_02082214[k + 1];
            vx = (int)(((long long)cs * 0x1a + 0x800) >> 12);
            self->mStone[i].x = self->mStone[idx].x + (vx << 12);
            sn = data_02082214[k];
            vy = (int)(((long long)sn * 0x1a + 0x800) >> 12);
            self->mStone[i].y = self->mStone[idx].y + (vy << 12);
            for (j = 0; j < 5; j++) {
                int dx2;
                int dy2;
                int dist2;
                u16 ang2;

                if (self->mStone[j].active == 0) continue;
                if (i == j) continue;
                dx2 = (self->mStone[j].x - self->mStone[i].x) >> 12;
                dy2 = (self->mStone[j].y - self->mStone[i].y) >> 12;
                dist2 = cstd::sqrt((u64)(dx2 * dx2 + dy2 * dy2));
                ang2 = _ZN4cstd5atan2E5Fix12IiES1_(dy2, dx2);
                if (dist2 > 0x18) continue;
                {
                    int cs2;
                    int sn2;
                    int vx2;
                    int vy2;

                    k = (ang2 >> 4) * 2;
                    cs2 = data_02082214[k + 1];
                    vx2 = (int)(((long long)cs2 * 0x1a + 0x800) >> 12);
                    self->mStone[j].x = self->mStone[i].x + (vx2 << 12);
                    sn2 = data_02082214[k];
                    vy2 = (int)(((long long)sn2 * 0x1a + 0x800) >> 12);
                    self->mStone[j].y = self->mStone[i].y + (vy2 << 12);
                    func_02012718(0xe8, self->mStone[idx].x);
                    return;
                }
            }
            return;
        }
    }
}

// @symbol func_ov006_020e20bc
/* Stone idx has just moved, so find the first other stone it overlaps and
 * resolve the collision. The two exchange their velocity components along
 * the line between their centres and keep the components across it; the
 * moving stone is then pushed back to one stone width (0x1b000) along that
 * line and clamped to the board, with any overshoot passed on to the stone
 * it hit. Both stones end up moving, the hit stone is flagged fast above
 * 0x3800, and the knock sound plays panned to the x.
 *
 * Shapes that must stay: the contact angle is negated right after atan2,
 * before the velocities are read; the three fields written after a call
 * are reached through pointers taken beside each stone's reads, which puts
 * their addresses in the frame ahead of the temporaries; and the moving
 * stone's new x velocity reuses the outer dx. */
extern "C" void func_ov006_020e20bc(dScMgCurling_c *self, int idx)
{
    int i;
    int dx;
    int dy;

    for (i = 0; i < 5; i++) {
        if (self->mStone[i].active == 0) continue;
        if (idx == i) continue;
        if (self->mStone[i].state == 0) continue;
        if (self->mStone[i].state == 3) continue;
        dx = (self->mStone[i].x - self->mStone[idx].x) >> 12;
        dy = (self->mStone[i].y - self->mStone[idx].y) >> 12;
        if (cstd::sqrt((long long)(dx * dx + dy * dy)) > 0x18) continue;
        {
            u16 *pAngle;
            s32 *pSpeed;
            u16 *pHitAngle;
            int mTangent;
            int k;
            int cosA;
            int vmy;
            int sinA;
            u16 contact;
            int hTangent;
            int cosN;
            int hvx;
            u16 rel;
            int xi;
            int sinN;
            int mNormal;
            int yi;
            int vmx;
            int hNormal;
            int vhx;
            int mvy;
            int hvy;
            int vhy;

            /* Contact line, from the hit stone to the moving one. */
            dx = self->mStone[idx].x - self->mStone[i].x;
            dy = self->mStone[idx].y - self->mStone[i].y;
            contact = _ZN4cstd5atan2E5Fix12IiES1_(dy, dx);
            rel = -contact;

            /* Both velocities as x/y. A hit stone at rest takes half the
             * moving stone's y so the exchange below cannot stall. */
            pAngle = &self->mStone[idx].angle;
            pSpeed = &self->mStone[idx].speed;
            k = (self->mStone[idx].angle >> 4) * 2;
            vmx = FMUL(data_02082214[k + 1], self->mStone[idx].speed);
            vmy = FMUL(data_02082214[k], self->mStone[idx].speed);
            pHitAngle = &self->mStone[i].angle;
            k = (self->mStone[i].angle >> 4) * 2;
            vhx = FMUL(data_02082214[k + 1], self->mStone[i].speed);
            vhy = FMUL(data_02082214[k], self->mStone[i].speed);
            if (vhy == 0) vhy = vmy >> 1;

            /* sin/cos of -contact rotate into the contact frame; sin/cos of
             * +contact rotate back out. */
            k = (rel >> 4) * 2;
            sinN = data_02082214[k];
            cosN = data_02082214[k + 1];
            rel = -rel;
            k = (rel >> 4) * 2;
            sinA = data_02082214[k];
            cosA = data_02082214[k + 1];

            /* Normal and tangential components of each velocity. */
            mNormal = FMUL(cosN, vmx);
            mNormal -= FMUL(sinN, vmy);
            mTangent = FMUL(sinN, vmx) + FMUL(cosN, vmy);
            hNormal = FMUL(cosN, vhx) - FMUL(sinN, vhy);
            hTangent = FMUL(sinN, vhx) + FMUL(cosN, vhy);

            /* Swap the normal components and rotate back: dx/mvy for the
             * moving stone, hvx/hvy for the one it hit. */
            dx = FMUL(cosA, hNormal) - FMUL(sinA, mTangent);
            mvy = FMUL(sinA, hNormal) + FMUL(cosA, mTangent);
            hvx = FMUL(cosA, mNormal) - FMUL(sinA, hTangent);
            hvy = FMUL(sinA, mNormal) + FMUL(cosA, hTangent);

            *pAngle = _ZN4cstd5atan2E5Fix12IiES1_(mvy, dx);
            *pSpeed = cstd::sqrt((u64)((long long)dx * dx + (long long)mvy * mvy));

            /* Separate the stones along the contact line, then keep the
             * moving one on the board and hand any overshoot to the other. */
            self->mStone[idx].x = self->mStone[i].x + FMUL(cosA, 0x1b000);
            self->mStone[idx].y = self->mStone[i].y + FMUL(sinA, 0x1b000);
            xi = self->mStone[idx].x >> 12;
            yi = self->mStone[idx].y >> 12;
            if (xi - 0xc < 0) {
                xi = self->mStone[idx].x - 0xc000;
                self->mStone[i].x += xi;
                self->mStone[idx].x = 0xc000;
            }
            if (xi + 0xc > 0x100) {
                self->mStone[i].x += self->mStone[idx].x - 0xf4000;
                self->mStone[idx].x = 0xf4000;
            }
            if (yi - 0xc < -0xe0) {
                self->mStone[i].y += self->mStone[idx].y + 0xd4000;
                self->mStone[idx].y = -0xd4000;
            }

            *pHitAngle = _ZN4cstd5atan2E5Fix12IiES1_(hvy, hvx);
            self->mStone[i].speed = cstd::sqrt((u64)((long long)hvx * hvx + (long long)hvy * hvy));
            self->mStone[idx].state = 1;
            self->mStone[i].state = 1;
            if (self->mStone[i].speed >= 0x3800) {
                self->mStone[i].fast = 1;
            } else {
                self->mStone[i].fast = 0;
            }
            func_02012718(0xe8, self->mStone[idx].x);
            return;
        }
    }
}

// @symbol func_ov006_020e269c
/* Sets stone i's spin (0x4682) from the x component of its velocity:
 * -(cos(angle) * speed) / 4. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e269c(char *c, int i)
{
    char *o = c + i * 0x2c;
    int h = *(unsigned short *)(o + 0x4686);
    int idx = (((h >> 4) << 1) + 1) << 1;
    int s = *(short *)((char *)data_02082214 + idx);
    int v = *(int *)(o + 0x4668);
    long long m = (long long)s * v;
    int hi = (int)(((unsigned long long)(m + 0x800)) >> 12);
    *(short *)(o + 0x4682) = (short)((-hi) >> 2);
}
}

// @symbol func_ov006_020e26f8
/* Stone i while it is being dragged. With the stylus down the stone
 * follows the touch point plus the grab offset (0x4674/0x4678), its x kept
 * between 0xe and 0xf2 units, and the offset is recomputed; with the stylus
 * up the drag flag clears and, if the aimed stone (0x4eb0) overlaps it, the
 * aimed stone is moved just below it. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e26f8(DragView *w, int i)
{
    unsigned char idx = data_020a0e40[0];
    if (((struct B4 *)data_020a0de8)[idx].v) {
        int t, mm, nn;
        w->stone[i].x = w->stone[i].grabX + (data_020a0dea[idx].v << 12);
        t = w->stone[i].x >> 12;
        if (t < 0xe) w->stone[i].x = 0xe000;
        if (t > 0xf2) w->stone[i].x = 0xf2000;
        mm = (w->stone[i].x >> 12) - data_020a0dea[data_020a0e40[0]].v;
        nn = (w->stone[i].y >> 12) - data_020a0deb[idx].v;
        w->stone[i].grabX = mm << 12;
        w->stone[i].grabY = nn << 12;
    } else {
        int dx, dy;
        w->stone[i].dragging = 0;
        dx = (w->aimX - w->stone[i].x) >> 12;
        dy = (w->aimY - w->stone[i].y) >> 12;
        if (dx < -0x2e) return;
        if (dx > 0x2e) return;
        if (dy < -0x14) return;
        if (dy <= 0x14) w->aimY = w->stone[i].y + 0x15000;
    }
}
}

// @symbol func_ov006_020e285c
/* A tail-call veneer to func_ov006_020e20bc. */
extern "C" void func_ov006_020e285c(dScMgCurling_c *self, int idx)
{
    func_ov006_020e20bc(self, idx);
}

// @symbol func_ov006_020e2868
/* Stone idx while it slides. Moves it by its velocity, adds the spin to
 * the angle, bounces it off the four walls (with a sound), then takes
 * friction off the speed: a base of speed/512 (at least 0x1c), more in the
 * five bands below the house from the data_ov006_0212e478 tables, and a
 * steeper rate while the stone still travels upward above -0x20. At zero
 * speed the stone stops (state 2). The collision and spin helpers run, and
 * the slide sound at 0x467c is retuned to the speed. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e2868(char *c, int idx)
{
    int m;
    int zi;
    u16 *pang;
    int *p668;
    int *p660;
    int *p664;
    int x;
    int z;
    int xi;
    int v;
    int sbv;
    int i2;
    int s;
    int p;
    int w;
    int *pd;
    int sn;
    int cs;

    m = idx * 0x2c;

    pang = (u16 *)(c + 0x4686 + m);
    p668 = (int *)(c + 0x4668 + m);
    p660 = (int *)(c + 0x4660 + m);
    p664 = (int *)(c + 0x4664 + m);

    sn = data_02082214[((*pang) >> 4) * 2 + 1];
    *p660 += (int)(((long long)sn * *p668 + 0x800) >> 12);
    cs = data_02082214[((*pang) >> 4) * 2];
    *p664 += (int)(((long long)cs * *p668 + 0x800) >> 12);
    *(u16 *)(c + 0x4684 + m) += *(u16 *)(c + m + 0x4682);

    x = *p660;
    z = *p664;
    xi = x >> 12;
    zi = z >> 12;

    if (xi + 0xc >= 0x100) {
        *pang = 0x8000 - *pang;
        *p660 = 0xf4000;
        func_02012718(0x1d4, *p660);
    } else if (xi - 0xc < 0) {
        *pang = 0x8000 - *pang;
        *p660 = 0xc000;
        func_02012718(0x1d4, *p660);
    }

    if (zi + 0xc > 0xc0) {
        *(u16 *)(c + 0x4686 + m) = -*(u16 *)(c + 0x4686 + m);
        *p664 = 0xb4000;
        func_02012718(0x1d4, *p660);
    } else if (zi - 0xc < -0xe0) {
        *(u16 *)(c + 0x4686 + m) = -*(u16 *)(c + 0x4686 + m);
        *p664 = -0xd4000;
        func_02012718(0x1d4, *p660);
    }

    zi = *p668;
    v = zi >> 9;
    if (v <= 0x1c)
        v = 0x1c;
    for (i2 = 0, sbv = *p664 >> 12; i2 < 5; i2++) {
        if (sbv <= -(data_ov006_0212e478[i2] + 0x20)) {
            s = data_02082214[(*pang >> 4) * 2];
            if (s < 0) {
                v = zi >> data_ov006_0212e48c[i2];
                if (v < data_ov006_0212e4b4[i2])
                    v = data_ov006_0212e4b4[i2];
            } else if (s > 0) {
                v = zi >> data_ov006_0212e4a0[i2];
                if (v < data_ov006_0212e4c8[i2])
                    v = data_ov006_0212e4c8[i2];
            }
            break;
        }
    }

    if (sbv > -0x20) {
        s = data_02082214[(*pang >> 4) * 2];
        if (s > 0) {
            v = zi >> 3;
            if (v <= 0x180)
                v = 0x180;
        }
    }

    *(int *)(c + 0x4668 + m) -= v;
    pd = (int *)(c + 0x4668 + m);
    if (*(int *)(c + 0x4668 + m) <= 0) {
        *p668 = 0;
        *(u8 *)(c + idx * 0x2c + 0x4688) = 2;
    }

    func_ov006_020e20bc((dScMgCurling_c *)c, idx);
    func_ov006_020e269c(c, idx);

    v = *pd;
    w = -0xfa - ((0xc0 - (v >> 8)) * -0xfa) / 0xc0;
    p = v >> 7;
    if (p >= 0x7f)
        p = 0x7f;
    *(int *)(c + 0x467c + m) = func_02012468(*(int *)(c + 0x467c + m), 2, 0xe7, 7, p, w, func_020126e8(*p660), 0);
}
}

// @symbol func_ov006_020e2c08
/* Releases stone idx if the aimed stone (0x4eb0) overlaps it: it takes the
 * aim angle (0x4ede, clamped to the lower half turn) and the swing speed
 * (0x4ec8), is flagged fast above 0x3800, and starts sliding. The aim is
 * then put away, the stones are separated, and the throw sound plays at a
 * volume that depends on the fast flag. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e2c08(char *self, int idx)
{
    int n, v, w, vol;

    if (*(u8 *)(self + 0x4ee4) != 1) return;

    n = idx * 0x2c;
    v = (*(int *)(self + 0x4eb0) - *(int *)(self + 0x4660 + n)) >> 12;
    w = (*(int *)(self + 0x4eb4) - *(int *)(self + n + 0x4664)) >> 12;
    if (v < -0x2e) return;
    if (v > 0x2e) return;
    if (w < -0x14) return;
    if (w > 0x14) return;

    *(u8 *)(self + n + 0x4688) = 1;
    *(u16 *)(self + 0x4686 + n) = *(u16 *)(self + 0x4ede);
    *(int *)(self + n + 0x4668) = *(int *)(self + 0x4ec8);

    if (*(u16 *)(self + 0x4ede) < 0x9800u || *(u16 *)(self + 0x4ede) > 0xe800u) {
        if (*(u16 *)(self + 0x4ede) >= 0x4000u && *(u16 *)(self + 0x4ede) <= 0x9800u) {
            *(u16 *)(self + 0x4ede) = 0x9800;
        } else {
            *(u16 *)(self + 0x4ede) = 0xe800;
        }
        *(u16 *)(self + 0x4686 + n) = *(u16 *)(self + 0x4ede);
    }

    if (*(int *)(self + 0x4ec8) >= 0x3800) {
        *(u8 *)(self + 0x468b + n) = 1;
    } else {
        *(u8 *)(self + 0x468b + n) = 0;
    }

    *(u8 *)(self + 0x4ee7) = 1;
    *(u16 *)(self + 0x4ee0) = 0;
    func_ov006_020e1dc8((dScMgCurling_c *)self, idx);

    vol = 0x7f;
    if (*(u8 *)(self + 0x468b + n) == 0) vol = 0x3f;
    func_020126ac(0x1d3, 5, vol, 0, func_020126e8(*(int *)(self + 0x4660 + n)));
}
}

// @symbol func_ov006_020e2dbc
/* Brings out the next stone once the 0x4ee0 delay has run down: it starts
 * active at (0x80000, 0x80000), the stone count at 0x4ee6 goes up (with a
 * sound from the second stone on), and the aim point resets. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e2dbc(char *c)
{
    if (*(unsigned char *)(c + 0x4ee7) == 0) return;
    if (*(unsigned short *)(c + 0x4ee0) != 0)
    {
        *(unsigned short *)(((int)c + 0x4ee0)) -= 1;
        if ((short)*(unsigned short *)(c + 0x4ee0) <= 0)
            *(unsigned short *)(c + 0x4ee0) = 0;
        return;
    }
    *(unsigned char *)(c + 0x4ee7) = 0;
    {
        int idx = *(unsigned char *)(c + 0x4ee6);
        if (idx >= 5)
            return;
        {
            char *b = c + idx * 0x2c;
            *(unsigned char *)(b + 0x4689) = 1;
            *(unsigned char *)(b + 0x468a) = 1;
            *(int *)(b + 0x4660) = 0x80000;
            *(int *)(b + 0x4664) = 0x80000;
            *(unsigned short *)(b + 0x4680) = 0;
            *(unsigned char *)(b + 0x468b) = 0;
        }
    }
    if (*(unsigned char *)(c + 0x4ee6) != 0)
        Sound::PlayBank2_2D(0x1d7);
    (*(unsigned char *)(((int)c + 0x4ee6)))++;
    *(int *)(c + 0x4eb0) = 0x80000;
    *(int *)(c + 0x4eb4) = 0xb0000;
    *(unsigned char *)(c + 0x4ee4) = 0;
    *(unsigned char *)(c + 0x4ee5) = 1;
}
}

// @symbol func_ov006_020e2eb8
/* The scene's idle state. */
extern "C" void func_ov006_020e2eb8(void)
{
}

// @symbol func_ov006_020e2ebc
/* The scene's end state. After the 0x4ee2 delay it asks the base whether
 * the game is over, moves to state 4 with the base's own end state, and
 * clears the stones' visible flags. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e2ebc(char *thiz)
{
    int i;
    char *p;
    if (*(unsigned short *)(thiz + 0x4ee2) != 0) {
        *(unsigned short *)(((int)thiz + 0x4ee2)) -= 1;
        if (*(short *)(thiz + 0x4ee2) <= 0)
            *(unsigned short *)(thiz + 0x4ee2) = 0;
        return;
    }
    if (func_ov004_020adbe0() != 0) {
        *(unsigned char *)(thiz + 0x4000 + 0xee8) = 0;
        *(int *)(thiz + 0x4000 + 0xeac) = 4;
        func_ov004_020b0a54(0x10);
    } else {
        *(int *)(thiz + 0x4000 + 0xeac) = 4;
        func_ov004_020b0a54(0x10);
    }
    *(unsigned char *)(thiz + 0xc3) = 0;
    *(unsigned char *)(thiz + 0x4000 + 0xee5) = 0;
    p = thiz;
    for (i = 0; i < 5; i++) {
        *(unsigned char *)(p + 0x4000 + 0x68a) = 0;
        p += 0x2c;
    }
}
}

// @symbol func_ov006_020e2f78
/* The scoring state. Runs the popup countdowns, then after the 0x4ee2
 * delay either starts the next stone (state 1) or, once all five have been
 * thrown, ends the round (state 3) with the popups' points summed into the
 * score. The popups are cleared either way.
 *
 * The five score popups at 0x473c are spelled here with the u16 at +0xa
 * and the bytes at +0xc and +0xd as fields, which is how this function
 * clears them; ScorePopup above keeps func_ov006_020e1554's spelling. */
typedef struct {
    int x;
    int y;
    unsigned short points;
    unsigned short countdown;
    unsigned char shown;
    unsigned char done;
    unsigned char pad[2];
} PopupSlot;

typedef struct {
    char pad0[0x473c];
    PopupSlot slot[5];              /* 0x473c .. 0x478b */
    char pad1[0x4eac - 0x478c];
    int state;                      /* 0x4eac */
    char pad2[0x4ee2 - 0x4eb0];
    unsigned short delay;           /* 0x4ee2 */
    char pad3[0x4ee6 - 0x4ee4];
    unsigned char thrown;           /* 0x4ee6 */
    char pad4;                      /* 0x4ee7 */
    unsigned char roundOver;        /* 0x4ee8 */
} PopupView;

extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e2f78(char *c)
{
    PopupView *t = (PopupView *)c;
    int i;
    int sum;

    func_ov006_020e1608(c);

    if (t->delay != 0)
    {
        *(unsigned short *)(((int)c + 0x4ee2)) -= 1;
        if ((short)t->delay <= 0)
            t->delay = 0;
        return;
    }

    t->state = 1;
    if (t->thrown >= 5)
    {
        t->state = 3;
        t->delay = 0x80;
        t->roundOver = 1;
        sum = 0;
        Sound::PlayBank2_2D(0x1bc);
        for (i = 0; i < 5; i++)
            sum += t->slot[i].points;
        func_ov004_020adb1c(sum);
    }

    for (i = 0; i < 5; i++)
    {
        t->slot[i].x = 0;
        t->slot[i].y = 0;
        t->slot[i].points = 0;
        t->slot[i].countdown = 0;
        t->slot[i].done = 0;
        t->slot[i].shown = 0;
    }

    func_ov006_020e2dbc(c);
}
}

// @symbol func_ov006_020e3078
/* The playing state. After the 0x4ee2 delay it shows the HUD once, runs
 * the aim state (0x4ee4) through its table, then every active stone's own
 * state through the other table, remembering the last position of each.
 * When no stone is still moving, the popups are scored and the scene moves
 * to state 2 with a 0xc0 delay. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e3078(char *c)
{
    if (*(u16 *)(c + 0x4ee2) != 0) {
        u16 *q = (u16 *)(c + 0x4ee2);
        *q = *q - 1;
        return;
    }
    if (*(u8 *)(c + 0xc4) == 0) {
        *(u8 *)(c + 0xc3) = 1;
        *(u8 *)(c + 0xc4) = 1;
        *(u16 *)(c + 0xc0) = 0;
    }
    if (*(u8 *)(c + 0x4ee9) != 0) {
        u8 *q = (u8 *)(c + 0x4ee9);
        *q = *q - 1;
    }
    (((C *)c)->*data_ov006_021418b0[*(u8 *)(c + 0x4ee4)])();

    {
        int count = 0;
        int i = 0;
        char *p = c;
        for (; i < 5; i++, p += 0x2c) {
            if (*(u8 *)(p + 0x4689) != 0) {
                *(int *)(p + 0x466c) = *(int *)(p + 0x4660);
                *(int *)(p + 0x4670) = *(int *)(p + 0x4664);
                if (*(u16 *)(p + 0x4680) != 0) {
                    u16 *q = (u16 *)(p + 0x4680);
                    *q = *q - 1;
                }
                (((C *)c)->*data_ov006_02141910[*(u8 *)(p + 0x4688)])(i);
                if (*(u8 *)(p + 0x4688) != 2) count++;
            }
        }
        if (count != 0) return;
    }
    *(int *)(c + 0x4eac) = 2;
    func_ov006_020e1680(c);
    *(u16 *)(c + 0x4ee2) = 0xc0;
}
}

// @symbol func_ov006_020e3210
/* Starts a round: clears the stones, brings out the first one and enters
 * the aiming state. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e3210(char *c)
{
    func_ov006_020e3388(c);
    *(unsigned char *)(c + 0x4ee7) = 1;
    *(short *)(c + 0x4ee0) = 0;
    func_ov006_020e2dbc(c);
    *(int *)(c + 0x4eac) = 1;
}
}

// @symbol func_ov006_020e3250
/* Picks the house (0x4e9c): a fresh random one of three that differs from
 * the last, or the first one when the slot still holds its 0xff reset.
 * The house centre comes from data_ov006_0212e4dc and its background from
 * one of three files. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e3250(char *c)
{
    int m;
    void *file;
    if (*(int *)(c + 0x4e9c) == 0xff) {
        *(int *)(c + 0x4e9c) = 0;
    } else {
        m = (((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 3 >> 15;
        if (*(int *)(c + 0x4e9c) == m) {
            m += ((((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 2 >> 15) + 1;
            if (m >= 3) m -= 3;
        }
        *(int *)(c + 0x4e9c) = m;
    }
    *(int *)(c + 0x4e94) = data_ov006_0212e4dc[*(int *)(c + 0x4e9c) * 2] << 12;
    *(int *)(c + 0x4e98) = (data_ov006_0212e4dc[*(int *)(c + 0x4e9c) * 2 + 1] - 0xe0) << 12;
    if (*(int *)(c + 0x4e9c) == 0) {
        file = LoadFile(0x30);
    } else if (*(int *)(c + 0x4e9c) == 1) {
        file = LoadFile(0x2d);
    } else if (*(int *)(c + 0x4e9c) == 2) {
        file = LoadFile(0x31);
    }
    func_020563d4(file, 0, 0x800);
    Deallocate(file);
}
}

// @symbol func_ov006_020e3378
/* Marks the house as not chosen yet. */
extern "C" void func_ov006_020e3378(char *p)
{
    *(int *)(p + 0x4e9c) = 255;
}

#pragma push
#pragma opt_strength_reduction off
// @symbol func_ov006_020e3388
/* Zeroes the five stones, the five score popups and the aim fields, and
 * resets the score. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov006_020e3388(char *raw)
{
    int i;
    char *c = raw;
    char *r = c;
    for (i = 0; i < 5; i++) {
        *(int *)(r + 0x4660) = 0;
        *(int *)(r + 0x4664) = 0;
        *(int *)(r + 0x4668) = 0;
        *(int *)(r + 0x466c) = 0;
        *(int *)(r + 0x4670) = 0;
        *(int *)(r + 0x467c) = 0;
        *(short *)(r + 0x4682) = 0;
        *(short *)(r + 0x4684) = 0;
        *(short *)(r + 0x4686) = 0;
        *(unsigned char *)(r + 0x4688) = 0;
        *(unsigned char *)(r + 0x4689) = 0;
        *(unsigned char *)(r + 0x468a) = 0;
        r += 0x2c;
    }
    for (i = 0; i < 5; i++) {
        char *q = c + i * 0x10;
        *(int *)(q + 0x473c) = 0;
        *(int *)(q + 0x4740) = 0;
        *(short *)(q + 0x4744) = 0;
        *(short *)(q + 0x4746) = 0;
        *(unsigned char *)(q + 0x4748) = 0;
        *(unsigned char *)(q + 0x4749) = 0;
    }
    *(short *)(c + 0x4ee0) = 0;
    *(unsigned char *)(c + 0x4ee6) = 0;
    *(int *)(c + 0x4eb0) = 0;
    *(int *)(c + 0x4eb4) = 0;
    *(int *)(c + 0x4ec0) = 0;
    *(int *)(c + 0x4ec4) = 0;
    *(int *)(c + 0x4ec8) = 0;
    *(short *)(c + 0x4ede) = 0;
    *(unsigned char *)(c + 0x4ee4) = 0;
    *(unsigned char *)(c + 0x4ee5) = 0;
    *(unsigned char *)(c + 0x4ee7) = 0;
    *(unsigned char *)(c + 0x4ee8) = 0;
    *(short *)(c + 0x4ee2) = 0;
    *(short *)(c + 0x4edc) = 0;
    *(unsigned char *)(c + 0x4ee9) = 0;
    func_ov004_020adb1c(0);
}
}
#pragma pop

// @symbol _ZN14dScMgCurling_c13OnYoshiTryEatEi
/* Slot 18, one of dScMgBase_c's own undeclared slots; the inherited label
 * is the tree-wide mislabel and this is not a destructor. Restarts the
 * scene: state 0, the stones cleared, a house picked, the blend set up
 * again and the base's handle refreshed. */
void dScMgCurling_c::OnYoshiTryEat(int /* arg */)
{
    unk_4eac = 0;
    func_ov006_020e3388((char *)this);
    func_ov006_020e3250((char *)this);
    _ZN3G2x13SetBlendAlphaEPVttttj((volatile u16 *)0x4000050, 0, 0xd, 2, 0x10);
    _ZN3G2x13SetBlendAlphaEPVttttj((volatile u16 *)0x4001050, 0, 4, 2, 0x10);
    unk_4ed8 = func_ov004_020adc1c();
}

// @symbol _ZN14dScMgCurling_c6RenderEv
/* Slot 9. The base's frame, then the popups, the stones, the aim sprite
 * and the falling bits. */
s32 dScMgCurling_c::Render()
{
    char *c = (char *)this;
    func_ov004_020b19f0(func_ov004_020adc1c());
    func_ov006_020e1554((ScoreView *)c);
    func_ov006_020e1c68(c);
    func_ov006_020e17f8(c);
    func_ov006_020e0694(c);
    return 1;
}

// @symbol _ZN14dScMgCurling_c8BehaviorEv
/* Slot 6. The scene state through its table, then the falling bits. The
 * state index is read through PopupView, which already names the word at
 * 0x4eac; the table's receiver C stays incomplete. */
s32 dScMgCurling_c::Behavior()
{
    C *c = (C *)this;
    int j = ((PopupView *)this)->state;
    (c->*data_ov006_02141950[j])();
    func_ov006_020e12d0((char *)this);
    return 1;
}

// @symbol _ZN14dScMgCurling_c13InitResourcesEv
/* Slot 0. Loads the board and the stone graphics for both screens, the
 * palettes and the house, then starts the first round with a 0x40 delay. */
s32 dScMgCurling_c::InitResources()
{
    char *self = (char *)this;
    char *a = func_ov004_020adc74(&data_ov006_0213c394);
    char *b = func_ov004_020adc74(&data_ov006_0213c3b4);
    void *f;

    if (a == 0 || b == 0) return 0;

    data_0209d45c |= 4;
    *(volatile u16 *)0x400000c = (*(volatile u16 *)0x400000c & ~3) | 2;
    *(volatile u16 *)0x400000c = (*(volatile u16 *)0x400000c & 0x43) | 0x1220;
    DecompressLZ16(a, _ZN2G213GetBG2CharPtrEv());

    f = LoadFile(0x2f);
    _ZN2GX10LoadBGPlttEPKvjj(f, 0x60, 0x1a0);
    Deallocate(f);

    f = LoadFile(0x30);
    func_020563d4(f, 0, 0x800);
    Deallocate(f);

    {
        void *c7 = LoadFile(0xc7);
        void *c8 = LoadFile(0xc8);
        DecompressLZ16(c7, (void *)0x6400000);
        _ZN2GX11LoadOBJPlttEPKvjj(c8, 0, 0x100);

        data_0209d454 |= 4;
        *(volatile u16 *)0x400100c = (*(volatile u16 *)0x400100c & ~3) | 2;
        *(volatile u16 *)0x400100c = (*(volatile u16 *)0x400100c & 0x43) | 0x814;
        DecompressLZ16(b, (void *)_ZN3G2S13GetBG2CharPtrEv());

        f = LoadFile(0x33);
        _ZN3GXS10LoadBGPlttEPKvjj(f, 0x60, 0x1a0);
        Deallocate(f);

        f = LoadFile(0x34);
        func_02056374(f, 0, 0x800);
        Deallocate(f);

        Ov004_Deallocate(a);
        Ov004_Deallocate(b);

        DecompressLZ16(c7, (void *)0x6600000);
        _ZN3GXS11LoadOBJPlttEPKvjj(c8, 0, 0x100);
        Deallocate(c7);
        Deallocate(c8);
    }

    func_ov006_020e3388(self);
    func_ov006_020e3378(self);
    func_ov006_020e3250(self);
    *(unsigned char *)(self + 0x4ee7) = 1;
    *(u16 *)(self + 0x4ee0) = 0;
    func_ov006_020e2dbc(self);
    func_ov006_020e13a4(self);
    *(int *)(self + 0x4eac) = 1;
    func_ov004_020b04d0(0x20);
    *(u16 *)(self + 0x4ee2) = 0x40;
    *(int *)(self + 0xa4) = 1;
    _ZN3G2x13SetBlendAlphaEPVttttj((volatile u16 *)0x4000050, 0, 0xd, 2, 0x10);
    _ZN3G2x13SetBlendAlphaEPVttttj((volatile u16 *)0x4001050, 0, 4, 2, 0x10);
    *(int *)(self + 0x4ed8) = func_ov004_020adc1c();
    return 1;
}
