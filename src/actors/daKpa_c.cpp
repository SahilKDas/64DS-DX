//cpp
/* daKpa_c + daKpaTail_c -- Bowser and his tail, the ov060 boss-fight actors.
 * ROM span 0x02111900..0x021163f0, 79 functions: both destructor pairs,
 * the shared fight helpers, both classes' resource/load/behavior/render
 * methods, and both InitResources. The class names are the cartridge's
 * own RTTI spellings: ex-coined Bowser/BowserTail, renamed with the
 * aliased-vtable evidence (shared _ZTV addresses, typeinfo slots and
 * ROM-spelled classInit factories), the same S35 shape as daKpaFire_c.
 *
 * FUNCTION ORDER IS DELIBERATELY THE REVERSE OF THE ROM'S -- mwccarm 2004/b56
 * emits one .text section per function, in the REVERSE of source order, so
 * the highest-address ROM function is written FIRST here. Do not reorder;
 * see notes/tu-reconstruction-pilot-report.md sec 3 for the one documented
 * exception (a destructor's D0/D1/D2 group has compiler-chosen order).
 *
 * Folded from the 79 legacy one-function sources in ROM order (ordinals
 * 0-78, 0x02111900..0x021163f0), retired at promotion; per-symbol credit
 * survives in attribution.json path#symbol overrides.*/

/* TUBUILD NOTE -- codegen is deferred, so .text emits in reverse source
 * order and the function bodies below are ROM-descending. The exception is
 * the destructor window directly under the declaration block: bracketed by
 * `defer_codegen off/on` they generate eagerly in source order, and placed
 * before any deferred function both D1,D0 groups lead the object as the
 * ROM requires (left deferred the variants come out D2,D0,D1, and in a
 * later window the second class's group rides the deferred flush to the
 * end of the object). Same shape as the promoted daKpaFire_c. The legacy
 * func_ov060_02112bfc shard carried `#pragma opt_common_subs off` /
 * `opt_lifetimes off`; under a merged TU that state is file-global
 * last-wins at codegen, so it is spelled out in source instead: the walk
 * pointer is volatile and the flag/value locals are split. The
 * opt_common_subs off on func_ov060_021125f0's shard was not needed --
 * verified without it. */

/* Includes: union of the legacy files', first-seen in ROM-ascending
 * processing order. NOT verified for header ordering constraints (e.g. a
 * common.h-before-X rule) -- watch for new compile errors after this. */
/* common.h must come first. It carries Matrix4x3 behind a guard, and both
 * spellings are 0x30 bytes and both are real -- Matrix4x3_ApplyInPlace* wants
 * `m[i]` while the model headers want `.r`/`.t`. Whichever is seen first in a TU
 * stands. The class headers reach Model.h, so including them ahead of common.h
 * hands the TU the `.r`/`.t` form and the helpers below stop compiling. Same
 * ordering constraint daKpa2Bg_c documents. */
#include "common.h"
#include "daKpa_c.h"
#include "daKpaTail_c.h"
#include "types.h"
#include "decl_common.h"
#include "dBgCh_Gnd.h"
#include "dActor_c.h"
#include "dBgCh_Actr.h"
#include "SharedFilePtr.h"
#include "TextureSequence.h"
#include "Animation.h"

/* The opaque view of the object that Bowser_IsAnimAtLastFrame walks: the
 * Animation lives at 0x124. Animation itself is the real class (include/
 * Animation.h) rather than the local stub this shard carried. */
struct Obj {
    char pad[0x124];
    Animation anim;
};

/* Local shadow declarations carried from the legacy files verbatim.
 * NOT reconciled against real project headers -- check include/*.h for
 * each of these before compiling; a real header should usually win. */
/* The state table at data_ov060_0211ae9c holds pointers-to-member of the actor
 * itself, so PMF is a member pointer of daKpa_c, not of the opaque 4-byte `C`
 * this shard invented. The real class is what the ROM dispatches through. */
typedef void (daKpa_c::*PMF)();

/* shadow struct 'PmfEnt' */
struct PmfEnt { PMF pmf; };

/* Two unrelated two-word tables share this name in different shards:
 *
 *   data_ov060_0211aed4  the state table. Its second word is a packed target
 *                        (bit 0 selects "call through the vtable", the rest is
 *                        either the slot byte offset or a direct target), so
 *                        both words are read here.
 *   data_ov060_0211acd8  a plain two-int compare pair; only the SECOND word is
 *      _0211acf0        ever compared against, so it is named to say so.
 *
 * One struct cannot serve both, so the state table gets its own. */
struct TabEnt { int slot; int target; };

/* Compare pair: second word only. */
struct TabCmp { int unused; int match; };

/* shadow struct 'D0211ac88' */
struct D0211ac88 { int a, b; };

/* shadow typedef 'Vec3' */
typedef struct Vec3 { int x, y, z; } Vec3;

/* shadow class 'dActor_c' */
class dActor_c;

/* shadow class 'Player' */
class Player;

/* TUBUILD CONFLICT -- alternate body of typedef 'Vector3', from the legacy file for func_ov060_021132a4, NOT applied:
typedef struct { int x, y, z; } Vector3;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 'Vector3', from the legacy file for func_ov060_021135fc, NOT applied:
typedef struct Vector3 { int x, y, z; } Vector3;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 'Vector3', from the legacy file for func_ov060_02113a94, NOT applied:
typedef struct Vector3 { int x, y, z; } Vector3;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 's16', from the legacy file for func_ov060_021140c0, NOT applied:
typedef signed short s16;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 'Vector3', from the legacy file for func_ov060_021140c0, NOT applied:
typedef struct { s32 x, y, z; } Vector3;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 's32', from the legacy file for func_ov060_02114858, NOT applied:
typedef int s32;
*/

/* TUBUILD CONFLICT -- alternate body of typedef 'Vector3', from the legacy file for func_ov060_02114858, NOT applied:
typedef struct
{
  int x;
  int y;
  int z;
} Vector3;
*/

/* TUBUILD CONFLICT -- alternate body of struct 'TabEnt', from the legacy file for func_ov060_02114858, NOT applied:
struct TabEnt
{
  int a;
  int b;
};
*/

/* TUBUILD CONFLICT -- alternate body of typedef 'dBgCh_Gnd', from the legacy file for func_ov060_0211577c, NOT applied:
typedef struct dBgCh_Gnd { char filler[0x44]; int clsnY; char rest[0x8]; } dBgCh_Gnd;
*/

/* TUBUILD CONFLICT -- alternate body of struct 'Obj', from the legacy file for _ZN7daKpa_c6RenderEv, NOT applied:
struct Obj { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void m(void*); };
*/

#define LAUND(p) ((void *)(p))
#define M(p) (p)
#define P423(c) ((u8 *)((c) + 0x423))
/* TUBUILD CONFLICT -- alternate #define of LAUND, from the legacy file for func_ov060_021130c0, NOT applied: #define LAUND(p) ((void*)(p)) */

extern "C" {
/* Camera base pointer; Bowser publishes itself into the target slot at +0x114,
 * so the offset is a byte offset from this base, not a field of a known class. */
extern char *data_0209f318;
extern short data_02082214[];
extern void* _ZN8dActor_c13ClosestPlayerEv(void *thiz);
extern void _ZN6Camera9SetFlag_3Ev(void*);
extern void _ZN6Camera9SetLookAtERK7Vector3(void*, const struct Vector3*);
extern void _ZN6Camera6SetPosERK7Vector3(void*, const struct Vector3*);
extern int Vec3_HorzDist(const struct Vector3*, const struct Vector3*);
extern short Vec3_HorzAngle(const struct Vector3*, const struct Vector3*);
extern int func_020092c4(void*, void*, void*);
extern int _ZN6Player7IsInAirEv(void*);
extern int _Z14ApproachLinearRiii(int*, int, int);
extern int _ZN9Animation8FinishedEv(void*);
/* Every call site in this TU passes a third argument that the body never reads
 * (`c` and `idx` are the only parameters it uses). The shards disagreed on the
 * arity -- some declared 2, some 3, and the return type was spelled both int and
 * void -- which C++ rejects as illegal overloading. Unified here to the widest
 * form actually called, so all ~30 call sites agree. */
/* Both helpers receive this same actor's base pointer. Most shards in this TU
 * hold it as char*, but two hold it as void*, and char* converts to void*
 * implicitly while void* does not convert back. Typed as void* here so every
 * caller compiles unchanged; each definition casts on entry. */
void func_ov060_02111cc0(void *c, int idx, int unused);
extern int data_ov060_0211acd0[];
extern unsigned char data_ov060_02119264[];
extern PmfEnt data_ov060_0211aeb4[];
int _ZN8dActor_c14GetSubtractionEss(void* self, short a, short b);
extern dActor_c *_ZN8dActor_c10FindWithIDEj(unsigned int id);
extern int _ZN10dBgCh_Actr15ClearGroundFlagEv(char *c);
/* func_02012694 takes (soundId, position) -- see src/func_02012694.cpp, whose
 * body is a single Sound::Play(3, id, pos) forward. Two shards here passed a
 * third argument the callee does not take (a counter byte, and a value stored
 * on the line above); dropped here rather than widening the signature. */
extern void func_02012694(unsigned int id, const Vector3 *v);
void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void* self, const Vector3& v, u32 a, Fix12i b, u32 c, u32 d, u32 e);
void _ZN8dActor_c9UpdatePosEP5dCc_c(void* self, void* cc);
void func_02038408(void* p);
void dBgCh_Actr_UpdateContinuous_Veneer(void* p);
int _ZNK10dBgCh_Actr10IsOnGroundEv(void* self);
void* _ZNK10dBgCh_Actr14GetFloorResultEv(void* self);
void _ZNK11SurfaceInfo12CopyNormalToER7Vector3(void* self, Vector3* out);
int _ZNK10dBgCh_Actr13JustHitGroundEv(void* self);
int _ZN4cstd4fdivEii(int a, int b);
int func_ov060_02112ba8(char* c);
extern TabEnt data_ov060_0211aed4[];
extern s16 data_02082214[];
extern "C" void func_ov060_02113564(char *c);
extern "C" void func_ov060_021134ac(void* thiz);
extern "C" int func_ov060_02113404(char* c);
extern "C" int func_ov060_021130c0(char* c);
extern "C" int func_ov060_02112ee0(void *c);
extern int _ZN6Player9StartTalkER7fBase_cb(void *pl, void *a, int b);
extern int _ZN6Player12GetTalkStateEv(void *pl);
extern unsigned char NumStars(void);
extern int _ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(
void *pl, void *a, unsigned m, void *v, unsigned d, unsigned e);
extern void _ZN7Message11PrepareTalkEv(void);
extern void _ZN5Sound22StopLoadedMusic_Layer1Ej(unsigned a);
extern void func_ov060_021135fc(char *c);
extern void *_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
unsigned a, unsigned b, int x, int y, int z, void *v, void *cb);
extern void func_ov060_02113260(char *c);
extern void _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(unsigned int a, int b);
extern int func_ov060_021132a4(char *c);
/* Sound::PlayLong is declared in include/Sound.h as
 * (u32 handle, u32, u32, const Vector3 &pos, s16). The mangled name encodes that
 * order, so the bridge is spelled to match rather than through void*. */
extern int _ZN5Sound8PlayLongEjjjRK7Vector3s(unsigned int a, unsigned int b, unsigned int c, const Vector3 *v, short d);
/* Defined below as void(char*, char*) -- every call site passes a char* into
 * this same actor, so the declaration is spelled the same way. */
extern void func_ov060_02115a84(char* c, char* p);
extern void func_ov060_02112350(void* c);
extern struct D0211ac88 data_ov060_0211ac88;
/* The mangled name encodes the real signature: (u32, u32, Vector3*,
 * const Vector3_16*, int, int). Spelled to match so the two call sites that pass
 * a real Vector3_16* at c+0x92 bind without a cast. */
extern void* _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int kind, unsigned int b, Vector3 *pos, const Vector3_16 *rot, int e, int f);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int kind, int x, int y, int z);
extern void *_ZN9dBgCh_GndC1Ev(dBgCh_Gnd *self);
extern void _ZN9dBgCh_Gnd12SetObjAndPosERK7Vector3P8dActor_c(dBgCh_Gnd *self, Vec3 *pos, void *actor);
extern int _ZN9dBgCh_Gnd10DetectClsnEv(dBgCh_Gnd *self);
extern int _ZNK5dBgPi9GetClsnIDEv(void *self);
extern void _ZN9dBgCh_GndD1Ev(dBgCh_Gnd *self);
extern void func_ov060_02113a94(void *c);
/* Defined below as int(char*) -- every call site passes this actor's base. */
extern int func_ov060_021145d4(char *c);
/* Defined below as void(char*) -- every call site hands it this same actor's
 * base pointer, so the declaration is spelled the same way. */
/* Defined further down as void(char*); called from Behavior above it. */
extern void func_ov060_02115b84(char *c);
extern void func_ov060_0211577c(char *c);
extern void func_ov060_02115b0c(char *c);
extern void *_ZN8dActor_c15FindWithActorIDEjPS_(u32 id, void *p);
extern void func_ov060_02115018(void *c);
extern int Vec3_HorzLen(const Vector3* v);
extern int func_ov060_02113d20(dActor_c *self);
/* Defined below as (char*, unsigned short, int) -- the second parameter is a
 * frame threshold read as an unsigned halfword, not an int. */
extern int func_ov060_02113ff4(char *c, unsigned short a, int b);
extern int _ZNK9Animation12WillHitFrameEi(void* anim, int frame);
extern void _ZN8dActor_c13SpawnFireballERK7Vector3PK10Vector3_165Fix12IiES7_j( void* self, const void* pos, const void* rot, int speed, int gravity, u32 flags);
extern int _ZN6Player9GetHealthEv(void* player);
extern int RandomIntInternal(int* seed);
extern int data_0209e650;
extern int data_ov060_0211abe0[];
extern int func_ov060_0211469c(void *c);
extern void func_ov060_021145a8(void* c);
extern void func_0200fa04(void* c, void* v, int a);
extern struct TabCmp data_ov060_0211acd8;
extern struct TabCmp data_ov060_0211acf0;
extern int _Z14ApproachLinearRsss(short *value, short target, short step);
extern int func_ov060_02115744(void* c);
extern int func_ov060_02115718(void* c);
extern int func_ov060_021156ec(void* c);
extern int data_ov060_0211ac20[];
extern int data_ov060_0211ac68[];
extern int data_ov060_0211ac70[];
/* Takes this same actor; the shard below declared it dActor_c*, which every
 * caller satisfies from a char* base pointer. */
extern void func_ov060_021150d0(void *a);
/* Defined below against a field inside this actor; the caller passes the same
 * base pointer, so the declaration is spelled through void*. */
extern void func_ov060_021150c4(void *p);
extern int _ZN8dActor_c13DistToCPlayerEv(void *self);
extern int func_ov060_02111f08(void *arg0);
/* Defined below as void(char*) -- called from Behavior with this actor's base. */
extern void func_ov060_02115518(char *c);
extern s16 data_ov060_0211a4e0[];
extern s16 func_02010844(void *unused, void *v, s16 angle);
extern void Vec3_Asr(Vec3 *d, Vec3 *s, int sh);
extern void Matrix4x3_FromTranslation(Matrix4x3 *m, int x, int y, int z);
extern void Matrix4x3_ApplyInPlaceToRotationY(Matrix4x3 *m, s16 angY);
extern void Matrix4x3_ApplyInPlaceToRotationX(Matrix4x3 *m, s16 angX);
extern void Matrix4x3_ApplyInPlaceToRotationZ(Matrix4x3 *m, s16 angZ);
extern void _ZN9ModelBase12ApplyOpacityEjj(void *self, unsigned int opacity, unsigned int unused);
extern void MulMat4x3Mat4x3(void *dst, void *a, void *b);
extern void Vec3_LslInPlace(void *v, int sh);
extern void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void *c, void *sm, void *mtx, int rad, int h, unsigned int flags);
extern Matrix4x3 data_020a0e68;
extern void _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(void* self, struct Vector3* v, int f);
extern void func_ov060_021123a0(void *a, int b);
extern void func_ov060_021123c8(char *p);
extern void _ZN6Player9DropActorEv(void *p);
extern "C" int _ZN6Player15IsCollectingCapEv(Player *p);
extern "C" int _ZN6Player7TryGrabER8dActor_c(Player *p, dActor_c &a);
extern "C" void func_02011cfc(void);
/* One declaration per resource handle, typed from how this TU actually uses it.
 * The shards disagreed: the resource-loader shard passed them straight to
 * Model::LoadFile(SharedFilePtr) and called ->Release(), while the state shard
 * read the same addresses as flat int tables. One object cannot be both, and
 * C++ rejects the pair. Typed here from the loader, which is the real consumer.
 *
 *   data_ov060_021192dc / _0211927c  indexed by i, ->Release()  -> SharedFilePtr[]
 *   data_ov060_0211b208 is SharedFilePtr here, but decl_common.h owns an
 *   int[] decl for daFRing_c -- cast through it at the use sites.
 *   data_ov060_0211ac78              indexed [1] and .w[1]       -> SharedFilePtr[] */
extern SharedFilePtr *data_ov060_021192dc[];
extern SharedFilePtr *data_ov060_0211927c[];
extern SharedFilePtr data_ov089_02132c50;
extern SharedFilePtr *data_ov060_0211ac78[];
extern SharedFilePtr *data_ov060_0211ac28[];
extern void _ZN9Animation7AdvanceEv(void* a);
extern void _ZN15TextureSequence6UpdateER15ModelComponents(void* a, void* b);
/* Model::LoadFile is declared in include/Model.h as taking SharedFilePtr&.
 * The legacy shard bridged it as `void *`, which only worked while the handles
 * were untyped. Now that they are the SharedFilePtr they really are, the bridge
 * takes the reference the ROM symbol encodes, and the call sites pass the
 * handles themselves rather than casting to void*. */
extern void *_ZN5Model8LoadFileER13SharedFilePtr(SharedFilePtr &f);
extern void _ZN9ModelBase7SetFileEP8BMD_Fileii(void *self, void *f, int a, int b);
extern void _ZN9Animation8LoadFileER13SharedFilePtr(void *f);
extern void _ZN15TextureSequence8LoadFileER13SharedFilePtr(void *f);
extern void _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(void *self, void *btp, int a, int b, unsigned int d);
extern void _ZN9Animation8SetFlagsEi(void *self, int flags);
extern int _ZN11ShadowModel12InitCylinderEv(void *self);
extern void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(void *self, void *act, void *pos, int c3, int d, unsigned int e, unsigned int f);
extern void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void *self, void *act, int a, int b, void *d1, void *d2);
extern void _ZN10dBgCh_Actr13SetLimMovFlagEv(void *self);
extern void func_ov060_021123dc(void *c);
extern void func_02011d50(void *a);
int _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *self, void *actor, int fix, int t, unsigned int e, unsigned int f);
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzDist, from the legacy file for func_ov060_02112434, NOT applied: int Vec3_HorzDist(const Vector3* a, const Vector3* b); */
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzAngle, from the legacy file for func_ov060_02112434, NOT applied: short Vec3_HorzAngle(const Vector3* a, const Vector3* b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021125f0, NOT applied: extern int func_ov060_02111cc0(char *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02112724, NOT applied: extern void func_ov060_02111cc0(char *c, int idx, int a); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02112724, NOT applied: extern int Bowser_IsAnimAtLastFrame(char *o); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c10FindWithIDEj, from the legacy file for func_ov060_021128c0, NOT applied: void* _ZN8dActor_c10FindWithIDEj(u32 id); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c10FindWithIDEj, from the legacy file for func_ov060_02112d48, NOT applied: extern void *_ZN8dActor_c10FindWithIDEj(unsigned int id); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02112ee0, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b, int d); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player9StartTalkER7fBase_cb, from the legacy file for func_ov060_021130c0, NOT applied: extern int _ZN6Player9StartTalkER7fBase_cb(void* player, void* actorBase, int isTalk); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player12GetTalkStateEv, from the legacy file for func_ov060_021130c0, NOT applied: extern int _ZN6Player12GetTalkStateEv(void* player); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh, from the legacy file for func_ov060_021130c0, NOT applied: extern int _ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(void* player, void* actorBase, unsigned int msgId, const void* pos, unsigned int a, unsigned int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021130c0, NOT applied: extern void func_ov060_02111cc0(void* c, int a, int b, int d); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN5Sound22StopLoadedMusic_Layer1Ej, from the legacy file for func_ov060_021130c0, NOT applied: extern void _ZN5Sound22StopLoadedMusic_Layer1Ej(unsigned int a); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02113260, from the legacy file for func_ov060_021130c0, NOT applied: extern void func_ov060_02113260(void* c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_021135fc, from the legacy file for func_ov060_021130c0, NOT applied: extern void func_ov060_021135fc(void* c); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE, from the legacy file for func_ov060_021132a4, NOT applied: extern int _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(unsigned int a, unsigned int b, int fix, int t1, int t2, void *v, void *cb); */
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzDist, from the legacy file for func_ov060_02113404, NOT applied: extern int Vec3_HorzDist(const struct Vector3* a, const struct Vector3* b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c14GetSubtractionEss, from the legacy file for func_ov060_02113404, NOT applied: extern int _ZN8dActor_c14GetSubtractionEss(void* self, short a, short b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK10dBgCh_Actr13JustHitGroundEv, from the legacy file for func_ov060_021134ac, NOT applied: extern int _ZNK10dBgCh_Actr13JustHitGroundEv(void* clsn); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021134ac, NOT applied: extern void func_ov060_02111cc0(void* c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK10dBgCh_Actr10IsOnGroundEv, from the legacy file for func_ov060_021134ac, NOT applied: extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void* clsn); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02113564, NOT applied: extern void func_ov060_02111cc0(char *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02113564, NOT applied: extern void func_02012694(int a, void *p, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02113710, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02113710, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02113740, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02113740, NOT applied: extern void func_02012694(int a, void *p); */
/* TUBUILD CONFLICT -- alternate declaration of _Z14ApproachLinearRiii, from the legacy file for func_ov060_02113740, NOT applied: extern void _Z14ApproachLinearRiii(int *v, int target, int step); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02113740, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzAngle, from the legacy file for func_ov060_02113a94, NOT applied: extern s16 Vec3_HorzAngle(const Vector3* v0, const Vector3* v1); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02113b5c, NOT applied: void func_ov060_02111cc0(char* c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of _Z14ApproachLinearRiii, from the legacy file for func_ov060_02113b5c, NOT applied: void _Z14ApproachLinearRiii(int* v, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115a84, from the legacy file for func_ov060_02113b5c, NOT applied: void func_ov060_02115a84(char* c, char* arg); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02113b5c, NOT applied: int Bowser_IsAnimAtLastFrame(char* c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02113ff4, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02113ff4, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_021140c0, NOT applied: extern void func_02012694(int id, void* pos); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_021140c0, NOT applied: extern int Bowser_IsAnimAtLastFrame(void* self); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021140c0, NOT applied: extern void func_ov060_02111cc0(void* self, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021142b4, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_021142b4, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02114300, NOT applied: extern void func_02012694(int a, char *b, int cnt); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02114300, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_0211469c, from the legacy file for func_ov060_021143b8, NOT applied: extern int func_ov060_0211469c(void* c); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_021143b8, NOT applied: extern void func_02012694(int a, void* b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115018, from the legacy file for func_ov060_021143b8, NOT applied: extern void func_ov060_02115018(void* c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_021145d4, from the legacy file for func_ov060_021143b8, NOT applied: extern int func_ov060_021145d4(void* c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021143b8, NOT applied: extern void func_ov060_02111cc0(char* c, int idx, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115b0c, from the legacy file for func_ov060_021143b8, NOT applied: extern void func_ov060_02115b0c(char* c); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c15FindWithActorIDEjPS_, from the legacy file for func_ov060_021143b8, NOT applied: extern void* _ZN8dActor_c15FindWithActorIDEjPS_(unsigned int id, void* p); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_021143b8, NOT applied: extern int Bowser_IsAnimAtLastFrame(void* o); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK10dBgCh_Actr10IsOnGroundEv, from the legacy file for func_ov060_021145d4, NOT applied: extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_0200fa04, from the legacy file for func_ov060_021145d4, NOT applied: extern void func_0200fa04(void *c, void *v, int flag); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_021145d4, NOT applied: extern void func_02012694(int a, void *b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player7IsInAirEv, from the legacy file for func_ov060_021145d4, NOT applied: extern int _ZN6Player7IsInAirEv(void *p); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_0211469c, NOT applied: extern "C" void func_ov060_02111cc0(void *c, int a1, int a2); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_021146d0, NOT applied: extern void func_02012694(int a, void *b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021146d0, NOT applied: extern void func_ov060_02111cc0(char *c, int idx, int m); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115a84, from the legacy file for func_ov060_021146d0, NOT applied: extern void func_ov060_02115a84(char *c, char *arg); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN5Sound8PlayLongEjjjRK7Vector3s, from the legacy file for func_ov060_02114858, NOT applied: extern int _ZN5Sound8PlayLongEjjjRK7Vector3s(u32 a, u32 b, u32 cc, const Vector3 *v, u32 e); */
/* TUBUILD CONFLICT -- alternate declaration of RandomIntInternal, from the legacy file for func_ov060_02114858, NOT applied: extern int RandomIntInternal(int *seed); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as, from the legacy file for func_ov060_02114858, NOT applied: extern void _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(u32 id, u32 f, const Vector3 *v, const Vector3_16 *r, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02114858, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *o); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02114858, NOT applied: extern void func_ov060_02111cc0(char *c, int idx, int a); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02114858, NOT applied: extern void func_02012694(int a, void *p); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02114b60, NOT applied: extern void func_02012694(int a, void *b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c14GetSubtractionEss, from the legacy file for func_ov060_02114b60, NOT applied: extern int _ZN8dActor_c14GetSubtractionEss(char *self, s16 a, s16 b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK10dBgCh_Actr10IsOnGroundEv, from the legacy file for func_ov060_02114b60, NOT applied: extern int _ZNK10dBgCh_Actr10IsOnGroundEv(char *self); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c14GetSubtractionEss, from the legacy file for func_ov060_02114d08, NOT applied: extern int _ZN8dActor_c14GetSubtractionEss(void* actor, s16 a, s16 b); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02114e9c, NOT applied: extern "C" int func_02012694(int a, int* b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02114e9c, NOT applied: extern "C" int Bowser_IsAnimAtLastFrame(void* o); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02114e9c, NOT applied: extern "C" void func_ov060_02111cc0(void* o, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK9Animation12WillHitFrameEi, from the legacy file for func_ov060_02114e9c, NOT applied: extern "C" int _ZNK9Animation12WillHitFrameEi(void* self, int frame); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02114ff8, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *); */
/* TUBUILD CONFLICT -- alternate declaration of RandomIntInternal, from the legacy file for func_ov060_021151d4, NOT applied: extern int RandomIntInternal(int *seed); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115744, from the legacy file for func_ov060_021153f8, NOT applied: extern int func_ov060_02115744(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115718, from the legacy file for func_ov060_021153f8, NOT applied: extern int func_ov060_02115718(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_021156ec, from the legacy file for func_ov060_021153f8, NOT applied: extern int func_ov060_021156ec(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021153f8, NOT applied: extern void func_ov060_02111cc0(char *c, int idx, int extra); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111f08, from the legacy file for func_ov060_021154e8, NOT applied: extern int func_ov060_02111f08(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021154e8, NOT applied: extern int func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02115518, from the legacy file for func_ov060_021154e8, NOT applied: extern int func_ov060_02115518(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player9StartTalkER7fBase_cb, from the legacy file for func_ov060_02115518, NOT applied: extern int _ZN6Player9StartTalkER7fBase_cb(void *player, void *actor, int flag); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player12GetTalkStateEv, from the legacy file for func_ov060_02115518, NOT applied: extern int _ZN6Player12GetTalkStateEv(void *player); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh, from the legacy file for func_ov060_02115518, NOT applied: extern int _ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(void *player, void *actor, unsigned int msg, void *pos, unsigned int a, unsigned int b); */
/* TUBUILD CONFLICT -- alternate declaration of func_02012694, from the legacy file for func_ov060_02115518, NOT applied: extern void func_02012694(int a, void *v); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111f08, from the legacy file for func_ov060_02115518, NOT applied: extern int func_ov060_02111f08(void *self); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_021156ec, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_021156ec, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02115718, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02115718, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for func_ov060_02115744, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Bowser_IsAnimAtLastFrame, from the legacy file for func_ov060_02115744, NOT applied: extern int Bowser_IsAnimAtLastFrame(void *c); */
/* TUBUILD CONFLICT -- alternate declaration of _ZNK10dBgCh_Actr10IsOnGroundEv, from the legacy file for func_ov060_0211577c, NOT applied: extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void *self); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as, from the legacy file for func_ov060_02115b0c, NOT applied: extern void* _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int a, unsigned int b, struct Vector3* pos, void* rot, int e, int f); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c10FindWithIDEj, from the legacy file for func_ov060_02115c1c, NOT applied: extern void *_ZN8dActor_c10FindWithIDEj(unsigned int id); */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c10FindWithIDEj, from the legacy file for func_ov060_02115d68, NOT applied: extern "C" dActor_c *_ZN8dActor_c10FindWithIDEj(u32 id); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_021123a0, from the legacy file for func_ov060_02115d68, NOT applied: extern "C" void func_ov060_021123a0(void *a, int b); */
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzAngle, from the legacy file for _ZN7daKpa_c8BehaviorEv, NOT applied: extern s16 Vec3_HorzAngle(const Vector3* a, const Vector3* b); */
/* TUBUILD CONFLICT -- alternate declaration of Vec3_HorzDist, from the legacy file for _ZN7daKpa_c8BehaviorEv, NOT applied: extern Fix12i Vec3_HorzDist(const Vector3* a, const Vector3* b); */
/* TUBUILD CONFLICT -- alternate declaration of data_0209f318, from the legacy file for _ZN7daKpa_c8BehaviorEv, NOT applied: extern char* data_0209f318; */
/* TUBUILD CONFLICT -- alternate declaration of _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as, from the legacy file for _ZN7daKpa_c13InitResourcesEv, NOT applied: extern void *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int a, unsigned int b, void *pos, void *dir, int e, int f); */
/* TUBUILD CONFLICT -- alternate declaration of func_ov060_02111cc0, from the legacy file for _ZN7daKpa_c13InitResourcesEv, NOT applied: extern void func_ov060_02111cc0(void *c, int a, int b); */
}

/* The two destructor bodies are defined first so their D1/D0 groups lead the
 * object in ROM order. Under defer_codegen off each out-of-line destructor
 * emits its D1, D0 and a homeless D2 at the definition. With the deferred
 * queue still empty here both groups land before every deferred function;
 * placed later, the second class's group instead rides the deferred flush and
 * ends the object. */
#pragma defer_codegen off

/* -------------------------------------------------------------------------- */
/* ROM ordinals 0-1 -- _ZN7daKpa_cD1Ev, 0x02111900 / _ZN7daKpa_cD0Ev, 0x02111950 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_cD1Ev
// @symbol _ZN7daKpa_cD0Ev
#ifdef _MSC_VER
/* THE HOST NEEDS THE ROM'S FLAT D0 NAME, AND MSVC NEVER EMITS IT. MSVC
 * folds the Itanium destructor variants into the one ~daKpa_c() it emits,
 * so compiling the definition below as well would define that symbol
 * twice. This arm spells out, in terms of it, what the deleting destructor
 * does: the D1 body, called qualified so it is a direct call even where a
 * header declares the destructor virtual, then the class-specific
 * operator delete. Nothing here reaches mwccarm: it builds the #else arm
 * and emits the ROM bytes it always emitted. */
extern "C" daKpa_c *_ZN7daKpa_cD0Ev(daKpa_c *thiz)
{
    thiz->daKpa_c::~daKpa_c();        /* the D1 body, through the one host symbol */
    daKpa_c::operator delete(thiz);  /* the class-specific delete D0 ends with */
    return thiz;
}
#else
daKpa_c::~daKpa_c()
{
}
#endif

/* -------------------------------------------------------------------------- */
/* ROM ordinals 2-3 -- _ZN11daKpaTail_cD1Ev, 0x021119b4 / _ZN11daKpaTail_cD0Ev, 0x021119e4 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaTail_cD1Ev
// @symbol _ZN11daKpaTail_cD0Ev
#ifdef _MSC_VER
/* The host needs the ROM's flat D0 name; see the note on _ZN7daKpa_cD0Ev. */
extern "C" daKpaTail_c *_ZN11daKpaTail_cD0Ev(daKpaTail_c *thiz)
{
    thiz->daKpaTail_c::~daKpaTail_c();    /* the D1 body, through the one host symbol */
    daKpaTail_c::operator delete(thiz);  /* the class-specific delete D0 ends with */
    return thiz;
}
#else
daKpaTail_c::~daKpaTail_c()
{
}
#endif

#pragma defer_codegen on

/* -------------------------------------------------------------------------- */
/* ROM ordinal 78 -- _ZN11daKpaTail_c13InitResourcesEv, 0x021163b4, size 0x3c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaTail_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method */
int daKpaTail_c::InitResources()
{
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(((char *)this) + 0xd4, ((char *)this), 0x32000, 0x50000, 0x800000, 0x1000);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 77 -- _ZN7daKpa_c13InitResourcesEv, 0x02116130, size 0x284 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method
 *
 * The other half of daKpa_c::CleanupResources. Every handle this loads is one
 * the cleanup releases, in the same order and with the same counts -- one
 * single, a 0x1c-entry table, a six-entry table, then two more singles, the
 * last of which (data_ov089_02132c50) lives in ov089 rather than this overlay.
 * That pairing is why the siblings' CleanupResources are bare `return 1`s:
 * daKpa_c loads the whole fight's resources, so daKpa_c frees them.
 *
 * The two loops are reproduced rather than unrolled, for the same reason as in
 * the cleanup: 0x1c and 6 are the counts the ROM's own comparisons test.
 *
 * `Vector3 pos` was a local shadow typedef; it is the real types.h Vector3
 * here, which is layout-identical (Fix12i is s32) and costs nothing.
 *
 * The fields this used to spell as unk_ are the base classes' and are named now:
 * mVertAccel / mTerminalVelocity are dActor_c::mVertAccel and dActor_c::mTerminalVelocity -- and the
 * values written here, -0x2000 and -0x3c000, are fix12 gravity and terminal
 * velocity, which is the same evidence dActor_c.h cites from BooCage and daPiano_c.
 * mParam is fBase_c::param1, uniqueID is fBase_c::uniqueID, and mAreaId is
 * dActor_c::mAreaId -- which is why it is read as a signed char and handed straight
 * to dActor_c::Spawn's areaID parameter.
 *
 * The early `return 0` when ShadowModel::InitCylinder fails is the ROM's -- the
 * only failure path in the function.
 */
int daKpa_c::InitResources()
{
    int i;
    Vector3 pos;
    void *a1;
    void *a2;

    _ZN9ModelBase7SetFileEP8BMD_Fileii(&this->mModelAnim,
        _ZN5Model8LoadFileER13SharedFilePtr(*(SharedFilePtr *)data_ov060_0211ac78), 1, 0x16);

    for (i = 0; i < 0x1c; i++)
        _ZN9Animation8LoadFileER13SharedFilePtr((void *)data_ov060_021192dc[i]);

    for (i = 0; i < 6; i++)
        _ZN15TextureSequence8LoadFileER13SharedFilePtr((void *)data_ov060_0211927c[i]);

    _ZN5Model8LoadFileER13SharedFilePtr(*(SharedFilePtr *)data_ov060_0211b208);
    _ZN5Model8LoadFileER13SharedFilePtr(data_ov089_02132c50);

    if (_ZN11ShadowModel12InitCylinderEv(&this->mShadowModel) == 0)
        return 0;

    /* Same object as the other ~30 call sites, which hand it the base pointer as
     * char*. Inside a member function `this` is daKpa_c*, so cast it the same way
     * rather than widening the helper's signature for one caller. */
    func_ov060_02111cc0((char *)this, 0x10, 0);

    TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1],
                             *(BTP_File *)data_ov060_0211ac28[1]);

    _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
        &this->mTextureSequence, (void *)data_ov060_0211ac28[1], 0, 0x1000, 0);

    _ZN9Animation8SetFlagsEi(&this->mTextureSequence, 0x40000000);

    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
        &this->mdCcAcPos_c, this, &pos, 0x78000, 0x118000, 0x200004, 0);

    this->mHomePosX = this->mPosX;
    this->mHomePosY = this->mPosY;
    this->mHomePosZ = this->mPosZ;
    this->mVertAccel = -0x2000;
    this->mTerminalVelocity = -0x3c000;
    this->mTargetPlayer = 0;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        &this->mWithMeshClsn, this, 0x50000, 0x50000, 0, 0);
    _ZN10dBgCh_Actr13SetLimMovFlagEv(&this->mWithMeshClsn);

    this->mState = 0;
    this->mVariantID = (char)(this->param1 & 3);
    this->unk_416 = (char)(((unsigned int)this->param1 >> 2) & 1);
    this->mTimer = 0;
    this->unk_423 = 0;
    this->mDropsShadow = 1;
    this->mBounceOnLand = 0;
    this->mScaleX = 0x1000;
    this->mScaleY = 0x1000;
    this->mScaleZ = 0x1000;
    this->mAnimSpeed = 0x1000;
    this->unk_429 = 1;
    func_ov060_021123dc(this);

    this->mTalkStep = 0;
    this->mCutsceneStep = 0;

    /* Spawn takes the three position words as one Vector3. dActor_c stores them as
     * mPosX/Y/Z at 0x5c, so the argument is the vector, not a pointer to the first
     * scalar. (dActor_c::Pos() would say this directly, but that accessor is not on
     * this branch yet.) */
    a1 = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
        0x118, 0, (Vector3 *)&this->mPosX, 0, this->mAreaId, -1);
    *(int *)((char *)a1 + 0x2cc) = this->uniqueID;

    a2 = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
        0x116, 0, (Vector3 *)&this->mPosX, 0, this->mAreaId, -1);
    this->mUniqueID_3a8 = *(int *)((char *)a2 + 4);
    *(int *)((char *)a2 + 0x108) = this->uniqueID;
    this->unk_42a = 5;
    this->mCapActorAlive = 0;
    this->mParticleHandle = 0;
    this->mStompFxLatch = 0;
    this->mSoundHandle = 0;
    this->mSoundID = 0;
    func_02011d50(a2);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 76 -- _ZN11daKpaTail_c8BehaviorEv, 0x02116078, size 0xb8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaTail_c8BehaviorEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header
 *
 * The tail follows daKpa_c: find him by the uniqueID stashed in mBowserUniqueID, then park
 * this actor 0x8c units out from his position along his facing angle.
 * data_02082214 is a sin/cos table indexed by angle>>4, two shorts per entry.
 *
 * The one-line `struct dActor_c { static dActor_c* FindWithID(unsigned int); };` stand-in
 * this file used to carry is gone -- dActor_c.h already declared FindWithID, so it was
 * never needed.
 *
 * THE POINTER BUMP AND THE volatile ARE LOAD-BEARING, both measured. Reading
 * daKpa_c's fields the obvious way -- `bowser->mPrevAngleY`, `bowser->mPosX` and so
 * on, which the real dActor_c now makes possible -- compiles and does not reproduce
 * the ROM. The bump to +0x5c and the three loads off it are what the original
 * source did, and the offsets are dActor_c's: 0x94 is mPrevAngleY, 0x5c..0x64 are
 * mPosX/mPosY/mPosZ.
 */
int daKpaTail_c::Behavior()
{
    dActor_c* a = dActor_c::FindWithID(mBowserUniqueID);
    if (!a) return 1;

    int ang = *(short*)((char*)a + 0x94);        /* dActor_c::mPrevAngleY */
    a = (dActor_c*)((int)a + 0x5c);                 /* dActor_c::mPosX */
    int x = *(int*)a;
    volatile int v[3];
    v[0] = x;
    v[1] = ((int*)a)[1];                         /* mPosY */
    v[2] = ((int*)a)[2];                         /* mPosZ */
    int j = 2 * (((unsigned short)(short)(ang + 0x8000)) >> 4);
    mPosX = (short)data_02082214[j] * 0x8c + x;
    mPosY = v[1];
    mPosZ = (short)data_02082214[j + 1] * 0x8c + v[2];

    func_ov060_02115b84(((char*)this));
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 75 -- _ZN7daKpa_c8BehaviorEv, 0x02115f64, size 0x114 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_c8BehaviorEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header
 *
 * One frame of the fight: pick the closest player and record the angle and distance
 * to him, run the two state workers, advance the animation, publish `this` into the
 * global at data_0209f318+0x114, then rebuild the body cylinder 0x50000 in front of
 * the actor.
 *
 * FOUR STAND-IN STRUCTS ARE GONE -- `dActor_c`, `Animation`, `dCc_c` and
 * `dCcAcPos_c`, each declared here with just the one or two methods
 * this file called, then "defined" again below with a set of bodyless declarations
 * that existed only to stop the compiler mangling them differently. All four are
 * the real classes now, and the casts that reached them go with them:
 * mdCcAcPos_c IS a dCc_c by inheritance
 * (dCcAcPos_c -> dCcAc_c -> dCc_c), so Clear() and
 * Update() are called directly.
 *
 * The two fields this used to spell as its own were the ModelAnim's: `mAnimation`
 * at 0x124 is the Animation base inside mModelAnim at +0x50, and `unk_130` at 0x130
 * is that base's `speed` at +0x0c. Advancing "the animation" is advancing the model.
 */
int daKpa_c::Behavior()
{
    RandomIntInternal(&data_0209e650);
    mTargetPlayer = (dActor_c *)ClosestPlayer();
    if (mTargetPlayer != 0) {
        mAngleToTarget = Vec3_HorzAngle((Vector3*)((char*)&mPosX), (Vector3*)&mTargetPlayer->mPosX);
        mDistToTarget = Vec3_HorzDist((Vector3*)((char*)&mPosX), (Vector3*)&mTargetPlayer->mPosX);
    } else {
        mAngleToTarget = mAngleY;
        mDistToTarget = ~0x80000000;
    }
    func_ov060_02112434(((char*)this));
    func_ov060_02111a28(((char*)this));
    mPrevAngleY = mAngleY;
    mModelAnim.speed = mAnimSpeed;
    mModelAnim.Advance();
    func_ov060_0211577c(((char*)this));
    *(char**)(data_0209f318 + 0x114) = ((char*)this);
    mdCcAcPos_c.Clear();
    Vector3 v;
    v.z = 0x50000;
    v.x = 0;
    v.y = 0;
    mdCcAcPos_c.SetPosRelativeToActor(v);
    mdCcAcPos_c.Update();
    if (mCapActorAlive != 0) {
        dActor_c* f = dActor_c::FindWithActorID(0x10d, 0);
        if (f == 0) mCapActorAlive = 0;
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 74 -- _ZN11daKpaTail_c6RenderEv, 0x02115f5c, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaTail_c6RenderEv
/* recovered: shared header, real C++ method
 *
 * `return 1` and nothing else. The tail is drawn as part of Bowser's own
 * model rather than separately, so its render slot draws nothing.
 */
int daKpaTail_c::Render()
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 73 -- _ZN7daKpa_c6RenderEv, 0x02115f0c, size 0x50 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_c6RenderEv
/* recovered: named members + shared header, real C++ method */
int daKpa_c::Render()
{
  if(mOpacity < 8) return 1;
  _ZN9Animation7AdvanceEv((char*)&mTextureSequence);
  _ZN15TextureSequence6UpdateER15ModelComponents(((char*)this)+0x138, ((char*)this)+0xdc);
  /* The old shard declared a local `struct Obj` with a method `m` purely so this
   * call would compile; it is ModelAnim's real slot-5 Render(const Vector3*), which
   * takes the scale vector. daKpa2Bg_c::Render makes the same call as mModel2.Render. */
  mModelAnim.Render((const Vector3 *)&mScaleX);
  return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 72 -- _ZN7daKpa_c16OnPendingDestroyEv, 0x02115f08, size 0x4 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_c16OnPendingDestroyEv
/* recovered: shared header, real C++ method
 *
 * Empty -- the ROM body is a single `bx lr`. The override exists to suppress
 * whatever the base does on pending destroy, not to do anything itself.
 */
void daKpa_c::OnPendingDestroy()
{
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 71 -- _ZN11daKpaTail_c16CleanupResourcesEv, 0x02115f00, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaTail_c16CleanupResourcesEv
/* recovered: shared header, real C++ method
 *
 * `return 1`, no releases. The tail shares Bowser's translation unit and his
 * files -- tu_map puts both classes in one TU at 0x2111900..0x2116484 -- so
 * Bowser::CleanupResources frees everything and the tail takes no reference of
 * its own.
 */
int daKpaTail_c::CleanupResources()
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 70 -- _ZN7daKpa_c16CleanupResourcesEv, 0x02115e80, size 0x80 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN7daKpa_c16CleanupResourcesEv
/* recovered: shared header, real C++ method
 *
 * daKpa_c frees the whole fight. One single, then a 0x1c-entry table and a
 * six-entry table walked by index, then two more singles -- one of which
 * (data_ov089_02132c50) lives in ov089, not this overlay.
 *
 * The two loops are reproduced rather than unrolled: 0x1c and 6 are the counts
 * the ROM's own comparisons test against, and the tables are arrays of
 * POINTERS to handles, unlike the singles which are handles themselves.
 *
 * That is why his siblings release almost nothing -- daKpaFire_c and daKpaTail_c
 * hold no reference at all, and daFRing_c shares 0211b208 with him.
 */
int daKpa_c::CleanupResources()
{
    int i;
    ((SharedFilePtr *)(&data_ov060_0211ac78))->Release();
    for (i = 0; i < 0x1c; i++)
        data_ov060_021192dc[i]->Release();
    for (i = 0; i < 6; i++)
        data_ov060_0211927c[i]->Release();
    ((SharedFilePtr *)(&data_ov060_0211b208))->Release();
    ((SharedFilePtr *)(&data_ov089_02132c50))->Release();
    func_02011cfc();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 69 -- func_ov060_02115d68, 0x02115d68, size 0x118 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115d68
extern "C" void func_ov060_02115d68(char *self) {
    u32 *p = (u32 *)((char *)(((int)self + 0xec)));
    dActor_c *r5 = _ZN8dActor_c10FindWithIDEj(*(u32 *)(self + 0x108));
    *p &= ~1u;
    if (*(s32 *)((char *)r5 + 0x40c) != 0x13) goto skip_13;
    func_ov060_021123a0(r5, 1);
    return;
skip_13:
    if (*(s32 *)((char *)r5 + 0x40c) == 4) return;
    func_ov060_021123a0(r5, 1);
    if (*(u32 *)(self + 0xf8) == 0) return;
    if (!(*(u32 *)(self + 0xf4) & 0x1000)) return;
    dActor_c *r4 = _ZN8dActor_c10FindWithIDEj(*(u32 *)(self + 0xf8));
    if (!r4) return;
    if (_ZN6Player15IsCollectingCapEv((Player *)r4)) return;
    if (!_ZN6Player7TryGrabER8dActor_c((Player *)r4, *(dActor_c *)self)) return;
    s16 *ip = (s16 *)((char *)r5 + 0x8c);
    *(s16 *)((char *)r4 + 0x8c) = *(s16 *)((int)ip);
    *(s16 *)((char *)r4 + 0x8e) = ip[1];
    *(s16 *)((char *)r4 + 0x90) = ip[2];
    *(s16 *)((char *)r4 + 0x92) = *(s16 *)((int)ip);
    *(s16 *)((char *)r4 + 0x94) = ip[1];
    *(s16 *)((char *)r4 + 0x96) = ip[2];
    *(s32 *)(self + 0x110) = 2;
    *(s32 *)(self + 0x10c) = (s32)r4;
    func_ov060_021123a0(r5, 0);
    *(s32 *)((char *)r5 + 0x410) = 1;
    *(s32 *)((char *)r5 + 0x3a4) = (s32)r4;
    *(s16 *)(self + 0x116) = 0x96;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 68 -- func_ov060_02115d50, 0x02115d50, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115d50
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115d50(unsigned char *p){
  if (*(unsigned short*)(p + 0x114) > 0x1e)
    *(unsigned*)(p + 0x110) = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 67 -- func_ov060_02115c1c, 0x02115c1c, size 0x134 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115c1c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115c1c(char *self)
{
    char *r4;
    char *r1;
    int cond;

    r4 = (char *)_ZN8dActor_c10FindWithIDEj(*(unsigned int *)(self + 0x108));
    if (*(int *)(r4 + 0x40c) == 0x13) {
        func_ov060_021123a0(r4, 1);
        *(int *)(self + 0x110) = 0;
    }

    cond = (int)((*(int *)(self + 0xb0) & 0x400) != 0);
    if (cond != 0) {
        *(int *)(self + 0x110) = 1;
        *(void **)(self + 0x10c) = 0;
        func_ov060_021123c8(r4);
        *(int *)(r4 + 0x410) = 2;
        return;
    }

    r1 = *(char **)(self + 0x10c);
    if (r1 == 0) goto tail;

    cond = (int)(*(int *)(r1 + 0x358) != 0);
    if (cond == 0) goto do_drop;

    if (*(short *)(r1 + 0x600 + 0x9c) != 0) goto set_default;
    if (*(u16 *)(self + 0x100 + 0x16) == 0) goto tail;
    *(u16 *)(self + 0x116) -= 1;
    if (*(u16 *)(self + 0x100 + 0x16) != 0) goto tail;

do_drop:
    *(int *)(self + 0x110) = 1;
    _ZN6Player9DropActorEv(*(void **)(self + 0x10c));
    *(void **)(self + 0x10c) = 0;
    func_ov060_021123c8(r4);
    *(int *)(r4 + 0x410) = 3;
    goto tail;

set_default:
    *(u16 *)(self + 0x100 + 0x16) = 0x96;

tail:
    *(int *)(self + 0xec) |= 1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 66 -- func_ov060_02115b84, 0x02115b84, size 0x98 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115b84
/* This shard re-declared `struct C; typedef void (C::*PMF)();` against its own
 * forward declaration of C, which collided with the PMF already defined at the
 * top of this TU (on the real C). One typedef serves both; only the opaque
 * padding view of the object is local to this shard. */
extern PMF data_ov060_0211ae9c[];
extern "C" {

extern void _ZN5dCc_c5ClearEv(void* cc);
extern void _ZN5dCc_c6UpdateEv(void* cc);
}
struct C_func15b84 { char pad[0x800]; };
extern "C" void func_ov060_02115b84(char* c) {
  char* r5 = (char *)_ZN8dActor_c10FindWithIDEj(*(unsigned int*)(c + 0x108));
  int idx = *(int*)(c + 0x110);
  (((daKpa_c *)c)->*data_ov060_0211ae9c[idx])();
  if (*(int*)(r5 + 0x40c) == 4) {
    int* f = (int*)(((int)c + 0xec));
    *f = *f | 1;
  }
  {
    unsigned short* h = (unsigned short*)(((int)c + 0x114));
    *h = *h + 1;
  }
  if (idx != *(int*)(c + 0x110)) {
    short* z = (short*)(c + 0x100);
    z[0xa] = 0;
  }
  _ZN5dCc_c5ClearEv(c + 0xd4);
  _ZN5dCc_c6UpdateEv(c + 0xd4);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 65 -- func_ov060_02115b0c, 0x02115b0c, size 0x78 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115b0c
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115b0c(char* c)
{
    struct Vector3 v;
    if (*(unsigned char*)(c + 0x414) != 2) return;
    v.x = *(int*)(c + 0x5c);
    v.y = *(int*)(c + 0x60);
    v.z = *(int*)(c + 0x64);
    _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(c, &v, 0x7d0000);
    _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x119, 0, (struct Vector3*)(c + 0x5c), 0, *(signed char*)(c + 0xcc), -1);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 64 -- func_ov060_02115a84, 0x02115a84, size 0x88 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115a84
// @symbol func_ov060_02115a84
/* recovered: shared common types */
#include "common.h"
extern "C" {

extern int _ZNK10dBgCh_Actr13JustHitGroundEv(void* p);
extern void _ZN8dActor_c13LandingDustAtER7Vector3b(void* a, struct Vector3* v, int b);

void func_ov060_02115a84(char* c, char* arg){
  if(_ZNK10dBgCh_Actr13JustHitGroundEv(c+0x14c)==0) return;
  *(unsigned short*)arg = *(unsigned short*)arg + 1;
  if(*(unsigned short*)arg >= 4) return;
  struct Vector3 v;
  v.x = *(int*)(c+0x5c);
  v.y = *(int*)(c+0x60);
  v.z = *(int*)(c+0x64);
  _ZN8dActor_c13LandingDustAtER7Vector3b(c, &v, 0);
  func_02012694(0xbd, (const Vector3 *)(c + 0x74));
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 63 -- Bowser_IsAnimAtLastFrame, 0x02115a30, size 0x54 */
/* -------------------------------------------------------------------------- */
// @symbol Bowser_IsAnimAtLastFrame
// Bowser_IsAnimAtLastFrame at 0x02115a30 -- matched byte-for-byte with mwccarm 1.2/sp2p3 (ov060).
extern "C" bool Bowser_IsAnimAtLastFrame(void *o) {
    Obj *obj = (Obj *)o;
    return obj->anim.Finished() || obj->anim.WillHitFrame((unsigned short)(obj->anim.GetFrameCount() - 1));
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 62 -- func_ov060_0211577c, 0x0211577c, size 0x2b4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_0211577c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_0211577c(char *c)
{
    char pad[8];
    Matrix4x3 saved;
    Vec3 pos;
    Vec3 v;
    Vec3 v2;
    int zero;

    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c) == 0)
        goto skip_angles;
    if (*(int *)(c + 0x410) != 0)
        goto skip_angles;
    *(s16 *)(c + 0x8c) = func_02010844(c, c + 0x3bc, *(s16 *)(c + 0x8e));
    *(s16 *)(c + 0x90) = func_02010844(c, c + 0x3bc, (s16)(*(s16 *)(c + 0x8e) - 0x4000));
skip_angles:
    Vec3_Asr(&v, (Vec3 *)(c + 0x5c), 3);
    Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
    Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, *(s16 *)(c + 0x8e));
    Matrix4x3_ApplyInPlaceToRotationX(&data_020a0e68, *(s16 *)(c + 0x8c));
    Matrix4x3_ApplyInPlaceToRotationZ(&data_020a0e68, *(s16 *)(c + 0x90));
    *(Matrix4x3 *)(c + 0xf0) = data_020a0e68;
    _ZN9ModelBase12ApplyOpacityEjj(c + 0xd4, (unsigned char)((int)*(unsigned char *)(c + 0x41c) >> 3), 1);
    saved = data_020a0e68;
    zero = 0;
    *(int *)(c + 0x3e0) = zero;
    *(int *)(c + 0x3e4) = zero;
    *(int *)(c + 0x3e8) = zero;
    MulMat4x3Mat4x3(*(char **)(c + 0xe8) + 0x90, &data_020a0e68, &data_020a0e68);
    *(int *)(c + 0x3e0) = data_020a0e68.m[9];
    *(int *)(c + 0x3e4) = data_020a0e68.m[10];
    *(int *)(c + 0x3e8) = data_020a0e68.m[11];
    Vec3_LslInPlace(c + 0x3e0, 3);
    *(int *)(c + 0x3e4) = *(int *)(c + 0x60);
    *(int *)(c + 0x3d4) = zero;
    *(int *)(c + 0x3d8) = zero;
    *(int *)(c + 0x3dc) = zero;
    data_020a0e68 = saved;
    MulMat4x3Mat4x3(*(char **)(c + 0xe8) + 0x120, &data_020a0e68, &data_020a0e68);
    *(int *)(c + 0x3d4) = data_020a0e68.m[9];
    *(int *)(c + 0x3d8) = data_020a0e68.m[10];
    *(int *)(c + 0x3dc) = data_020a0e68.m[11];
    Vec3_LslInPlace(c + 0x3d4, 3);
    *(int *)(c + 0x3d8) = *(int *)(c + 0x60);
    if (*(unsigned char *)(c + 0x426) == 0)
        return;
    {
        dBgCh_Gnd rc;
        pos.x = *(int *)(c + 0x5c);
        pos.y = *(int *)(c + 0x60);
        pos.z = *(int *)(c + 0x64);
        pos.y = pos.y + 0x32000;
        rc.SetObjAndPos(*(Vector3 *)&pos, 0);
        if (rc.DetectClsn())
            pos.y = rc.clsnY;
        else
            pos.y = *(int *)(c + 0x60);
        Vec3_Asr(&v2, &pos, 3);
        Matrix4x3_FromTranslation(&data_020a0e68, v2.x, v2.y, v2.z);
        *(Matrix4x3 *)(c + 0x330) = data_020a0e68;
        _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(c, c + 0x308, c + 0x330, 0x140000, 0x64000, 0xf);
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 61 -- func_ov060_02115744, 0x02115744, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115744
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02115744(void *c)
{
    /* Both callees take the actor's base pointer as char* (Obj is the opaque
     * view of it); this shard holds it as void*, so the cast belongs at the call
     * rather than on the declaration. */
    func_ov060_02111cc0((char *)c, 0x13, 0x40000000);
    int r = Bowser_IsAnimAtLastFrame((Obj *)c);
    if (r != 0) {
        *(int*)((char*)c + 0x98) = 0x3000;
        return 1;
    }
    return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 60 -- func_ov060_02115718, 0x02115718, size 0x2c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115718
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02115718(void *c)
{
    func_ov060_02111cc0(c, 0x11, 0x0);
    *(int*)((char*)c + 0x98) = 12288;
    return Bowser_IsAnimAtLastFrame(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 59 -- func_ov060_021156ec, 0x021156ec, size 0x2c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021156ec
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_021156ec(void *c)
{
    func_ov060_02111cc0(c, 0x12, 0x40000000);
    *(int*)((char*)c + 0x98) = 0;
    return Bowser_IsAnimAtLastFrame(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 58 -- func_ov060_02115518, 0x02115518, size 0x1d4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115518
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115518(char *self)
{
  void *player = *((void **) (self + 0x3a0));
  u8 *new_var;
  if (player == 0)
  {
    return;
  }
  switch (*((u8 *) (self + 0x424)))
  {
    case 0:
      if (_ZN6Player9StartTalkER7fBase_cb(player, self, 1) == 0)
    {
      return;
    }
      *((int *) (self + 0x40c)) = 6;
    {
      u8 *p = (u8 *) ((void *) (((long long) ((int) (self + 0x424)))));
      *p = (*p) + 1;
    }
      return;

    case 1:
      if (_ZN6Player12GetTalkStateEv(player) != 0)
    {
      return;
    }
    {
      int pos[3];
      int x;
      int y;
      int z;
      int y2;
      int zero;
      int scale;
      int msg;
      s16 *tbl;
      s16 *msgs;
      u16 ang;
      s16 sx;
      s16 sz;
      x = *((int *) (self + 0x5c));
      tbl = data_02082214;
      pos[0] = x;
      y = *((int *) (self + 0x60));
      scale = 0xc8;
      pos[1] = (scale) ? (y) : (y);
      z = *((int *) (self + 0x64));
      y2 = y + 0xc8000;
      pos[2] = z;
      tbl = (s16 *) ((void *) data_02082214);
      ang = *((u16 *) (self + 0x8e));
      zero = 0;
      msgs = data_ov060_0211a4e0;
      sx = (s16) tbl[(ang >> 4) * 2];
      pos[0] = (((s16) sx) * ((s16) scale)) + x;
      tbl = data_02082214;
      ang = *((u16 *) (self + 0x8e));
      sz = (s16) tbl[((ang >> 4) * 2) + 1];
      pos[1] = y2;
      pos[2] = (((s16) sz) * ((s16) scale)) + z;
      msg = (s16) msgs[*((u8 *) (self + 0x414))];
      if (_ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(*((void **) (self + 0x3a0)), self, msg, pos, zero, zero) == 0)
      {
        return;
      }
    }
    {
      u8 *p = (u8 *) ((void *) (((long long) ((int) (self + 0x424)))));
      *p = (*p) + 1;
    }
      func_02012694(0xb7, (const Vector3 *)(self + 0x74));
      return;

    case 2:
      if (_ZN6Player12GetTalkStateEv(player) != (-1))
    {
      return;
    }
      if ((*((u8 *) (self + 0x414))) == 1)
    {
      *((int *) (self + 0x40c)) = 0xd;
    }
    else
    {
      *((int *) (self + 0x40c)) = 0;
    }
    {
      u8 *p = (u8 *) ((void *) (((long long) ((int) (self + 0x424)))));
      *p = (*p) + 1;
    }
      new_var = (u8 *) (self + 0x444);
      *new_var = 4;
      func_ov060_02111f08(self);
      return;

  }

}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 57 -- func_ov060_021154e8, 0x021154e8, size 0x30 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021154e8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021154e8(void *c) {
    func_ov060_02111f08(c);
    *(int*)((char*)c + 0x98) = 0;
    func_ov060_02111cc0(c, 0x10, 0);
    func_ov060_02115518((char *)c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 56 -- func_ov060_021153f8, 0x021153f8, size 0xf0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021153f8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021153f8(char *c)
{
    u8 state;
    func_ov060_02111f08(c);
    state = *(u8 *)(c + 0x423);
    if (state == 0) {
        if (func_ov060_02115744(c) == 0) return;
        {
            u8 *p = P423(c);
            *p = *p + 1;
        }
        return;
    }
    if (state <= 2) {
        if (func_ov060_02115718(c) == 0) return;
        {
            u8 *p = P423(c);
            *p = *p + 1;
            if (*(u8 *)(c + 0x414) == 2) {
                *p = *p + 1;
            }
        }
        return;
    }
    if (state == 3) {
        if (func_ov060_021156ec(c) == 0) return;
        func_ov060_02111cc0(c, 0x10, 0);
        {
            u8 *p = P423(c);
            *p = *p + 1;
        }
        return;
    }
    func_ov060_02115518((char *)c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 55 -- func_ov060_02115314, 0x02115314, size 0xe4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115314
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115314(char* c)
{
    if (*(unsigned char*)(c + 0x415) == 0) {
        if (*(int*)(c + 0x418) & 2) {
            if (*(int*)(c + 0x3ec) < 0x5dc000)
                *(int*)(c + 0x40c) = 0xf;
            else
                *(int*)(c + 0x40c) = 0x11;
        } else {
            *(int*)(c + 0x40c) = 0xe;
        }
        unsigned char* q = (unsigned char*)(((int)c + 0x415));
        *q = *q + 1;
        *(int*)(c + 0x3f8) = 0x1000;
    } else {
        *(unsigned char*)(c + 0x415) = 0;
        if (*(unsigned char*)(c + 0x429) == 0) {
            unsigned int v = (unsigned int)RandomIntInternal(&data_0209e650) >> 0x10;
            if (v % 10 == 0)
                *(int*)(c + 0x40c) = 3;
            else
                *(int*)(c + 0x40c) = 0xe;
        } else {
            *(unsigned char*)(c + 0x429) = 0;
            *(int*)(c + 0x40c) = 0xe;
        }
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 54 -- func_ov060_021151d4, 0x021151d4, size 0x140 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021151d4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021151d4(char *a)
{
    unsigned int m;

    if (*(unsigned char *)(a + 0x415) != 0)
        goto cold;

    if ((*(int *)(a + 0x418) & 2) == 0)
        goto setE;

    if (_ZN8dActor_c13DistToCPlayerEv(a) < 0x514000) {
        m = ((unsigned int)RandomIntInternal(&data_0209e650) >> 0x10) % 10;
        if (m < *(unsigned char *)(a + 0x42a)) {
            *(int *)(a + 0x40c) = 0x10;
            if (*(unsigned char *)(a + 0x42a) > 3)
                *(unsigned char *)(a + 0x42a) = 3;
            else
                *(unsigned char *)(a + 0x42a) = 1;
        } else {
            *(int *)(a + 0x40c) = 9;
            *(unsigned char *)(a + 0x42a) = 5;
        }
        goto tail;
    }

    *(int *)(a + 0x40c) = 7;
    if (*(int *)(a + 0x3f4) > 0x1f4000 && *(int *)(a + 0x3f4) < 0x5dc000) {
        m = ((unsigned int)RandomIntInternal(&data_0209e650) >> 0x10) % 10;
        if (m < 5)
            *(int *)(a + 0x40c) = 0xd;
    }
    goto tail;

setE:
    *(int *)(a + 0x40c) = 0xe;
tail:
    {
        unsigned char *p = (unsigned char *)(a + 0x415);
        *p = *p + 1;
    }
    return;

cold:
    *(unsigned char *)(a + 0x415) = 0;
    *(int *)(a + 0x40c) = 0xe;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 53 -- func_ov060_021150d0, 0x021150d0, size 0x104 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021150d0
#include "dActor_c.h"
extern "C" {
extern int RandomIntInternal(int* seed);
extern int data_0209e650;

void func_ov060_021150d0(void *v) {
    dActor_c *a = (dActor_c *)v;
    char* c = (char*)a;
    if (*(int*)(c + 0x418) & 2) {
        if (a->DistToCPlayer() < 0x3e8000) {
            if (((unsigned)RandomIntInternal(&data_0209e650) >> 0x10) % 10 < 4) {
                *(int*)(c + 0x40c) = 9;
            } else if (((unsigned)RandomIntInternal(&data_0209e650) >> 0x10) % 10 < 8) {
                *(int*)(c + 0x40c) = 8;
            } else {
                *(int*)(c + 0x40c) = 0xf;
            }
            *(short*)(c + 0x400) = 0;
            *(int*)(c + 0x3f8) = 0x1000;
        } else if (((unsigned)RandomIntInternal(&data_0209e650) >> 0x10) % 10 < 5) {
            *(int*)(c + 0x40c) = 0xd;
        } else {
            *(int*)(c + 0x40c) = 7;
        }
    } else {
        *(int*)(c + 0x40c) = 0xe;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 52 -- func_ov060_021150c4, 0x021150c4, size 0xc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021150c4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021150c4(void *v)
{
    int *p = (int *)v;
    p[259] = 13;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 51 -- func_ov060_02115060, 0x02115060, size 0x64 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115060
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115060(char *c)
{
  if (*((unsigned char *) (c + 0x415)) == 0)
  {
    if (*((unsigned char *) (c + 0x416)) == 0)
    {
      func_ov060_021150d0(c);
    }
    else
    {
      func_ov060_021150c4(c);
    }
    (*((unsigned char *) ((((unsigned int) c) + 0x415))))++;
    return;
  }
  *((unsigned char *) (c + 0x415)) = 0;
  *((int *) (c + 0x40c)) = 0xe;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 50 -- func_ov060_02115018, 0x02115018, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02115018
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115018(void *c) {
    int r1 = *(int *)((char *)c + 0xa8);
    if (r1 >= 0) return;
    int thresh = *(int *)((char *)c + 0x3b4) - 0x12c000;
    int cur = *(int *)((char *)c + 0x60);
    if (cur >= thresh) return;
    *(int *)((char *)c + 0x64) = 0;
    *(int *)((char *)c + 0x5c) = *(int *)((char *)c + 0x64);
    *(int *)((char *)c + 0x60) = *(int *)((char *)c + 0x3b4) + 0x7d0000;
    *(int *)((char *)c + 0xa8) = 0;
    *(int *)((char *)c + 0x98) = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 49 -- func_ov060_02114ff8, 0x02114ff8, size 0x20 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114ff8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02114ff8(void *c) {
    int r = Bowser_IsAnimAtLastFrame(c);
    if (r) {
        *(int*)((char*)c + 0x40c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 48 -- func_ov060_02114f88, 0x02114f88, size 0x70 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114f88
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02115314(char* c);
void func_ov060_021151d4(char* c);
void func_ov060_02115060(char* c);
void func_ov060_02114f88(char* c){
  c[0x422] = 0;
  func_ov060_02111cc0(c, 0x10, 0);
  *(short*)(c+0x402) = 0;
  *(int*)(c+0x98) = 0;
  *(int*)(c+0xa8) = 0;
  if(*(unsigned char*)(c+0x414) == 0) func_ov060_02115314(c);
  else if(*(unsigned char*)(c+0x414) == 1) func_ov060_021151d4(c);
  else func_ov060_02115060(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 47 -- func_ov060_02114e9c, 0x02114e9c, size 0xec */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114e9c
extern "C" void func_ov060_02114e9c(char* c)
{
    *(int*)(c + 0x98) = 0;
    if (*(int*)(c + 0x134) == data_ov060_0211ac20[1]) {
        if (_ZNK9Animation12WillHitFrameEi(c + 0x124, 8)) {
            func_02012694(0xb5, (const Vector3 *)(c + 0x74));
        }
    }
    if (Bowser_IsAnimAtLastFrame(c) == 0) return;
    if (*(int*)(c + 0x134) == data_ov060_0211ac20[1]) {
        func_ov060_02111cc0(c, 6, 0x40000000);
        return;
    }
    if (*(int*)(c + 0x134) == data_ov060_0211ac68[1]) {
        func_ov060_02111cc0(c, 7, 0x40000000);
        return;
    }
    if (*(int*)(c + 0x134) == data_ov060_0211ac70[1]) {
        *(int*)(c + 0x40c) = 0;
        return;
    }
    func_ov060_02111cc0(c, 8, 0x40000000);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 46 -- func_ov060_02114d08, 0x02114d08, size 0x194 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114d08
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02114d08(char* self)
{
    int r4;
    short step;

    r4 = _ZN8dActor_c14GetSubtractionEss(self, *(s16*)(self + 0x8e), *(s16*)(self + 0x406));

    if (*(unsigned char*)(self + 0x414) == 1) {
        step = 0x400;
    } else {
        int t = *(signed char*)(self + 0x41e);
        if (t > 2) {
            step = 0x400;
        } else if (t == 2) {
            step = 0x300;
        } else {
            step = 0x200;
        }
    }
    _Z14ApproachLinearRsss((s16*)(self + 0x8e), *(s16*)(self + 0x406), step);

    if (*(unsigned char*)(self + 0x423) == 0) {
        *(short*)(self + 0x3fe) = 0;
        if (func_ov060_02115744(self) == 0) return;
        (*(unsigned char*)(((int)self + 0x423)))++;
        return;
    }
    if (*(unsigned char*)(self + 0x423) <= 2) {
        if (func_ov060_02115718(self) == 0) return;
        (*(u16*)(((int)self + 0x3fe)))++;
        if (*(int*)(self + 0x418) & 0x20000) {
            if (*(u16*)(self + 0x3fe) < 5) return;
            *(int*)(((int)self + 0x418)) &= ~0x20000;
            return;
        }
        if (r4 >= 0x2000) return;
        *(int*)(self + 0x12c) = 0;
        (*(unsigned char*)(((int)self + 0x423)))++;
        *(short*)(self + 0x3fe) = 0;
        return;
    }
    if (func_ov060_021156ec(self) != 0)
        *(int*)(self + 0x40c) = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 45 -- func_ov060_02114b60, 0x02114b60, size 0x1a8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114b60
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02114b60(char *c)
{
  int *pflag = (int *) ((((int) c) + 0x378));
  *pflag |= 1;
  switch (*((u8 *) (c + 0x423)))
  {
    case 0:
      *((u8 *) (c + 0x41d)) = 0;
      *((s16 *) (c + 0x3fe)) = 0x1e;
      if ((*((u16 *) (c + 0x3fc))) == 0)
    {
      func_02012694(0xb9, (const Vector3 *)(c + 0x74));
    }
      if ((*((u8 *) (c + 0x41c))) != 0)
    {
      return;
    }
    {
      u8 *ps = (u8 *) ((((int) c) + 0x423));
      *ps = (*ps) + 1;
    }
      *((s16 *) (c + 0x8e)) = *((s16 *) (c + 0x406));
      return;

    case 1:
    {
      int r4 = 0;
      u16 *pd = (u16 *) ((((int) c) + 0x3fe));
      char *base3 = c + 0x300;
      int sub;
      u16 h = *pd;
      u16 h2 = *((u16 *) (base3 + 0xfe));
      *((u16 *) ((((int) c) + 0x3fe))) = h - 1;
      if (h2 != 0)
      {
        *((int *) (c + 0x98)) = 0x64000;
      }
      else
      {
        r4 = 1;
      }
      sub = _ZN8dActor_c14GetSubtractionEss(c, *((s16 *) (c + 0x8e)), *((s16 *) (c + 0x406)));
      if (sub > 0x4000)
      {
        if ((*((int *) (c + 0x3ec))) > 0x1f4000)
        {
          r4 = 1;
        }
      }
      if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c) == 0)
      {
        *((int *) (c + 0x5c)) = *((int *) (c + 0x3c8));
        *((int *) (c + 0x60)) = *((int *) (c + 0x3cc));
        *((int *) (c + 0x64)) = *((int *) (c + 0x3d0));
        r4 = 1;
        *((int *) (c + 0x98)) = 0;
      }
      if (r4 == 0)
      {
        return;
      }
      *((u8 *) (c + 0x423)) = 2;
      *((s16 *) (c + 0x8e)) = *((s16 *) (c + 0x406));
      return;
    }

    case 2:
      *((int *) (c + 0x98)) = 0;
      *((u8 *) (c + 0x41d)) = 0xff;
      if ((*((u8 *) (c + 0x41c))) == 0xff)
    {
      *((int *) (c + 0x40c)) = 0;
      *pflag &= ~1;
    }
      return;

    default:
      return;

  }

}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 44 -- func_ov060_02114858, 0x02114858, size 0x308 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114858
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02114858(void *self)
{
  char *c = (char *) self;
  int new_var;
  int new_var2;
  s32 v134 = *((s32 *) (c + 0x134));
  if ((v134 == data_ov060_0211acd8.match) || ((v134 == data_ov060_0211acf0.match) && ((((u32) (((u32) (*((s32 *) (c + 0x12c)))) << 4)) >> 16) > 0x2a)))
  {
    s32 v12c = *((s32 *) (c + 0x12c));
    s32 v450 = *((volatile s32 *) (c + 0x450));
    s32 r4 = ((u32) (v12c << 4)) >> 16;
    if (v450 != 0x180)
    {
      *((s32 *) (c + 0x44c)) = 0;
    }
    *((s32 *) (c + 0x450)) = 0x180;
    new_var = 10;
    *((s32 *) (c + 0x44c)) = _ZN5Sound8PlayLongEjjjRK7Vector3s(*((s32 *) (c + 0x44c)), 3, *((s32 *) (c + 0x450)), (const Vector3 *) (c + 0x74), 0);
    u16 t = *((u16 *) (c + 0x400));
    if ((t % 5) == 0)
    {
      s32 pos[3];
      pos[0] = *((s32 *) (c + 0x5c));
      pos[1] = *((s32 *) (c + 0x60));
      pos[2] = *((s32 *) (c + 0x64));
      {
        s32 y = pos[1];
        u16 ang = *((u16 *) (c + 0x94));
        s16 *tbl = data_02082214;
        s16 sx = tbl[(ang >> 4) * 2];
        new_var2 = (sx * 0xc8) + pos[0];
        pos[1] = y + 0xb4000;
        pos[0] = new_var2;
        ang = *((u16 *) (c + 0x94));
        pos[2] = (tbl[((ang >> 4) * 2) + 1] * 0xc8) + pos[2];
      }
      if ((r4 >= 0xf) && (r4 < 0x14))
      {
        s32 rnd = RandomIntInternal(&data_0209e650);
        u32 hi = ((u32) rnd) >> 16;
        u32 m = hi % 10u;
        _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, (m << new_var) | 0x11, (Vector3 *) pos, (const Vector3_16 *) (c + 0x92), (int)*((s8 *)(c + 0xcc)), -1);
      }
      else
      {
        s32 rnd = RandomIntInternal(&data_0209e650);
        u32 hi = ((u32) rnd) >> 16;
        u32 m = hi % 10u;
        _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, (m << 10) | 1, (Vector3 *) pos, (const Vector3_16 *) (c + 0x92), (int)*((s8 *)(c + 0xcc)), -1);
      }
    }
    {
      u16 *pt = (u16 *) ((((int) c) + 0x400));
      *pt = (*pt) + 1;
    }
  }
  {
    int *p = (int *) ((((int) c) + 0x418));
    *p |= 0x20000;
  }
  if (Bowser_IsAnimAtLastFrame(c) == 0)
  {
    return;
  }
  new_var = 0x423;
  switch (*((u8 *) (c + new_var)))
  {
    case 0:
      func_ov060_02111cc0(c, 0x16, 0x40000000);
      func_02012694(0xb5, (const Vector3 *)(c + 0x74));
    {
      u8 *ps = (u8 *) ((((int) c) + 0x423));
      *ps = (*ps) + 1;
    }
      return;

    case 1:
      func_ov060_02111cc0(c, 0x15, 0);
    {
      u8 *ps = (u8 *) ((((int) c) + 0x423));
      *ps = (*ps) + 1;
    }
      return;

    case 2:
      func_ov060_02111cc0(c, 0x17, 0x40000000);
    {
      u8 *ps = (u8 *) ((((int) c) + 0x423));
      *ps = (*ps) + 1;
    }
      return;

    case 3:
      *((s32 *) (c + 0x40c)) = 0;
    {
      int *p = (int *) (((long long) ((int) (c + 0x418))));
      *p &= ~0x20000;
    }
      return;

  }

}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 43 -- func_ov060_021146d0, 0x021146d0, size 0x188 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021146d0
/* func_ov060_021146d0 at 0x021146d0 (ov060), size 0x188
 * Matched byte-for-byte with mwccarm 1.2/sp2p3.
 * flags: -O4,p -enum int -lang c99 -char signed -interworking -proc arm946e -gccext,on -msgstyle gcc
 */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021146d0(char *c) {
    if (*(u16*)(c + 0x300 + 0xfc) == 0) {
        *(int*)(c + 0x98) = -0x1c000;
        *(int*)(c + 0xa8) = 0x50000;
        *(s16*)(c + 0x8e) = *(s16*)(c + 0x400 + 8) + 0x8000;
        *(u8*)(c + 0x422) = 1;
        *(s16*)(c + 0x8c) = -0xc00;
        func_02012694(0xb1, (const Vector3 *)(c + 0x74));
    } else {
        if (*(u16*)(c + 0x8c) > 0xc00) {
            s16 *p8c = (s16*)((char*)c + 0x8c);
            *p8c = *p8c - 0xc00;
        } else {
            *(u16*)(c + 0x8c) = 0;
        }
    }
    {
        u8 st = *(u8*)(c + 0x423);
        if (st == 0) {
            func_ov060_02111cc0(c, 1, 0x40000000);
            {
                u8 *p = (u8*)((char*)c + 0x423);
                *p = *p + 1;
            }
            *(u16*)(c + 0x300 + 0xfe) = 0;
            return;
        }
        if (st == 1) {
            func_ov060_02115a84(c, c + 0x3fe);
            if (*(u16*)(c + 0x300 + 0xfe) == 1)
                func_ov060_02111cc0(c, 2, 0x40000000);
            if (*(u16*)(c + 0x300 + 0xfe) < 3) return;
            *(int*)(c + 0xa8) = 0;
            *(int*)(c + 0x98) = 0;
            {
                u8 *p = (u8*)((char*)c + 0x423);
                *p = *p + 1;
            }
            return;
        }
        if (st != 2) return;
        if (Bowser_IsAnimAtLastFrame(c) != 0) {
            if (*(s8*)(c + 0x400 + 0x1e) == 1) *(int*)(c + 0x40c) = 3;
            else *(int*)(c + 0x40c) = 0;
        }
        *(u8*)(c + 0x422) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 42 -- func_ov060_0211469c, 0x0211469c, size 0x34 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_0211469c
extern "C" int func_ov060_0211469c(void *c) {
    func_ov060_02111cc0(c, 0xb, 0x40000000);
    int r = ((Animation*)((char*)c + 0x124))->WillHitFrame(0x20);
    if (r != 0) return 1;
    return 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 41 -- func_ov060_021145d4, 0x021145d4, size 0xc8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021145d4
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_021145d4(char *c){
  struct Vector3 v;
  int b;
  if(_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c)){
    *(int*)(c + 0x98) = 0;
    *(int*)(c + 0xa8) = 0;
    v.x = *(int*)(c + 0x5c);
    v.y = *(int*)(c + 0x60);
    v.z = *(int*)(c + 0x64);
    func_0200fa04(c, &v, 0);
    func_ov060_02111cc0(c, 0xc, 0x40000000);
    func_02012694(0xb6, (const Vector3 *)(c + 0x74));
    if(*(unsigned char*)(c + 0x414) == 0){
      b = (*(int*)(c + 0x3ec) >= 0x352000);
      if(!_ZN6Player7IsInAirEv(*(void**)(c + 0x3a0)))
        func_ov002_020c56f0(*(unsigned char**)(c + 0x3a0), b);
    }
    return 1;
  }
  return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 40 -- func_ov060_021145a8, 0x021145a8, size 0x2c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021145a8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021145a8(void *c) {
    if (*(unsigned char*)((char*)c + 0x414) != 2) return;
    if (!(*(int*)((char*)c + 0x418) & 0x10000)) return;
    if (*(int*)((char*)c + 0x3f4) > 0x3e8000) {
        *(int*)((char*)c + 0x98) = 0x1e000;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 39 -- func_ov060_021143b8, 0x021143b8, size 0x1f0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021143b8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021143b8(char* self)
{
    unsigned char st = *(unsigned char*)(self + 0x423);
    int vec[3];

    if (st == 0) {
        if (func_ov060_0211469c(self) == 0) return;
        if (*(unsigned char*)(self + 0x414) == 2 && (*(int*)(self + 0x418) & 0x10000)) {
            *(int*)(self + 0xa8) = 0x28000;
        } else {
            *(int*)(self + 0xa8) = 0x32000;
        }
        func_02012694(0xb1, (const Vector3 *)(self + 0x74));
        *(short*)(self + 0x3fe) = 0;
        *(int*)(self + 0x98) = 0;
        func_ov060_021145a8(self);
        if (*(int*)(self + 0x98) == 0) {
            *(int*)(self + 0x5c) = *(int*)(self + 0x3c8);
            *(int*)(self + 0x64) = *(int*)(self + 0x3d0);
        }
        (*(unsigned char*)(((int)self + 0x423)))++;
        *(int*)(self + 0x3f8) = 0x1000;
        return;
    }
    if (st == 1) {
        if (*(unsigned char*)(self + 0x414) == 2 && (*(int*)(self + 0x418) & 0x10000)) {
            func_ov060_02115018(self);
        }
        if (func_ov060_021145d4(self) == 0) {
            if (*(int*)(self + 0x60) >= *(int*)(self + 0x3cc)) return;
        }
        if (*(int*)(self + 0xa8) != 0) {
            vec[0] = *(int*)(self + 0x5c);
            vec[1] = *(int*)(self + 0x60);
            vec[2] = *(int*)(self + 0x64);
            func_0200fa04(self, vec, 0);
            func_ov060_02111cc0(self, 0xc, 0x40000000);
        }
        *(int*)(((int)self + 0x418)) &= ~0x10000;
        *(int*)(self + 0x98) = 0;
        *(int*)(self + 0xa8) = 0;
        *(int*)(self + 0x60) = *(int*)(self + 0x3cc);
        (*(unsigned char*)(((int)self + 0x423)))++;
        func_ov060_02115b0c(self);
        if (*(unsigned char*)(self + 0x414) == 1) {
            void* a;
            *(int*)(self + 0x40c) = 0x13;
            a = _ZN8dActor_c15FindWithActorIDEjPS_(0xa6, 0);
            if (a != 0) {
                *(int*)(self + 0x3ac) = *(int*)((char*)a + 4);
            }
        }
        return;
    }
    if (Bowser_IsAnimAtLastFrame(self) != 0) {
        *(int*)(self + 0x40c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 38 -- func_ov060_02114300, 0x02114300, size 0xb8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02114300
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02114300(char *c)
{
    unsigned char k = *(unsigned char*)(c + 0x423);
    if (k == 0) {
        if (!func_ov060_0211469c(c)) return;
        {
            int v;
            *(int*)(c + 0xa8) = 0x32000;
            *(int*)(c + 0x98) = 0x19000;
            *(short*)(c + 0x3fe) = 0;
            v = *(unsigned char*)(((int)c + 0x423)) + 1;
            *(unsigned char*)(((int)c + 0x423)) = (unsigned char)v;
            func_02012694(0xb1, (const Vector3 *)(c + 0x74));
        }
    } else if (k == 1) {
        if (!func_ov060_021145d4(c)) return;
        {
            unsigned char *p = (unsigned char*)(((int)c + 0x423));
            *p = *p + 1;
        }
    } else {
        if (Bowser_IsAnimAtLastFrame(c)) *(int*)(c + 0x40c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 37 -- func_ov060_021142b4, 0x021142b4, size 0x4c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021142b4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021142b4(void *c)
{
    *(int*)((char*)c + 0x98) = 0;
    unsigned short* p = (unsigned short*)((char*)c + 0x3fc);
    if (*p == 0) {
        *(unsigned short*)((char*)c + 0x3fe) = 0;
    }
    func_ov060_02111cc0(c, 0x1b, 0x40000000);
    int r = Bowser_IsAnimAtLastFrame(c);
    if (r != 0) {
        *(int*)((char*)c + 0x40c) = 0xb;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 36 -- func_ov060_021140c0, 0x021140c0, size 0x1f4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021140c0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021140c0(char* c)
{
    int new_var;
    if (*(u16*)((c + 0x300) + 0xfc) == 0) {
        void* player = *(void**)(c + 0x3a0);
        if (player == 0 || _ZN6Player9GetHealthEv(player) > 4) {
            s32 rnd = RandomIntInternal(&data_0209e650);
            u32 hi = ((u32)rnd) >> 16;
            u32 m = hi % 10u;
            c[0x428] = (char)((m % 3u) + 1);
        } else {
            c[0x428] = 3;
        }
    }
    if (*(int*)(c + 0x134) == data_ov060_0211abe0[1]) {
        if (_ZNK9Animation12WillHitFrameEi(c + 0x124, 5)) {
            Vector3 pos;
            u16 dir[3];
            u16 ax, ay, az;
            s16* tbl;
            s16 scale;
            int grav;
            /* `dp` must be a live pointer here, not a folded `&dir` at the call: the
               address has to be materialised before the two smlabb steps so it holds
               r2 across them, which is what pushes pos.x to r3 and `scale` to ip. */
            u16* dp;

            pos.x = *(int*)(c + 0x5c);
            tbl = data_02082214;
            pos.y = *(int*)(c + 0x60);
            scale = 0xe8;
            pos.z = *(int*)(c + 0x64);
            ax = *(u16*)(c + 0x8c);
            ay = *(u16*)(c + 0x8e);
            dir[1] = ay;
            dir[0] = ax;
            az = *(u16*)(c + 0x90);
            dir[2] = az;
            pos.x = (tbl[(dir[1] >> 4) * 2] * scale) + pos.x;
            new_var = pos.y + 0x58000;
            dp = dir;
            pos.z = (tbl[((dp[1] >> 4) * 2) + 1] * scale) + pos.z;
            /* Scheduling barrier, not dead code. Both operands are provably non-null,
               so this changes nothing at runtime, but the short-circuit `&&` splits the
               block and stops the scheduler hoisting the 0xa000 constant into the slot
               the ROM gives to `dp`. Without it this function is 11 words off. */
            if (dp != 0 && c != 0) {
            }
            pos.y = new_var;
            dir[0] = 0x1000;
            grav = 0xa000;
            _ZN8dActor_c13SpawnFireballERK7Vector3PK10Vector3_165Fix12IiES7_j(
                c, &pos, dp, 0x1e000, grav, 0);
            func_02012694(0x122, (const Vector3 *)(c + 0x74));
        }
    }
    if (Bowser_IsAnimAtLastFrame(c) != 0) {
        if (*(int*)(c + 0x134) == data_ov060_0211abe0[1]) {
            unsigned char* p = (unsigned char*)(c + 0x423);
            *p = (*p) + 1;
            if (*(unsigned char*)(c + 0x423) >= *(unsigned char*)(c + 0x428)) {
                *(int*)(c + 0x40c) = 0;
            }
        } else {
            func_ov060_02111cc0(c, 0x14, 0);
        }
        *(int*)(c + 0x12c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 35 -- func_ov060_02113ff4, 0x02113ff4, size 0xcc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113ff4
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02113ff4(char *c, unsigned short arg1, int arg2)
{
    int r;
    unsigned char f = *(unsigned char *)(c + 0x423);
    if (f == 0) {
        func_ov060_02111cc0(c, 0x13, 0x40000000);
        if (Bowser_IsAnimAtLastFrame(c) != 0) {
            unsigned char *p = (unsigned char *)(((int)c + 0x423));
            *p = *p + 1;
        }
    } else if (f == 1) {
        func_ov060_02111cc0(c, 0x12, 0x40000000);
        if (Bowser_IsAnimAtLastFrame(c) != 0) {
            unsigned char *p = (unsigned char *)(((int)c + 0x423));
            *p = *p + 1;
        }
    } else {
        func_ov060_02111cc0(c, 0x10, 0);
    }
    r = 0;
    *(int *)(c + 0x98) = 0;
    {
        short *q = (short *)(((int)c + 0x8e));
        *q += arg2;
    }
    if (*(unsigned short *)(c + 0x3fc) >= arg1)
        r = 1;
    return r;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 34 -- func_ov060_02113fcc, 0x02113fcc, size 0x28 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113fcc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113fcc(char *c) {
    int r0 = func_ov060_02113ff4(c, 0x3e, 0x200);
    if (r0 != 0) {
        *(int *)(c + 0x40c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 33 -- func_ov060_02113d8c, 0x02113d8c, size 0x240 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113d8c
bool ApproachLinear(short &value, short target, short step);

/* This shard carried a one-method `struct dActor_c { GetSubtraction(...); }`
 * stand-in. dActor_c is the real class (include/dActor_c.h, already included
 * above) and GetSubtraction is declared there, so the local copy is dropped. */

extern "C" {
void func_ov060_02111cc0(void *v, int idx, int fix);

int _Z14ApproachLinearRiii(int *dst, int target, int step);
}

extern "C" void func_ov060_02113d8c(char *r4)
{
    if (*(u16 *)(r4 + 0x3fc) == 0)
        *(int *)(r4 + 0x98) = 0;

    switch (*(u8 *)(r4 + 0x423)) {
    case 0:
        func_ov060_02111cc0(r4, 0x18, 0x40000000);
        *(u16 *)(r4 + 0x3fe) = 0;
        if (Bowser_IsAnimAtLastFrame(r4) != 0)
            *(u8 *)(r4 + 0x423) = 1;
        break;
    case 1:
        func_ov060_02111cc0(r4, 0x19, 0);
        *(int *)(r4 + 0x98) = 0x2a000;
        if (Bowser_IsAnimAtLastFrame(r4) != 0) {
            u16 *p = (u16 *)(r4 + 0x3fe);
            *p = *p + 1;
            if (*(u16 *)((r4 + 0x300) + 0xfe) > 0xa)
                *(u8 *)(r4 + 0x423) = 3;
            if (*(u16 *)((r4 + 0x300) + 0xfe) >= 2) {
                if (((dActor_c *)r4)->GetSubtraction(*(short *)(r4 + 0x406), *(short *)(r4 + 0x8e)) > 0x2000) {
                    *(u8 *)(r4 + 0x423) = 3;
                    *(int *)(r4 + 0x448) = 0;
                }
            }
        }
        ApproachLinear(*(short *)(r4 + 0x8e), *(short *)(r4 + 0x406), 0x200);
        break;
    case 3:
        *(u16 *)(r4 + 0x3fe) = 0;
        func_ov060_02111cc0(r4, 0x1a, 0x40000000);
        *(unsigned int *)(r4 + 0x448) = (unsigned int)_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
            *(unsigned int *)(r4 + 0x448), 0x101, *(int *)(r4 + 0x5c), *(int *)(r4 + 0x60) + 0x32000, *(int *)(r4 + 0x64), 0, 0);
        if (_Z14ApproachLinearRiii((int *)(r4 + 0x98), 0, 0x1000) != 0)
            *(u8 *)(r4 + 0x423) = 2;
        break;
    case 2:
        *(int *)(r4 + 0x98) = 0;
        if (Bowser_IsAnimAtLastFrame(r4) != 0) {
            if (*(u8 *)(r4 + 0x414) == 2)
                *(u16 *)(r4 + 0x3fc) = 0xa;
            else
                *(u16 *)(r4 + 0x3fc) = 0x1e;
            if (*(u16 *)((r4 + 0x300) + 0xfe) > *(u16 *)((r4 + 0x300) + 0xfc)) {
                *(int *)(r4 + 0x40c) = 0;
                *(int *)(r4 + 0x3f8) = 0x1000;
            }
            {
                u16 *p = (u16 *)(r4 + 0x3fe);
                *p = *p + 1;
            }
        }
        break;
    default:
        break;
    }

    if (((dBgCh_Actr *)(r4 + 0x14c))->IsOnGround())
        return;
    *(int *)(r4 + 0x40c) = 0xa;
    *(int *)(r4 + 0x5c) = *(int *)(r4 + 0x3c8);
    *(int *)(r4 + 0x60) = *(int *)(r4 + 0x3cc);
    *(int *)(r4 + 0x64) = *(int *)(r4 + 0x3d0);
    *(int *)(r4 + 0x98) = 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 32 -- func_ov060_02113d20, 0x02113d20, size 0x6c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113d20
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
// func_ov060_02113d20 at 0x02113d20
// Matched byte-for-byte with mwccarm 1.2/sp2p3 (ov060).
extern "C" int func_ov060_02113d20(dActor_c *self)
{
    Vector3 v;
    dActor_c *closest = self->ClosestWithActorID(0x11c);
    if (closest) {
        v.x = *(int *)((char *)self + 0x5c);
        v.y = *(int *)((char *)self + 0x60);
        v.z = *(int *)((char *)self + 0x64);
        if (func_ov060_02118544(closest, &v)) {
            func_ov060_021185c4(closest);
            return 1;
        }
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 31 -- func_ov060_02113b5c, 0x02113b5c, size 0x1c4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113b5c
/* recovered: shared common types */
extern "C" void func_ov060_02113b5c(char* c)
{
    if (*(u16*)(c + 0x300 + 0xfc) < 2) {
        *(u16*)(c + 0x300 + 0xfe) = 0;
    }

    int r4 = *(s32*)(c + 0x60);
    if (*(s32*)(c + 0x60) > *(s32*)(c + 0x3b4)) {
        Vector3 v;
        dBgCh_Gnd rg;
        int base = *(s32*)(c + 0x3b4);
        int zz = *(s32*)(c + 0x64);
        int xx = *(s32*)(c + 0x5c);
        int yy = base + 0x96000;
        v.x = xx;
        v.y = yy;
        v.z = zz;
        rg.SetObjAndPos(v, (dActor_c*)c);
        if (rg.DetectClsn() != 0) {
            int hy = rg.clsnY;
            if (hy >= *(s32*)(c + 0x3b4) - 0x64000) r4 = hy;
        }
    }

    if (*(u8*)(c + 0x423) == 0) {
        func_ov060_02111cc0(c, 9, 0);

        int dy = *(s32*)(c + 0x60) - r4;
        if (dy > 0xc8000 || *(s32*)(c + 0x3f4) > 0xdac000) {
            if (*(s32*)(c + 0x98) >= 0x1e000) {
                _Z14ApproachLinearRiii((int*)(c + 0x98), 0x1e000, 0x4000);
            }
        }

        func_ov060_02115a84(c, c + 0x3fe);

        if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c) != 0) {
            if (_ZNK10dBgCh_Actr13JustHitGroundEv(c + 0x14c) == 0) {
                *(s32*)(c + 0x98) = 0;
                *(u8*)(c + 0x427) = 0;
                u8* p = (u8*)(((int)c + 0x423));
                *p = *p + 1;
                func_ov060_02111cc0(c, 0xd, 0x40000000);
            }
        }
    } else {
        if (Bowser_IsAnimAtLastFrame(c) != 0) {
            *(s32*)(c + 0x40c) = 0;
        }
    }

    if (func_ov060_02113d20((dActor_c *)c) != 0) {
        signed char* q = (signed char*)(((int)c + 0x41e));
        *q = *q - 1;
        if (*(signed char*)(c + 0x400 + 0x1e) <= 0) {
            *(s32*)(c + 0x40c) = 4;
        } else {
            *(s32*)(c + 0x40c) = 0xc;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 30 -- func_ov060_02113a94, 0x02113a94, size 0xc8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113a94
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113a94(void* thiz)
{
    char* c = (char*)thiz;
    *(unsigned char*)(c + 0x41d) = 0;
    if (*(unsigned char*)(c + 0x41c) != 0) return;
    *(int*)(c + 0x98) = 0;
    *(int*)(c + 0xa8) = 0;
    *(int*)(c + 0x60) = *(int*)(c + 0x3b4) - 0x3e8000;
    if (Vec3_HorzLen((Vector3*)(c + 0x5c)) < 0xed8000) return;
    {
        Vector3 zero;
        int a;
        zero.x = 0;
        zero.y = 0;
        zero.z = 0;
        a = (int)(unsigned short)Vec3_HorzAngle(&zero, (Vector3*)(c + 0x5c)) >> 4;
        *(int*)(c + 0x5c) = (short)data_02082214[a * 2] * (short)0xed8;
        *(int*)(c + 0x64) = (short)data_02082214[a * 2 + 1] * (short)0xed8;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 29 -- func_ov060_02113740, 0x02113740, size 0x354 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113740
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113740(char *c)
{
    Vec3 pos;
    int hit;
    int ground;

    ground = *(int*)(c + 0x3b4) - 0x7d0000;
    hit = 0;
    if (*(int*)(c + 0x60) >= *(int*)(c + 0x3b4)) {
        dBgCh_Gnd rc;
        {
            int pz = *(int*)(c + 0x64);
            int py = *(int*)(c + 0x3b4) + 0x96000;
            int px = *(int*)(c + 0x5c);
            pos.x = px;
            pos.y = py;
            pos.z = pz;
        }
        rc.SetObjAndPos(*(Vector3 *)&pos, (dActor_c *)c);
        if (rc.DetectClsn()) {
            ground = rc.clsnY;
            /* The original shard read the collision id through a hand-rolled struct whose
             * `char result[0x34]` sat at offset 0x10, so `&rc.result` was simply rc + 0x10 --
             * a probe-state field, not a member of dBgCh_Gnd. The real class does not name
             * it, so the offset is spelled directly. */
            if ((int)((dBgPi *)&rc)->GetClsnID() != -1)
                hit = 1;
        }
    }

    *(int*)(c + 0x418) |= 0x10000;

    switch (*(u8*)(c + 0x423)) {
    case 0:
    {
        s16 *p8c = (s16*)(c + 0x8c);
        s16 *p90 = (s16*)(c + 0x90);
        if (*(u16*)(c + 0x3fc) == 0) {
            *(s16*)(c + 0x90) = 0;
            *(s16*)(c + 0x8c) = *(s16*)(c + 0x90);
        }
        *p8c += 0x800;
        *p90 += 0x800;
        if ((*(s16*)(c + 0x8c) & 0xffff) == 0)
            (*(u8*)((int)c + 0x423))++;
        func_ov060_02113a94(c);
        return;
    }
    case 1:
        func_ov060_02111cc0(c, 0xb, 0x40000000);
        if (((*(u32*)(c + 0x12c) << 4) >> 16) >= 0x20) {
            *(s16*)(c + 0x8e) = *(s16*)(c + 0x408);
            *(s16*)(c + 0x94) = *(s16*)(c + 0x8e);
            *(int*)(c + 0xa8) = 0x28000;
            func_02012694(0xb1, (const Vector3 *)(c + 0x74));
            *(int*)(c + 0x9c) = 0;
            *(u8*)(c + 0x41d) = 0xff;
            *(s16*)(c + 0x3fe) = 0;
            (*(u8*)((int)c + 0x423))++;
            return;
        }
        func_ov060_02113a94(c);
        return;
    case 2:
    {
        int thr;
        if (*(u8*)(c + 0x414) == 1) thr = 0x8fc000; else thr = 0x9c4000;
        if (*(int*)(c + 0x60) >= *(int*)(c + 0x3b4)) {
            *(int*)(c + 0x9c) = -0x1000;
            if (*(int*)(c + 0x3f4) < thr) {
                int d = ground - *(int*)(c + 0x3b4);
                if (d < 0) d = -d;
                if (d < 0x64000)
                    _Z14ApproachLinearRiii((int*)(c + 0x98), 0, 0x5000);
                else
                    _Z14ApproachLinearRiii((int*)(c + 0x98), 0x4b000, 0x2000);
            } else {
                _Z14ApproachLinearRiii((int*)(c + 0x98), 0x4b000, 0x2000);
            }
        }
        if (func_ov060_021145d4(c)) {
            (*(u8*)((int)c + 0x423))++;
            if (hit == 0) {
                func_ov060_02115b0c(c);
            } else {
                if (*(u8*)(c + 0x414) == 2) {
                    *(int*)(c + 0x40c) = 0xd;
                    *(int*)(c + 0x3f8) = 0x4000;
                    *(int*)(c + 0x9c) = -0x2000;
                }
            }
            if (*(u8*)(c + 0x414) == 1) {
                char *r;
                *(int*)(c + 0x40c) = 0x13;
                r = (char*)_ZN8dActor_c15FindWithActorIDEjPS_(0xa6, 0);
                if (r) *(int*)(c + 0x3ac) = *(int*)(r + 4);
                *(int*)(c + 0x9c) = -0x2000;
            }
        }
        func_ov060_02115018(c);
        return;
    }
    case 3:
        if (Bowser_IsAnimAtLastFrame(c) != 0) {
            *(int*)(c + 0x40c) = 0;
            *(int*)((int)c + 0x418) &= ~0x10000;
            *(int*)(c + 0x9c) = -0x2000;
        }
        return;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 28 -- func_ov060_02113710, 0x02113710, size 0x30 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113710
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113710(void *c) {
    func_ov060_02111cc0(c, 0xf, 0);
    if (Bowser_IsAnimAtLastFrame(c) != 0) {
        *(int *)((char *)c + 0x40c) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 27 -- func_ov060_021135fc, 0x021135fc, size 0x114 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021135fc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021135fc(char *c)
{
    volatile Vector3 base;
    int px, py, pz;
    px = *(int*)(c+0x5c);
    base.x = px;
    py = *(int*)(c+0x60);
    base.y = py;
    pz = *(int*)(c+0x64);
    base.z = pz;
    py = py + 0x32000;
    base.y = py;

    *(int*)(int)(c+0x378) |= 1;

    if (*(unsigned char*)(c+0x414) == 2) {
        Vector3 pos;
        pos.x = *(int*)(c+0x5c);
        pos.y = *(int*)(c+0x60);
        pos.z = *(int*)(c+0x64);
        pos.y = pos.y + 0xa0000;
        _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x11b, 7, &pos, 0, *(signed char*)(c+0xcc), -1);
    } else {
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xad, base.x, base.y, base.z);
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xae, base.x, base.y, base.z);
        void *spawned = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x11a, *(unsigned char*)(c+0x414), (Vector3*)(c+0x5c), 0, *(signed char*)(c+0xcc), -1);
        *(short*)((char*)spawned + 0x440) = *(short*)(c + 0x402);
        func_02012694(0xbb, (Vector3*)(c+0x74));
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 26 -- func_ov060_02113564, 0x02113564, size 0x98 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113564
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113564(char *c)
{
    func_ov060_02111cc0(c, 0, 0x40000000);
    if (*(unsigned char *)(c + 0x414) == 2)
        *(int *)(c + 0x98) = -0x1c000;
    else
        *(int *)(c + 0x98) = -0x19000;
    *(int *)(c + 0xa8) = 0x50000;
    *(int *)(c + 0x9c) = -0x2000;
    *(short *)(c + 0x8e) = (short)(*(short *)(c + 0x408) + 0x8000);
    *(short *)(c + 0x3fe) = 0;
    *(unsigned char *)(((int)c + 0x423)) += 1;
    *(int *)(c + 0x364) = 0xb4000;
    func_02012694(0xb1, (const Vector3 *)(c + 0x74));
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 25 -- func_ov060_021134ac, 0x021134ac, size 0xb8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021134ac
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021134ac(void* thiz)
{
    char* c = (char*)thiz;
    *(unsigned char*)(c + 0x422) = 1;
    func_ov060_02115a84(c, c + 0x3fe);
    if (_ZNK10dBgCh_Actr13JustHitGroundEv(c + 0x14c)) {
        if (*(int*)(c + 0x134) == data_ov060_0211ac88.b) {
            func_ov060_02111cc0(c, 5, 0x40000000);
        }
        {
            int* p = (int*)(((int)c + 0x98));
            *p = *p >> 1;
        }
    }
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c)) {
        if (!_ZNK10dBgCh_Actr13JustHitGroundEv(c + 0x14c)) {
            *(int*)(c + 0x98) = 0;
            {
                unsigned char* p = (unsigned char*)(((int)c + 0x423));
                *p = *p + 1;
            }
        }
    }
    func_ov060_02112350(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 24 -- func_ov060_02113404, 0x02113404, size 0xa8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113404
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02113404(char* c) {
  int r = 0;
  func_ov060_02112350(c);
  if (*(int*)(c+0x134) == *(int*)(data_ov060_0211ac60+4)) {
    char* base = *(char**)(c+0x3a0);
    if (base != 0) {
      int* o = (int*)(((int)base + 0x5c));
      struct Vector3 v;
      s16 ang;
      v.x = o[0];
      v.y = o[1];
      v.z = o[2];
      ang = *(s16*)(*(char**)(c+0x3a0) + 0x8e);
      if (Vec3_HorzDist((struct Vector3*)(c+0x5c), &v) < 0x258000) {
        if (_ZN8dActor_c14GetSubtractionEss(c, ang, *(s16*)(c+0x406)) > 0x6000) r = 1;
      }
    }
  }
  *(s16*)(c+0x3fe) = 0;
  return r;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 23 -- func_ov060_021132a4, 0x021132a4, size 0x160 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021132a4
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_021132a4(char *c)
{
    int r4 = 0;
    volatile Vector3 pos;
    int ytmp;
    int z;

    pos.x = *(int *)(c + 0x5c);
    ytmp = *(int *)(c + 0x60);
    pos.y = ytmp;
    z = *(int *)(c + 0x64);
    pos.z = z;
    pos.y = ytmp + 0x32000;

    *(int *)(c + 0x448) = (int)_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        *(volatile int *)(c + 0x448), 0x99, pos.x, pos.y, z, 0, 0);

    if (*(int *)(c + 0x80) < 0xccc) {
        short *p402 = (short *)(int)M(c + 0x402);
        *p402 = (short)(*p402 + 0x80);
    }

    if (*(int *)(c + 0x80) > 0x334) {
        int *p80 = (int *)(int)M(c + 0x80);
        int *p88 = (int *)(int)M(c + 0x88);
        *p80 = *p80 - 0x52;
        *p88 = *p88 - 0x52;
    } else {
        int *p84 = (int *)(int)M(c + 0x84);
        *p84 = *p84 - 0x29;
        *(int *)(c + 0xa8) = 0xa000;
        *(int *)(c + 0x9c) = 0;
    }

    if (*(int *)(c + 0x84) < 0x800)
        r4 = 1;

    {
        short *p8e = (short *)(int)M(c + 0x8e);
        short *p402b = (short *)(int)M(c + 0x400);
        *p8e = (short)(*p8e + p402b[1]);
    }

    if (*(unsigned char *)(c + 0x41c) > 2) {
        unsigned char *p41c = (unsigned char *)(int)M(c + 0x41c);
        *p41c = (unsigned char)(*p41c - 2);
    }

    if (r4 == 0) {
        if (*(int *)(c + 0x450) != 0xba)
            *(int *)(c + 0x44c) = 0;
        *(int *)(c + 0x450) = 0xba;
        *(int *)(c + 0x44c) = _ZN5Sound8PlayLongEjjjRK7Vector3s(
            *(int *)(c + 0x44c), 3, *(int *)(c + 0x450), (const Vector3 *)(c + 0x74), 0);
    }
    return r4;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 22 -- func_ov060_02113260, 0x02113260, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02113260
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02113260(char *c)
{
    int y = *(int *)(c + 0x60);

    *(int *)(c + 0x5c) = 0;
    *(int *)(c + 0x60) = y;
    *(int *)(c + 0x64) = 0;
    *(int *)(c + 0x80) = 0;
    *(int *)(c + 0x84) = 0;
    *(int *)(c + 0x88) = 0;
    *(int *)(c + 0x98) = 0;
    *(int *)(c + 0xa8) = 0;
    *(int *)(c + 0x9c) = 0;
    *(unsigned char *)(c + 0x426) = 0;
    *(int *)(((int)c + 0x378)) |= 1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 21 -- func_ov060_021130c0, 0x021130c0, size 0x1a0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021130c0
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_021130c0(char* c)
{
    int ret = 0;
    u16 outer = *(u16*)(c + 0x3fe);

    if (outer <= 1) {
        u8 inner;

        if (outer == 0) {
            u16* op = (u16*)LAUND(c + 0x3fe);
            *op = *op + 1;
            *(u8*)(c + 0x424) = 0;
        }

        inner = *(u8*)(c + 0x424);
        switch (inner) {
        case 0:
            if (_ZN6Player9StartTalkER7fBase_cb(*(void**)(c + 0x3a0), c, 1)) {
                _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x14, 0x15666);
                {
                    u8* p = (u8*)LAUND(c + 0x424);
                    *p = *p + 1;
                }
            }
            break;

        case 1:
            if (_ZN6Player12GetTalkStateEv(*(void**)(c + 0x3a0)) == 0) {
                int msg = (*(u8*)(c + 0x414) == 0) ? 0xcd : 0xcf;
                if (_ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(
                        *(void**)(c + 0x3a0), c, msg, c + 0x5c, 0, 2)) {
                    u8* p = (u8*)LAUND(c + 0x424);
                    *p = *p + 1;
                }
            }
            break;

        case 2:
            if (_ZN6Player12GetTalkStateEv(*(void**)(c + 0x3a0)) == -1) {
                u16* op = (u16*)LAUND(c + 0x3fe);
                u8* ip = (u8*)LAUND(c + 0x424);
                int v;
                *op = *op + 1;
                v = *ip + 1;
                *ip = v;
                func_ov060_02111cc0(c, 4, 0x40000000);
                _ZN5Sound22StopLoadedMusic_Layer1Ej(0x3c);
                _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x7f, 0x7222);
            }
            break;
        }
    } else {
        if (func_ov060_021132a4(c)) {
            func_ov060_02113260(c);
            func_ov060_021135fc(c);
            ret = 1;
        }
    }

    return ret;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- func_ov060_02112ee0, 0x02112ee0, size 0x1e0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112ee0
extern "C" int func_ov060_02112ee0(void *cc)
{
    char *c = (char *)cc;
    int ret = 0;
    unsigned short mode =
        *(unsigned short *)((char *)LAUND(c + 0x300) + 0xfe);
    volatile int v[4];

    if (mode <= 1) {
        if (mode == 0) {
            unsigned short *op = (unsigned short *)LAUND(c + 0x3fe);
            *op = *op + 1;
            *(unsigned char *)(c + 0x424) = 0;
        }

        switch (*(unsigned char *)(c + 0x424)) {
        case 0:
            if (_ZN6Player9StartTalkER7fBase_cb(
                    *(void **)(c + 0x3a0), c, 1)) {
                unsigned char *p =
                    (unsigned char *)LAUND(c + 0x424);
                *p = *p + 1;
            }
            break;

        case 1:
            if (_ZN6Player12GetTalkStateEv(*(void **)(c + 0x3a0)) == 0) {
                unsigned m = (NumStars() != 0x96) ? 0xd1 : 0xd2;
                if (_ZN6Player11ShowMessageER7fBase_cjPK7Vector3hh(
                        *(void **)(c + 0x3a0), c, m, c + 0x5c, 0, 2)) {
                    unsigned char *p =
                        (unsigned char *)LAUND(c + 0x424);
                    *p = *p + 1;
                    _ZN7Message11PrepareTalkEv();
                }
            }
            break;

        case 2:
            if (_ZN6Player12GetTalkStateEv(*(void **)(c + 0x3a0)) == -1) {
                _ZN5Sound22StopLoadedMusic_Layer1Ej(0x3c);
                func_ov060_021135fc(c);

                unsigned short *op =
                    (unsigned short *)LAUND(c + 0x3fe);
                unsigned char *ip =
                    (unsigned char *)LAUND(c + 0x424);
                *op = *op + 1;
                int t = *ip + 1;
                *ip = t;
                func_ov060_02111cc0(c, 4, 0x40000000);
            }
            break;
        }
    } else {
        if (*(unsigned char *)(c + 0x41c) > 4) {
            *(unsigned char *)LAUND(c + 0x41c) -= 4;
            int y, z;
            v[0] = *(int *)(c + 0x5c);
            v[1] = y = *(int *)(c + 0x60);
            v[2] = z = *(int *)(c + 0x64);
            v[1] = y + 0x32000;

            *(void **)(c + 0x448) =
                _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                    *(volatile unsigned *)(c + 0x448), 0x99,
                    v[0], v[1], z, 0, 0);
        } else {
            func_ov060_02113260(c);
            ret = 1;
        }
    }

    return ret;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- func_ov060_02112ddc, 0x02112ddc, size 0x104 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112ddc
extern "C" void func_ov060_02112ddc(char *self)
{
    unsigned char *p;
    switch (*(unsigned char*)(self + 0x423)) {
    case 0: func_ov060_02113564(self); break;
    case 1: func_ov060_021134ac(self); break;
    case 2:
        if (func_ov060_02113404(self) == 0) break;
        *(short*)(self + 0x3fe) = 0;
        if (*(unsigned char*)(self + 0x414) == 2) { *(unsigned char*)(self + 0x423) = 0xa; break; }
        p = (unsigned char*)((long long)((int)self + 0x423));
        *p += 1;
        break;
    case 3:
        if (func_ov060_021130c0(self) == 0) break;
        p = (unsigned char*)((long long)((int)self + 0x423));
        *p += 1;
        break;
    case 10:
        if (func_ov060_02112ee0(self) != 0) { p = (unsigned char*)((long long)((int)self + 0x423)); *p += 1; }
        break;
    case 11: break;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- func_ov060_02112d48, 0x02112d48, size 0x94 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112d48
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02112d48(char *c, int arg)
{
    char *o = (char *)_ZN8dActor_c10FindWithIDEj(*(unsigned int *)(c + 0x3ac));
    if (o == 0)
        return;
    short angle = *(short *)(c + 0x408);
    unsigned int idx = (unsigned short)(short)(angle + 0x8000) >> 4;
    int k = idx * 2;
    *(short *)(o + 0x31e) = (short)((arg * data_02082214[k + 1]) >> 12);
    *(short *)(o + 0x322) = (short)((arg * -data_02082214[k]) >> 12);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- func_ov060_02112bfc, 0x02112bfc, size 0x14c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112bfc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02112d48(char* c, int arg);
void func_ov060_02117a3c(char* self);
extern short data_ov060_02119294[];
extern short data_ov060_02119296[];
extern short data_ov060_02119298[];
/* p is volatile so the walk reloads the sentinel for each test -- lifetimes
 * on would keep p[2] in a register and skip the ROM's second load, and the
 * flag and the computed value are split so the flag keeps r6. */
void func_ov060_02112bfc(char* c){
    char* found; int i; int flag; volatile short* p;
    *(int*)(c + 0xa8) = *(int*)(c + 0xa0);
    found = (char*)_ZN8dActor_c10FindWithIDEj(*(unsigned int*)(c + 0x3ac));
    if (found == 0) { *(int*)(c + 0x40c) = 0; return; }
    p = data_ov060_02119294;
    i = 0;
    flag = 1;
    while (p[2] != 0) {
        int r1 = *(unsigned short*)(c + 0x3fc);
        if (r1 < p[2]) {
            int off = i * 6;
            short a = *(short*)((char*)data_ov060_02119294 + off);
            short b = *(short*)((char*)data_ov060_02119296 + off);
            int v;
            if (a > 0) {
                v = (short)(b * (*(short*)((char*)data_ov060_02119298 + off) - 1 - r1));
            } else {
                i -= 1; off = i * 6;
                v = (short)(b * (r1 - *(short*)((char*)data_ov060_02119298 + off)));
            }
            func_ov060_02112d48(c, v);
            if (v != 0 && (*(unsigned short*)(c + 0x3fc) & 1)) { func_ov060_02117a3c(found); }
            flag = 0; break;
        }
        p += 3;
        i += 1;
    }
    if (flag != 0) {
        short* q = (short*)(((int)found + 0x8c));
        *(int*)(c + 0x40c) = 0;
        *(short*)(found + 0x31e) = 0;
        *(short*)(found + 0x320) = 0;
        *(short*)(found + 0x322) = 0;
        q[0] = 0; q[1] = 0; q[2] = 0;
        *(int*)(c + 0xa8) = 0;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- func_ov060_02112ba8, 0x02112ba8, size 0x54 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112ba8
extern "C" {
extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void*);
int func_ov060_02112ba8(char* c){
  int s = *(int*)(c+0x40c);
  if(s != 2 && s != 0x13){
    if(*(int*)(c+0x60) < *(int*)(c+0x3b4) - 0x3e8000) return 1;
    _ZNK10dBgCh_Actr10IsOnGroundEv((char*)c+0x14c);
  }
  return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- func_ov060_021128c0, 0x021128c0, size 0x2e8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021128c0
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" void func_ov060_021128c0(char* c)
{
    *(u8*)(c + 0x425) = 0;
    u32 id;
    if (*(s32*)(c + 0x40c) != 4 && (id = *(u32*)(c + 0x384)) != 0) {
        void* f = (void *)_ZN8dActor_c10FindWithIDEj(id);
        if (f != 0) {
            int b = (*(u16*)((char*)f + 0xc) == 0xbf);
            if (b) {
                Vector3 v;
                v.x = *(s32*)(c + 0x5c);
                v.y = *(s32*)(c + 0x60);
                v.z = *(s32*)(c + 0x64);
                _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(f, v, 2, 0x8000, 1, 0, 1);
            }
        }
    }

    s32 idx = *(s32*)(c + 0x40c);
    {
        TabEnt* e = &data_ov060_0211aed4[idx];
        int off = e->target;
        void* base = (void*)(c + (off >> 1));
        void (*fn)(void*);
        if (off & 1)
            fn = (void (*)(void*))*(void**)((char*)(*(void***)base) + e->slot);
        else
            fn = (void (*)(void*))e->slot;
        fn(base);
    }

    *(u16*)(((int)c + 0x3fc)) =
        *(u16*)(((int)c + 0x3fc)) + 1;
    if (*(s32*)(c + 0x40c) != idx) {
        *(u8*)(c + 0x423) = 0;
        *(u16*)((char*)(((int)c + 0x300)) + 0xfc) = 0;
    }

    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x360);

    if (*(u8*)(c + 0x414) == 1)
        func_02038408(c + 0x14c);
    else
        dBgCh_Actr_UpdateContinuous_Veneer(c + 0x14c);

    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c)) {
        void* fr = _ZNK10dBgCh_Actr14GetFloorResultEv(c + 0x14c);
        _ZNK11SurfaceInfo12CopyNormalToER7Vector3((char*)fr + 4, (Vector3*)(c + 0x3bc));
        *(s32*)(c + 0x3c8) = *(s32*)(c + 0x5c);
        *(s32*)(c + 0x3cc) = *(s32*)(c + 0x60);
        *(s32*)(c + 0x3d0) = *(s32*)(c + 0x64);
        if (*(u8*)(c + 0x427) != 0 && _ZNK10dBgCh_Actr13JustHitGroundEv(c + 0x14c)) {
            *(s32*)(c + 0xa8) = (*(s32*)(c + 0xa8) * -60) / 100;
            if (*(s32*)(c + 0xa8) >= 0x14000)
                *(s32*)(c + 0xa8) = 0x14000;
        } else if (*(s32*)(c + 0x3c0) != 0) {
            *(s32*)(c + 0xa8) = -(_ZN4cstd4fdivEii(
                (int)(((s64)*(s32*)(c + 0x3bc) * *(s32*)(c + 0xa4) + 0x800) >> 12)
              + (int)(((s64)*(s32*)(c + 0x3c4) * *(s32*)(c + 0xac) + 0x800) >> 12),
                *(s32*)(c + 0x3c0)) + 0x8000);
        }
    }

    if (data_ov060_02119268[idx] != 0) {
        if (!_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x14c)) {
            char* p400 = (char*)(((int)c + 0x400));
            s32* px = (s32*)(((int)c + 0x5c));
            s32* pz = (s32*)(((int)c + 0x64));
            s16* tab = data_02082214;
            *(s32*)(c + 0x5c) = *(s32*)(c + 0x3c8);
            *(s32*)(c + 0x60) = *(s32*)(c + 0x3cc);
            *(s32*)(c + 0x64) = *(s32*)(c + 0x3d0);
            {
                s32 a = (*(u16*)(p400 + 8) >> 4);
                *px = *px + ((s32)tab[a * 2] << 3);
            }
            {
                s32 a = (*(u16*)(p400 + 8) >> 4);
                *pz = *pz + ((s32)tab[a * 2 + 1] << 3);
            }
        } else {
            *(s32*)(c + 0x3c8) = *(s32*)(c + 0x5c);
            *(s32*)(c + 0x3cc) = *(s32*)(c + 0x60);
            *(s32*)(c + 0x3d0) = *(s32*)(c + 0x64);
        }
    }

    if (func_ov060_02112ba8(c) == 0)
        return;
    *(s32*)(c + 0x40c) = 2;
    *(u8*)(c + 0x423) = 0;
    *(u16*)((char*)(((int)c + 0x300)) + 0xfc) = 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov060_02112724, 0x02112724, size 0x19c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112724
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02112724(char *c)
{
    int v;
    short a;
    int i;
    int j;
    Vector3 *s;

    *(unsigned int *)(c + 0x418) &= ~0x20000;

    switch (*(unsigned char *)(c + 0x425)) {
    case 0:
        *(unsigned int *)(c + 0x378) |= 1;
        func_02012694(0xb2, (const Vector3 *)(c + 0x74));
        *(int *)(c + 0x40c) = 1;
        func_ov060_02111cc0(c, 0xa, 0);
        *(int *)(c + 0x3f8) = 0x1000;
        (*(unsigned char *)(c + 0x425))++;
        break;
    case 1:
        if (Bowser_IsAnimAtLastFrame(c)) {
            func_ov060_02111cc0(c, 9, 0);
            (*(unsigned char *)(c + 0x425))++;
        }
        break;
    case 2:
        break;
    }

    *(int *)(c + 0x3f0) = *(short *)(*(char **)(c + 0x3a4) + 0x69c);
    v = *(int *)(c + 0x3f0);
    a = *(short *)(*(char **)(c + 0x3a4) + 0x8e);
    if (v < 0) {
        v = -v;
    }
    *(short *)(c + 0x8c) = -v;
    *(short *)(c + 0x8e) = a;



    s = (Vector3 *)(*(char **)(c + 0x3a4) + 0x5c);
    *(int *)(c + 0x5c) = s->x;
    *(int *)(c + 0x60) = s->y;
    *(int *)(c + 0x64) = s->z;

    i = ((unsigned short)a >> 4) * 2;

    *(int *)(c + 0x5c) += data_02082214[i] * 0xa0;
    j = (*(unsigned short *)(c + 0x8c) >> 4) * 2;
    *(int *)(c + 0x60) += 0x18000 - data_02082214[j] * 0xa0;
    *(int *)(c + 0x64) += data_02082214[i + 1] * 0xa0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov060_021125f0, 0x021125f0, size 0x134 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021125f0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021125f0(char *c)
{
    int v, nv;
    char *a;

    func_ov060_02111cc0(c, 0xe, 0x40000000);
    if (*(int *)(c + 0x410) == 3) {
        if (Bowser_IsAnimAtLastFrame(c) == 0)
            return;
    }
    *(unsigned char *)(c + 0x425) = 0;
    *(int *)(c + 0x410) = 0;
    {
        int *q = (int *)(c + 0x378);
        *q &= ~1;
    }
    *(int *)(c + 0x40c) = 1;
    v = *(int *)(c + 0x3f0);
    if (v < 0)
        v = -v;
    v = v * 0x46 / 6000;
    if (v > 0x2d)
        v = v * 0x19 / 10;
    nv = -v;
    *(int *)(c + 0x98) = v * data_02082214[(*(unsigned short *)(c + 0x8c) >> 4 << 1) + 1];
    *(int *)(c + 0xa8) = nv * data_02082214[*(unsigned short *)(c + 0x8c) >> 4 << 1];
    a = (char *)_ZN8dActor_c10FindWithIDEj(*(unsigned int *)(c + 0x3a8));
    if (a != 0) {
        *(int *)(a + 0x110) = 1;
        {
            char *p = (char *)(a + 0x100);
            *(short *)(p + 0x14) = 0;
        }
    }
    *(short *)(c + 0x8c) = 0;
    {
        char *p = (char *)(c + 0x300);
        *(short *)(p + 0xfc) = 0;
    }
    {
        char *mesh = c + 0x14c;
        *(unsigned char *)(c + 0x423) = 0;
        _ZN10dBgCh_Actr15ClearGroundFlagEv(mesh);
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov060_02112434, 0x02112434, size 0x1bc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112434
/* recovered: shared common types */
extern "C" void func_ov060_02112434(void *tv)
{
    unsigned char *thiz = (unsigned char *)tv;
    Vector3 zero;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    *(int*)(thiz + 0x3f4) = Vec3_HorzDist((Vector3*)(thiz + 0x5c), &zero);
    *(short*)(thiz + 0x408) = Vec3_HorzAngle((Vector3*)(thiz + 0x5c), &zero);

    int s0 = _ZN8dActor_c14GetSubtractionEss(thiz, *(short*)(thiz + 0x8e), *(short*)(thiz + 0x406));
    int s1 = _ZN8dActor_c14GetSubtractionEss(thiz, *(short*)(thiz + 0x8e), *(short*)(thiz + 0x408));

    *(int*)(int)(thiz + 0x418) &= ~0xff;
    if (s0 < 0x2000)
        *(int*)(int)(thiz + 0x418) |= 2;
    if (s1 < 0x3800)
        *(int*)(int)((long long)((int)thiz + 0x418)) |= 4;
    if (*(int*)(thiz + 0x3f4) < 0x3e8000)
        *(int*)((int)thiz + 0x418) |= 0x10;
    if (*(int*)(thiz + 0x3ec) < 0x352000)
        *(int*)(int)((unsigned long long)((unsigned)thiz + 0x418)) |= 8;

    (((daKpa_c *)thiz)->*data_ov060_0211aeb4[*(int*)(thiz + 0x410)].pmf)();

    if (*(int*)(thiz + 0x40c) == 4) return;

    unsigned char lo = thiz[0x41c];
    unsigned char hi = thiz[0x41d];
    if (hi == lo) return;
    if (hi > lo) {
        int v = lo + 0x14;
        if (v >= 0xff) {
            thiz[0x41c] = 0xff;
            return;
        }
        *(unsigned char*)(int)(thiz + 0x41c) =
            *(unsigned char*)(int)(thiz + 0x41c) + 0x14;
        return;
    }
    {
        int v = lo - 0x14;
        if (v <= 0) {
            thiz[0x41c] = 0;
        } else {
            *(unsigned char*)(int)((long long)((int)thiz + 0x41c)) =
                *(unsigned char*)(int)((long long)((int)thiz + 0x41c)) - 0x14;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov060_021123dc, 0x021123dc, size 0x58 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021123dc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021123dc(void* c){
  char* p = (char*)c;
  *(unsigned char*)(p+0x415)=1;
  *(unsigned char*)(p+0x41c)=0xff;
  *(unsigned char*)(p+0x41d)=0xff;
  if(*(unsigned char*)(p+0x414)==3) *(unsigned char*)(p+0x414)=0;
  *(unsigned char*)(p+0x41e)=data_ov060_02119264[*(unsigned char*)(p+0x414)];
  *(int*)(p+0x40c)=5;
  *(int*)(p+0x410)=0;
  *(unsigned short*)(p+0x420)=0;
  *(unsigned char*)(p+0x422)=0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov060_021123c8, 0x021123c8, size 0x14 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021123c8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021123c8(char *p)
{
    *(char *)(p + 0x427) = 1;
    *(int *)(p + 0x3a4) = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov060_021123a0, 0x021123a0, size 0x28 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021123a0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021123a0(void *v, int f) {
    char *c = (char *)v;
    if (f)
        (*(int *)(((int)c + 0x378))) &= ~1;
    else
        (*(int *)(((int)c + 0x378))) |= 1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov060_02112350, 0x02112350, size 0x50 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02112350
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02112350(void* c){
  int r=_ZN9Animation8FinishedEv((char*)c+0x124);
  if(!r) return;
  if(*(int*)((char*)c+0x134) != data_ov060_0211acd0[1]) return;
  func_ov060_02111cc0(c,3,0);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov060_02111f08, 0x02111f08, size 0x448 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02111f08
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02111f08(void* arg0)
{
    char* self = (char*)arg0;
    void* cam = data_0209f318;
    char* player = (char*)_ZN8dActor_c13ClosestPlayerEv(arg0);
    struct Vector3 sp;
    struct Vector3* pv;
    unsigned char* p;
    int dist;
    int v;
    int k;
    int d;

    if (player == 0)
        return 1;

    switch (*(unsigned char*)(self + 0x444)) {
    case 0:
        _ZN6Camera9SetFlag_3Ev(cam);
        pv = (struct Vector3*)(player + 0x5c);
        sp.x = pv->x;
        sp.y = pv->y;
        sp.z = pv->z;
        sp.y = *(int*)(self + 0x60);
        _ZN6Camera9SetLookAtERK7Vector3(cam, &sp);
        *(int*)(self + 0x42c) = sp.x;
        *(int*)(self + 0x430) = sp.y;
        *(int*)(self + 0x434) = sp.z;
        dist = Vec3_HorzDist((struct Vector3*)(self + 0x5c), &sp);
        k = (unsigned short)(short)(Vec3_HorzAngle((struct Vector3*)(self + 0x5c), &sp) - 0x2000) >> 4;
        v = (int)(((long long)dist * 0xA00 + 0x800) >> 12);
        *(int*)(self + 0x438) = *(int*)(self + 0x5c) + (int)(((long long)v * data_02082214[k * 2] + 0x800) >> 12);
        *(int*)(self + 0x43c) = *(int*)(self + 0x60) + 0xc8000;
        *(int*)(self + 0x440) = *(int*)(self + 0x64) + (int)(((long long)v * data_02082214[k * 2 + 1] + 0x800) >> 12);
        _ZN6Camera6SetPosERK7Vector3(cam, (struct Vector3*)(self + 0x438));
        p = (unsigned char*)(self + 0x444);
        *p = *p + 1;
        break;
    case 1:
        func_020092c4(cam, (char*)cam + 0x80, self + 0x42c);
        if (_ZN6Player7IsInAirEv(player) != 0)
            *(unsigned char*)(self + 0x445) = 0;
        else {
            p = (unsigned char*)(self + 0x445);
            *p = *p + 1;
        }
        if (*(unsigned char*)(self + 0x445) > 0x1e) {
            p = (unsigned char*)(self + 0x444);
            *p = *p + 1;
        }
        break;
    case 2:
        pv = (struct Vector3*)(player + 0x5c);
        sp.x = pv->x;
        sp.y = pv->y;
        sp.z = pv->z;
        dist = Vec3_HorzDist((struct Vector3*)(self + 0x5c), &sp);
        d = (int)(((long long)dist * 0x600 + 0x800) >> 12);
        if (d < 0x1f4000)
            d = 0x1f4000;
        k = (unsigned short)(short)(Vec3_HorzAngle((struct Vector3*)(self + 0x5c), &sp) - 0x1000) >> 4;
        sp.x = *(int*)(self + 0x5c) + (int)(((long long)d * data_02082214[k * 2] + 0x800) >> 12);
        sp.z = *(int*)(self + 0x64) + (int)(((long long)d * data_02082214[k * 2 + 1] + 0x800) >> 12);
        _Z14ApproachLinearRiii((int*)(self + 0x438), sp.x, 0x4000);
        _Z14ApproachLinearRiii((int*)(self + 0x440), sp.z, 0x4000);
        _Z14ApproachLinearRiii((int*)(self + 0x42c), *(int*)(self + 0x5c), 0x1e000);
        _Z14ApproachLinearRiii((int*)(self + 0x434), *(int*)(self + 0x64), 0x1e000);
        func_020092c4(cam, (char*)cam + 0x8c, self + 0x438);
        if (func_020092c4(cam, (char*)cam + 0x80, self + 0x42c) != 0) {
            p = (unsigned char*)(self + 0x444);
            *p = *p + 1;
        }
        break;
    case 3: {
        int ty, tz, tx;
        k = (int)(*(unsigned short*)(self + 0x8e)) >> 4;
        tz = data_02082214[k * 2 + 1] * 0xc0 + *(int*)(self + 0x64);
        ty = *(int*)(self + 0x60) + 0xfa000;
        tx = data_02082214[k * 2] * 0xc0 + *(int*)(self + 0x5c);
        sp.x = tx;
        sp.y = ty;
        sp.z = tz;
    }
        _Z14ApproachLinearRiii((int*)(self + 0x42c), sp.x, 0xa000);
        _Z14ApproachLinearRiii((int*)(self + 0x430), sp.y, 0xa000);
        _Z14ApproachLinearRiii((int*)(self + 0x434), sp.z, 0xa000);
        if (func_020092c4(cam, (char*)cam + 0x80, self + 0x42c) != 0)
            return 1;
        break;
    case 4:
        *(int*)((char*)cam + 0x154) &= ~8;
        break;
    default:
        break;
    }
    return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov060_02111cc0, 0x02111cc0, size 0x248 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02111cc0
#include "TextureSequence.h"
struct BMD_File;
struct BTP_File;
extern "C" {
/* The six BTP_File handles below were declared by this shard through a local
 * `struct E { int w[2]; }` overlay and read as `x.w[1]`, while the neighbouring
 * resource-loader shard declares the same addresses as SharedFilePtr tables and
 * reads them as `x[1]`. One object cannot be both. The loader view is the real
 * one -- these are the SharedFilePtr handles Model::LoadFile populates and the
 * cleanup path releases -- so they are declared once, there, and this shard's
 * `.w[1]` reads are respelled `[1]`. */
extern SharedFilePtr *data_ov060_021192dc[];
extern SharedFilePtr *data_ov060_0211ac78[];
extern SharedFilePtr *data_ov060_0211ac40[];
extern SharedFilePtr *data_ov060_0211acb8[];
extern SharedFilePtr *data_ov060_0211ac10[];
extern SharedFilePtr *data_ov060_0211abf0[];
extern SharedFilePtr *data_ov060_0211ac30[];
extern SharedFilePtr *data_ov060_0211ac28[];
extern void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *anim, void *file, int a, int d, unsigned e);
extern void _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(void *ts, void *file, int a, int d, unsigned e);
extern void _ZN9Animation8SetFlagsEi(void *anim, int flags);

void func_ov060_02111cc0(void *v, int idx, int fix)
{
    char *c = (char *)v;
    int a;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0xd4, (void *)((int *)data_ov060_021192dc[idx])[1], a, 0x1000, 0);
    switch (idx) {
    case 1:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211ac40[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211ac40[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0x40000000);
        return;
    case 2:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211acb8[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211acb8[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0x40000000);
        return;
    case 3:
    case 5:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211ac10[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211ac10[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0);
        return;
    case 14:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211abf0[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211abf0[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0);
        return;
    case 10:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211ac30[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211ac30[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0x40000000);
        return;
    case 0:
    default:
        TextureSequence::Prepare(*(BMD_File *)data_ov060_0211ac78[1], *(BTP_File *)data_ov060_0211ac28[1]);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x138, (void *)data_ov060_0211ac28[1], 0, 0x1000, 0);
        _ZN9Animation8SetFlagsEi(c + 0x138, 0);
        return;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov060_02111c68, 0x02111c68, size 0x58 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02111c68
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_02111c68(char *c){
  int v = *(int*)(c+0x134);
  if(v == (int)data_ov060_0211ac20[1]){
    unsigned int t = (*(unsigned int*)(c+0x12c) << 4) >> 0x10;
    if(t >= 0x31) return t - 0x31;
  }
  if(v == (int)data_ov060_0211ac68[1]){
    return ((*(unsigned int*)(c+0x12c) << 4) >> 0x10) + 0xb;
  }
  return -1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov060_02111a28, 0x02111a28, size 0x240 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02111a28
// @symbol func_ov060_02111a28
/* recovered: shared common types */
#include "common.h"
extern "C" {

/* The six resource handles at 0x1acb0..0x1ac90 are read only through their
 * second word (`x.w[1]`), compared against a value loaded from the actor. Spelled
 * as a two-int array rather than a named struct: mwccarm rejects a class declared
 * between function bodies in this position, reading the type as undeclared. */
extern int data_ov060_0211acb0[];
extern int data_ov060_0211ac00[];
extern int data_ov060_0211ace0[];
extern int data_ov060_0211aca0[];
extern int data_ov060_0211abf8[];
extern int data_ov060_0211ac90[];
extern void _ZN8dActor_c17HugeLandingDustAtER7Vector3b(void *a, void *v, int b);
extern void _ZN5Sound4PlayEjjRK7Vector3(unsigned a, unsigned b, void *v);

void func_ov060_02111a28(char *c)
{
    /* (unsigned short)(u32 >> 12) forces ROM prologue load order
       (frame@0x12c into r0, then flag@0x446 into r1) + lsl#4/lsr#16 extract. */
    int n = (unsigned short)((unsigned)*(int *)(c + 0x12c) >> 12);
    int r3 = (*(unsigned char *)(c + 0x446) != 0) ? 1 : 0;
    int v = *(int *)(c + 0x134);
    int r1 = 0;
    *(unsigned char *)(c + 0x446) = 0;

    if (v == data_ov060_0211acb0[1]) {
        if (n >= 0x14 && n <= 0x17)
            r1 = 2;
        else if (n == 0x29 || n < 2)
            r1 = 1;
    } else if (v == data_ov060_0211ac00[1]) {
        if (n >= 0xf && n <= 0x12)
            r1 = 2;
    } else if (v == data_ov060_0211ace0[1]) {
        if (n >= 0x1c)
            r1 = 1;
    } else if (v == data_ov060_0211aca0[1]) {
        if (n >= 6 && n <= 9)
            r1 = 2;
        else if (n >= 0xf)
            r1 = 1;
    } else if (v == data_ov060_0211abf8[1]) {
        if (n >= 0x10 && n <= 0x13)
            r1 = 1;
        else if (n >= 0x1e && n <= 0x21)
            r1 = 2;
        else if (n >= 0x2d && n <= 0x30)
            r1 = 1;
    } else if (v == data_ov060_0211ac90[1]) {
        if (n >= 0xa && n <= 0xd)
            r1 = 1;
        else if (n >= 0x18 && n <= 0x1b)
            r1 = 2;
    }

    if (r1 == 0)
        return;
    *(unsigned char *)(c + 0x446) = 1;
    if (r3 != 0)
        return;

    if (r1 == 1) {
        Vector3 dust;
        dust.x = *(int *)(c + 0x3d4);
        dust.y = *(int *)(c + 0x3d8);
        dust.z = *(int *)(c + 0x3dc);
        _ZN8dActor_c17HugeLandingDustAtER7Vector3b(c, &dust, 0);
    } else {
        Vector3 dust;
        dust.x = *(int *)(c + 0x3e0);
        dust.y = *(int *)(c + 0x3e4);
        dust.z = *(int *)(c + 0x3e8);
        _ZN8dActor_c17HugeLandingDustAtER7Vector3b(c, &dust, 0);
    }
    _ZN5Sound4PlayEjjRK7Vector3(3, 0xb0, c + 0x74);
    {
        Vector3 quake;
        quake.x = *(int *)(c + 0x5c);
        quake.y = *(int *)(c + 0x60);
        quake.z = *(int *)(c + 0x64);
        _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(c, &quake, 0x320000);
    }
}
}
