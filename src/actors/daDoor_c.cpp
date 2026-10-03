//cpp
/* daDoor_c -- the plain warp door (registry profile DOOR).
 *
 * param1's high halfword picks the door's entry in the 16-byte table at
 * data_ov100_02148204 (its model and key-model files, the star or key it
 * needs, the save-flag bit it sets and the message it shows). A door is a
 * state machine: the pointer at +0x140 (mCallbackNode) names the current
 * state, a pair of pointers-to-member -- enter, then execute -- that
 * func_ov100_021453d8 stores and calls the enter member of, and Behavior
 * calls the execute member of every frame with the player
 * func_ov100_02145370 returns. The nine states are the 16-byte tables
 * data_ov100_021488a4 .. data_ov100_02148924, which the module's static
 * initializer fills from the pointer-to-member constants at
 * 0x021480d4..0x02148144; every helper from func_ov100_02144468 to
 * func_ov100_02144cf8 is one of those members, the rest are what they call.
 *
 * This file is the whole linker unit 0x021443f4..0x021458d4, 32 functions:
 * D1 and D0 (daWanwan2_c_classInit, src/d_a_wanwan2.cpp, ends exactly at
 * 0x021443f4 below them), the 24 helpers func_ov100_02144468 through
 * func_ov100_021453d8, CleanupResources, OnPendingDestroy, Render,
 * Behavior, InitResources, and last the registry factory daDoor_c_classInit
 * (0x0214589c); daStarGate_c's first function
 * (src/game/actors/d_a_star_gate.cpp) starts exactly at 0x021458d4 above
 * it. The out-of-line destructor is the key function, so this TU also
 * emits the vtable and the RTTI.
 *
 * It replaces the one-function sources for _ZN8daDoor_cD1Ev,
 * _ZN8daDoor_cD0Ev, func_ov100_02144468 .. func_ov100_021453d8,
 * _ZN8daDoor_c16CleanupResourcesEv, _ZN8daDoor_c16OnPendingDestroyEv,
 * _ZN8daDoor_c6RenderEv, _ZN8daDoor_c8BehaviorEv,
 * _ZN8daDoor_c13InitResourcesEv and daDoor_c_classInit. Each member keeps
 * the provenance notes its source carried.
 *
 * `#pragma defer_codegen off` keeps this file in ROM order.
 *
 * Leftover: the helpers keep their C-ABI cartridge names and reach the door
 *   and the player through raw offsets; their Player calls are real member
 *   calls through include/Player.h, except Unk_020ca488 (see its declaration).
 * Leftover: the callees with Fix12<int> parameters (Sound, ModelAnim::SetAnim),
 *   the Animation, Camera and SaveData calls, and the Model loaders
 *   InitResources calls stay spelled as mangled extern-C free functions.
 * Leftover: the static vectors guarded by data_ov100_0214870c ..
 *   data_ov100_02148720 have the shape of function-local statics (guard bit,
 *   construct, register Vector3's destructor through func_020731dc); they
 *   stay spelled out against the cartridge's own objects.
 */

#pragma defer_codegen off

#include "daDoor_c.h"
#include "Player.h"
#include "types.h"
#include "common.h"
#include "SharedFilePtr.h"

/* Three plain words: the stack and static vectors of the helpers that were
   C, which carry none of Vector3's empty destructor. */
struct Vec3i { s32 x, y, z; };

/* The door's matrix, as twelve words: a copy through the C++ Matrix4x3 is
   the struct-copy DIFF of notes/cpp-class-form.md. */
struct M48 { int w[12]; };

/* One entry of data_ov100_02148204, indexed by param1. */
struct DoorEntry {
    SharedFilePtr *file;     /* 0x0: the door model */
    SharedFilePtr *keyFile;  /* 0x4: the model hung on the door, or null */
    s8  b8;                  /* 0x8: >= 0: the star count the door needs */
    s8  b9;                  /* 0x9: >= 0: a key door, the key's index */
    s8  flagShift;           /* 0xa: the door's bit in data_0209caa0's flags */
    s8  pad_b;
    s16 msg;                 /* 0xc: the message shown when it stays shut */
    s16 pad_e;
};

/* data_0209caa0, as far as the door reads it. */
struct GlobCaa0 { int w0; unsigned int flags; unsigned int w2; };

/* func_ov100_021453d8's view of the door: a non-polymorphic stand-in
   holding the state pointer at +0x140. The pointer-to-member it calls takes
   the player; Behavior calls the same pairs' execute half on the real
   daDoor_c. */
struct DoorPmfSelf;
typedef int (DoorPmfSelf::*DoorPmf)(int);
struct DoorPmfSelf {
    char pad[0x140];
    DoorPmf *state;          /* 0x140, daDoor_c::mCallbackNode */
};

struct Vector3_16;

extern "C" {
/* Defined below, called before their definitions. */
int func_ov100_02144950(char *c, char *pl, int unused);
int func_ov100_02144f84(void);
int func_ov100_02144fcc(void);
int func_ov100_02145014(void);
void func_ov100_02145070(int v);
int func_ov100_02145080(char *c, void *pl);
void func_ov100_02145170(char *door, char *pl, Vector3 *a, Vector3 *b);
int func_ov100_021451c4(char *r6, void *r5, char *r4);
int func_ov100_021452e4(char *door, char *pl);
void *func_ov100_02145370(char *c);
int func_ov100_021453d8(void *c, void *p, int a2);

void _ZN8SaveData17SetCharacterIntroEi(int);
int _ZN8SaveData22NumGlowingRabbitsFoundEv(void);
void _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(unsigned int a, int b);
int _ZN5Sound7PlaySubEjjj5Fix12IiEb(unsigned int, unsigned int, unsigned int, int, int);
void _ZN5Sound13PlayCharVoiceEjjRK7Vector3(unsigned int a, unsigned int b, void *pos);
void _ZN9Animation7AdvanceEv(void *a);
int _ZN9Animation8FinishedEv(void *a);
int _ZNK9Animation13GetFrameCountEv(void *a);
int _ZNK9Animation12WillHitFrameEi(void *a, int f);
void *_ZN9Animation8LoadFileER13SharedFilePtr(void *fp);
/* local extern: include/Player.h declares Player::Unk_020ca488 void, and
   func_ov100_021449c8 tests the value it leaves in r0. */
int _ZN6Player12Unk_020ca488Ev(void *p);
void _ZN6Camera14GoBehindPlayerEj(void *self, unsigned int a);
void *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
    u32 actorID, u32 param1, const Vector3 *pos, const Vector3_16 *rot,
    s32 areaID, s32 deathTableID);
void *_ZN5Model8LoadFileER13SharedFilePtr(void *fp);
void _ZN9ModelBase7SetFileEP8BMD_Fileii(void *self, void *f, int a, int b);
void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *self, void *f, int a, int fix, unsigned int j);
void *_ZN5ModelC1Ev(void *self);
void *_Znwj(unsigned int sz);
void func_0201277c(int a);
void func_02012790(int a);
int func_02012694(int a, void *b);
unsigned char DecIfAbove0_Byte(void *p);
int DecIfAbove0_Short(void *p);
int func_ov002_020ca78c(void *p);
void func_ov089_0213115c(char *actor, int a);
void func_020731dc(void *object, void *destructor, void *node);
void Vec3_RotateYAndTranslate(void *res, void *translation, short angY, void *v);
void Vec3_Sub(void *out, void *a, void *b);
void Vec3_Asr(void *d, void *s, int sh);
void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
void Matrix4x3_ApplyInPlaceToRotationY(void *m, short angY);
int AngleDiff(int a, int b);
unsigned char NumStars(void);
void ChangeArea(int areaID);
void ShowArea(int idx);
unsigned char IsAreaShowing(int idx);
void SetTouchScreenDelay(void);
void LoadKeyModels(int idx);
void UnloadKeyModels(int idx);
void *_ZN7Vector3D1Ev(void *object);

extern DoorEntry data_ov100_02148204[];
extern unsigned char data_ov100_021480d0[];
extern unsigned char data_ov100_02148704;
extern unsigned short data_ov100_02148708[];
extern unsigned int data_ov100_0214870c;
extern int data_ov100_02148710;
extern unsigned int data_ov100_02148714;
extern int data_ov100_02148718;
extern int data_ov100_0214871c;
extern int data_ov100_02148720;
extern SharedFilePtr data_ov100_02148744;
extern void *data_ov100_02148790;
extern Vec3i data_ov100_0214879c;
extern void *data_ov100_021487b4;
extern Vec3i data_ov100_021487c0;
extern int data_ov100_021487cc[];
extern void *data_ov100_021487d8;
extern int data_ov100_021487e4[];
extern Vec3i data_ov100_021487f0;
extern int data_ov100_021487fc[];
extern int data_ov100_02148808[];
extern void *data_ov100_02148850;
extern Vec3i data_ov100_02148880;
/* The nine states, each a 16-byte {enter, execute} pointer-to-member pair
   filled in by the module's static initializer. Declared as the int that
   include/decl_common.h gives two of them; only their addresses are used. */
extern int data_ov100_021488a4;
extern int data_ov100_021488b4;
extern int data_ov100_021488c4;
extern int data_ov100_021488d4;
extern int data_ov100_021488e4;
extern int data_ov100_021488f4;
extern int data_ov100_02148904;
extern int data_ov100_02148914;
extern int data_ov100_02148924;

extern GlobCaa0 data_0209caa0;
extern char *data_0209f318;
extern unsigned char data_0209f250;
extern char *data_0209f394[];
extern unsigned char data_0209f2d8;
extern char data_02092110;
extern int data_0209b490[];
extern int data_0209b49c[];
extern int data_020a0ebc;
extern M48 data_020a0e68;
extern int data_ov002_0211094c;
extern int data_ov089_02132c50;
/* ov089's key-model file table, not a function: eight SharedFilePtr*
   entries in that overlay's .rodata, the table LoadKeyModels loads into
   (bounds-checked `idx >= 8`). src/actors/daObjKey_c.cpp, home of
   LoadKeyModels and UnloadKeyModels, declares it under this name with this
   element type. */
extern void *data_ov089_02132894[];
}

/* -------------------------------------------------------------------------- */
/* ROM ordinals 0 and 1 -- _ZN8daDoor_cD1Ev, 0x021443f4, size 0x30;
 *                         _ZN8daDoor_cD0Ev, 0x02144424, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_cD1Ev
// @symbol _ZN8daDoor_cD0Ev
/* recovered: real C++ destructor -- the compiler emits the whole body.
 *
 * Store this class's vtable over the one the base constructor left, destroy
 * mModel, then run dActor_c's destructor; D0 adds the inline operator
 * delete. All of it follows from `struct daDoor_c : dActor_c` and the member
 * types in the header. */
daDoor_c::~daDoor_c()
{
}

#ifdef _MSC_VER
/* The host needs the ROM's flat D0 name, and MSVC never emits it: it folds
 * the Itanium destructor variants into the one ~daDoor_c() above. This arm
 * spells out what the deleting destructor does -- the D1 body, called
 * qualified so it is a direct call, then the class-specific operator
 * delete. Nothing here reaches mwccarm. */
extern "C" daDoor_c *_ZN8daDoor_cD0Ev(daDoor_c *thiz)
{
    thiz->daDoor_c::~daDoor_c();
    daDoor_c::operator delete(thiz);
    return thiz;
}
#endif

/* -------------------------------------------------------------------------- */
/* ROM ordinal 2 -- func_ov100_02144468, 0x02144468, size 0x80 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144468
/* State member: once func_ov100_02145080 lets the player out, clear the
   data_ov100_02148704 latch, set the character intro (or, for door type
   0xd, restore the music volume) and go to state data_ov100_021488b4.
   Was a C source. */
extern "C" int func_ov100_02144468(char *c, int p)
{
    if (!func_ov100_02145080(c, (void *)p)) {
        data_ov100_02148704 = 0;
        if (*(int *)(c + 8) != 0xd) {
            _ZN8SaveData17SetCharacterIntroEi(*(signed char *)(c + 0x100 + 0x44) - 1);
        } else {
            _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x7f, 0xcb33);
        }
        func_ov100_021453d8(c, &data_ov100_021488b4, p);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 3 -- func_ov100_021444e8, 0x021444e8, size 0x40 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021444e8
/* State member: go to state data_ov100_021488b4 once func_ov100_02144fcc
   reports its countdown done. Was a C source. */
extern "C" int func_ov100_021444e8(char *c, char *a1)
{
    if (func_ov100_02144fcc()) {
        func_ov100_021453d8(c, &data_ov100_021488b4, (int)a1);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov100_02144528, 0x02144528, size 0x1d0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144528
/* State member: run the door animation; when it finishes, switch the
   player to the door's character (or handle door type 0xd), set the
   +0x145 countdown, and when that runs out send the player through with
   func_ov100_021451c4. Was a C source. */
extern "C" int func_ov100_02144528(char *c, char *pl)
{
    _ZN9Animation7AdvanceEv(c + 0x124);
    if (_ZN9Animation8FinishedEv(c + 0x124) != 0) {
        if (*(unsigned char *)(c + 0x145) == 0) {
            if (*(int *)(c + 8) != 0xd) {
                unsigned int ch = data_ov100_021480d0[*(signed char *)(c + 0x144) - 1];
                if (ch == ((unsigned char *)&data_0209caa0)[0x41])
                    ch = 3;
                ((Player *)pl)->SetRealCharacter(ch);
                if (data_ov100_02148704 != 0) {
                    *(unsigned char *)(pl + 0x71a) = 1;
                    func_0201277c(0x7e);
                    *(unsigned char *)(c + 0x145) = 0x40;
                    _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0, 0x19666);
                } else {
                    func_0201277c(0x7f);
                }
            } else {
                *(unsigned char *)(c + 0x145) = 0x40;
                if (data_ov100_02148704 != 0)
                    func_02012790(0xa1);
                else
                    func_02012790(0xa2);
                _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0, 0x19666);
            }
            func_02012694(*(int *)(c + 8) == 0x10 ? 7 : 5, c + 0x74);
        }
        if (DecIfAbove0_Byte((unsigned char *)(c + 0x145)) == 0) {
            void *r1 = data_ov100_02148704 != 0 ? 0 : (void *)&data_ov100_021488f4;
            if (func_ov100_021451c4(c, r1, pl) != 0) {
                if (data_ov100_02148704 != 0)
                    ((Player *)pl)->OpenBigDoor();
            } else {
                unsigned char *q = (unsigned char *)(((int)c + 0x145));
                *q += 1;
            }
        } else {
            if (data_ov100_02148704 == 0
                && *(unsigned char *)(c + 0x145) == 0xa
                && *(int *)(c + 8) == 0xd) {
                _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x7f, 0xcb33);
            }
        }
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov100_021446f8, 0x021446f8, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021446f8
/* State member: open the big door and reset through func_ov100_02144950.
   Was a C source. */
extern "C" void func_ov100_021446f8(void *r0, void *r1)
{
    void *r4 = r1;
    void *r5 = r0;
    ((Player *)r4)->OpenBigDoor();
    *(u8 *)((char *)r5 + 0x145) = 0;
    func_ov100_02144950((char *)r5, (char *)r4, 0);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov100_02144730, 0x02144730, size 0x1ec */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144730
/* State member: the door swinging. Near the end of the animation it lets
   func_ov100_02145080 run; when the animation finishes it moves the player
   into the area on the far side (the low byte of mAngleX or mAngleZ,
   whichever side the player came from) and goes to state
   data_ov100_021488a4 or data_ov100_021488b4; on the way it plays the
   character voice and turns the camera behind the player. */
extern "C" int func_ov100_02144730(char *self, char *arg1)
{
    _ZN9Animation7AdvanceEv(self + 0x124);
    if (data_ov100_02148704 != 0) {
        int f = *(int *)(self + 0x12c);
        if ((unsigned)(f << 4) >> 16 > (u16)(_ZNK9Animation13GetFrameCountEv(self + 0x124) - 0x28))
            func_ov100_02145080(self, arg1);
    }

    if (_ZN9Animation8FinishedEv(self + 0x124) != 0) {
        int t;
        if (data_ov100_02148704 != 0)
            func_ov100_021453d8(self, &data_ov100_021488a4, (int)arg1);
        else
            func_ov100_021453d8(self, &data_ov100_021488b4, (int)arg1);
        if (*(int *)(self + 0x88) < 0)
            t = *(s16 *)(self + 0x8c);
        else
            t = *(s16 *)(self + 0x90);
        t = (s8)t;
        *(s8 *)(arg1 + 0xcc) = t;
        ChangeArea(t);
        {
            int *p = (int *)(data_0209f318 + 0x154);
            *p &= ~0xc00;
        }
        func_02012694(*(int *)(self + 8) == 0x10 ? 7 : 5, self + 0x74);
    } else if (*(s8 *)(self + 0x144) != 0) {
        if (_ZNK9Animation12WillHitFrameEi(self + 0x124, (u16)(_ZNK9Animation13GetFrameCountEv(self + 0x124) - 0x1c)) != 0) {
            int *p = (int *)(data_0209f318 + 0x154);
            *p &= ~0xc00;
            if (*(int *)(self + 8) != 0xd) {
                _ZN5Sound13PlayCharVoiceEjjRK7Vector3(((unsigned char *)&data_0209caa0)[0x41], 0x21, self + 0x74);
            } else if (data_ov100_02148704 == 0) {
                ((Player *)arg1)->PlayMammaMiaSound();
            }
        }
    } else {
        if (_ZNK9Animation12WillHitFrameEi(self + 0x124, (u16)(_ZNK9Animation13GetFrameCountEv(self + 0x124) - 0x18)) != 0) {
            if (*(s8 *)&data_02092110 >= 0)
                *(int *)(self + 0x130) = 0;
            _ZN6Camera14GoBehindPlayerEj(data_0209f318, data_0209f250);
        }
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov100_0214491c, 0x0214491c, size 0x34 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_0214491c
/* State member: start the door animation 0x18 frames from its end. Was a
   C source. */
extern "C" int func_ov100_0214491c(char *c)
{
    int n = _ZNK9Animation13GetFrameCountEv(c + 0x124);
    int f = (int)((unsigned short)(n - 0x18)) << 12;
    *(int *)(c + 0x12c) = f;
    *(unsigned char *)(c + 0x145) = 0;
    return 1;
}

/* func_ov100_02144950's halfword read, an inline function in its C source;
   the call is inlined away. */
static inline short HalfAt(char *arg0, int arg1)
{
    return *((short *)(arg0 + arg1));
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov100_02144950, 0x02144950, size 0x78 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144950
/* State member: rewind the door animation, show both areas the door joins
   (the low bytes of mAngleX and mAngleZ) and play the open sound. Was a C
   source; func_ov100_021446f8 calls it with the player and a third
   argument it never reads. */
extern "C" int func_ov100_02144950(char *c, char *pl, int unused)
{
    *((int *)(c + 0x12c)) = 0;
    ShowArea((signed char)HalfAt(c, 0x8c));
    ShowArea((signed char)HalfAt(c, 0x90));
    *(int *)((int)data_0209f318 + 0x154) |= 0xc00;
    func_02012694(((*((int *)(c + 8))) == 0x10) ? (6) : (4), c + 0x74);
    *((unsigned char *)(c + 0x145)) = 0;
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov100_021449c8, 0x021449c8, size 0x70 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021449c8
/* State member: unless the player is still busy, set this door's 0x100
   save-flag bit and go to state data_ov100_021488b4. */
extern "C" int func_ov100_021449c8(char *c, char *a2)
{
    if (!_ZN6Player12Unk_020ca488Ev(a2)) {
        signed char sh = (((signed char *)data_ov100_02148204) + (*(int *)(c + 8) << 4))[0xa];
        data_0209caa0.flags |= 0x100 << sh;
        func_ov100_021453d8(c, &data_ov100_021488b4, (int)a2);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov100_02144a38, 0x02144a38, size 0x1bc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144a38
/* State member: open the big door, place the player at the door's front or
   back (func_ov100_02145170), and spawn actor 0x11a beside the player. Was
   a C source. */
extern "C" int func_ov100_02144a38(char *c, char *p)
{
    Vec3i pos;
    char *e;

    ((Player *)p)->OpenBigDoor();

    if ((data_ov100_02148720 & 1) == 0) {
        data_ov100_02148880.x = 0x4b000;
        data_ov100_02148880.y = 0;
        data_ov100_02148880.z = 0x6e000;
        func_020731dc(&data_ov100_02148880, (void *)_ZN7Vector3D1Ev, &data_ov100_02148850);
        data_ov100_02148720 |= 1;
    }

    if ((data_ov100_02148718 & 1) == 0) {
        data_ov100_0214879c.x = 0x4b000;
        data_ov100_0214879c.y = 0;
        data_ov100_0214879c.z = -0x6e000;
        func_020731dc(&data_ov100_0214879c, (void *)_ZN7Vector3D1Ev, &data_ov100_02148790);
        data_ov100_02148718 |= 1;
    }

    func_ov100_02145170(c, p, (Vector3 *)&data_ov100_02148880, (Vector3 *)&data_ov100_0214879c);

    if ((data_ov100_0214871c & 1) == 0) {
        data_ov100_021487f0.x = -0x4c000;
        data_ov100_021487f0.y = 0;
        data_ov100_021487f0.z = 0x6d000;
        func_020731dc(&data_ov100_021487f0, (void *)_ZN7Vector3D1Ev, &data_ov100_021487d8);
        data_ov100_0214871c |= 1;
    }

    e = (char *)data_ov100_02148204 + *(int *)(c + 8) * 0x10;

    Vec3_RotateYAndTranslate(&pos, p + 0x5c, *(s16 *)(p + 0x8e), &data_ov100_021487f0);

    {
        void *actor = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
            0x11a, (u32)*(s8 *)(e + 9), (Vector3 *)&pos,
            (Vector3_16 *)(p + 0x8c), *(s8 *)(p + 0xcc), -1);
        if (actor != 0) {
            func_ov089_0213115c((char *)actor, 2);
        }
    }

    if (*(s8 *)(c + 0x100 + 0x44) != 0) {
        data_ov100_02148704 = 1;
    }

    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov100_02144bf4, 0x02144bf4, size 0x70 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144bf4
/* State member: unless the player is opening the door with a star, set
   this door's 0x8000 save-flag bit and send the player through. */
extern "C" int func_ov100_02144bf4(char *c, char *a2)
{
    if (!((Player *)a2)->IsOpeningDoorWithStar()) {
        signed char sh = (((signed char *)data_ov100_02148204) + (*(int *)(c + 8) << 4))[0xa];
        data_0209caa0.flags |= 0x8000 << sh;
        func_ov100_021451c4(c, &data_ov100_021488f4, a2);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov100_02144c64, 0x02144c64, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144c64
/* State member that does nothing. Was a C source. */
extern "C" int func_ov100_02144c64(void)
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov100_02144c6c, 0x02144c6c, size 0x60 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144c6c
/* State member: tick the shared countdown; once the player is not talking
   and has left the door's box, go to state data_ov100_02148924. Was a C
   source. */
extern "C" int func_ov100_02144c6c(char *r0, char *r1)
{
    func_ov100_02145014();
    if (((Player *)r1)->GetTalkState() == -1) {
        if (func_ov100_021452e4(r0, r1) == 0) {
            func_ov100_021453d8(r0, &data_ov100_02148924, (int)r1);
        }
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov100_02144ccc, 0x02144ccc, size 0x2c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144ccc
/* State member: play sound 0xb8 and start the shared countdown at 0x87.
   Was a C source. */
extern "C" int func_ov100_02144ccc(char *c)
{
    func_02012694(0xb8, c + 0x74);
    func_ov100_02145070(0x87);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- func_ov100_02144cf8, 0x02144cf8, size 0x28c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144cf8
/* State member: the closed door. When the player is in its box, decide:
   a star door with too few stars shows its message, one with enough asks
   to open; a key door without the key shows its message (0x28/0x23 for
   index 6 by the rabbit count, else the entry's message or 0x17), one with
   the key opens through TryTalkToKeyDoor; otherwise the player goes
   through. Was a C source. */
extern "C" int func_ov100_02144cf8(char *a, char *b)
{
    signed char *entry;

    if (func_ov100_021452e4(a, b)) {
        entry = (signed char *)data_ov100_02148204 + *(int *)(a + 8) * 16;
        if (entry[8] >= 0) {
            if (data_0209caa0.flags & (0x8000 << entry[0xa])) goto L240;
            if (NumStars() < entry[8]) {
                if (func_ov100_02144f84() == 0) return 1;
                if (((Player *)b)->ShowMessage(*(fBase_c *)a, *(short *)(entry + 0xc), (Vector3 *)(a + 0xa4), 0, 2) == 0) goto L240;
                func_ov100_021453d8(a, &data_ov100_021488c4, (int)b);
                return 1;
            } else {
                if (((Player *)b)->TryTalkToDoor(0) == 0) goto L240;
                func_ov100_021453d8(a, &data_ov100_021488d4, (int)b);
                return 1;
            }
        } else {
            int e9 = entry[9];
            int val;
            if (e9 < 0) goto L240;
            val = data_0209caa0.flags;
            if (val & (0x100 << entry[0xa])) goto L240;
            if (val & (2 << e9)) goto L210;
            if (func_ov100_02144f84() == 0) return 1;
            {
                int msg;
                if (entry[9] == 6) {
                    msg = _ZN8SaveData22NumGlowingRabbitsFoundEv() ? 0x28 : 0x23;
                } else {
                    int idx2 = *(int *)(a + 8);
                    int sel = 1;
                    int inRange = (unsigned)idx2 >= 9 && (unsigned)idx2 <= 0xb;
                    if (!inRange) {
                        if (!(data_0209caa0.flags & 0xbe)) sel = 0;
                    }
                    msg = sel ? *(short *)(entry + 0xc) : 0x17;
                }
                if (((Player *)b)->ShowMessage(*(fBase_c *)a, (short)msg, (Vector3 *)(a + 0xa4), 0, 2) == 0) goto L240;
            }
            func_ov100_021453d8(a, &data_ov100_021488c4, (int)b);
            if (entry[9] == 5)
                data_0209caa0.w2 |= 0x10000;
            return 1;
        L210:
            if (((Player *)b)->TryTalkToKeyDoor() == 0) goto L240;
            func_ov100_021453d8(a, &data_ov100_021488e4, (int)b);
            return 1;
        }
    L240:
        func_ov100_021451c4(a, (*(signed char *)(a + 0x144) != 0) ? &data_ov100_02148904 : &data_ov100_021488f4, b);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- func_ov100_02144f84, 0x02144f84, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144f84
/* 1 when the shared countdown is idle and data_0209b490/data_0209b49c are
   in their resting values. Was a C source. */
extern "C" int func_ov100_02144f84(void)
{
    if (*data_ov100_02148708 != 0) goto fail;
    if (*data_0209b490 != 0x7f000) goto fail;
    if (*data_0209b49c == 0) return 1;
fail:
    return 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- func_ov100_02144fcc, 0x02144fcc, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02144fcc
extern "C" int func_ov100_02144fcc(void)
{
    int r = func_ov100_02145014();
    if (r == 0) return 0;
    return _ZN5Sound7PlaySubEjjj5Fix12IiEb(0x24, 0x7f, 0, 0x7222, 0);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- func_ov100_02145014, 0x02145014, size 0x5c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02145014
/* Tick the shared countdown data_ov100_02148708 (with a sound while it
   runs); 1 once it is zero. Was a C source. */
extern "C" int func_ov100_02145014(void)
{
    int r = DecIfAbove0_Short(data_ov100_02148708);
    if (r) {
        _ZN5Sound7PlaySubEjjj5Fix12IiEb(0x24, 0x14, 0x7f, 0x15666, 0);
    }
    return data_ov100_02148708[0] == 0 ? 1 : 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- func_ov100_02145070, 0x02145070, size 0x10 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02145070
/* Start the shared countdown. Was a C source. */
extern "C" void func_ov100_02145070(int v)
{
    data_ov100_02148708[0] = v;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- func_ov100_02145080, 0x02145080, size 0xf0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02145080
/* The exit through a white door with a star (or, for door type 0xd,
   TryExitWhiteDoorWithStar), timed on the +0x145 countdown. Returns 0 on
   the frame the player is let out. Was a C source. */
extern "C" int func_ov100_02145080(char *c, void *arg1)
{
    if (*(int *)(c + 8) != 0xd) {
        if (*(unsigned char *)(c + 0x145) == 0) {
            if (func_ov002_020ca78c(arg1) == 0)
                goto ret1;
            _ZN5Sound7PlaySubEjjj5Fix12IiEb(0x2b, 0, 0x7f, 0x15666, 0);
            *(unsigned char *)(c + 0x145) = 0x78;
            goto ret1;
        }
        if (DecIfAbove0_Byte((char *)c + 0x145) != 0)
            goto ret1;
        _ZN5Sound7PlaySubEjjj5Fix12IiEb(0x2b, 0x7f, 0, 0x7222, 0);
        {
            unsigned char *p = (unsigned char *)(((int)c + 0x145));
            *p += 1;
        }
        return 0;
    }
    if (*(unsigned char *)(c + 0x145) != 0)
        goto ret0;
    *(unsigned char *)(c + 0x145) = ((Player *)arg1)->TryExitWhiteDoorWithStar();
    goto ret1;
ret0:
    return 0;
ret1:
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 21 -- func_ov100_02145170, 0x02145170, size 0x54 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02145170
/* Place the player at the door: offset a, facing the door's mAngleY turned
   half round, when the player is behind it (+0x88 > 0); offset b, facing
   mAngleY, otherwise -- the new facing also copied to the player's +0x94,
   and either offset rotated by mAngleY about the door's position. Was a C
   source. */
extern "C" void func_ov100_02145170(char *r0, char *r1, Vector3 *a, Vector3 *b)
{
    Vector3 *v;
    if (*(int *)(r0 + 0x88) > 0) {
        *(short *)(r1 + 0x8e) = *(short *)(r0 + 0x8e) + 0x8000;
        v = a;
    } else {
        *(short *)(r1 + 0x8e) = *(short *)(r0 + 0x8e);
        v = b;
    }
    *(short *)(r1 + 0x94) = *(short *)(r1 + 0x8e);
    Vec3_RotateYAndTranslate(r1 + 0x5c, r0 + 0x5c, *(short *)(r0 + 0x8e), v);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 22 -- func_ov100_021451c4, 0x021451c4, size 0x120 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021451c4
/* Send the player through: unless no state was given (then
   data_ov100_021488f4 is used without asking), the player must be able to
   enter from its side; move it 0x64 in front of or behind the door, go to
   the given state and hold the touch screen. */
extern "C" int func_ov100_021451c4(char *r6, void *r5, char *r4)
{
    if (r5 == 0) {
        r5 = &data_ov100_021488f4;
    } else if (((Player *)r4)->CanEnterDoor(*(int *)(r6 + 0x88) <= 0) == 0) {
        return 0;
    }
    if (!(data_ov100_0214870c & 1)) {
        data_ov100_021487e4[0] = 0;
        data_ov100_021487e4[1] = 0;
        data_ov100_021487e4[2] = 0x64000;
        func_020731dc(data_ov100_021487e4, (void *)_ZN7Vector3D1Ev, data_ov100_021487cc);
        data_ov100_0214870c |= 1;
    }
    if (!(data_ov100_02148714 & 1)) {
        data_ov100_02148808[0] = 0;
        data_ov100_02148808[1] = 0;
        data_ov100_02148808[2] = -0x64000;
        func_020731dc(data_ov100_02148808, (void *)_ZN7Vector3D1Ev, data_ov100_021487fc);
        data_ov100_02148714 |= 1;
    }
    func_ov100_02145170(r6, r4, (Vector3 *)data_ov100_021487e4, (Vector3 *)data_ov100_02148808);
    func_ov100_021453d8(r6, r5, (int)r4);
    SetTouchScreenDelay();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 23 -- func_ov100_021452e4, 0x021452e4, size 0x8c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021452e4
/* 1 when the player is inside the door's box (the player's door-relative
   offset at +0x80/+0x84/+0x88 within 0x4b/0x32/0x6e) and facing the door
   within 0x2000. Was a C source. */
extern "C" int func_ov100_021452e4(char *r0, char *r1)
{
    int v, z, a;
    v = *(int *)(r0 + 0x80); if (v < 0) v = -v; if (v > 0x4b000) goto fail;
    v = *(int *)(r0 + 0x84); if (v < 0) v = -v; if (v > 0x32000) goto fail;
    z = *(int *)(r0 + 0x88);
    v = (z < 0) ? -z : z; if (v > 0x6e000) goto fail;
    if (z < 0) a = *(short *)(r0 + 0x8e);
    else a = *(short *)(r0 + 0x8e) + 0x8000;
    a = (short)a;
    if (AngleDiff(a, *(short *)(r1 + 0x8e)) < 0x2000) return 1;
fail:
    return 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 24 -- func_ov100_02145370, 0x02145370, size 0x68 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02145370
/* The player's position in the door's frame, stored into the door's
   +0x80/+0x84/+0x88 (mScaleX/Y/Z, which a door does not use as a scale);
   returns the player. Was a C source. */
extern "C" void *func_ov100_02145370(char *c)
{
    Vec3i v;
    char *r5 = data_0209f394[data_0209f250];
    Vec3_Sub(&v, r5 + 0x5c, c + 0x5c);
    Vec3_RotateYAndTranslate(c + 0x80, &data_020a0ebc, (short)(-(*(short *)(c + 0x8e))), &v);
    return r5;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 25 -- func_ov100_021453d8, 0x021453d8, size 0x54 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021453d8
/* Enter a state: store the pair at +0x140 and call its enter member with
   the player, if it has one. */
extern "C" int func_ov100_021453d8(void *self, void *p, int a2)
{
    DoorPmfSelf *c = (DoorPmfSelf *)self;
    c->state = (DoorPmf *)p;
    DoorPmf *q = c->state;
    if (*q == 0) return 1;
    return (c->**q)(a2);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 26 -- _ZN8daDoor_c16CleanupResourcesEv, 0x0214542c, size 0x98 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_c16CleanupResourcesEv
/* vtable slot 3. `delete key` IS the ROM's `ldr r1,[r0]; ldr r1,[r1,#4];
 * blx r1` -- vtable slot 1, Model's deleting destructor -- and it is also
 * where the doubled null check comes from: the explicit `if` emits one and
 * `delete` emits its own. */
s32 daDoor_c::CleanupResources()
{
    DoorEntry *e = &data_ov100_02148204[param1];
    Model *key;
    e->file->Release();
    data_ov100_02148744.Release();
    key = mKeyModel;
    if (key != 0) {
        delete key;
        e->keyFile->Release();
    }
    if (mKeyFile != 0) {
        unsigned int v = param1;
        if (v >= 9 && v <= 0xd) UnloadKeyModels(v - 7);
        ((SharedFilePtr *)(mKeyFile))->Release();
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 27 -- _ZN8daDoor_c16OnPendingDestroyEv, 0x021454c4, size 0x4 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_c16OnPendingDestroyEv
/* vtable slot 12. The ROM body is empty: the override exists only to
   occupy the slot. */
void daDoor_c::OnPendingDestroy()
{
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 28 -- _ZN8daDoor_c6RenderEv, 0x021454c8, size 0x88 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_c6RenderEv
/* vtable slot 9. Draw the door if either area it joins is showing, then
 * hand the door's bone transforms to the key model hanging off 0x138 and
 * draw that too.
 *
 *   0x08c / 0x090   dActor_c::mAngleX / mAngleZ, which a daDoor_c does NOT
 *                   use as angles: each holds a signed BYTE area id, one
 *                   per side (`ldrsh`, then sign-extended through byte
 *                   width before IsAreaShowing sees it). Only mAngleY is a
 *                   real angle for this class.
 *   0x0e8           mModel.data.transforms, the door's own bone transform
 *                   array (ModelBase::data at +0x08, ModelComponents::
 *                   transforms at +0x0c).
 *
 * The two virtual calls are Model's slot 4 Virtual10(Matrix4x3&) and slot
 * 5 Render(const Vector3*), exactly what the ROM dispatches. */
s32 daDoor_c::Render()
{
    if (IsAreaShowing((char)mAngleX) == 0) {
        if (IsAreaShowing((char)mAngleZ) == 0) goto done;
    }
    {
        /* &mModel is taken HERE and not at the top of the function: hoisting
           it costs a callee-saved register and a stack adjust the ROM does
           not have. The ROM computes it lazily, so this does too. */
        ModelAnim *body = &mModel;
        Model *key;
        body->Render(0);
        key = mKeyModel;
        if (key == 0) goto done;
        key->Virtual10(*mModel.data.transforms);
        mKeyModel->Render(0);
    }
done:
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 29 -- _ZN8daDoor_c8BehaviorEv, 0x02145550, size 0x50 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_c8BehaviorEv
/* vtable slot 6. Run the current state's execute member with the player.
 *
 * THE CALLBACK IS A daDoor_c MEMBER FUNCTION, and that is measured rather
 * than assumed: spelt as `void (daDoor_c::*)(int)` and called on the
 * daDoor_c directly, mwcc emits the ROM's full member-pointer dispatch --
 * `asr #1` on the stored offset, bit 0 tested for "this is a vtable index
 * rather than a direct address", both arms present. A cheaper
 * representation would have collapsed that sequence. */
typedef void (daDoor_c::*DoorExecute)(int);
struct DoorState {
    char enter[8];           /* called by func_ov100_021453d8 */
    DoorExecute execute;
};

s32 daDoor_c::Behavior()
{
    int res = (int)func_ov100_02145370((char *)this);
    DoorState *node = (DoorState *)mCallbackNode;
    if (*(int *)&node->execute != 0) {
        (this->*(node->execute))(res);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 30 -- _ZN8daDoor_c13InitResourcesEv, 0x021455a0, size 0x2fc */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daDoor_c13InitResourcesEv
/* vtable slot 0. Was a C source taking the object explicitly.
 *
 * 0x05c/0x060/0x064 are mPosX/Y/Z and 0x08e mAngleY, so the opening call
 * rotates the door's own position about its own facing angle; the two
 * matrix stores are the same store to the door model and the key model.
 *
 * The block near the end writes mPosX, mPosY + 0xb4000 and mPosZ into
 * 0x0a4/0x0a8/0x0ac. dActor_c.h names the middle of those three
 * `mVertSpeed`, which cannot be what a daDoor_c is storing there -- three
 * consecutive words taking a position triple say 0x0a4..0x0af is a second
 * Vector3 for at least this class. Left spelt as dActor_c has it, because
 * renaming a base field on one derived class's evidence is a dActor_c
 * change, not a daDoor_c one. */
s32 daDoor_c::InitResources()
{
    unsigned int idx;
    DoorEntry *e;
    void *f;
    void *an;
    void *m;
    Vec3i tmp;
    int r4;
    int v;
    int b;
    int x;
    int y;
    int z;

    /* The spawn word arrives packed: this door's variant index is param1's
       high halfword, and every later read of param1 is that index. Spelt
       plainly (`param1 = param1 >> 0x10;`), 2004/b56 value-numbers both
       sides together and materialises the address once -- one instruction
       longer than the ROM, which keeps the offset folded into both
       accesses. The redundant cast on the read side reaches the folded
       form. */
    param1 = (u32)param1 >> 0x10;

    if (!(data_ov100_02148710 & 1)) {
        data_ov100_021487c0.x = 0x4b000;
        data_ov100_021487c0.y = 0;
        data_ov100_021487c0.z = 0;
        func_020731dc(&data_ov100_021487c0, (void *)_ZN7Vector3D1Ev, &data_ov100_021487b4);
        data_ov100_02148710 |= 1;
    }

    Vec3_RotateYAndTranslate(&mPosX, &mPosX, mAngleY, &data_ov100_021487c0);

    idx = param1;
    e = &data_ov100_02148204[idx];
    f = _ZN5Model8LoadFileER13SharedFilePtr(e->file);
    _ZN9ModelBase7SetFileEP8BMD_Fileii(&mModel, f, 1, 1);

    b = data_0209f2d8;
    b = b == 0;
    if (b != 0) {
        if (e->b8 > 0) {
            mKeyFile = &data_ov002_0211094c;
        } else if (e->b9 >= 0) {
            unsigned int t = param1;
            if (t >= 9 && t <= 0xd) {
                mKeyModelIdx = (signed char)(t - 8);
                LoadKeyModels(mKeyModelIdx + 1);
                mKeyFile = data_ov089_02132894[mKeyModelIdx + 1];
                if (param1 == 0xc)
                    mKeyModelIdx = 0;
            } else {
                mKeyFile = &data_ov089_02132c50;
            }
        }
        if (mKeyFile != 0)
            _ZN5Model8LoadFileER13SharedFilePtr(mKeyFile);
    }

    an = _ZN9Animation8LoadFileER13SharedFilePtr(&data_ov100_02148744);
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModel, an, 0x40000000, 0x1000, 0);

    Vec3_Asr(&tmp, &mPosX, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, tmp.x, tmp.y, tmp.z);
    Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, mAngleY);
    *(M48 *)&mModel.mat4x3 = data_020a0e68;

    if (e->keyFile != 0) {
        m = _Znwj(0x50);
        if (m != 0)
            m = _ZN5ModelC1Ev(m);
        mKeyModel = (Model *)m;
        f = _ZN5Model8LoadFileER13SharedFilePtr(e->keyFile);
        _ZN9ModelBase7SetFileEP8BMD_Fileii(mKeyModel, f, 1, -1);
        *(M48 *)&mKeyModel->mat4x3 = data_020a0e68;
    }

    {
        int w;
        y = mPosY;
        z = mPosZ;
        x = mPosX;
        w = y + 0xb4000;
        unk_0a4 = x;
        mVertSpeed = w;
        {
            unsigned char bi = data_0209f250;
            unk_0ac = z;
            r4 = (int)data_0209f394[bi];
        }
    }
    func_ov100_02145370((char *)this);

    v = mScaleX;
    if (v < 0)
        v = -v;
    if (v > 0x4b000)
        goto big;
    v = mScaleY;
    if (v < 0)
        v = -v;
    if (v > 0x32000)
        goto big;
    v = mScaleZ;
    if (v < 0)
        v = -v;
    if (v > 0x1f4000)
        goto big;

    func_ov100_021453d8(this, &data_ov100_02148914, r4);
    goto done;
big:
    func_ov100_021453d8(this, &data_ov100_021488b4, r4);
done:
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 31 -- daDoor_c_classInit, 0x0214589c, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol daDoor_c_classInit
/* Reconstructed source-style name: SM64DS proves daDoor_c through RTTI,
 * allocation size, vtable identity, and the DOOR registry profile; later
 * EAD lineage supplies classInit. Exact original spelling is not
 * preserved. Historical alias: Door_Spawn. */
extern "C" daDoor_c *daDoor_c_classInit()
{
    return new daDoor_c;
}
