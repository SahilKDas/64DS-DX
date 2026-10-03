//cpp
/* dScMgAmida_c -- the ladder-lottery (Amida) minigame, ov006, 28 functions
 * (.text 0x020d1018..0x020d5a54).
 *
 * This file is the class's whole ov006 unit, in ROM order, lowest address
 * first, under `#pragma defer_codegen off`: from the destructor pair at
 * 0x020d1018 to func_ov006_020d5a50, the piece array's element constructor.
 * The previous unit ends at 0x020d1018 with func_ov006_020d100c, and
 * dScMgBomroom_c's D1 starts the next at 0x020d5a54. tu_map stops this unit
 * at 0x020d5974; the relocation census in the manifest shows that the
 * factory and func_ov006_020d5a50 above it belong to it too. The destructor
 * is the key function, so this file also emits the vtable and RTTI.
 *
 * The fifteen functions below func_ov006_020d3624 came from one-function
 * files. Their bodies are kept as they matched there, with these changes:
 *   - func_ov006_020d122c, func_ov006_020d1958, func_ov006_020d1a3c and
 *     func_ov006_020d27dc called slot 36 through stand-in vtable structs;
 *     they now call Unk36().
 *   - func_ov006_020d2580 took the two rung ends as int pointers; it now
 *     takes them as by-value Points, the way the pen handler already
 *     declared it, and uses the member names.
 *   - data_020a0dea/deb are declared as 4-byte rows, the form
 *     func_ov006_020d1958 matched under. Indexing them as flat arrays costs
 *     it four words.
 *   - each shard's file-wide optimisation pragma is now a push/pop bracket
 *     around its own function.
 *   - func_ov006_020d27dc was the last to match. It was an unmatched draft
 *     from 2026-09-14 and was matched on 2026-10-02.
 *
 * Still raw: the func_ and data_ helpers are unnamed in symbols.txt; the
 * pattern table is five separate data symbols (0x0212e1a8..0x0212e1b8), so
 * it is read by byte offset rather than as one array of structs; the words
 * at 0x5370 and 0x5378..0x539b are padding in dScMgAmida_c.h; Virtual88
 * reads the class through a local layout struct, and func_ov006_020d122c,
 * func_ov006_020d1450, func_ov006_020d1958, func_ov006_020d1a3c,
 * func_ov006_020d25fc and func_ov006_020d27dc through raw offsets; and the
 * factory builds the object by hand because the class has no constructor
 * declared yet.
 */

#include "types.h"
#include "decl_common.h"
#include "dScMgAmida_c.h"


extern "C" {
/* --- shared ov004 and main helpers --- */
extern void func_ov004_020afdd0(void *a0, int a1, int a2, int a3, int a4);
extern void func_ov004_020b1e34(void *a0, int a1, int a2, int a3);
extern int  func_ov004_020ae5c4(void *a, int b, int c, int d, int e, int f, int g);
extern void MultiCopyHalf(void *dst, void *src, int nbytes);
extern void *_ZN2G212GetBG3ScrPtrEv(void);  /* decl_common.h declares a data array named G2 */
extern void MultiStore16(u16 val, void *dst, int nbytes);
extern int  RandomIntInternal(int *seed);
extern int  Vec2_Len(int *v);
extern void FreeGfxSlotsById(int a);
extern u32  LoadCompressedFileAt(int fileID, void *target);
extern int  LoadFile(int handle);
extern void _ZN3G2x13SetBlendAlphaEPVttttj(void *p, u16 a, u16 b, u16 c, u16 d);
extern void SetSubBg2Offset(int a, int b);
extern void __cxa_vec_ctor(void *obj, int a, int b, void *cb1, void *cb2);
extern void func_0203d738(void *p);
extern void func_02012dbc(int a);
extern int  func_020126e8(int a);
extern void func_020126ac(int a0, int a1, int a2, int a3, int a4);
extern int  func_02012468(int handle, int a, int sfx, int b, int c, int d, int pan, short e);
extern void func_02012790(int sfx);
extern int  GetGameLanguage(void);
extern unsigned int _ZN3G2S13GetBG1CharPtrEv(void);
extern void *_ZN3G2S12GetBG1ScrPtrEv(void);

/* --- the class's own functions defined below --- */
extern int  func_ov006_020d2580(dScMgAmida_c *self, dScMgAmida_c::Point p1, dScMgAmida_c::Point p2);
extern void func_ov006_020d3624(void *);
extern void func_ov006_020d3668(void *sb);
extern void func_ov006_020d36a4(dScMgAmida_c *self);
extern void func_ov006_020d452c(void *thiz);
extern void func_ov006_020d47f4(char *c);
extern void func_ov006_020d5a50(void);
extern void *dScMgAmida_c_classInit(void);

/* --- data --- */
extern s16 data_02082214[];
extern u8  data_0209d45c;
extern u8  data_0209d454;
extern u8  data_020a0e40;
extern u8  data_020a0de8[];
extern u8  data_020a0de9[];
extern u8  data_020a0dea[][4];
extern u8  data_020a0deb[][4];
extern int data_ov006_0212e1c0[];
extern s32 data_0208ee44;
extern char data_ov006_0212e1a8[];
extern char data_ov006_0212e1ac[];
extern char data_ov006_0212e1b0[];
extern char data_ov006_0212e1b4[];
extern char data_ov006_0212e1b8[];
extern void *data_ov006_0213a32c;
extern void *data_ov006_0213a35c;
extern int  data_ov006_0213a338[];
extern void *data_ov006_0213a390[];
extern void *data_ov006_0213a458[];
extern void *data_ov006_0213a4b0[];
extern int  data_ov006_0213a4c0[];
extern int  data_ov006_0213a568[];
extern int  data_ov006_0213b84c[13];
extern s32  data_ov006_02141640[];
extern s32  data_ov006_02141650[];
}

namespace CP15 {
    void FlushAndInvalidateDataCache(u32 addr, u32 size);
}

namespace GX {
    void LoadBGPltt(const void *data, u32 offset, u32 size);
    void LoadOBJPltt(const void *data, u32 offset, u32 size);
}

namespace GXS {
    void LoadBGPltt(const void *data, u32 offset, u32 size);
    void LoadOBJPltt(const void *data, u32 offset, u32 size);
}

namespace G2S {
    char *GetBG0CharPtr();
    void *GetBG2ScrPtr();
    void *GetBG3ScrPtr();
    char *GetBG3CharPtr();
}

namespace Sound {
    void PlayBank2_2D(unsigned int id);
}

namespace Memory {
    void *Allocate(unsigned int size);
    void Deallocate(void *ptr);
}

typedef struct { int a; int b; } Pair;
extern Pair data_ov006_0213b8b8[11];

typedef struct { int v[14]; } Buf14;
extern Buf14 data_ov006_0213b880;

/* The scene seen through a char pointer, for the free helpers below. */
#define AMIDA(p) ((dScMgAmida_c *)(p))
#define PIECE_AT(p) ((dScMgAmida_c_Piece *)((p) + 0x4768))

/* Launder helpers for Behavior's piece walk: AT makes the compiler
   materialise each address on its own instead of folding it into a
   neighbouring add, which is what the ROM does there. */
#define AT(p,off) ((void*)(int)((char*)(p)+(off)))
#define IP(p,o) (*(int*)((char*)(p)+(o)))
#define IAP(p,o) (*(int*)AT(p,(o)))
#define MULFX(a,b) ((int)(((s64)(a)*(b)+0x800)>>12))

#pragma defer_codegen off

// @symbol _ZN12dScMgAmida_cD1Ev
// @symbol _ZN12dScMgAmida_cD0Ev
/* The four member arrays go down last-built first: the 0x80 pieces, then
   the lane velocities, the lane positions and the cells. Both element
   destructors do nothing. D1 stores the vptr and ends in dScMgBase_c's D2;
   D0 repeats the same body and adds the delete. */
dScMgAmida_c::~dScMgAmida_c()
{
    __cxa_vec_cleanup((char *)this + 0x4768, 0x80, 0x18, (void *)func_ov006_020d116c);
    __cxa_vec_cleanup((char *)this + 0x4744, 4, 8, (void *)NullDestructor_0203d47c);
    __cxa_vec_cleanup((char *)this + 0x4724, 4, 8, (void *)NullDestructor_0203d47c);
    __cxa_vec_cleanup((char *)this + 0x4660, 4, 8, (void *)NullDestructor_0203d47c);
}

#ifdef _MSC_VER
/* THE HOST NEEDS THE ROM'S FLAT D0 NAME, AND MSVC NEVER EMITS IT. MSVC
 * folds the Itanium destructor variants into the one ~dScMgAmida_c() above,
 * so this spells out what the deleting destructor does in terms of it: the
 * D1 body, called qualified so it is a direct call, then the class-specific
 * operator delete. mwccarm never defines _MSC_VER, so nothing here reaches
 * the cartridge object. */
extern "C" dScMgAmida_c *_ZN12dScMgAmida_cD0Ev(dScMgAmida_c *thiz)
{
    thiz->dScMgAmida_c::~dScMgAmida_c();
    dScMgAmida_c::operator delete(thiz);
    return thiz;
}
#endif

// @symbol func_ov006_020d116c
/* Element destructor for the 0x80 falling pieces: nothing to undo. */
extern "C" void func_ov006_020d116c(void)
{
}

// @symbol _ZN12dScMgAmida_c9Virtual8CEv
/* Slot 35, the family's only override of it. The base asks whether the low
   byte of fBase_c::param1 is nonzero; this asks whether it is exactly 1.
   Nothing in this class calls it; the callers are in dScMgCoin_c,
   dScMgPanel_c, dScMgSound_c and dScMgSnowball_c. */
int dScMgAmida_c::Virtual8C()
{
    return (param1 & 0xff) == 1;
}

// @symbol _ZN12dScMgAmida_c5Unk36Ev
/* Slot 36, this class's own: whether the low byte of the word at this+8,
   a field from above dScMgBase_c, is 2. Left as a raw offset, as
   dScMgSlot1_c.h does for the same word. */
int dScMgAmida_c::Unk36()
{
    return ((*(int *)((char *)this + 8)) & 0xff) == 2;
}

// @symbol _ZN12dScMgAmida_c9Virtual7CEv
/* Slot 31: the sub screen's BG1 set up for this game. Same shape as the
   base's, with 4 for the BG1CNT base bits and this class's own language
   table. Not a Kill; see the slot-31 block in include/dScMgBase_c.h. */
int dScMgAmida_c::Virtual7C()
{
    volatile unsigned short *reg = (volatile unsigned short*)0x400100a;
    int id;
    void *t;
    *reg = (unsigned short)((*reg & 0x43) | 4);
    *reg = (unsigned short)(*reg & ~0x40);
    *reg = (unsigned short)(*reg & ~3);
    SetSubBg1Offset(0, 0);
    data_0209d454 = (unsigned char)(data_0209d454 & ~2);
    id = GetGameLanguage();
    t = (void*)_ZN3G2S13GetBG1CharPtrEv();
    LoadCompressedFileAt(data_ov006_0213b838[id], t);
    LoadCompressedFileAt(0x5b, _ZN3G2S12GetBG1ScrPtrEv());
}

// @symbol func_ov006_020d122c
/* Refills and shuffles the four-entry table at 0x4714. When Unk36() holds,
   the entries come from the pattern index, are copied to 0x46a4, and the
   table is reshuffled until it differs somewhere from that copy as seen
   through 0x4694; otherwise the first arg1 entries (at most 4) are 1 and
   the rest 0, shuffled once. */
#pragma push
#pragma opt_strength_reduction off
#define A714(i) (((int *)(self + 0x4714))[i])
#define A46A4(i) (((int *)(self + 0x46a4))[i])
#define A4694(i) (((int *)(self + 0x4694))[i])

extern "C" void func_ov006_020d122c(char *self, int arg1)
{
    int i;
    int k;
    int j;
    int t;
    int u;
    int *pi;

    if (AMIDA(self)->Unk36()) {
        switch (*(int *)(self + 0x53d4)) {
        case 0:
            A714(0) = 0;
            A714(1) = 0;
            A714(2) = 3;
            A714(3) = 3;
            break;
        case 1:
        case 3:
        case 5:
            A714(0) = 0;
            A714(1) = 0;
            A714(2) = 1;
            A714(3) = 3;
            break;
        default:
            for (i = 0; i < 4; i++)
                A714(i) = i;
            break;
        }
        for (i = 0; i < 4; i++)
            A46A4(i) = A714(i);
        {
            int off = 0x4714;
            arg1 = 1;
            do {
                for (i = 0; i < 4; i++) {
                    t = RandomIntInternal(&data_0209d4b8);
                    pi = (int *)(self + (i << 2) + off);
                    j = ((u32)(((u32)t >> 16) & 0x7fff) * 4) >> 15;
                    t = *pi;
                    {
                        int *pjx = (int *)(self + (j << 2) + off);
                        u = *pjx;
                        *pi = u;
                        *pjx = t;
                    }
                }
                for (k = 0; k < 4; k++) {
                    if (A46A4(k) != A714(A4694(k))) {
                        arg1 = 0;
                        break;
                    }
                }
            } while (arg1 == 1);
        }
    } else {
        if (arg1 > 4)
            arg1 = 4;
        for (k = 0; k < arg1; k++)
            A714(k) = 1;
        for (; arg1 < 4; arg1++)
            A714(arg1) = 0;
        {
            int off = 0x4714;
            for (i = 0; i < 4; i++) {
                t = RandomIntInternal(&data_0209d4b8);
                pi = (int *)(self + (i << 2) + off);
                j = ((u32)(((u32)t >> 16) & 0x7fff) * 4) >> 15;
                t = *pi;
                {
                    int *pjx = (int *)(self + (j << 2) + off);
                    u = *pjx;
                    *pi = u;
                    *pjx = t;
                }
            }
        }
    }
}
#pragma pop
#undef A714
#undef A46A4
#undef A4694

// @symbol func_ov006_020d1450
extern "C" void func_ov006_020d1450(char *self)
{
    int row = 0;
    int col;
    int rowoff = 0;
    int one = 0;
    int val = 0;
    for (; row < 0x100; row++) {
        for (col = 0; col < 0x158; col++) {
            unsigned char *base = *(unsigned char **)(self + 0x4000 + 0x70c);
            unsigned char *p = (unsigned char *)(rowoff + (int)base) + col;
            if (*p != 0) *p = val;
        }
        rowoff += 0x158;
    }
    func_ov006_020d3624(self);
    *(unsigned char *)(self + 0x4704) = 0;
}

// @symbol _ZN12dScMgAmida_c9Virtual88Eiiii
/* Slot 34, and the only override in the family that
   CALLS the base rather than replacing it.

   Amida is the ghost-leg game: you draw a line and it must not cross one that
   is already there.  So this override is the collision half and the base is the
   drawing half.  Before painting it probes the 0x158-stride occupancy grid at
   +0x4710 -- the cell under (x, y), and for a diagonal step also the two cells
   the diagonal would cut between -- and sets the foul flag at +0x4709 if any of
   them is already >= 3.  Then it calls the base brush, with a size of 2 or 4
   chosen per case rather than passed in, which is why this body has one fewer
   explicit parameter than the slot's signature: the fourth argument arrives on
   the stack and is never read.

   +0x46f8 / +0x46fc are the previous point, which is how it knows a step is
   diagonal at all. */
typedef struct Ctx {
    u8 pad_0000[0x46d8];
    int unk46d8;
    int unk46dc;
    int unk46e0;
    int unk46e4;
    u8 pad_46e8[0x10];
    int unk46f8;
    int unk46fc;
    u8 pad_4700[4];
    u8 unk4704;
    u8 unk4705;
    u8 unk4706;
    u8 unk4707;
    u8 unk4708;
    u8 unk4709;
    u8 unk470a;
    u8 pad_470b;
    u8* unk470c;
    u8* unk4710;
} Ctx;

void dScMgAmida_c::Virtual88(int y, int x, int arg3, int /* size */)
{
    Ctx *ctx = (Ctx *)this;

    u8 f705;
    u8 f707;
    int x2;

    if (ctx->unk4708 == 1) {
        int py;
        int pym;
        int pyp;
        if ((ctx->unk4710 + y * 0x158)[x + 0xc0] >= 3u) {
            ctx->unk4709 = 1;
        }
        py = ctx->unk46f8;
        if (y != py || x != ctx->unk46fc) {
            pym = py - 1;
            if (y == pym && x == ctx->unk46fc - 1) {
                u8* b = ctx->unk4710;
                if ((b + (y + 1) * 0x158)[x + 0xc0] >= 3u) {
                    ctx->unk4709 = 1;
                } else if ((b + y * 0x158)[x + 0xc1] >= 3u) {
                    ctx->unk4709 = 1;
                }
            } else {
                pyp = py + 1;
                if (y == pyp && x == ctx->unk46fc - 1) {
                    u8* b = ctx->unk4710;
                    if ((b + (y - 1) * 0x158)[x + 0xc0] >= 3u) {
                        ctx->unk4709 = 1;
                    } else if ((b + y * 0x158)[x + 0xc1] >= 3u) {
                        ctx->unk4709 = 1;
                    }
                } else if (y == pym && x == ctx->unk46fc + 1) {
                    u8* b = ctx->unk4710;
                    if ((b + (y + 1) * 0x158)[x + 0xc0] >= 3u) {
                        ctx->unk4709 = 1;
                    } else if ((b + y * 0x158)[x + 0xbf] >= 3u) {
                        ctx->unk4709 = 1;
                    }
                } else if (y == pyp && x == ctx->unk46fc + 1) {
                    u8* b = ctx->unk4710;
                    if ((b + (y - 1) * 0x158)[x + 0xc0] >= 3u) {
                        ctx->unk4709 = 1;
                    } else if ((b + y * 0x158)[x + 0xbf] >= 3u) {
                        ctx->unk4709 = 1;
                    }
                }
            }
        }
        ctx->unk46f8 = y;
        ctx->unk46fc = x;
        return;
    }

    if (ctx->unk4706 == 1) {
        dScMgBase_c::Virtual88(y, x, arg3, 4);
        if (y < 0) return;
        if (y >= 0x100) return;
        if (x < -0xc0) return;
        if (x >= 0x98) return;
        (ctx->unk4710 + y * 0x158)[x + 0xc0] = ctx->unk470a;
        return;
    }

    f705 = ctx->unk4705;
    if (f705 == 1) {
        dScMgBase_c::Virtual88(y, x, arg3, 2);
        return;
    }

    f707 = ctx->unk4707;
    x2 = x + 0xc0;
    if (f707 != 0) {
        dScMgBase_c::Virtual88(y, x, arg3, 4);
        if (y >= 0 && y < 0x100 && x2 >= 0 && x2 < 0x158) {
            (ctx->unk4710 + y * 0x158)[x2] = ctx->unk470a;
        }
    } else {
        u8 b;
        if (y < 0 || y >= 0x100 || x2 < 0 || x2 >= 0x158) {
            dScMgBase_c::Virtual88(y, x, arg3, 4);
            return;
        }
        if (y == 0x20 || y == 0x60 || y == 0xa0 || y == 0xe0) {
            if (ctx->unk4704 == 0) {
                ctx->unk4704 = 1;
                ctx->unk46d8 = y;
                ctx->unk46dc = x;
                ctx->unk46e0 = y;
                ctx->unk46e4 = x;
                return;
            }
            if (f705 != 0) return;
            if (y == ctx->unk46d8) {
                ctx->unk46e4 = x;
            } else {
                ctx->unk4705 = 1;
                ctx->unk46e0 = y;
                ctx->unk46e4 = x;
            }
            return;
        }
        if (ctx->unk4704 == 0) {
            dScMgBase_c::Virtual88(y, x, arg3, 2);
            return;
        }
        b = (ctx->unk470c + y * 0x158)[x2];
        if (b != 0 && ctx->unk470a != b) goto fail;
        b = (ctx->unk4710 + y * 0x158)[x2];
        if (b != 0 && ctx->unk470a != b) {
        fail:
            ctx->unk4704 = 0;
            ctx->unk4705 = 1;
            return;
        }
    }

    dScMgBase_c::Virtual88(y, x, arg3, 2);
    (ctx->unk470c + y * 0x158)[x2] = ctx->unk470a;
}

// @symbol func_ov006_020d1958
extern "C" void func_ov006_020d1958(char *c)
{
    if (AMIDA(c)->Unk36() == 0) return;
    if (*(unsigned char *)(c + 0x5000 + 0x3dd) == 1) return;
    unsigned int i = 0;
    int b3 = data_020a0e40;
    if (data_020a0de8[b3 << 2] != 0) {
        unsigned int off = b3 << 2;
        if (data_020a0de9[off] != 0) i = 1;
    }
    if (i == 0) return;
    int v1 = data_020a0dea[b3][0];
    int v0 = data_020a0deb[b3][0];
    if (v1 < 0x60) return;
    if (v1 >= 0xa0) return;
    if (v0 < 0xa0) return;
    if (v0 >= 0xc0) return;
    *(unsigned char *)(c + 0x5000 + 0x3dd) = 1;
    Sound::PlayBank2_2D(0x151);
}

// @symbol func_ov006_020d1a3c
extern "C" void func_ov006_020d1a3c(void *p)
{
    int idx;
    int flag;
    int a;
    int b;
    char *base;

    if (AMIDA(p)->Unk36()) return;

    base = (char *)p;
    idx = data_020a0e40;
    flag = 0;
    if (data_020a0de8[idx * 4]) {
        if (data_020a0de9[idx * 4] != 0) flag = 1;
    }
    if (flag != 0) {
        *(u8 *)(base + 0x53de) = 0;
        *(int *)(base + 0x53d8) = 0;
    }
    idx = data_020a0e40;
    if (data_020a0de8[idx * 4] != 0) {
        a = data_020a0dea[idx][0];
        b = data_020a0deb[idx][0];
        if ((a >= 0 && a < 0x10 && b >= 0x40 && b < 0x80) ||
            (a >= 0xf0 && a < 0x100 && b >= 0x40 && b < 0x80)) {
            if (*(int *)(base + 0x53d8) >= 5) {
                *(u8 *)(base + 0x53dc) = 1;
            } else {
                *(int *)(base + 0x53d8) += 1;
                *(u8 *)(base + 0x53dc) = 0;
            }
            *(u8 *)(base + 0x53de) = 1;
        } else {
            *(u8 *)(base + 0x53dc) = 0;
            *(u8 *)(base + 0x53de) = 0;
            *(int *)(base + 0x53d8) = 0;
        }
    } else {
        *(u8 *)(base + 0x53dc) = 0;
        *(int *)(base + 0x53d8) = 0;
    }
}

// @symbol func_ov006_020d1ba0
/* The pen handler: follows the stylus while it is down, and on release
   turns the stroke into a rung between two neighbouring lanes, probed with
   func_ov006_020d2580 before it is painted. Point's special members are
   what shape the calls; see include/dScMgAmida_c.h. */
extern "C" void func_ov006_020d1ba0(dScMgAmida_c *self)
{
    int oldX;
    int oldY;

    if (self->mLineCount >= 0xff) return;
    if (self->unk_53dd == 1) return;

    oldX = self->mPen.x;
    oldY = self->mPen.y;

    if (data_020a0de8[data_020a0e40 * 4] != 0) {
        int newX = data_020a0dea[data_020a0e40][0];
        int newY = data_020a0deb[data_020a0e40][0];
        if (newX < 0) return;
        if (newX >= 0x100) return;
        if (newY < 0) return;
        if (newY >= self->mLineEndY) return;
        if (self->unk_53de != 0) return;
        self->mPen.x = newX;
        self->mPen.y = newY;
        if (self->mPenStart.x == -1 || self->mPenStart.y == -1) {
            self->mPenStart.x = newX;
            self->mPenStart.y = newY;
            func_02012718(0xdc, self->mPenStart.x << 12);
        }
        if (oldX == -1 && oldY == -1) return;
        if (oldX == self->mPen.x && oldY == self->mPen.y) {
            self->mPenTapped = 1;
            return;
        }
        func_ov004_020ae5c4(self, oldX, oldY, self->mPen.x, self->mPen.y, self->unk_53e4, 1);
        if (self->mPenTapped == 1) {
            self->mPenSoundHandle = func_02012468(self->mPenSoundHandle, 2, 0xde, 4, 0, 0, func_020126e8(self->mPen.x << 12), 0);
            return;
        }
        self->mPenSoundHandle = func_02012468(self->mPenSoundHandle, 2, 0xdd, 4, 0, 0, func_020126e8(self->mPen.x << 12), 0);
        return;
    } else {
        int released;
        if (data_020a0de8[data_020a0e40 * 4] == 0 && data_020a0de9[data_020a0e40 * 4] != 0) {
            released = 1;
        } else {
            released = 0;
        }
        if (released != 0) {
            if (self->mPenStart.x != -1 && self->mPenStart.y != -1 && oldX != -1 && oldY != -1) {
                if (self->mLineStartSet == 0) {
                    int d1 = oldX - self->mPenStart.x;
                    if (d1 < 0) d1 = -d1;
                    if (d1 < 0x10) {
                        int d2 = oldY - self->mPenStart.y;
                        if (d2 < 0) d2 = -d2;
                        if (d2 >= 0x10) goto classify;
                    } else {
                    classify:
                        if (oldX >= 0x21 && oldX <= 0x3f) {
                            if (self->mPenStart.x >= 0x41 && self->mPenStart.x <= 0x5f) {
                                self->mLineStart.x = 0x60;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0x20;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        } else if (oldX >= 0x41 && oldX <= 0x5f) {
                            if (self->mPenStart.x >= 0x21 && self->mPenStart.x <= 0x3f) {
                                self->mLineStart.x = 0x20;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0x60;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        } else if (oldX >= 0x61 && oldX <= 0x7f) {
                            if (self->mPenStart.x >= 0x81 && self->mPenStart.x <= 0x9f) {
                                self->mLineStart.x = 0xa0;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0x60;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        } else if (oldX >= 0x81 && oldX <= 0x9f) {
                            if (self->mPenStart.x >= 0x61 && self->mPenStart.x <= 0x7f) {
                                self->mLineStart.x = 0x60;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0xa0;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        } else if (oldX >= 0xa1 && oldX <= 0xbf) {
                            if (self->mPenStart.x >= 0xc1 && self->mPenStart.x <= 0xdf) {
                                self->mLineStart.x = 0xe0;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0xa0;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        } else if (oldX >= 0xc1 && oldX <= 0xdf) {
                            if (self->mPenStart.x >= 0xa1 && self->mPenStart.x <= 0xbf) {
                                self->mLineStart.x = 0xa0;
                                self->mLineStart.y = self->mPenStart.y;
                                self->mLineEnd.x = 0xe0;
                                self->mLineEnd.y = self->mPen.y;
                            }
                        }
                        if (self->mLineStart.x >= 0 && self->mLineEnd.x >= 0) {
                            if (func_ov006_020d2580(self, self->mLineStart, self->mLineEnd) == 0) {
                                self->mLineStartSet = 1;
                                self->mLineEndSet = 1;
                            }
                        }
                    }
                } else if (self->mLineEndSet == 0) {
                    int dx, dsx, adx, adsx;
                    dx = oldX - self->mLineStart.x;
                    adx = dx < 0 ? -dx : dx;
                    dsx = self->mPenStart.x - self->mLineStart.x;
                    adsx = dsx < 0 ? -dsx : dsx;
                    if (adx >= adsx) {
                        dx = dx < 0 ? -dx : dx;
                        if (dx >= 0x20) {
                            if (oldX < self->mLineStart.x) {
                                self->mLineEnd.x = self->mLineStart.x - 0x40;
                            } else {
                                self->mLineEnd.x = self->mLineStart.x + 0x40;
                            }
                            self->mLineStart.y = self->mLineEnd.y;
                            self->mLineEnd.y = self->mPen.y;
                        }
                    } else {
                        dsx = dsx < 0 ? -dsx : dsx;
                        if (dsx >= 0x20) {
                            if (self->mPenStart.x < self->mLineStart.x) {
                                self->mLineEnd.x = self->mLineStart.x - 0x40;
                            } else {
                                self->mLineEnd.x = self->mLineStart.x + 0x40;
                            }
                            self->mLineEnd.y = self->mPenStart.y;
                        }
                    }
                    if (self->mLineStart.x >= 0 && self->mLineEnd.x >= 0) {
                        if (func_ov006_020d2580(self, self->mLineStart, self->mLineEnd) == 0) {
                            self->mLineEndSet = 1;
                        }
                    }
                }
            }

            if (self->mLineStartSet == 0 || self->mLineEndSet == 0) {
                if (self->unk_53de == 0 && self->unk_53dd == 0) func_02012790(0xe);
                func_ov006_020d1450((char *)self);
                self->mLineStart.x = -1;
                self->mLineStart.y = -1;
                self->mLineEnd.x = -1;
                self->mLineEnd.y = -1;
                return;
            }
            if (self->mLineEnd.x == self->mLineStart.x) {
                func_ov006_020d3624(self);
                int row = 0;
                int col;
                int rowoff = 0;
                int one = 0;
                int val = 0;
                for (; row < 0x100; row++) {
                    for (col = 0; col < 0x158; col++) {
                        (self->unk_470c + rowoff)[col] = val;
                    }
                    rowoff += 0x158;
                }
            } else {
                dScMgAmida_c::Point a(self->mLineStart);
                dScMgAmida_c::Point b(self->mLineEnd);
                int hit;
                if (a.x < b.x) {
                    a.x += 2;
                    b.x -= 2;
                } else {
                    a.x -= 2;
                    b.x += 2;
                }
                {
                    if (func_ov006_020d2580(self, a, b) == 1) {
                        if (self->unk_53de == 0 && self->unk_53dd == 0) func_02012790(0xe);
                        func_ov006_020d1450((char *)self);
                        self->mLineStart.x = -1;
                        self->mLineStart.y = -1;
                        self->mLineEnd.x = -1;
                        self->mLineEnd.y = -1;
                        return;
                    }
                }
                if (self->mLineStart.x < self->mLineEnd.x) {
                    u8 *row = self->unk_4710 + self->mLineEnd.x * 0x158;
                    hit = *(row + self->mLineEnd.y + 0x218);
                    if (hit == 0) {
                        hit = *(self->unk_4710 + self->mLineStart.x * 0x158 + self->mLineStart.y - 0x98);
                    }
                } else {
                    u8 *row = self->unk_4710 + self->mLineEnd.x * 0x158;
                    hit = *(row + self->mLineEnd.y - 0x98);
                    if (hit == 0) {
                        hit = *(self->unk_4710 + self->mLineStart.x * 0x158 + self->mLineStart.y + 0x218);
                    }
                }
                if (hit == 0) {
                    func_ov006_020d1450((char *)self);
                    self->mLineCommit = 1;
                    func_ov004_020ae5c4(self, a.x, a.y, b.x, b.y, self->unk_53e4, 1);
                    if (a.x < b.x) {
                        func_ov004_020ae5c4(self, a.x - 1, a.y, a.x - 1, a.y, self->unk_53e4, 1);
                        func_ov004_020ae5c4(self, b.x + 1, b.y, b.x + 1, b.y, self->unk_53e4, 1);
                    } else {
                        func_ov004_020ae5c4(self, a.x + 1, a.y, a.x + 1, a.y, self->unk_53e4, 1);
                        func_ov004_020ae5c4(self, b.x - 1, b.y, b.x - 1, b.y, self->unk_53e4, 1);
                    }
                    self->mLineCommit = 0;
                    func_ov006_020d3668(self);
                    self->mLineCount++;
                    Sound::PlayBank2_2D(0xdf);
                } else {
                    if (self->unk_53de == 0 && self->unk_53dd == 0) func_02012790(0xe);
                    func_ov006_020d1450((char *)self);
                    self->mLineStart.x = -1;
                    self->mLineStart.y = -1;
                    self->mLineEnd.x = -1;
                    self->mLineEnd.y = -1;
                    return;
                }
            }
        }
        self->mPen.x = -1;
        self->mPen.y = -1;
        self->mPenStart.x = -1;
        self->mPenStart.y = -1;
        self->mLineStartSet = 0;
        self->mLineEndSet = 0;
        self->mPenTapped = 0;
    }
}

// @symbol func_ov006_020d2580
/* Probes the rung from p1 to p2 without painting it: slot 34 runs in probe
   mode over the line and reports whether any cell it crosses is taken. */
extern "C" int func_ov006_020d2580(dScMgAmida_c *self, dScMgAmida_c::Point p1, dScMgAmida_c::Point p2)
{
    int hi, lo;
    self->mProbeHit = 0;
    self->mProbeMode = 1;
    hi = p1.y;
    lo = p1.x;
    self->mPrev.x = lo;
    self->mPrev.y = hi;
    func_ov004_020ae5c4(self, p1.x, p1.y, p2.x, p2.y, self->unk_53e4, 1);
    self->mProbeMode = 0;
    return self->mProbeHit;
}

// @symbol func_ov006_020d25fc
#pragma push
#pragma opt_common_subs off
extern "C" int func_ov006_020d25fc(char *p, int b, int dir)
{
    int col, row;
    u8 cur;
    u8 *pcur;

    col = *(int *)(p + b * 8 + 0x4660);
    row = *(int *)(p + b * 8 + 0x4664) + 0xc0;

    switch (dir) {
        case 0: col--; row--; break;
        case 1: row--; break;
        case 2: col++; row--; break;
        case 3: col--; break;
        case 4: col++; break;
        case 5: col--; row++; break;
        case 6: row++; break;
        case 7: col++; row++; break;
        default: return 0;
    }

    {
        u8 *t = (u8 *)(p + 0x4680);
        cur = t[b];
        pcur = t + b;
    }

    if (cur >= 2) {
        u8 mv = *(u8 *)(*(u8 **)(p + 0x4710) + col * 0x158 + row);
        if (mv != cur)
            return 0;

        *(int *)(p + b * 4 + 0x4684) = dir;
        *(int *)(p + b * 8 + 0x4660) = col;
        *(int *)(p + b * 8 + 0x4664) = row - 0xc0;
        return 1;
    } else {
        int q, r;

        if (*(u8 *)(*(u8 **)(p + 0x4710) + col * 0x158 + row) < 2)
            return 0;

        *(int *)(p + b * 4 + 0x4684) = dir;
        *pcur = *(u8 *)(*(u8 **)(p + 0x4710) + col * 0x158 + row);
        *(int *)(p + b * 8 + 0x4660) = col;
        *(int *)(p + b * 8 + 0x4664) = row - 0xc0;

        q = (*(int *)(p + b * 8 + 0x4664) + 0xd4) * 0x1f4 /
            (*(int *)(p + 0x4700) + 0xd4);
        r = func_020126e8(*(int *)(p + b * 8 + 0x4660) << 0xc);
        func_020126ac(0x1bf, 6, 0, q, r);
        return 1;
    }
}
#pragma pop

// @symbol func_ov006_020d27dc
/* The per-step routine Behavior runs mScrollAccum / 16 times a frame:
   counts down the per-walker timers at 0x46b8, then steps each walker
   along the drawn lines with func_ov006_020d25fc; once every walker is in
   (0x46cc reaches 0x46c8) it closes the round.
   The `if (*(s32 *)(p + 0x5374) >= 5)` arm returns on its own while the
   0x1e path and the next-round store share the one `return` after the
   `sl == 1` if/else. That keeps the two arms in different blocks, so
   mwccarm keeps the ROM's branch there instead of predicating it
   (notes/mwccarm-codegen.md 6cn). */
#pragma push
#pragma opt_strength_reduction off
#pragma opt_common_subs off
extern "C" void func_ov006_020d27dc(void *arg0)
{
    char *p = (char *)arg0;
    s32 var_r4;
    s32 idx;
    s32 count;

    for (var_r4 = 0; var_r4 < (count = *(s32 *)(p + 0x46c8)); ) {
        s32 *slot = (s32 *)((int)p + var_r4 * 4 + 0x46b8);
        s32 v = *slot;
        var_r4++;
        if (v > 0) {
            *slot = v - 1;
        }
    }

    idx = 0;
    if (count <= 0) {
        return;
    }

    {
        s32 v20 = 0xd7;
        s32 v1c = 0xcf;
        s32 v18 = 0xc7;
        s32 v14 = 0xbf;
        s32 v10 = 0xe0;
        s32 v34 = 2;
        s32 v30 = 5;
        s32 v38 = 7;
        s32 vC = 0;
        s32 v60 = 0;
        s32 v5c = 0;
        s32 v58 = 0;
        s32 v54 = 0;
        s32 v50 = 0;
        s32 v4c = 0;
        s32 v48 = 0;
        s32 v44 = 0;
        s32 v40 = 0;
        s32 c1 = 1;
        s32 c4 = 4;
        s32 c6 = 6;
        s32 v24 = 0x1c0;
        s32 c3 = 3;
        s32 v3c = 0;
        s32 v2c = 0;
        s32 v28 = 0;

        do {
            if (*(s32 *)(p + idx * 4 + 0x46b8) > 0) {
                continue;
            }

            {
                int ent = (int)p + idx * 8;
                s32 tmp;
                s32 *distp = (s32 *)(ent + 0x4664);
                s32 dist = *distp;
                s32 lim = *(s32 *)(p + 0x4700);

                if (dist > lim) {
                    u8 *flag = (u8 *)(p + idx + 0x46b4);
                    if (*flag != 1) {
                        s32 sl = vC;
                        s32 *dirSlot;
                        s32 dirVal;

                        if (AMIDA(p)->Unk36() != 0) {
                            u32 want = *(u32 *)(p + idx * 4 + 0x46a4);
                            dirSlot = (s32 *)(p + idx * 8 + 0x4660);
                            dirVal = *dirSlot;
                            switch (dirVal) {
                            case 0x20:
                                if (*(s32 *)(p + 0x4714) == (s32)want) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x5398) = c1;
                                }
                                break;
                            case 0x60:
                                if (*(s32 *)(p + 0x4718) == (s32)want) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x5399) = c1;
                                }
                                break;
                            case 0xa0:
                                if (*(s32 *)(p + 0x471c) == (s32)want) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x539a) = c1;
                                }
                                break;
                            case 0xe0:
                                if (*(s32 *)(p + 0x4720) == (s32)want) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x539b) = c1;
                                }
                                break;
                            }
                        } else {
                            dirSlot = (s32 *)(ent + 0x4660);
                            dirVal = *dirSlot;
                            switch (dirVal) {
                            case 0x20:
                                if (*(s32 *)(p + 0x4714) == 1) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x5398) = c1;
                                }
                                break;
                            case 0x60:
                                if (*(s32 *)(p + 0x4718) == 1) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x5399) = c1;
                                }
                                break;
                            case 0xa0:
                                if (*(s32 *)(p + 0x471c) == 1) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x539a) = c1;
                                }
                                break;
                            case 0xe0:
                                if (*(s32 *)(p + 0x4720) == 1) {
                                    sl = c1;
                                } else {
                                    *(u8 *)(p + 0x539b) = c1;
                                }
                                break;
                            }
                        }

                        if (sl == 1) {
                            if (AMIDA(p)->Unk36() != 0) {
                                if (*(u8 *)(p + 0x46d5) == 0) {
                                    func_02012718(v10, *dirSlot << 0xc);
                                    u32 w2 = *(u32 *)(p + idx * 4 + 0x46a4);
                                    switch (w2) {
                                    default:
                                        break;
                                    case 0:
                                        func_02012718(v14, *dirSlot << 0xc);
                                        break;
                                    case 1:
                                        func_02012718(v18, *dirSlot << 0xc);
                                        break;
                                    case 2:
                                        func_02012718(v1c, *dirSlot << 0xc);
                                        break;
                                    case 3:
                                        func_02012718(v20, *dirSlot << 0xc);
                                        break;
                                    }
                                } else {
                                    func_02012718(0x1c3, *dirSlot << 0xc);
                                }
                                *flag = c1;
                                *(s32 *)(p + 0x46cc) += 1;
                                if (*(s32 *)(p + 0x46cc) < *(s32 *)(p + 0x46c8)) {
                                    continue;
                                }
                                if (*(u8 *)(p + 0x46d5) == 0) {
                                    *(s32 *)((int)p + 0x5374) += 1;
                                    *(s32 *)(p + 0x53e8) += 1;
                                    if (*(s32 *)(p + 0x53e8) > 0x270f) {
                                        *(s32 *)(p + 0x53e8) = 0x270f;
                                    }
                                    *(s32 *)(p + 0x53e0) = 0x78;
                                    return;
                                }
                                *(s32 *)(p + 0x53e0) = 0x1e;
                            } else {
                                s32 *d2 = (s32 *)((int)(p + idx * 8) + 0x4660);
                                s32 t;
                                func_02012718(v10, *d2 << 0xc);
                                func_02012718(0x1c1, *d2 << 0xc);
                                *flag = c1;
                                *(s32 *)(p + 0x46cc) += 1;
                                if (*(s32 *)(p + 0x46cc) < *(s32 *)(p + 0x46c8)) {
                                    continue;
                                }
                                *(s32 *)((int)p + 0x5374) += 1;
                                *(s32 *)(p + 0x53e8) += 1;
                                if (*(s32 *)(p + 0x53e8) > 0x270f) {
                                    *(s32 *)(p + 0x53e8) = 0x270f;
                                }
                                t = *(s32 *)((char *)data_ov006_0212e1c0 + *(s32 *)(p + 0x53d4) * 0x1c);
                                switch (t) {
                                case 0:
                                    break;
                                case 1:
                                    if ((*(s32 *)(p + 0x5374) % 2) == 0) {
                                        *(s32 *)(p + 0x5368) += 5;
                                    }
                                    break;
                                case 2:
                                    *(s32 *)(p + 0x5368) += 5;
                                    break;
                                }
                                if (*(s32 *)(p + 0x5368) > 0x64) {
                                    *(s32 *)(p + 0x5368) = 0x64;
                                }
                                if (*(s32 *)(p + 0x5374) >= 5) {
                                    *(s32 *)(p + 0x53c0) = 0x3c;
                                    *(s32 *)(p + 0x46d0) = 2;
                                    *(u8 *)(p + 0x46d4) = 1;
                                    return;
                                }
                                *(s32 *)(p + 0x46d0) = 0;
                            }
                            return;
                        }

                        if (AMIDA(p)->Unk36() != 0) {
                            func_02012dbc(5);
                            if (*(u8 *)(p + 0x46d5) == 0) {
                                func_02012718(0x1c2, *(s32 *)(p + idx * 8 + 0x4660) << 0xc);
                            } else {
                                func_02012718(0x1c3, *(s32 *)(p + idx * 8 + 0x4660) << 0xc);
                            }
                            *flag = 1;
                            *(u8 *)(p + 0x46d5) = 1;
                            *(s32 *)(p + 0x46cc) += 1;
                            if (*(s32 *)(p + 0x46cc) >= *(s32 *)(p + 0x46c8)) {
                                *(s32 *)(p + 0x53e0) = 0x3c;
                            }
                            return;
                        }
                        func_02012718(0xe1, *(s32 *)(p + idx * 8 + 0x4660) << 0xc);
                        *(s32 *)(p + 0x53c0) = 0x3c;
                        *(s32 *)(p + 0x46d0) = 2;
                        *(u8 *)(p + 0x46d5) = 1;
                        return;
                    }
                    continue;
                }

                if (dist < -0xc0 || dist >= 0x98) {
                    *distp += 1;
                } else {
                    u8 *flag2 = (u8 *)(p + idx + 0x4680);
                    if ((u32)*flag2 <= 1) {
                        *distp = dist + 1;
                        if (func_ov006_020d25fc(p, idx, c3) == 0) {
                            func_ov006_020d25fc(p, idx, c4);
                        }
                    } else {
                        tmp = ((dist + 0xd4) * 0x1f4) / (lim + 0xd4);
                        s32 *statePtr = (s32 *)(p + idx * 4 + 0x4684);
                        u32 state = *statePtr;
                        switch (state) {
                        default:
                            break;
                        case 0:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x21 || *(s32 *)(p + idx * 8 + 0x4660) == 0x61 ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0xa1 || *(s32 *)(p + idx * 8 + 0x4660) == 0xe1) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp -= 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v28, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, v2c) == 0 && func_ov006_020d25fc(p, idx, c1) == 0 &&
                                       func_ov006_020d25fc(p, idx, c3) == 0 && func_ov006_020d25fc(p, idx, v30) == 0) {
                                if (func_ov006_020d25fc(p, idx, v34) == 0) {
                                    *statePtr = v38;
                                }
                            }
                            break;
                        case 1:
                            if (func_ov006_020d25fc(p, idx, c1) == 0 && func_ov006_020d25fc(p, idx, v3c) == 0 &&
                                func_ov006_020d25fc(p, idx, v34) == 0 && func_ov006_020d25fc(p, idx, c3) == 0) {
                                if (func_ov006_020d25fc(p, idx, c4) == 0) {
                                    *statePtr = c6;
                                }
                            }
                            break;
                        case 2:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x1f || *(s32 *)(p + idx * 8 + 0x4660) == 0x5f ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0x9f || *(s32 *)(p + idx * 8 + 0x4660) == 0xdf) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp += 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v40, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, v34) == 0 && func_ov006_020d25fc(p, idx, c1) == 0 &&
                                       func_ov006_020d25fc(p, idx, c4) == 0 && func_ov006_020d25fc(p, idx, v44) == 0) {
                                if (func_ov006_020d25fc(p, idx, v38) == 0) {
                                    *statePtr = v30;
                                }
                            }
                            break;
                        case 3:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x21 || *(s32 *)(p + idx * 8 + 0x4660) == 0x61 ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0xa1 || *(s32 *)(p + idx * 8 + 0x4660) == 0xe1) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp -= 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v48, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, c3) == 0 && func_ov006_020d25fc(p, idx, v4c) == 0 &&
                                       func_ov006_020d25fc(p, idx, v30) == 0 && func_ov006_020d25fc(p, idx, c1) == 0) {
                                if (func_ov006_020d25fc(p, idx, c6) == 0) {
                                    *statePtr = c4;
                                }
                            }
                            break;
                        case 4:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x1f || *(s32 *)(p + idx * 8 + 0x4660) == 0x5f ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0x9f || *(s32 *)(p + idx * 8 + 0x4660) == 0xdf) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp += 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v50, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, c4) == 0 && func_ov006_020d25fc(p, idx, v34) == 0 &&
                                       func_ov006_020d25fc(p, idx, v38) == 0 && func_ov006_020d25fc(p, idx, c1) == 0) {
                                if (func_ov006_020d25fc(p, idx, c6) == 0) {
                                    *statePtr = c3;
                                }
                            }
                            break;
                        case 5:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x21 || *(s32 *)(p + idx * 8 + 0x4660) == 0x61 ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0xa1 || *(s32 *)(p + idx * 8 + 0x4660) == 0xe1) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp -= 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v54, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, v30) == 0 && func_ov006_020d25fc(p, idx, c6) == 0 &&
                                       func_ov006_020d25fc(p, idx, c3) == 0 && func_ov006_020d25fc(p, idx, v58) == 0) {
                                if (func_ov006_020d25fc(p, idx, v38) == 0) {
                                    *statePtr = v34;
                                }
                            }
                            break;
                        case 6:
                            if (func_ov006_020d25fc(p, idx, c6) == 0 && func_ov006_020d25fc(p, idx, v30) == 0 &&
                                func_ov006_020d25fc(p, idx, v38) == 0 && func_ov006_020d25fc(p, idx, c3) == 0) {
                                if (func_ov006_020d25fc(p, idx, c4) == 0) {
                                    *statePtr = c1;
                                }
                            }
                            break;
                        case 7:
                            if (*(s32 *)(p + idx * 8 + 0x4660) == 0x1f || *(s32 *)(p + idx * 8 + 0x4660) == 0x5f ||
                                *(s32 *)(p + idx * 8 + 0x4660) == 0x9f || *(s32 *)(p + idx * 8 + 0x4660) == 0xdf) {
                                {
                                    s32 *dirp = (s32 *)((int)(p + idx * 8) + 0x4660);
                                    *dirp += 1;
                                    *statePtr = c6;
                                    *flag2 = c1;
                                    func_020126ac(v24, c6, v5c, tmp, func_020126e8(*dirp << 0xc));
                                }
                            } else if (func_ov006_020d25fc(p, idx, v38) == 0 && func_ov006_020d25fc(p, idx, c6) == 0 &&
                                       func_ov006_020d25fc(p, idx, c4) == 0 && func_ov006_020d25fc(p, idx, v30) == 0 &&
                                       func_ov006_020d25fc(p, idx, v34) == 0) {
                                *statePtr = v60;
                            }
                            break;
                        }
                    }
                }
            }
        } while (++idx < *(s32 *)(p + 0x46c8));
    }
}
#pragma pop

// @symbol func_ov006_020d3624
extern "C" void func_ov006_020d3624(void *) {
  char *mainChars = (char *)func_02054efc();
  MultiCopyHalf((char *)func_02054efc() + 0xc000, mainChars, 0x6000);
  char *subChars = G2S::GetBG0CharPtr();
  MultiCopyHalf(G2S::GetBG0CharPtr() + 0x6000, subChars, 0x6000);
}

// @symbol func_ov006_020d3668
/* The scene pointer its caller passes is never read. */
extern "C" void func_ov006_020d3668(void *) {
  char *mainChars = (char *)func_02054efc() + 0xc000;
  MultiCopyHalf((char *)func_02054efc(), mainChars, 0x6000);
  char *subChars = G2S::GetBG0CharPtr() + 0x6000;
  MultiCopyHalf(G2S::GetBG0CharPtr(), subChars, 0x6000);
}

// @symbol func_ov006_020d36a4
extern "C" void func_ov006_020d36a4(dScMgAmida_c *self)
{
    int i;
    int j;
    int k;
    int pick;
    int tmp;
    u32 rand;
    int again;
    u8 seenNow[4];
    u8 seenBefore[4];

    self->unk_53dd = 0;
    if (self->Unk36() != 0) {
        again = 1;
        for (i = 0; i < 4; i++) {
            self->unk_4694[i] = i;
        }
        do {
            for (k = 0; k < 4; k++) {
                rand = (u32)RandomIntInternal(&data_0209d4b8) >> 16;
                pick = ((rand & 0x7fff) * 4) >> 15;
                tmp = self->unk_4694[k];
                self->unk_4694[k] = self->unk_4694[pick];
                self->unk_4694[pick] = tmp;
            }
            if (self->mRoundCount == 0) {
                again = 0;
                switch (self->mPatternIndex) {
                case 0:
                    self->unk_46a4[0] = 0;
                    self->unk_46a4[1] = 0;
                    self->unk_46a4[2] = 3;
                    self->unk_46a4[3] = 3;
                    break;
                case 1:
                case 3:
                case 5:
                    self->unk_46a4[0] = 0;
                    self->unk_46a4[1] = 0;
                    self->unk_46a4[2] = 1;
                    self->unk_46a4[3] = 3;
                    break;
                default:
                    for (i = 0; i < 4; i++) {
                        self->unk_46a4[i] = i;
                    }
                    break;
                }
                for (j = 0; j < 4; j++) {
                    data_ov006_02141640[self->unk_4694[j]] = self->unk_46a4[j];
                }
            } else {
                for (i = 0; i < 4; i++) {
                    data_ov006_02141650[i] = data_ov006_02141640[i];
                }
                for (j = 0; j < 4; j++) {
                    data_ov006_02141640[self->unk_4694[j]] = self->unk_46a4[j];
                }
                for (j = 0; j < 4; j++) {
                    if (data_ov006_02141650[j] != data_ov006_02141640[j]) {
                        again = 0;
                        break;
                    }
                }
            }
        } while (again == 1);
    } else if (self->unk_46c8 == 1) {
        if (self->mRoundCount == 0) {
            pick = ((((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 4) >> 15;
            tmp = self->unk_4694[0];
            self->unk_4694[0] = self->unk_4694[pick];
            self->unk_4694[pick] = tmp;
        } else {
            pick = (((((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 3) >> 15) + 1;
            tmp = self->unk_4694[0];
            self->unk_4694[0] = self->unk_4694[pick];
            self->unk_4694[pick] = tmp;
        }
    } else {
        again = 1;
        for (int n = 0; n < 4; n++) {
            seenBefore[n] = 0;
        }
        for (j = 0; j < self->unk_46c8; j++) {
            seenBefore[self->unk_4694[j]] = 1;
        }
        do {
            for (int n = 0; n < 4; n++) {
                seenNow[n] = 0;
            }
            for (i = 0; i < 4; i++) {
                self->unk_4694[i] = i;
            }
            for (k = 0; k < 4; k++) {
                rand = (u32)RandomIntInternal(&data_0209d4b8) >> 16;
                pick = ((rand & 0x7fff) * 4) >> 15;
                tmp = self->unk_4694[k];
                self->unk_4694[k] = self->unk_4694[pick];
                self->unk_4694[pick] = tmp;
            }
            for (j = 0; j < self->unk_46c8; j++) {
                seenNow[self->unk_4694[j]] = 1;
            }
            if (self->mRoundCount == 0) {
                again = 0;
            } else {
                for (int n = 0; n < 4; n++) {
                    if (seenNow[n] != seenBefore[n]) {
                        again = 0;
                        break;
                    }
                }
            }
        } while (again == 1);
    }

    for (i = 0; i < 4; i++) {
        if (self->Unk36() != 0) {
            self->unk_46b8[i] = 0;
        } else {
            self->unk_46b8[i] = i * *(s32 *)(data_ov006_0212e1a8 + self->mPatternIndex * 0x1c + 0x14) * 0x3c;
        }
        self->unk_4660[i][0] = (self->unk_4694[i] << 6) + 0x20;
        if (self->Unk36() != 0) {
            self->unk_4660[i][1] = -0xcc;
        } else {
            self->unk_4660[i][1] = -0xd4;
        }
        self->unk_4684[i] = -1;
        self->unk_4680[i] = 0;
        self->unk_46b4[i] = 0;
    }
    self->unk_46cc = 0;
    self->mRoundTimer = 0;
}

// @symbol func_ov006_020d3ba0
#pragma push
#pragma opt_strength_reduction off
extern "C" void func_ov006_020d3ba0(char *raw)
{
    int level;
    int shuffle;
    Pair rungs[11];
    int skip[11];

    AMIDA(raw)->mFinished = 0;
    AMIDA(raw)->unk_46d5 = 0;

    if (AMIDA(raw)->mScore > 0x270f) {
        AMIDA(raw)->mScore = 0x270f;
    }
    shuffle = 0;

    level = AMIDA(raw)->unk_0bc;
    if (AMIDA(raw)->Unk36() != 0) {
        if ((u32) AMIDA(raw)->unk_0bc >= 7) {
            level = 7;
            shuffle = 1;
        }
    } else if (level >= 0xa) {
        shuffle = 1;
        level = (((u32)(((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 5) >> 15) + 4;
        if (level == AMIDA(raw)->mPatternIndex) {
            level = 9;
        }
    }
    AMIDA(raw)->mPatternIndex = level;

    if (AMIDA(raw)->Unk36() != 0) {
        AMIDA(raw)->unk_46c8 = 4;
    } else {
        AMIDA(raw)->unk_46c8 = *(int *)(data_ov006_0212e1a8 + AMIDA(raw)->mPatternIndex * 0x1c);
    }

    AMIDA(raw)->mScrollSpeed = ((*(int *)(data_ov006_0212e1b0 + AMIDA(raw)->mPatternIndex * 0x1c) - 1) * 5) + 0xb;

    {
        u32 stage = AMIDA(raw)->unk_0bc;
        if (stage >= 0x13) {
            AMIDA(raw)->mScrollSpeed = (int)((stage / 10 - 1) * 5) + AMIDA(raw)->mScrollSpeed;
            if (AMIDA(raw)->mScrollSpeed > 0x64) {
                AMIDA(raw)->mScrollSpeed = 0x64;
            }
        }
    }

    {
        char *d;
        volatile u16 v1;
        volatile u16 v2;
        d = (char *)func_02054efc();
        v1 = 0;
        MultiStore16(v1, d, 0x6000);
        d = G2S::GetBG0CharPtr();
        v2 = 0;
        MultiStore16(v2, d, 0x6000);
    }

    AMIDA(raw)->mLineStart.x = -1;
    AMIDA(raw)->mLineStart.y = -1;
    AMIDA(raw)->mLineEnd.x = -1;
    AMIDA(raw)->mLineEnd.y = -1;
    AMIDA(raw)->mPen.x = -1;
    AMIDA(raw)->mPen.y = -1;
    AMIDA(raw)->mPenStart.x = -1;
    AMIDA(raw)->mPenStart.y = -1;

    {
        int z = 0;
        AMIDA(raw)->mLineStartSet = z;
        AMIDA(raw)->mLineEndSet = z;
        AMIDA(raw)->mProbeMode = z;
        AMIDA(raw)->mProbeHit = z;
        AMIDA(raw)->mPenTapped = z;
        AMIDA(raw)->mPenSoundHandle = z;
        do {
            AMIDA(raw)->unk_4694[z] = z;
            z += 1;
        } while (z < 4);
    }

    AMIDA(raw)->mRoundCount = 0;
    func_ov006_020d36a4(AMIDA(raw));
    func_ov006_020d122c(raw, *(int *)(data_ov006_0212e1ac + AMIDA(raw)->mPatternIndex * 0x1c));
    func_ov004_020b04d0(0x20);

    AMIDA(raw)->unk_4707 = 1;
    AMIDA(raw)->mLineCount = 1;
    /* Both buffers are cleared through a raw read of the pointer: indexing
       unk_470c and unk_4710 as members changes the code. */
    {
        int i, j, off;
        for (i = 0, off = 0; i < 0x100; i++, off += 0x158) {
            for (j = 0; j < 0x158; j++) {
                *(*(u8 **)(raw + 0x470c) + off + j) = 0;
                *(*(u8 **)(raw + 0x4710) + off + j) = 0;
            }
        }
    }

    if (AMIDA(raw)->Unk36() != 0) {
        func_ov004_020ae5c4(raw, 0x20, -0xb4, 0x20, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0x60, -0xb4, 0x60, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0xa0, -0xb4, 0xa0, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0xe0, -0xb4, 0xe0, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
    } else {
        func_ov004_020ae5c4(raw, 0x20, -0xd4, 0x20, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0x60, -0xd4, 0x60, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0xa0, -0xd4, 0xa0, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
        func_ov004_020ae5c4(raw, 0xe0, -0xd4, 0xe0, AMIDA(raw)->mLineEndY, AMIDA(raw)->unk_53e4, 1);
    }

    AMIDA(raw)->mLineCount++;
    {
        typedef struct { Pair e[11]; } Blk88;
        *(Blk88 *)rungs = *(Blk88 *)data_ov006_0213b8b8;
    }

    {
        int i;
        for (i = 0; i < 0xb; i++) {
            skip[i] = 0;
        }
    }

    if (shuffle == 1) {
        if (AMIDA(raw)->Unk36() != 0) {
            int msk = 0x7fff;
            int cnt = 3;
            do {
                int idx = ((u32)(((u32)RandomIntInternal(&data_0209d4b8) >> 16) & msk) * 0xb) >> 15;
                if (skip[idx] == 0) {
                    cnt--;
                    skip[idx] = 1;
                }
            } while (cnt > 0);
        } else {
            int sel = *(int *)(data_ov006_0212e1b4 + AMIDA(raw)->mPatternIndex * 0x1c);
            switch (sel) {
            case 0:
                break;
            case 1:
                skip[((u32)(((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 5) >> 15] = 1;
                break;
            case 2: {
                int cnt = (((u32)(((u32)RandomIntInternal(&data_0209d4b8) >> 16) & 0x7fff) * 3) >> 15) + 1;
                int j = 0;
                if (cnt > 0) {
                    int *seedp2 = &data_0209d4b8;
                    do {
                        skip[((u32)(((u32)RandomIntInternal(seedp2) >> 16) & 0x7fff) * 0xb) >> 15] = 1;
                        j++;
                    } while (j < cnt);
                }
                break;
            }
            default:
                break;
            }
        }
    }

    {
        int sel2;
        if (AMIDA(raw)->Unk36() != 0) {
            switch (AMIDA(raw)->mPatternIndex) {
            case 0:
            case 1:
            case 2:
                sel2 = 0;
                break;
            case 3:
            case 4:
                sel2 = 1;
                break;
            default:
                sel2 = 2;
                break;
            }
        } else {
            sel2 = *(int *)(data_ov006_0212e1b4 + AMIDA(raw)->mPatternIndex * 0x1c);
        }

        switch (sel2) {
        case 0:
            break;
        case 1: {
            int i;
            for (i = 0; i < 5; i++) {
                if (skip[i] == 0) {
                    int x = rungs[i].a;
                    int y = rungs[i].b - 0x20;
                    func_ov004_020ae5c4(raw, x, y, x + 0x40, y, AMIDA(raw)->unk_53e4, 1);
                }
            }
            break;
        }
        case 2:
        case 3: {
            int i;
            for (i = 0; i < 0xb; i++) {
                if (skip[i] == 0) {
                    int x = rungs[i].a;
                    int y = rungs[i].b - 0x20;
                    func_ov004_020ae5c4(raw, x, y, x + 0x40, y, AMIDA(raw)->unk_53e4, 1);
                }
            }
            break;
        }
        default:
            break;
        }
    }

    AMIDA(raw)->mLineCount++;
    if (AMIDA(raw)->Unk36() == 0) {
        int v = *(int *)(data_ov006_0212e1b8 + AMIDA(raw)->mPatternIndex * 0x1c);
        if (v != 0 && v == 1) {
            func_ov004_020ae5c4(raw, 0x60, 0x2d, 0xa0, 0x2d, AMIDA(raw)->unk_53e4, 1);
            func_ov004_020ae5c4(raw, 0x20, 0x5a, 0x60, 0x5a, AMIDA(raw)->unk_53e4, 1);
            func_ov004_020ae5c4(raw, 0xa0, 0x5a, 0xe0, 0x5a, AMIDA(raw)->unk_53e4, 1);
        }
    }

    AMIDA(raw)->unk_4707 = 0;
    AMIDA(raw)->mLineCount++;
    {
        int i, j, off;
        for (i = 0, off = 0; i < 0x100; i++, off += 0x158) {
            for (j = 0; j < 0x158; j++) {
                *(*(u8 **)(raw + 0x470c) + off + j) = 0;
            }
        }
    }

    func_ov006_020d3668(raw);
    *(s32 *)(raw + 0x5370) = 1;
    *(volatile u16 *)0x04000050 = 0;
    _ZN3G2x13SetBlendAlphaEPVttttj((void *)0x04001050, 4, 8, 6, 0x10);
    func_ov004_020b0cac(0xd, 0x80, 0x60, 1, -1, 0xd);

    AMIDA(raw)->mStartBannerTimer = 0x3c;
    AMIDA(raw)->mState = 1;
    {
        int i = 0;
        for (; i < 4; i++) {
            ((s32 *)(raw + 0x5378))[i] = 0;
            ((s32 *)(raw + 0x5388))[i] = 0;
            ((u8 *)(raw + 0x5398))[i] = 0;
            AMIDA(raw)->mLaneAnimTimer[i] = 0;
            AMIDA(raw)->mLaneAnimFrame[i] = 0;
        }
        AMIDA(raw)->mBgScrollPhase = 0;
        AMIDA(raw)->mEndDelayTimer = 0;
        *(s32 *)(raw + 0x53d8) = 0;
        AMIDA(raw)->unk_53dc = 0;
        AMIDA(raw)->unk_53dd = 0;
        AMIDA(raw)->unk_53de = 0;
        {
            int z = 0;
            int xw[2];
            xw[0] = 0x20;
            xw[1] = 0;
            for (; z < 4; z++) {
                AMIDA(raw)->mLanePos[z][0] = xw[0] << 12;
                AMIDA(raw)->mLanePos[z][1] = 0xb0000;
                AMIDA(raw)->mLaneVel[z][0] = xw[1];
                AMIDA(raw)->mLaneVel[z][1] = xw[1];
                xw[0] += 0x40;
            }
            AMIDA(raw)->unk_4764 = xw[1];
            {
                int v = 0;
                for (; v < 0x80; v++) {
                    AMIDA(raw)->mPieces[0].active = 0;
                    AMIDA(raw)->mPieces[0].timer = 0;
                    raw += 0x18;
                }
            }
        }
    }
}
#pragma pop

// @symbol func_ov006_020d452c
#pragma push
#pragma opt_strength_reduction off
#pragma opt_common_subs off
extern "C" void func_ov006_020d452c(void *scene)
{
    typedef struct { int a[13]; } T13;
    unsigned char *raw = (unsigned char *)scene;
    int noPalette = -1;
    int i;
    int y;

    i = 0;
    y = 0x20;
    for (; i < 4; y += 0x40, i++) {
        int *lane = (int *)(raw + i * 4);
        if (*(int *)((char *)lane + 0x4714) != 0) {
            int *animTimer = (int *)(((int)lane + 0x539c));
            (*animTimer)++;
            if (*animTimer >= 6) {
                int *animFrame = (int *)(((int)lane + 0x53ac));
                *animTimer = 0;
                (*animFrame)++;
                if (*animFrame >= 14)
                    *animFrame = 0;
            }
            func_ov004_020afdd0(data_ov006_0213a458[AMIDA(raw)->mLaneAnimFrame[i]],
                                AMIDA(raw)->mLanePos[i][0] >> 12,
                                (AMIDA(raw)->mLanePos[i][1] >> 12) - 4,
                                noPalette, 0);
        } else {
            int *timer;
            int *frame;
            int frames[13];
            if (*(raw + i + 0x5398) == 0) {
                timer = (int *)(((int)lane + 0x5378));
                frame = (int *)(((int)lane + 0x5388));
                (*timer)++;
                if (*frame >= 12) {
                    if (*timer >= 6) {
                        *timer = 0;
                        *frame = 0;
                    }
                } else if (*timer >= 4) {
                    *timer = 0;
                    (*frame)++;
                }
            } else {
                timer = (int *)(((int)lane + 0x5378));
                frame = (int *)(((int)lane + 0x5388));
                (*timer)++;
                if (*frame >= 12) {
                    if (*timer >= 5) {
                        *timer = 0;
                        *frame = 0;
                    }
                } else if (*timer >= 2) {
                    *timer = 0;
                    (*frame)++;
                }
            }
            *(T13 *)frames = *(T13 *)data_ov006_0213b84c;
            func_ov004_020afdd0(data_ov006_0213a390[frames[*frame]],
                                y, 0xb8, noPalette, 0);
        }
    }
    {
        int j;
        for (j = 0; j < AMIDA(raw)->unk_46c8; j++) {
            func_ov004_020afdd0(data_ov006_0213a32c,
                                AMIDA(raw)->unk_4660[j][0],
                                AMIDA(raw)->unk_4660[j][1],
                                -1, 0);
        }
    }
    func_ov004_020afdd0(data_ov006_0213a35c, 8, 0x60, -1, 0);
    func_ov004_020afdd0(data_ov006_0213a35c, 0xf8, 0x60, -1, 0);
}
#pragma pop

// @symbol func_ov006_020d47f4
#pragma push
#pragma opt_strength_reduction off
#pragma opt_common_subs off
extern "C" void func_ov006_020d47f4(char *raw){
  int noPalette = -1;
  int i;
  int j;
  int x;
  if(AMIDA(raw)->unk_53dd==0){
    func_ov004_020afdd0((void*)data_ov006_0213a338[0],0x80,0xb0,noPalette,0);
  }
  for(i=0;i<4;i++){
    func_ov004_020afdd0((void*)data_ov006_0213a568[AMIDA(raw)->unk_46a4[i]],
                        AMIDA(raw)->unk_4660[i][0],
                        AMIDA(raw)->unk_4660[i][1],
                        noPalette,0);
  }
  j = 0;
  x = 0x20;
  for(; j < 4; x += 0x40, j++){
    func_ov004_020afdd0((void*)data_ov006_0213a4c0[AMIDA(raw)->unk_4714[j]],
                        x,0x78,noPalette,0);
  }
}
#pragma pop

// @symbol _ZN12dScMgAmida_c6RenderEv
/* These two pragmas are load-bearing: without them Render compiles 0xc
 * bytes larger than the ROM (0x2ac vs 0x2a0) and rombuild drops to 102/106. */
#pragma push
#pragma opt_strength_reduction off
#pragma opt_common_subs off
s32 dScMgAmida_c::Render()
{
    char *raw = (char *)this;

    mBgScrollPhase += 0xc0;
    {
        u16 idx = mBgScrollPhase;
        int sine = data_02082214[(idx >> 4) << 1];
        int off = (sine + (int)((unsigned)(sine >> 7) >> 24)) >> 8;
        SetSubBg2Offset(off, 0);
    }
    func_ov004_020b1e34(raw, 0xe0, 0x14, 1);

    if (mState == 3 && mEndDelayTimer == 0) {
        data_0209d45c &= ~1;
        data_0209d454 &= ~1;
        if (Unk36() == 0 && mFinished == 1) {
            int i;
            for (i = 0; i < 4; i++) {
                if (unk_4714[i] != 0) {
                    int *counterA = (int*)(raw + i * 4 + 0x539c);
                    (*counterA)++;
                    Buf14 periods = data_ov006_0213b880;
                    if (*counterA >= periods.v[i]) {
                        int *counterB;
                        *counterA = 0;
                        counterB = (int*)(raw + i * 4 + 0x53ac);
                        (*counterB)++;
                        if (*counterB >= 0xe)
                            *counterB = 0;
                    }
                    func_ov004_020afdd0(
                        data_ov006_0213a458[mLaneAnimFrame[i]],
                        mLanePos[i][0] >> 12,
                        (mLanePos[i][1] >> 12) - 4,
                        -1,
                        0);
                }
            }

            int piece = 0;
            int zero = 0;
            int noPalette = -1;
            for (; piece < 0x80; piece++, raw += 0x18) {
                if (*(u8*)(raw + 0x477c) != 0) {
                    int age = *(int*)(raw + 0x4778);
                    int x = *(int*)(raw + 0x4768);
                    int idx = age / 4;
                    int y = *(int*)(raw + 0x476c);
                    func_ov004_020afdd0(data_ov006_0213a4b0[idx], x >> 12, y >> 12, noPalette, zero);
                }
            }

        }
        return 1;
    } else {
        data_0209d45c |= 1;
        data_0209d454 |= 1;
        if (Unk36() != 0) {
            func_ov006_020d47f4(raw);
        } else {
            func_ov006_020d452c(raw);
        }
        return 1;
    }
}
#pragma pop

// @symbol _ZN12dScMgAmida_c8BehaviorEv
#pragma push
#pragma opt_strength_reduction off
s32 dScMgAmida_c::Behavior()
{
    char *raw = (char *)this;
    int piece;
    char *piecePtr;
    int *vel;
    int speed;
    int steps;
    int k;
    int accum;
    int touch, j4, touched;
    int lane;
    int j;
    int slot;
    char *scan;
    int randA, randB;
    u32 bv, av;
    s16 cA;
    int idxA;
    int cosB, sinA;

    switch (mState) {
    case 0:
        if (mFinished == 1) {
            func_ov006_020d3ba0((char *)this);
        } else if (unk_46d5 == 1) {
            func_ov006_020d3ba0((char *)this);
        } else {
            func_ov006_020d36a4(this);
        }
        mState = 1;
        // fall through
    case 1:
        func_ov006_020d1a3c(raw);
        func_ov006_020d1958(raw);
        func_ov006_020d1ba0(AMIDA(raw));
        if (Unk36() != 0) {
            if (unk_53dd == 1) {
                mScrollAccum += mRoundCount * 5 + 0x20;
            }
        } else if (unk_53dc == 1) {
            mScrollAccum += 0x64;
        } else {
            mScrollAccum += mScrollSpeed;
        }
        accum = mScrollAccum;
        mScrollAccum &= 0xf;
        steps = accum / 16;
        if (Unk36() == 0 || unk_53dd != 0) {
            if (mRoundTimer > 0) {
                mRoundTimer -= 1;
                if (mRoundTimer == 0) {
                    if (unk_46d5 == 1) {
                        mResultWaitTimer = 0x3c;
                        mState = 2;
                    } else if (mRoundCount < 5) {
                        mState = 0;
                    } else {
                        mResultWaitTimer = 0x3c;
                        mState = 2;
                        mFinished = 1;
                    }
                }
            }
            k = 0;
            if (steps > 0) {
                do {
                    if (mState != 1)
                        break;
                    func_ov006_020d27dc(raw);
                    k++;
                } while (k < steps);
            }
        }
        if (mStartBannerTimer > 0) {
            mStartBannerTimer -= 1;
            if (mStartBannerTimer <= 0) {
                FreeGfxSlotsById(0xd);
                if (mPromptBlinkCount == 0) {
                    mPromptEnabled = 1;
                    mPromptBlinkCount = 1;
                    mPromptBlinkTimer = 0;
                }
            }
        }
        func_ov004_020adb1c(mScore);
        mHudScore = mScore;
        break;
    case 2:
        unk_53dc = 0;
        if (Unk36() != 0) {
            if (mResultWaitTimer > 0)
                mResultWaitTimer -= 1;
        } else {
            if (mResultWaitTimer > 0)
                mResultWaitTimer -= 1;
        }
        if (mResultWaitTimer != 0)
            break;
        mPromptEnabled = 0;
        if (mFinished == 1) {
            mState = 3;
            func_ov004_020b0a54(0);
        } else {
            mState = 3;
            func_ov004_020b0a54(0x12);
            mEndDelayTimer = 0xb4;
            unk_4764 = 0;
        }
        break;
    case 3:
        if (mEndDelayTimer > 0)
            mEndDelayTimer -= 1;
        touched = 0;
        touch = data_020a0e40;
        j4 = touch * 4;
        if (data_020a0de8[touch * 4] != 0) {
            if (data_020a0de9[j4] != 0)
                touched = 1;
        }
        if (touched != 0)
            mEndDelayTimer = 0;
        if (Unk36() != 0)
            break;
        if (mFinished != 1)
            break;
        unk_4764 += 1;
        lane = 0;
        do {
            mLaneVel[lane][1] -= 0x100;
            mLanePos[lane][0] += mLaneVel[lane][0];
            mLanePos[lane][1] += mLaneVel[lane][1];
            lane++;
        } while (lane < 4);

        piecePtr = raw;
        vel = &mPieces[0].velX;
        for (piece = 0; piece < 0x80; piece++) {
            if (PIECE_AT(piecePtr)->active != 0) {
                IAP(piecePtr, 0x4768) += IP(piecePtr, 0x4770);
                IAP(piecePtr, 0x476c) += IP(piecePtr, 0x4774);
                speed = Vec2_Len(vel) * 7 / 8;
                if (func_0203d434(vel) != 0)
                    func_0203d630(vel, speed);
                *(int*)AT(piecePtr, 0x4778) = *(int*)AT(piecePtr, 0x4778) + 1;
                {
                    if (PIECE_AT(piecePtr)->timer >= 0x10)
                        PIECE_AT(piecePtr)->active = 0;
                }
            }
            piecePtr += 0x18;
            vel += 6;
        }

        for (j = 0; j < 4; j++) {
            if (unk_4714[j] != 0) {
                slot = 0;
                scan = raw;
                do {
                    if (PIECE_AT(scan)->active != 1) {
                        randA = RandomIntInternal(&data_0209d4b8);
                        randB = RandomIntInternal(&data_0209d4b8);
                        bv = ((u32)randB >> 16) & 0x7fff;
                        av = ((u32)randA >> 16) & 0x7fff;
                        cosB = data_02082214[((int)((bv << 15) >> 16) >> 4) * 2 + 1];
                        idxA = ((int)((av << 17) >> 16) >> 4) * 2;
                        cA = data_02082214[idxA + 1];
                        {
                            mPieces[slot].posX = MULFX(cosB, (int)((0x8000LL * cA + 0x800) >> 12));
                            sinA = data_02082214[idxA];
                            mPieces[slot].posY = MULFX(cosB, (int)((0x8000LL * sinA + 0x800) >> 12));
                            mPieces[slot].posX += mLanePos[j][0];
                            mPieces[slot].posY += mLanePos[j][1];
                            mPieces[slot].velX = MULFX(cosB, (int)((0x1000LL * (int)(cA) + 0x800) >> 12));
                            mPieces[slot].velY = MULFX(cosB, (int)((0x1000LL * sinA + 0x800) >> 12));
                        }
                        mPieces[slot].velY -= 0x400;
                        mPieces[slot].active = 1;
                        mPieces[slot].timer = 0;
                        break;
                    }
                    slot++;
                    scan += 0x18;
                } while (slot < 0x80);
            }
        }
        break;
    }
    return 1;
}
#pragma pop

// @symbol _ZN12dScMgAmida_c13OnYoshiTryEatEi
/* Slot 18: counts a win (arg 0) or resets the score, then sets up the next board. */
void dScMgAmida_c::OnYoshiTryEat(int arg)
{
    if (arg == 0) {
        if (unk_0bc >= 0x7cf)
            goto final;
        unk_0bc = unk_0bc + 1;
        if (unk_0bc > 0x270e)
            unk_0bc = 0x270e;
    } else {
        mScore = 0;
        unk_0bc = 0;
        if (unk_0bc > 0x270e)
            unk_0bc = 0x270e;
        func_ov004_020adb1c(mScore);
        mHudScore = mScore;
    }
final:
    func_ov006_020d3ba0((char *)this);
}

// @symbol _ZN12dScMgAmida_c13InitResourcesEv
s32 dScMgAmida_c::InitResources()
{
    volatile u16 zeroMain;
    volatile u16 zeroSub;
    void *file;

    unk_470c = (u8 *)Memory::Allocate(0x15800);
    unk_4710 = (u8 *)Memory::Allocate(0x15800);

    if (Unk36() != 0) {
        mLineEndY = 0x78;
        unk_53e4 = 2;
    } else {
        mLineEndY = 0x98;
        unk_53e4 = 2;
    }

    data_0208ee44 = 1;
    *(volatile u16 *)0x4000008 = (*(volatile u16 *)0x4000008 & 0x43) | 0x3218;
    *(volatile u16 *)0x4000008 &= ~0x40;
    *(volatile s32 *)0x4000010 = 0;
    *(volatile u16 *)0x4000008 = (*(volatile u16 *)0x4000008 & ~3) | 1;

    file = func_02054efc();
    zeroMain = 0;
    MultiStore16((u16)zeroMain, file, 0x6000);

    *(volatile u16 *)0x4001008 = (*(volatile u16 *)0x4001008 & 0x43) | 0x2214;
    *(volatile u16 *)0x4001008 &= ~0x40;
    *(volatile s32 *)0x4001010 = 0;
    *(volatile u16 *)0x4001008 = (*(volatile u16 *)0x4001008 & ~3) | 1;

    file = G2S::GetBG0CharPtr();
    zeroSub = 0;
    MultiStore16((u16)zeroSub, file, 0x6000);

    func_ov004_020af2f8((char *)this, 1, 0, 2);

    *(volatile u16 *)0x400000e = (*(volatile u16 *)0x400000e & 0x43) | 0x140c;
    *(volatile u16 *)0x400000e &= ~0x40;
    *(volatile s32 *)0x400001c = 0;
    *(volatile u16 *)0x400000e = (*(volatile u16 *)0x400000e & ~3) | 3;

    if (Unk36() != 0) {
        LoadCompressedFileAt(0x1f, (void *)func_02054d88());
        file = (void *)LoadFile(0x20);
        CP15::FlushAndInvalidateDataCache((u32)file, 0x1a0);
        GX::LoadBGPltt(file, 0x60, 0x1a0);
        Deallocate(file);
        LoadCompressedFileAt(0x21, _ZN2G212GetBG3ScrPtrEv());
    } else {
        LoadCompressedFileAt(0x15, (void *)func_02054d88());
        file = (void *)LoadFile(0x16);
        CP15::FlushAndInvalidateDataCache((u32)file, 0x1a0);
        GX::LoadBGPltt(file, 0x60, 0x1a0);
        Deallocate(file);
        LoadCompressedFileAt(0x17, _ZN2G212GetBG3ScrPtrEv());
    }

    *(volatile u16 *)0x400100c = (*(volatile u16 *)0x400100c & 0x43) | 0x40c;
    *(volatile u16 *)0x400100c &= ~0x40;
    *(volatile s32 *)0x4001018 = 0;
    *(volatile u16 *)0x400100c &= ~3;
    *(volatile u16 *)0x400100e = (*(volatile u16 *)0x400100e & 0x43) | 0x60c;
    *(volatile u16 *)0x400100e &= ~0x40;
    *(volatile s32 *)0x400101c = 0;
    *(volatile u16 *)0x400100e = (*(volatile u16 *)0x400100e & ~3) | 3;

    if (Unk36() != 0) {
        LoadCompressedFileAt(0x1c, G2S::GetBG3CharPtr());
        file = (void *)LoadFile(0x1d);
        GXS::LoadBGPltt(file, 0x60, 0x1a0);
        Deallocate(file);
        LoadCompressedFileAt(0x1e, G2S::GetBG3ScrPtr());
        data_0209d45c |= 9;
        data_0209d454 |= 9;
    } else {
        LoadCompressedFileAt(0x1a, G2S::GetBG3CharPtr());
        file = (void *)LoadFile(0x1b);
        GXS::LoadBGPltt(file, 0x60, 0x1a0);
        Deallocate(file);
        LoadCompressedFileAt(0x19, G2S::GetBG2ScrPtr());
        LoadCompressedFileAt(0x18, G2S::GetBG3ScrPtr());
        *(volatile s16 *)0x4000050 = 0;
        _ZN3G2x13SetBlendAlphaEPVttttj((void *)0x4001050, 4, 8, 6, 0x10);
        data_0209d45c |= 9;
        data_0209d454 |= 0xd;
    }

    if (Unk36() != 0) {
        if (GetOwnerLanguage() == 5) {
            LoadCompressedFileAt(0xf, (void *)0x6400000);
            LoadCompressedFileAt(0xf, (void *)0x6600000);
        } else if (GetOwnerLanguage() == 4) {
            LoadCompressedFileAt(0xc, (void *)0x6400000);
            LoadCompressedFileAt(0xc, (void *)0x6600000);
        } else if (GetOwnerLanguage() == 3) {
            LoadCompressedFileAt(0xb, (void *)0x6400000);
            LoadCompressedFileAt(0xb, (void *)0x6600000);
        } else if (GetOwnerLanguage() == 2) {
            LoadCompressedFileAt(0xa, (void *)0x6400000);
            LoadCompressedFileAt(0xa, (void *)0x6600000);
        } else {
            LoadCompressedFileAt(0xd, (void *)0x6400000);
            LoadCompressedFileAt(0xd, (void *)0x6600000);
        }
        file = (void *)LoadFile(0xe);
        GX::LoadOBJPltt(file, 0, 0x100);
        GXS::LoadOBJPltt(file, 0, 0x100);
        Deallocate(file);
    } else {
        LoadCompressedFileAt(0x10, (void *)0x6400000);
        LoadCompressedFileAt(0x10, (void *)0x6600000);
        file = (void *)LoadFile(0x11);
        GX::LoadOBJPltt(file, 0, 0x100);
        GXS::LoadOBJPltt(file, 0, 0x100);
        Deallocate(file);
    }

    mPatternIndex = 0;
    mScore = unk_0bc * 5;
    func_ov006_020d3ba0((char *)this);
    return 1;
}

// @symbol _ZN12dScMgAmida_c21AfterCleanupResourcesEj
/* Returns before the base call too when vfSuccess != 2, as the ROM does. */
void dScMgAmida_c::AfterCleanupResources(u32 vfSuccess)
{
    if (vfSuccess != 2)
        return;
    Memory::Deallocate(unk_470c);
    Memory::Deallocate(unk_4710);
    dScMgBase_c::AfterCleanupResources(vfSuccess);
}

extern int _ZTV12dScMgAmida_c[];

// @symbol dScMgAmida_c_classInit
/* Builds the scene by hand (operator new, base constructor, vtable,
   member arrays) because the class has no constructor declared yet.
   This file emits the vtable, so the store skips its two-word preamble
   to reach slot 0. */
extern "C" void *dScMgAmida_c_classInit(void) {
    char *scene = (char *)_ZN7fBase_cnwEj(0x53fc);
    if (scene != 0) {
        _ZN11dScMgBase_cC2Ev(scene);
        *(int *)scene = (int)&_ZTV12dScMgAmida_c[2];
        __cxa_vec_ctor(scene + 0x4660, 4, 8, (void *)func_0203d738, (void *)NullDestructor_0203d47c);
        __cxa_vec_ctor(scene + 0x4724, 4, 8, (void *)func_0203d738, (void *)NullDestructor_0203d47c);
        __cxa_vec_ctor(scene + 0x4744, 4, 8, (void *)func_0203d738, (void *)NullDestructor_0203d47c);
        __cxa_vec_ctor(scene + 0x4768, 0x80, 0x18, (void *)func_ov006_020d5a50, (void *)func_ov006_020d116c);
    }
    return scene;
}

// @symbol func_ov006_020d5a50
/* Element constructor for the 0x80 falling pieces. */
extern "C" void func_ov006_020d5a50(void)
{
}
