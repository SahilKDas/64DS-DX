//cpp
/* da1up_c -- the mushroom, in all fourteen of the ways it can behave.
 *
 * One class covers the 1-Up and the Mega Mushroom and the ways they are placed:
 * waiting in the open, hidden until a condition is met, moving off, spinning in
 * place, falling to the ground. mMushroomType picks one of the 14
 * behaviours out of a dispatch array and Behavior calls it every frame; the
 * class also answers to Yoshi (OnYoshiTryEat) and to being turned into an egg
 * (OnTurnIntoEgg).
 *
 * The TU is the contiguous linker run 0x020aee40..0x020b0530, ROM ordinals
 * 0..35, 36 functions; config/tu_manifest.d/ov002/da1up_c.json names each one.
 *
 * IDENTITY IS THE CARTRIDGE'S, NOT THE TREE'S. ov002 file offset 0x5ad10 ==
 * address 0x02108370 holds `7da1up_c\0`, the length-prefixed Itanium type-name
 * string, and _ZTI7da1up_c at 0x0210837c is the matching __si_class_type_info
 * whose +8 word reaches _ZTI12dEnemyBase_c at 0x021081c0. The tree's former
 * spelling `OneUpMushroom` is in no image in any encoding tested; the fact
 * file kept at notes/data/class-facts/OneUpMushroom.json records the result.
 *
 * SOURCE ORDER IS ROM-ASCENDING AND `#pragma defer_codegen off` IS
 * LOAD-BEARING; they are ONE decision, exactly as on ov006/dScMgPanel_c. With
 * codegen deferred (the default) mwccarm 2004/b56 emits one .text section per
 * function in the REVERSE of source order, which would demand a forward
 * declaration for all 36 members and force one canonical spelling on every
 * shared helper. Generating at parse time emits in source order instead, so
 * every callee but two is already defined above its caller and each member's
 * independently recovered view of a helper survives untouched.
 *
 * THE DESTRUCTOR STAYS OUT OF LINE AND IS DECLARED FIRST, SO THIS TU OWNS THE
 * CLASS'S KEY FUNCTION. The cartridge orders D1 (0x020aee40, 0x48) BELOW D0
 * (0x020aee88, 0x5c) and carries no D2; out-of-line + `defer_codegen off` emits
 * D1, D0, D2, which is that order with a homeless D2 trailing where it is
 * deadstripped. Ordinals 0 and 1 are two shards of ONE C++ definition, so only
 * one destructor body is written below. Owning the key function is also why
 * this TU emits the whole chain's vtable and typeinfo as vague-linkage
 * passengers -- see the manifest's compiler_only_output block.
 *
 * EACH `extern "C"` MEMBER KEEPS ITS OWN DECLARATIONS, INSIDE ITS OWN BODY.
 * mwccarm 2004/b56 gives a block-scope declaration in an `extern "C"` region C
 * linkage (measured on ov002/Player, 301 members), so contradictory recovered
 * views of one ROM symbol coexist without a call site being rewritten.
 * Measured on this file before the readability pass: `_ZN5Sound9PlayBank3EjRK7Vector3`
 * was declared at SEVEN sites and no two spellings were identical, and
 * `func_ov002_020aefb8` at seven sites in five distinct spellings -- `char *`,
 * `unsigned char *` and `void *` first parameters, plus a no-argument `(void)`
 * in ordinal 24, which really does pass nothing. The pass kept all seven
 * sites of each and re-spelled the class-pointer ones: PlayBank3 now shows
 * three parameter views (`Vector3 *`, `void *`, `const void *` after the id)
 * and aefb8 three (`void *`, `da1up_c *`, `(void)`; its definition is
 * `char *`).
 * THE EIGHT C++ DEFINITIONS ARE THE EXCEPTION, and they have to be: a class
 * member function cannot sit inside an `extern "C"` region, so the same
 * block-scope declaration there gets C++ LINKAGE and the reference mangles.
 * Their external FUNCTION declarations therefore live in two declaration-only
 * file-scope `extern "C"` regions, each placed immediately above the members
 * that need it. The file's full region census is 30 file-scope `extern "C"`
 * regions: 27 one-member wrappers, those 2, and one at the top of the file for
 * the two upward intra-TU calls. Their external DATA declarations do not: mwccarm leaves
 * a file-scope variable's name unmangled in C++, so those stay in the bodies.
 *
 * `decl_common.h` IS DELIBERATELY NOT INCLUDED. It declares 11 of these 36
 * symbols and 3 of the 11 have different parameter views from their definitions:
 * `func_ov002_020af4ec` (`char *` vs the definition's `void *`),
 * `func_ov002_020afc68` (`char *` vs the definition's `da1up_c *`), and
 * `func_ov002_020af684` (`void *, int, int` vs the definition's
 * `da1up_c *, int, Player *`).
 * The other eight definitions that take the class (aeee4, aefa4, aefb8,
 * af1dc, af3a8, af474, afa6c, afde4) keep decl_common.h's `char *` (and af1dc's
 * `int` return) as their parameter spelling and cast it to `da1up_c *` on
 * their first line; retyping them to `da1up_c *` would be a new declaration
 * disagreement, and decl_common.h is shared.
 * The shared void egg-turn contract reconciles the former return disagreements
 * for `func_ov002_020af684` and `func_ov002_020afa6c`. Pulling it in makes each an
 * `illegal function overloading`
 * error against a byte-matched body. ov002/Player and ov006/dScMgPanel_c, the
 * two largest promoted TUs, exclude it for the same reason.
 *
 * NINE OF THE 36 SYMBOLS ARE METHODS, written as EIGHT `da1up_c::`
 * definitions (ordinals 0 and 1 are two shards of the one destructor), and all
 * nine are this class's own vtable slots: ordinals 0/1 the destructor pair
 * (slots 16/17), 9 OnTurnIntoEgg (19), 10 OnYoshiTryEat (18), 31
 * CleanupResources (3), 32 OnPendingDestroy (12), 33 Render (9), 34 Behavior
 * (6) and 35 InitResources (0). These nine symbols already had native member
 * definitions in the shard sources; promotion consolidates and renames them.
 * The other 27 stay free functions. Fourteen of those are ROM-proven
 * non-static members of this class -- the 14 {function pointer, 0} descriptors
 * at 0x02108300..0x02108370 are pointer-to-member-function objects with a zero
 * `this` adjustment, and __sinit_ov002_02100adc copies them into the 14-element
 * dispatch array at 0x0210dc00 that ordinal 34 indexes by mMushroomType. Their
 * member-ness and their INDEX are proven; their original NAMES are not.
 * They retain address-derived spellings in this packaging step. Explicitly
 * coined names and typed methods remain reconstruction work; the other 13
 * helpers need separate membership evidence. The 14 are, by index:
 * 0 020aff10, 1 020afe4c, 2 020afd10,
 * 3 020afc44, 4 020afbb4, 5 020afa98, 6 020afa6c, 7 020af950, 8 020af924,
 * 9 020af838, 10 020af7cc, 11 020afa50, 12 020af908, 13 020af724.
 * Leftover fold adds the two classInit factories at 0x020b0530/0x020b0580,
 * so the licensed run is 38 functions through 0x020b05d0.
 *
 * Known walls -- these do NOT byte-match if you convert them:
 * - dBgCh_Actr::Init / dCcAc_c::Init / DropShadowRadHeight / ReflectAngle 6az
 *   (Fix12i mangles as i; ROM is Fix12<int> -- method form Undefined)
 * - Particle::System::New / NewSimple: no method declaration in include/
 * - Behavior 0x100: named ++mStateTimer size-DIFF vs unsigned-short launder
 *   (the u16 casts of dEnemyBase_c::mStateTimer at 0x100 stay everywhere they
 *   appear. The u16 at 0x38c is a distinct field, mStateFrames.)
 * - func_ov002_020af0c0: reading the player's position directly instead of
 *   through the laundered int* changes the bytes, so the launder stays.
 * - struct C PMF stand-in (mwccarm PMF representation depends on the class)
 * - SharedFilePtr has no recovered fields; handles stay data_ov002_*
 * - decl_common.h stays out (3 of 11 declarations disagree with MATCH bodies)
 *
 * Readability pass: the da1up_c fields at 0x378..0x394 are named, the mushroom
 * types are the da1up_MushroomType enum, and actor, sound, cylinder-flag and
 * particle ids are enums. Leftover:
 * - The da1up_MushroomType names describe what each type does when it runs;
 *   they are not recovered original names (nor are the address-named
 *   handlers).
 * - unk_0a4 / unk_0ac are dActor_c's velocity x/z words (UpdatePosWithHorzSpeedAndAng
 *   writes them from mHorzSpeed and mPrevAngleY); they stay unnamed here because
 *   naming them is a shared dActor_c.h change.
 * - The SND3_* names and the effect ids are known by use only (0x68 is played
 *   as a mushroom launches, 0x69 once when type 13 starts).
 * - The vulnFlags/hitFlags bit names come from the best-effort table in
 *   include/dCc_c.h.
 * - The PlayBank3 / IsPlayerInRange / ReflectAngle / DropShadowRadHeight /
 *   Fix12 externs keep their per-site spellings (see the walls above).
 */

#pragma defer_codegen off

#include "types.h"
#include "common.h"
#include "da1up_c.h"
#include "dBgCh_Actr.h"
#include "dBgCh_Gnd.h"
#include "Player.h"
#include "Sound.h"
#include "SharedFilePtr.h"

bool ApproachLinear(short &value, short target, short step);

/* Actor IDs, from the profile ids in symbols/actor_debug_names.tsv: 276 and
   277 are this class's two profiles, 331 is the 1UP logo it spawns. */
enum {
    ACTOR_ONEUPKINOKO     = 276,
    ACTOR_SCALEUP_KINOKO  = 277,
    ACTOR_OBJ_1UPLOGO     = 331
};

/* Sound::PlayBank3 ids. SND3_COIN / SND3_COIN_UNDERWATER are the pair
   dActor_c::GivePlayerCoins plays for a coin (the second when the player is
   underwater); SND3_GIVE_LIFE is played alongside GiveLives(1) here and in
   daObjMarioCap_c; SND3_LAUNCH is played as a mushroom pops out; SND3_UNK_69
   is played once when type 13 starts. Only their use is known. */
enum {
    SND3_COIN            = 0x11,
    SND3_COIN_UNDERWATER = 0x12,
    SND3_LAUNCH          = 0x68,
    SND3_UNK_69          = 0x69,
    SND3_GIVE_LIFE       = 0x6e
};

/* Bit values of the cylinder and actor flag words used below. dCc_c::flags bit
   0 disables the cylinder (dCc_c::Update bails on it); the vulnFlags and
   hitFlags bits are read from the best-effort table in include/dCc_c.h; the
   mFlags bit is the clip-test enable from the table in include/dActor_c.h. */
enum {
    CC_FLAGS_DISABLED    = 0x1,
    CC_VULN_YOSHI_TONGUE = 0x8000,
    CC_HIT_PLAYER        = 0x400000,
    ACTOR_FLAG_CLIP_TEST = 0x1,
    ACTOR_FLAG_OFF_SCREEN = 0x8
};

/* Particle effect ids handed to Particle::System::New / NewSimple. */
enum {
    PTCL_SCALEUP_KINOKO_TRAIL = 0x108, /* the effect func_ov002_020aeee4 starts for actorID 277 ("trail" is this file's word for it) */
    PTCL_TYPE_11_12_CLEANUP   = 0xd2   /* started by CleanupResources for types 11 and 12 */
};

/* The only two intra-TU calls that run UPWARD in ROM address order, so the only
   two that ROM-ascending source order cannot satisfy from the definition above:
   ordinal 21 (0x020afa50) calls ordinal 22 (0x020afa6c), and ordinal 18
   (0x020af908) calls ordinal 19 (0x020af924). Both spellings are the
   definitions' own, so nothing below has to be adapted to them. */
extern "C" {
void func_ov002_020afa6c(char *c);
void func_ov002_020af924(da1up_c *c);
}

/*                         _ZN7da1up_cD0Ev, 0x020aee88, size 0x5c              */
// @symbol _ZN7da1up_cD1Ev
// @symbol _ZN7da1up_cD0Ev
/* ONE definition, both variants. The complete-object destructor (D1) tears the
   four members down in exact reverse of the factories' construction order --
   ShadowModel at 0x350, Model at 0x300, dBgCh_Actr at 0x144, dCcAc_c at 0x110,
   then ~dEnemyBase_c -- and every one of those is a typed member of this class,
   so the body is empty and the compiler writes the chain. The deleting
   destructor (D0) inlines that same teardown and then calls
   Memory::Deallocate(this, GAME_HEAP_PTR) through dEnemyBase_c's own inline
   `operator delete`, which is why nothing below mentions a heap. Under
   `#pragma defer_codegen off` the variants come out D1, D0, D2; the trailing D2
   is homeless and is deadstripped. */
da1up_c::~da1up_c()
{
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020aeee4
/* Trail effect: asks Particle::System::New for one effect at the mushroom's
   position, 30 units (0x1e000) above it. The effect id is 0x108 for the Mega
   Mushroom (actorID 277) and 0 for the 1-Up. The previous frame's handle is read
   back through a volatile pointer and the new one stored in mParticleID. While
   the actor is off screen (mFlags bit 3) it only runs when CURRENT_GAMEMODE
   (data_0209f2d8) is 1. */
extern "C" {
void func_ov002_020aeee4(char* raw) {
    da1up_c* self = (da1up_c*)raw;
    extern unsigned int _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        unsigned int uniqueID, unsigned int effectID,
        int x, int y, int z, const void* dir, void* callback);
    extern unsigned char data_0209f2d8;

    int t1 = (self->actorID == ACTOR_SCALEUP_KINOKO);
    unsigned int effectID = 0;
    if (t1 != false) effectID = PTCL_SCALEUP_KINOKO_TRAIL;

    int t2 = ((self->mFlags & ACTOR_FLAG_OFF_SCREEN) != 0);
    if (t2 != false) {
        int t3 = (data_0209f2d8 == 1);
        if (t3 == false) return;
    }

    Vector3 pos;
    pos.x = self->mPosX;
    pos.y = self->mPosY;
    pos.z = self->mPosZ;
    pos.y += 0x1e000;
    volatile Vector3* vp = &pos;
    self->mParticleID = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        *(volatile unsigned int*)&self->mParticleID, effectID, vp->x, vp->y, pos.z, 0, 0);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020aefa4
/* Sets bit 0x8000 in the cylinder's vulnFlags -- the "yoshi tongue" bit of the
   table in include/dCc_c.h (a best-effort reading there) -- so the mushroom can
   be eaten. func_ov002_020af218 calls it while the player is in range.
   MEASURED: this definition must stay `void`. Declaring it `int` -- so that
   ordinal 7's `return func_ov002_020aefa4(c);` would type-check against a
   file-scope declaration -- costs four of this function's five words. Ordinal 7
   keeps its own `int` view at block scope instead, which is exactly the C
   linkage the enclosing `extern "C"` region gives it. */
extern "C" {
void func_ov002_020aefa4(char *raw)
{
    da1up_c *self = (da1up_c*)raw;
    self->mdCcAc_c.vulnFlags |= CC_VULN_YOSHI_TONGUE;
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020aefb8
/* The shared per-frame physics step. Moves by mHorzSpeed along mPrevAngleY
   (UpdatePosWithHorzSpeedAndAng). While on the ground it adds ten times the
   floor normal's x and z to the words at 0xa4 / 0xac (the x and z of a velocity
   whose y is mVertSpeed: func_ov002_020afd10 saves and restores them as that
   triple), sets mVertSpeed to -0.4 times its value on the frame it lands and to
   0 otherwise, and raises mHorzSpeed to the horizontal length of that vector
   when that is larger, capped at 15 units (0xf000). Then it integrates position
   (UpdatePosWithOnlySpeed), runs the wall/floor collision update, and on a wall
   hit reflects mPrevAngleY about the wall normal.
   The shard carried shadow `dActor_c`/`dEnemyBase_c` tags to name three
   non-virtual methods. The merged TU has both real classes complete through
   da1up_c.h, so the shadows are gone and the calls go through the real types --
   which mangle identically, the class name being the whole of the difference.
   _ZN8dActor_c12ReflectAngleE5Fix12IiES1_s stays spelt out. Its reconstructed
   symbol encodes by-value Fix12<int> parameters; the recorded typed-call
   experiment changed the bytes. This bridge remains pending further
   signature/codegen work. */
extern "C" {
void func_ov002_020aefb8(char* raw) {
    da1up_c* self = (da1up_c*)raw;
    extern short _ZN8dActor_c12ReflectAngleE5Fix12IiES1_s(void *, int, int, short);
    extern int Vec3_HorzLen(void*);

    self->UpdatePosWithHorzSpeedAndAng();
    if (self->mWithMeshClsn.IsOnGround()) {
        self->unk_0a4 += self->mFloorNormalX * 0xa;
        self->unk_0ac += self->mFloorNormalZ * 0xa;
        if (self->mWithMeshClsn.JustHitGround()) {
            self->mVertSpeed = -(self->mVertSpeed << 2) / 10;
        } else {
            self->mVertSpeed = 0;
        }
        if (Vec3_HorzLen(&self->unk_0a4) > self->mHorzSpeed) {
            self->mHorzSpeed = Vec3_HorzLen(&self->unk_0a4);
            if (self->mHorzSpeed >= 0xf000) self->mHorzSpeed = 0xf000;
        }
    }
    self->UpdatePosWithOnlySpeed((dCc_c*)&self->mdCcAc_c);
    self->UpdateWMClsn(self->mWithMeshClsn, 0);
    if (!self->mWithMeshClsn.IsOnWall()) return;
    self->mPrevAngleY = _ZN8dActor_c12ReflectAngleE5Fix12IiES1_s(self, self->mWallNormalX, self->mWallNormalZ, self->mPrevAngleY);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af0c0
/* Steering toward the player, called from state 1 of type 7. Takes the closest
   player; if there is one, builds the vector to it -- the aim point is the
   player's y lowered by 80 units (0x50000) when func_ov002_020d0d2c says the
   player is in one of two Player states and raised by 120 units (0x78000)
   otherwise -- and steps mPrevAngleY toward the horizontal bearing and
   mPrevAngleX toward the pitch (cstd::atan2(horizontal length, height
   difference)), at most 0x1000 (22.5 degrees) per frame each. mVertSpeed and
   mHorzSpeed are then 30 units (0x1e) times the two s16 words of
   data_02082214's entry for mPrevAngleX >> 4 -- the sin/cos table, 0x1000 = 1.0
   -- mVertSpeed from word 1 and mHorzSpeed from word 0. Always finishes with
   func_ov002_020af3a8. */
extern "C" {
void func_ov002_020af0c0(da1up_c* c){
    extern short data_02082214[];
    extern int func_ov002_020d0d2c(void*);
    extern int Vec3_HorzLen(const Vector3*);
    extern short _ZN4cstd5atan2E5Fix12IiES1_(int, int);
    extern short Vec3_HorzAngle(const Vector3*, const Vector3*);
    /* Forward: ordinal 11 sits above this one in ROM order. */
    extern void func_ov002_020af3a8(da1up_c* thiz);

    Player* p = c->ClosestPlayer();
    if(p != 0){
        Vector3 diff;
        Vector3 ppos;
        /* The player's position is read through an int* laundered via
           (void*)(int); reading p->mPosX/Y/Z directly here changes the bytes
           (tried: it did not byte-match), so the launder stays. */
        int* s = (int*)((void*)(int)&p->mPosX);
        ppos.x = s[0];
        ppos.y = s[1];
        ppos.z = s[2];
        diff.x = ppos.x - c->mPosX;
        if(func_ov002_020d0d2c(p) != 0)
            diff.y = ppos.y - c->mPosY - 0x50000;
        else
            diff.y = ppos.y - c->mPosY + 0x78000;
        diff.z = ppos.z - c->mPosZ;
        int len = Vec3_HorzLen(&diff);
        short pitch = _ZN4cstd5atan2E5Fix12IiES1_(len, diff.y);
        short yaw = Vec3_HorzAngle((Vector3*)&c->mPosX, &ppos);
        ApproachLinear(c->mPrevAngleY, yaw, 0x1000);
        ApproachLinear(c->mPrevAngleX, pitch, 0x1000);
        c->mVertSpeed = (short)data_02082214[(*(unsigned short*)&c->mPrevAngleX >> 4)*2+1] * (short)0x1e;
        c->mHorzSpeed = (short)data_02082214[(*(unsigned short*)&c->mPrevAngleX >> 4)*2] * (short)0x1e;
    }
    func_ov002_020af3a8(c);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af1dc
/* The player touching the mushroom, or 0. mdCcAc_c.otherOwner is the unique ID
   of the other cylinder's owner (cleared by dCc_c::Clear, which Behavior calls
   every frame); it is looked up with dActor_c::FindWithID and returned only
   when hitFlags bit 0x400000 is set -- the "player" bit of the table in
   include/dCc_c.h, a best-effort reading there that the callers' use of the
   result as a Player agrees with. */
extern "C" {
int func_ov002_020af1dc(char* raw){
  da1up_c* c = (da1up_c*)raw;
  Player* r=0;
  unsigned int id=c->mdCcAc_c.otherOwner;
  if(id && (r=(Player*)dActor_c::FindWithID(id)) && (c->mdCcAc_c.hitFlags&CC_HIT_PLAYER))
    return (int)r;
  return 0;
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af218
/* Stores in mShown whether the player is within `range` whole units
   (IsPlayerInRange shifts it into 20.12), and when it is calls
   func_ov002_020aefa4 to open the mushroom to Yoshi. Returns 0 when out of range,
   otherwise that call's result. Every caller in this TU passes 0xbb8 (3000
   units).
   The second parameter is FORWARDED, not merely declared. The callers
   below pass 0xbb8 in r1 and this body hands that same word to
   _ZN8dActor_c15IsPlayerInRangeEi, whose ROM name mangles as
   dActor_c::IsPlayerInRange(int): `this` in r0 and one `int` in r1, exactly as
   include/decl_Actor.h declares it and as the other four call sites in this
   file already spell it. The ROM emits no `mov` before the `bl` because r1
   still holds the incoming range, so naming the argument is byte-neutral here
   and stops the call from handing the callee whatever r1 happens to hold on a
   host ABI. */
extern "C" {
int func_ov002_020af218(da1up_c* c, int range){
  extern int _ZN8dActor_c15IsPlayerInRangeEi(void*, int);
  extern int func_ov002_020aefa4(void*);
  c->mShown=(char)_ZN8dActor_c15IsPlayerInRangeEi(c, range);
  unsigned char v=c->mShown;
  if(v==0) return v;
  return func_ov002_020aefa4((void*)c);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af248
/* The expiry countdown, called with n = 30 from state 2 of the behaviours that
   have one. Nothing happens while mStateFrames < n; for the next 40 frames
   (n <= mStateFrames < n + 0x28) mBlinkOn follows the low bit of mStateFrames
   (visible on odd frames); from mStateFrames == n + 0x28 on the actor is removed
   (KillAndTrackInDeathTable) and 1 is returned. Returns 0 otherwise. */
extern "C" {
int func_ov002_020af248(da1up_c* c, int n){
  int v = c->mStateFrames;
  if(v < n) return 0;
  if(v < n + 0x28){
    c->mBlinkOn = (v & 1) != 0;
  } else {
    c->KillAndTrackInDeathTable();
    return 1;
  }
  return 0;
}
}

/* The first of the two file-scope `extern "C"` regions. Ordinal 9 is a class
   member function, so it cannot sit in one and a declaration written in its body
   would mangle; these three have to be here. The shared actor egg-turn hook
   and its forwarding helpers return void. Promotion had changed ordinal 14
   from void to int to agree with the old hook declaration despite its void
   terminal call. Ordinals 9, 19 and 22 now use this one void declaration;
   ordinals 19 and 22 retain the player lookup result and its null test. */
extern "C" {
void GiveLives(int count);
void func_ov002_020af684(da1up_c* self, int target, Player* player);
void func_ov002_020bdf8c(Player* player);
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c13OnTurnIntoEggER6Player
/* Vtable slot 19, verified against config/arm9/overlays/ov002/relocs.txt:
   _ZTV7da1up_c (0x021083c8) + 0x4c relocates to 0x020af2b0, this address.
   Collects the mushroom for `player` (the hook for a mushroom being turned
   into an egg, per its slot). Types 11 and 12 go through
   func_ov002_020af684 with target 5 and 7, the same hand-off their touch
   handlers make. Otherwise a 1-Up (actorID 276) plays sound 0x6e, gives one
   life, spawns the 1UP logo actor (331) 180 units (0xb4000) above itself and
   removes itself; any other actor ID calls Player::func_ov002_020bdf8c on
   `player` and removes itself. */
void da1up_c::OnTurnIntoEgg(Player &player)
{
    if (mMushroomType == MUSHROOM_SPIN_TRIGGER_FOR_5) {
        return func_ov002_020af684(this, MUSHROOM_HIDDEN_FLEE, &player);
    }
    if (mMushroomType == MUSHROOM_SPIN_TRIGGER_FOR_7) {
        return func_ov002_020af684(this, MUSHROOM_HIDDEN_CHASE, &player);
    }
    unsigned isMatch = (actorID == ACTOR_ONEUPKINOKO);
    if (isMatch) {
        Vector3 vec;
        Sound::PlayBank3(SND3_GIVE_LIFE, *(Vector3 *)&mCamSpacePosX);
        GiveLives(1);
        vec.x = mPosX;
        vec.y = mPosY;
        vec.z = mPosZ;
        vec.y += 0xb4000;
        Spawn(ACTOR_OBJ_1UPLOGO, 8, vec, 0, mAreaId, -1);
        KillAndTrackInDeathTable();
    } else {
        ((Player *)(&player))->func_ov002_020bdf8c();
        KillAndTrackInDeathTable();
    }
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c13OnYoshiTryEatEv
/* Vtable slot 18. Two instructions: mov r0,#4; bx lr. */
s32 da1up_c::OnYoshiTryEat()
{
    return 4;
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af3a8
/* The touch-collection step, called from every behaviour except types 6, 8, 9,
   10, 11 and 12. Does nothing unless func_ov002_020af1dc finds a player touching
   the cylinder. Then: the Mega Mushroom (actorID 277) calls
   Player::func_ov002_020bdf8c on that player; the 1-Up (actorID 276) plays sound
   0x6e, gives one life and spawns the 1UP logo actor (331) 180 units (0xb4000)
   above itself; either way the mushroom then removes itself. */
extern "C" {
void func_ov002_020af3a8(char* raw)
{
    da1up_c* c = (da1up_c*)raw;
    extern void _ZN5Sound9PlayBank3EjRK7Vector3(u32 id, struct Vector3* v);

    Player* r = (Player*)func_ov002_020af1dc(raw);
    if (r == 0)
        return;

    unsigned short h = c->actorID;
    unsigned is115 = (h == ACTOR_SCALEUP_KINOKO);
    if (is115) {
        r->func_ov002_020bdf8c();
    } else {
        unsigned is114 = (h == ACTOR_ONEUPKINOKO);
        if (is114) {
            struct Vector3 vec;
            _ZN5Sound9PlayBank3EjRK7Vector3(SND3_GIVE_LIFE, (struct Vector3*)&c->mCamSpacePosX);
            GiveLives(1);
            vec.x = c->mPosX;
            vec.y = c->mPosY;
            vec.z = c->mPosZ;
            vec.y += 0xb4000;
            dActor_c::Spawn(
                ACTOR_OBJ_1UPLOGO, 8, vec, 0, c->mAreaId, -1);
        }
    }
    c->KillAndTrackInDeathTable();
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af474
/* The launch arc of the types that pop out (0, 1, 5 and 7). For the first five
   frames of the state (mStateTimer < 5) it only sets mVertSpeed to 40 units
   (0x28000). After that, every frame it lowers mPrevAngleX by 0x1000 (22.5
   degrees) and reads the s16 pair of data_02082214 (the sin/cos table, 0x1000 =
   1.0) at index mPrevAngleX >> 4: mVertSpeed = 30 units (0x1e) times word 1 plus
   2 units (0x2000); mHorzSpeed = -30 units times word 0. */
extern "C" {
void func_ov002_020af474(char* raw)
{
    da1up_c* o = (da1up_c*)raw;
    extern s16 data_02082214[];
    int a;

    if (*(u16*)&o->mStateTimer < 5) {
        o->mVertSpeed = 0x28000;
        return;
    }

    {
        s16* p = &o->mPrevAngleX;
        *p = *p - 0x1000;
    }

    a = (int)*(u16*)&o->mPrevAngleX >> 4;
    o->mVertSpeed = (s16)data_02082214[a * 2 + 1] * (s16)0x1e + 0x2000;

    a = (int)*(u16*)&o->mPrevAngleX >> 4;
    o->mHorzSpeed = (s16)data_02082214[a * 2] * (s16)-0x1e;
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af4ec
/* Per-frame model matrix and drop shadow, run at the end of every Behavior,
   including the frames UpdateYoshiEat returns nonzero. mModel's matrix gets
     types 11 and 12: a Y rotation by mAngleY and translation mPos >> 3;
     other types:     translation mPos >> 3 only (through Vec3_Asr).
   Nothing more happens while mShown is 0. Otherwise the shadow size is chosen and
   handed to dActor_c::DropShadowRadHeight (shadow, matrix, radius, depth,
   opacity) with opacity word 0xf; the local named `radius` is passed in the
   callee's radius slot and `depth` in its depth slot:
     types 11 and 12: radius = depth = 80 units (0x50000);
     airborne:        dBgCh_Gnd probes the floor from 40 units (0x28000) above
                      the actor; depth = the actor's height over the hit (1 unit
                      at least), radius = twice (cylinder radius - 10 units)
                      minus depth * 0x180 / 0x1000 (a Fix12 multiply by 0.09375),
                      10 units at least, and then depth += 60 units (0x3c000);
     on the ground:   depth = 60 units, radius = twice (cylinder radius - 10 units). */
extern "C" {
void func_ov002_020af4ec(void* raw)
{
    da1up_c* self = (da1up_c*)raw;
    extern void Matrix4x3_FromRotationY(void* m, int angle);
    extern void Vec3_Asr(struct Vector3* d, struct Vector3* s, int sh);
    extern void Matrix4x3_FromTranslation(void* m, int x, int y, int z);
    extern void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void* thiz, void* shadow, void* mtx, int radius, int depth, unsigned int x);

    int depth;
    int radius;
    struct Vector3 v2;
    struct Vector3 v1;

    if ((unsigned)(self->mMushroomType - 0xb) <= 1) {
        Matrix4x3_FromRotationY(&self->mModel.mat4x3, self->mAngleY);
        self->mModel.mat4x3.m[9] = self->mPosX >> 3;
        self->mModel.mat4x3.m[10] = self->mPosY >> 3;
        self->mModel.mat4x3.m[11] = self->mPosZ >> 3;
    } else {
        Vec3_Asr(&v1, (struct Vector3*)&self->mPosX, 3);
        Matrix4x3_FromTranslation(&self->mModel.mat4x3, v1.x, v1.y, v1.z);
    }

    if (self->mShown == 0) return;

    if ((unsigned)(self->mMushroomType - 0xb) <= 1) {
        radius = 0x50000;
        depth = 0x50000;
    } else if (!self->mWithMeshClsn.IsOnGround()) {
        int y = self->mPosY;
        int z = self->mPosZ;
        int adjustedY;
        int x = self->mPosX;
        adjustedY = y + 0x28000;
        v2.x = x;
        v2.y = adjustedY;
        v2.z = z;
        dBgCh_Gnd rg;
        rg.SetObjAndPos(v2, 0);
        depth = v2.y;
        if (rg.DetectClsn()) {
            depth = rg.clsnY;
        }
        depth = self->mPosY - depth;
        if (depth <= 0x1000) depth = 0x1000;
        radius = (self->mdCcAc_c.radius - 0xa000) * 2 - (int)(((long long)depth * 0x180 + 0x800) >> 12);
        if (radius < 0xa000) radius = 0xa000;
        depth += 0x3c000;
    } else {
        depth = 0x3c000;
        radius = (self->mdCcAc_c.radius - 0xa000) * 2;
    }

    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(self, &self->mShadowModel, &self->mModel.mat4x3, radius, depth, 0xf);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af684
/* Shared with the egg-turn hook and dispatch indices 8 and 6. This helper
   finishes by killing the actor and returns no value. Its callers use the same
   void contract; the lookup result in ordinals 19 and 22 is still needed as
   the player argument. This reconstructs a consistent interface, not an
   original return type recovered from an unused register.
   What it does: walks the actors that share this mushroom's actorID
   (FindWithActorID) and, on the first one whose mMushroomType equals `target`,
   decrements its mUnlockCount. When this mushroom is type 11 or 12 it also gives
   the player one coin (GiveCoins with the player number) and Heal(0x100),
   playing sound 0x12 when the player is underwater and 0x11 otherwise -- the
   same calls and sounds dActor_c::GivePlayerCoins makes for a single coin.
   Always ends by removing the mushroom. */
extern "C" {
void func_ov002_020af684(da1up_c* self, int target, Player* player){
    /* dActor_c is the real class from the includes above. A block-scope
       `struct dActor_c;` here would declare a LOCAL class instead, and C++
       gives a local class no linkage, so the extern below would be
       ill-formed (MSVC C2624). The declaration is dropped; the pointer
       arithmetic under it is unchanged and so is the object. */
    extern void GiveCoins(int idx, int amount);
    extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned int id, void* pos);

    Player* p = player;
    dActor_c* found = 0;
    for (;;) {
        found = dActor_c::FindWithActorID(self->actorID, found);
        if (found == 0)
            break;
        if (target == ((da1up_c*)found)->mMushroomType) {
            ((da1up_c*)found)->mUnlockCount--;
            break;
        }
    }
    if ((unsigned int)(self->mMushroomType - 0xb) <= 1) {
        GiveCoins(p->mPlayerNo, 1);
        p->Heal(0x100);
        if (p->mIsUnderwater)
            _ZN5Sound9PlayBank3EjRK7Vector3(SND3_COIN_UNDERWATER, &self->mCamSpacePosX);
        else
            _ZN5Sound9PlayBank3EjRK7Vector3(SND3_COIN, &self->mCamSpacePosX);
    }
    self->KillAndTrackInDeathTable();
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af724
/* Dispatch-table index 13 (MUSHROOM_FALL_THEN_WAIT). Physics runs every frame.
   State 0 plays sound 0x69 and moves on; state 1 waits until the actor's floor
   collision (mWithMeshClsn) reports ground contact, then enables the cylinder
   (clears bit 0 of its flags, which disables it while set) and moves on; state 2
   runs the touch check. Then the range check (3000 units) and the trail effect. */
extern "C" {
void func_ov002_020af724(da1up_c *self)
{
    extern void func_ov002_020aefb8(void *c);
    extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned int id, struct Vector3 *v);
    extern void func_ov002_020af3a8(void *c);
    extern int func_ov002_020af218(void *c, int a);
    extern void func_ov002_020aeee4(void *c);

    func_ov002_020aefb8(self);
    switch (self->mState) {
    case 0:
        _ZN5Sound9PlayBank3EjRK7Vector3(SND3_UNK_69, (struct Vector3 *)&self->mCamSpacePosX);
        self->mState += 1;
        break;
    case 1:
        if (self->mWithMeshClsn.IsOnGround() != 0) {
            self->mdCcAc_c.flags &= ~CC_FLAGS_DISABLED;
            self->mState += 1;
        }
        break;
    case 2:
        func_ov002_020af3a8(self);
        break;
    }
    func_ov002_020af218(self, 0xbb8);
    func_ov002_020aeee4(self);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af7cc
/* Dispatch-table index 10 (MUSHROOM_RISE_THEN_POP_OUT). Marks the mushroom
   shown and, while it is below 100 units (0x64000) above mSpawnPosY, raises it 5
   units (0x5000) per frame. On the frame it reaches that height it is clamped
   to it and turned into type 0 in state 0, with mStateTimer and mStateFrames set
   to 0xffff so the increments Behavior makes right after the call wrap them to
   0. */
extern "C" {
void func_ov002_020af7cc(da1up_c* c)
{
    c->mShown = 1;
    if (c->mPosY >= c->mSpawnPosY + 0x64000) return;
    c->mPosY += 0x5000;
    if (c->mPosY < c->mSpawnPosY + 0x64000) return;
    c->mPosY = c->mSpawnPosY + 0x64000;
    c->mMushroomType = 0;
    c->mState = 0;
    *(unsigned short*)&c->mStateTimer = 0xffff;
    c->mStateFrames = 0xffff;
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af838
/* Dispatch-table index 9 (MUSHROOM_SPAWNER). Spawns three mushrooms of the same
   actor ID around mSpawnPos and then removes itself: spawn param 0x25 (type 5
   with mUnlockCount 2) 50 units (0x32000) above the spawn point, and two of
   param 0xb (type 11) at the spawn height, 500 units (0x1f4000) to the -x and +x
   side. */
extern "C" {
void func_ov002_020af838(da1up_c* c)
{

    struct Vector3 vec;

    vec.x = c->mSpawnPosX;
    vec.y = c->mSpawnPosY;
    vec.z = c->mSpawnPosZ;
    vec.y = c->mSpawnPosY + 0x32000;
    dActor_c::Spawn(
        c->actorID, 0x25, vec, 0, c->mAreaId, -1);

    vec.y = c->mSpawnPosY;
    vec.x = c->mSpawnPosX - 0x1f4000;
    dActor_c::Spawn(
        c->actorID, 0xb, vec, 0, c->mAreaId, -1);

    vec.x = c->mSpawnPosX + 0x1f4000;
    dActor_c::Spawn(
        c->actorID, 0xb, vec, 0, c->mAreaId, -1);

    c->KillAndTrackInDeathTable();
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af908
/* Dispatch-table index 12 (MUSHROOM_SPIN_TRIGGER_FOR_7). Turns mAngleY by 0xc00
   (16.875 degrees) a frame, then runs index 8's handler. */
extern "C" {
void func_ov002_020af908(da1up_c *self) {
    self->mAngleY += 0xc00;
    func_ov002_020af924(self);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af924
/* Dispatch-table index 8 (MUSHROOM_TRIGGER_FOR_7). If a player is touching it,
   hands off to func_ov002_020af684 with target 7 (a waiting type 7 loses one
   from its mUnlockCount). */
extern "C" {
void func_ov002_020af924(da1up_c* c){
  extern Player* func_ov002_020af1dc(void*);
  Player* r=func_ov002_020af1dc(c);
  if(!r) return;
  func_ov002_020af684(c, 7, r);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020af950
/* Dispatch-table index 7 (MUSHROOM_HIDDEN_CHASE). State 0: hidden (mShown 0)
   until mUnlockCount is 0; then it appears -- mVertSpeed 40 units (0x28000),
   state 3, shown, opened to Yoshi (func_ov002_020aefa4), sound 0x68, mFlags bit
   0 cleared. State 3 is the launch arc (func_ov002_020af474) under physics, with
   the trail effect once mStateTimer is past 17 (0x11); at mStateTimer 0x25 (37)
   it enables the cylinder (clears bit 0 of its flags), goes to state 1, sets
   mVertAccel to 0 and mHorzSpeed to 10 units (0xa000). State 1 steers toward the
   player (func_ov002_020af0c0) under physics. */
extern "C" {
void func_ov002_020af950(da1up_c *self)
{
  extern void func_ov002_020aefa4(da1up_c *thiz);
  extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned int, const void *);
  extern void func_ov002_020af0c0(da1up_c *thiz);
  extern void func_ov002_020aefb8(da1up_c *thiz);
  extern void func_ov002_020aeee4(da1up_c *thiz);
  extern void func_ov002_020af474(da1up_c *thiz);

  switch (self->mState)
  {
    case 0:
      self->mShown = 0;
      if (self->mUnlockCount != 0)
        return;

      self->mVertSpeed = 0x28000;
      self->mState = 3;
      self->mShown = 1;
      func_ov002_020aefa4(self);
      _ZN5Sound9PlayBank3EjRK7Vector3(SND3_LAUNCH, &self->mCamSpacePosX);
      self->mFlags &= ~ACTOR_FLAG_CLIP_TEST;
      return;

    case 1:
      func_ov002_020af0c0(self);
      func_ov002_020aefb8(self);
      return;

    case 3:
      func_ov002_020aefb8(self);
      if (*(unsigned short *)&self->mStateTimer > 0x11)
        func_ov002_020aeee4(self);
      func_ov002_020af474(self);
      if (*(unsigned short *)&self->mStateTimer != 0x25)
        return;

      self->mdCcAc_c.flags &= ~CC_FLAGS_DISABLED;
      self->mState = 1;
      self->mVertAccel = 0;
      self->mHorzSpeed = 0xa000;
      return;
  }
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afa50
/* Dispatch-table index 11 (MUSHROOM_SPIN_TRIGGER_FOR_5). Turns mAngleY by 0xc00
   (16.875 degrees) a frame, then runs index 6's handler. */
extern "C" {
void func_ov002_020afa50(da1up_c *self) {
    self->mAngleY += 0xc00;
    func_ov002_020afa6c((char*)self);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afa6c
/* Dispatch-table index 6 (MUSHROOM_TRIGGER_FOR_5). If a player is touching it,
   hands off to func_ov002_020af684 with target 5 (a waiting type 5 loses one
   from its mUnlockCount). */
extern "C" {
void func_ov002_020afa6c(char* raw){
  da1up_c* c = (da1up_c*)raw;
  extern Player* func_ov002_020af1dc(void*);
  Player* r=func_ov002_020af1dc(c);
  if(!r) return;
  func_ov002_020af684(c, 5, r);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afa98
/* Dispatch-table index 5 (MUSHROOM_HIDDEN_FLEE). State 0: hidden until
   mUnlockCount is 0, then it appears exactly as index 7 does (state 3, sound
   0x68, and so on). State 3 is the launch arc under physics with the trail
   effect once mStateTimer is past 17; at mStateTimer 0x25 (37) it enables the
   cylinder, goes to state 1 and sets mHorzSpeed to 8 units (0x8000). State 1
   walks away from the player (func_ov002_020afde4) under physics with the trail
   effect; state 2 is physics, the touch check and the expiry countdown (30 frames, then 40 blinking, then removed)
   (func_ov002_020af248). */
extern "C" {
void func_ov002_020afa98(da1up_c *c)
{
    extern void _ZN5Sound9PlayBank3EjRK7Vector3(u32 id, struct Vector3 *v);
    extern void func_ov002_020aefa4(da1up_c *thiz);
    extern void func_ov002_020aefb8(da1up_c *thiz);
    extern void func_ov002_020afde4(da1up_c *thiz);
    extern void func_ov002_020aeee4(da1up_c *thiz);
    extern void func_ov002_020af3a8(da1up_c *thiz);
    extern void func_ov002_020af474(da1up_c *thiz);
    extern int func_ov002_020af248(da1up_c *thiz, int n);

    switch (c->mState) {
    case 0:
        c->mShown = 0;
        if (c->mUnlockCount != 0)
            return;
        c->mVertSpeed = 0x28000;
        c->mState = 3;
        c->mShown = 1;
        func_ov002_020aefa4(c);
        _ZN5Sound9PlayBank3EjRK7Vector3(SND3_LAUNCH, (struct Vector3 *)&c->mCamSpacePosX);
        c->mFlags &= ~ACTOR_FLAG_CLIP_TEST;
        return;
    case 1:
        func_ov002_020aefb8(c);
        func_ov002_020afde4(c);
        func_ov002_020aeee4(c);
        return;
    case 2:
        func_ov002_020aefb8(c);
        func_ov002_020af3a8(c);
        func_ov002_020af248(c, 0x1e);
        return;
    case 3:
        func_ov002_020aefb8(c);
        if (*(u16 *)&c->mStateTimer > 0x11) {
            func_ov002_020aeee4(c);
        }
        func_ov002_020af474(c);
        if (*(u16 *)&c->mStateTimer != 0x25)
            return;
        c->mdCcAc_c.flags &= ~CC_FLAGS_DISABLED;
        c->mState = 1;
        c->mHorzSpeed = 0x8000;
        return;
    }
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afbb4
/* Dispatch-table index 4 (MUSHROOM_WAIT_THEN_FACE_AWAY). State 0 waits for the
   player to come within 1000 units (0x3e8), then sets mVertSpeed to 40 units
   (0x28000) and goes to state 1. State 1 turns away from the player
   (func_ov002_020afde4) with the trail effect; unlike types 1 and 5 it sets no
   horizontal speed here; state 2 is the touch check plus
   the expiry countdown (30 frames, then 40 blinking, then removed). The 3000-unit range check runs every frame. The two
   `func_ov002_020aefb8()` calls really do pass
   no argument -- r0 already carries the object -- so this member keeps its own
   nullary view of that symbol at block scope. */
extern "C" {
void func_ov002_020afbb4(da1up_c* c)
{
    extern int _ZN8dActor_c15IsPlayerInRangeEi(da1up_c* thiz, int r);
    extern void func_ov002_020aefb8(void);
    extern void func_ov002_020afde4(da1up_c* thiz);
    extern void func_ov002_020aeee4(da1up_c* thiz);
    extern void func_ov002_020af3a8(da1up_c* thiz);
    extern int func_ov002_020af248(da1up_c* thiz, int n);
    extern int func_ov002_020af218(da1up_c* thiz, int n);

    switch (c->mState) {
    case 0:
        if (_ZN8dActor_c15IsPlayerInRangeEi(c, 0x3e8)) {
            c->mVertSpeed = 0x28000;
            c->mState = 1;
        }
        break;
    case 1:
        func_ov002_020aefb8();
        func_ov002_020afde4(c);
        func_ov002_020aeee4(c);
        break;
    case 2:
        func_ov002_020aefb8();
        func_ov002_020af3a8(c);
        func_ov002_020af248(c, 0x1e);
        break;
    }
    func_ov002_020af218(c, 0xbb8);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afc44
/* Dispatch-table index 3 (MUSHROOM_STATIONARY). Only the touch check and the
   3000-unit range check; no physics and no state. */
extern "C" {
int func_ov002_020afc44(da1up_c* c){
  extern int func_ov002_020af3a8(void*);
  extern int func_ov002_020af218(void*, int);
  func_ov002_020af3a8(c);
  return func_ov002_020af218(c, 0xbb8);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afc68
/* State 1 of index 2 (MUSHROOM_WAIT_THEN_ACCELERATE). On the ground it adds 25
   units (0x19000) to mHorzSpeed and zeroes mVertSpeed. In the air it multiplies
   mHorzSpeed by 0xfae / 0x1000 (about 0.98) and passes the product through
   cstd::fdiv(t, 0x1000). mHorzSpeed is capped at 40 units (0x28000). When the
   player is not within 5000 units (0x1388) it goes to state 2. */
extern "C" {
void func_ov002_020afc68(da1up_c *self)
{
    extern int _ZN4cstd4fdivEii(int a, int b);
    extern int _ZN8dActor_c15IsPlayerInRangeEi(void *thiz, int r);

    if (self->mWithMeshClsn.IsOnGround() != 0) {
        self->mHorzSpeed += 0x19000;
        self->mVertSpeed = 0;
    } else {
        int t = (int)(((s64)self->mHorzSpeed * 0xfae + 0x800) >> 12);
        self->mHorzSpeed = _ZN4cstd4fdivEii(t, 0x1000);
    }
    if (self->mHorzSpeed > 0x28000) {
        self->mHorzSpeed = 0x28000;
    }
    if (_ZN8dActor_c15IsPlayerInRangeEi(self, 0x1388) == 0) {
        self->mState = 2;
    }
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afd10
/* Dispatch-table index 2 (MUSHROOM_WAIT_THEN_ACCELERATE). From state 1 on, the
   words at 0xa4 / 0xa8 / 0xac are saved before the physics step and the first
   and last restored afterwards. State 0: range check (3000 units), and when the
   player comes within 1000 units (0x3e8) it sets mVertAccel to -4 units
   (-0x4000) and goes to state 1. State 1: func_ov002_020afc68. State 2: the
   expiry countdown (30 frames, then 40 blinking, then removed). Every frame ends with the touch check and the trail effect. */
extern "C" {
void func_ov002_020afd10(da1up_c* c)
{
    extern int _ZN8dActor_c15IsPlayerInRangeEi(da1up_c* thiz, int r);
    extern void func_ov002_020aefb8(da1up_c* thiz);
    extern void func_ov002_020afc68(da1up_c* thiz);
    extern void func_ov002_020af3a8(da1up_c* thiz);
    extern void func_ov002_020aeee4(da1up_c* thiz);
    extern int func_ov002_020af218(da1up_c* thiz, int n);
    extern int func_ov002_020af248(da1up_c* thiz, int n);

    volatile Fix12i v[3];

    if (c->mState != 0) {
        v[0] = c->unk_0a4;
        v[1] = c->mVertSpeed;
        v[2] = c->unk_0ac;
        func_ov002_020aefb8(c);
        c->unk_0a4 = v[0];
        c->unk_0ac = v[2];
    }

    switch (c->mState) {
    case 0:
        func_ov002_020af218(c, 0xbb8);
        if (_ZN8dActor_c15IsPlayerInRangeEi(c, 0x3e8)) {
            c->mVertAccel = -0x4000;
            c->mState = 1;
        }
        break;
    case 1:
        func_ov002_020afc68(c);
        break;
    case 2:
        func_ov002_020af248(c, 0x1e);
        break;
    }

    func_ov002_020af3a8(c);
    func_ov002_020aeee4(c);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afde4
/* Walking away from the player, called from state 1 of types 1, 4 and 5. Sets
   mPrevAngleY to the bearing from the mushroom to the closest player plus half
   a turn (0x8000), runs the touch check, and goes to state 2 when the floor
   collision reports a wall or the player is not within 3000 units. */
extern "C" {
void func_ov002_020afde4(char* raw){
  da1up_c* c = (da1up_c*)raw;
  extern short Vec3_HorzAngle(void*, void*);
  extern void func_ov002_020af3a8(da1up_c*);
  extern int _ZN8dActor_c15IsPlayerInRangeEi(da1up_c*, int);
  Player* p = c->ClosestPlayer();
  if(p){
    c->mPrevAngleY = Vec3_HorzAngle(&c->mPosX, &p->mPosX) + 0x8000;
  }
  func_ov002_020af3a8(c);
  if(c->mWithMeshClsn.IsOnWall()) c->mState=2;
  if(_ZN8dActor_c15IsPlayerInRangeEi(c, 0xbb8)==0) c->mState=2;
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020afe4c
/* Dispatch-table index 1 (MUSHROOM_POP_OUT_FLEE). Physics every frame. State 0:
   sound 0x68 on the first frame and the launch arc; at mStateTimer 0x25 (37) it
   enables the cylinder, goes to state 1 and sets mHorzSpeed to 8 units (0x8000).
   State 1 walks away from the player (func_ov002_020afde4). State 2 is the
   expiry countdown (30 frames, then 40 blinking, then removed) and the touch check. Every frame ends with the range check
   and the trail effect. */
extern "C" {
void func_ov002_020afe4c(da1up_c* c) {
    extern void func_ov002_020aefb8(da1up_c* thiz);
    extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned a, void* v);
    extern void func_ov002_020af474(da1up_c* thiz);
    extern void func_ov002_020afde4(da1up_c* thiz);
    extern int func_ov002_020af248(da1up_c* thiz, int n);
    extern void func_ov002_020af3a8(da1up_c* thiz);
    extern int func_ov002_020af218(da1up_c* thiz, int n);
    extern void func_ov002_020aeee4(da1up_c* thiz);

    func_ov002_020aefb8(c);
    switch (c->mState) {
    case 0:
        if (*(unsigned short*)&c->mStateTimer == 0) {
            _ZN5Sound9PlayBank3EjRK7Vector3(SND3_LAUNCH, &c->mCamSpacePosX);
        }
        func_ov002_020af474(c);
        if (*(unsigned short*)&c->mStateTimer == 0x25) {
            c->mdCcAc_c.flags &= ~CC_FLAGS_DISABLED;
            c->mState = 1;
            c->mHorzSpeed = 0x8000;
        }
        break;
    case 1:
        func_ov002_020afde4(c);
        break;
    case 2:
        func_ov002_020af248(c, 0x1e);
        func_ov002_020af3a8(c);
        break;
    }
    func_ov002_020af218(c, 0xbb8);
    func_ov002_020aeee4(c);
}
}

/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020aff10
/* Dispatch-table index 0 (MUSHROOM_POP_OUT_DRIFT). Physics every frame.
   State 0: sound 0x68 on the first frame and the launch arc; at mStateTimer
   0x25 (37) it enables the cylinder, goes to state 1 and sets mHorzSpeed to
   2 units (0x2000), which this handler never clears. State 1: touch check, and
   state 2 once mStateTimer exceeds 300 (0x12c). State 2: the expiry countdown
   (30 frames, then 40 blinking, then removed) and the touch check. Every frame ends with the range check and the trail effect. */
extern "C" {
void func_ov002_020aff10(da1up_c* c){
  extern void _ZN5Sound9PlayBank3EjRK7Vector3(unsigned int a, void* v);
  extern void func_ov002_020aefb8(da1up_c* thiz);
  extern void func_ov002_020af474(da1up_c* thiz);
  extern void func_ov002_020af3a8(da1up_c* thiz);
  extern void func_ov002_020aeee4(da1up_c* thiz);
  extern int func_ov002_020af248(da1up_c* thiz, int n);
  extern int func_ov002_020af218(da1up_c* thiz, int n);

  func_ov002_020aefb8(c);
  switch(c->mState){
  case 0:
    if(*(unsigned short*)&c->mStateTimer == 0) _ZN5Sound9PlayBank3EjRK7Vector3(SND3_LAUNCH, &c->mCamSpacePosX);
    func_ov002_020af474(c);
    if(*(unsigned short*)&c->mStateTimer != 0x25) break;
    c->mdCcAc_c.flags &= ~CC_FLAGS_DISABLED;
    c->mState = 1;
    c->mHorzSpeed = 0x2000;
    break;
  case 1:
    if(*(unsigned short*)&c->mStateTimer > 0x12c) c->mState = 2;
    func_ov002_020af3a8(c);
    break;
  case 2:
    func_ov002_020af248(c, 0x1e);
    func_ov002_020af3a8(c);
    break;
  }
  func_ov002_020af218(c, 0xbb8);
  func_ov002_020aeee4(c);
}
}

/* The second file-scope `extern "C"` region, for the five class members below.
   Every one of these is spelt as the shard that uses it recovered it; none of
   the 29 members above declares any of them, so nothing here overrides a
   recovered view. `func_ov002_020af4ec` needs no entry -- ordinal 13's
   definition is already visible with C linkage. */
extern "C" {
void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void* thiz, dActor_c* a, int r, int h, unsigned int e, unsigned int g);
void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void* thiz, dActor_c* a, int r, int h, Vector3_16* p, int q);
int IsStarCollectedInCurLevel(int a);
}

/* Ordinal 34 dispatches through a pointer-to-member-function, and mwccarm's
   representation of one depends on the class it names, so the shard's own
   opaque stand-in is kept rather than pointed at the real da1up_c: merging a
   shared shadow struct is a codegen hazard, measured on ov006/dScMgSound_c. */
struct C;
typedef void (C::*PMF)();
struct C {
  char pad[0x500];
};

/* Ordinal 35's view of data_ov002_0210d9b8: a cached model handle whose second
   word is the BMD file pointer. */
struct ModelCache { int pad0; BMD_File* file; };

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c16CleanupResourcesEv
/* Vtable slot 3. Releases the model file InitResources loaded -- the 1-Up's
   (data_ov002_0210d9d8) for actorID 276, the other's (data_ov002_0210da30)
   otherwise -- except for types 11 and 12, which use the shared model
   data_ov002_0210d9b8 and release nothing here. For types 11 and 12 it also
   starts particle effect 0xd2 through Particle::System::NewSimple at the
   mushroom's position, 40 units (0x28000) above it. */
int da1up_c::CleanupResources()
{
  extern SharedFilePtr data_ov002_0210d9d8;
  extern SharedFilePtr data_ov002_0210da30;

  int s = mMushroomType;
  if (s != 0xb && s != 0xc){
    int b = (actorID == ACTOR_ONEUPKINOKO);
    if (b != 0) data_ov002_0210d9d8.Release();
    else data_ov002_0210da30.Release();
  }
  if ((unsigned int)(mMushroomType - 0xb) <= 1)
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(PTCL_TYPE_11_12_CLEANUP, mPosX, mPosY + 0x28000, mPosZ);
  return 1;
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c16OnPendingDestroyEv
/* Vtable slot 12. One instruction: bx lr. */
void da1up_c::OnPendingDestroy()
{
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c6RenderEv
/* Vtable slot 9. Draws mModel unless mShown or mBlinkOn is 0 or mFlags bit
   0x40000 -- one of the yoshi-mouth states named in dActor_c.h -- is set.
   Returns 1 either way. */
int da1up_c::Render()
{
    if (mShown == 0 || mBlinkOn == 0)
        return 1;
    {
        int b = (mFlags & 0x40000) ? 1 : 0;
        if (b)
            return 1;
    }
    mModel.Render(0);
    return 1;
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c8BehaviorEv
/* Vtable slot 6, and the only literal-pool reference to the 14-element dispatch array at
   0x0210dc00 in the arm9/overlay images besides __sinit_ov002_02100adc, which fills it
   from the 14 descriptors at 0x02108300..0x02108370. mMushroomType is the
   index. The array stays `extern`: this TU claims .text only, so the sinit, the
   descriptors and the array itself remain their own shards.
   When UpdateYoshiEat returns nonzero only the matrix and shadow
   (func_ov002_020af4ec) and the cylinder Clear run. Otherwise it zeroes
   mEatingPlayer, calls the type's handler, advances mStateTimer and
   mStateFrames by one, and if the handler changed mState zeroes both. */
int da1up_c::Behavior()
{
  extern PMF data_ov002_0210dc00[];

  if(UpdateYoshiEat(mWithMeshClsn) != 0){
    func_ov002_020af4ec(this);
    mdCcAc_c.Clear();
    return 1;
  }
  mEatingPlayer = 0;
  {
    int old = mState;
    C* self = (C*)((char*)this);
    (self->*data_ov002_0210dc00[mMushroomType])();
    /* Named ++mStateTimer / mStateTimer = 0 size-DIFF vs this recovered
       unsigned-short launder; keep MATCH form. */
    ++*(unsigned short*)&mStateTimer;
    ++mStateFrames;
    if(old != mState){
      *(unsigned short*)&mStateTimer = 0;
      mStateFrames = 0;
    }
  }
  mdCcAc_c.Clear();
  mdCcAc_c.Update();
  func_ov002_020af4ec(this);
  return 1;
}

/* -------------------------------------------------------------------------- */
// @symbol _ZN7da1up_c13InitResourcesEv
/* Vtable slot 0, the largest member in the TU, and -- because the destructor is
   declared out of line above it -- NOT this class's key function.
   Order of work: mMushroomType from param1 bits 0..3; the model (the shared
   data_ov002_0210d9b8 for types 11 and 12, otherwise the 1-Up's or the other
   actor's file by actorID) and the shadow cylinder; the dCcAc_c cylinder
   (types 6, 8, 11 and 12: radius 100 units, height 64 units, and 11 / 12 also
   get the yoshi-tongue bit in vulnFlags; otherwise 65 / 65 units for actorID 277
   and 50 / 50 for the other), with Init flags 0x100002 and vulnFlags 0; mState 0;
   two 14-byte per-type tables -- data_ov002_020ff040, where a 0 sets bit 0 of
   the cylinder's flags (disabled until a handler clears it; types 0, 1, 5, 7, 9,
   10 and 13), and data_ov002_020ff050, where a 0 clears mFlags bit 0 (the
   clip-test bit; every type except 3, 6 and 8); mShown 1 for types 11 and 12 and
   0 for the rest; mBlinkOn 1; mUnlockCount from param1 bits 4..7; the spawn
   point; gravity -2 units (-0x2000) and terminal velocity -50 units (-0x32000);
   the dBgCh_Actr (50 / 50 units) with its limited-movement flag. Finally, when
   LEVEL_ID (data_0209f2f8) is 7 and the actor is at y = 3500 units (0xdac000),
   z = 0, and either STAR_ID (data_0209f220) is 1 or star 1 is not collected in
   the current level, it calls MarkForDestruction and returns 0. */
int da1up_c::InitResources()
{
    extern ModelCache data_ov002_0210d9b8;
    extern SharedFilePtr data_ov002_0210d9d8;
    extern SharedFilePtr data_ov002_0210da30;
    extern signed char data_0209f2f8;
    extern unsigned char data_0209f220;
    extern unsigned char data_ov002_020ff040[];
    extern unsigned char data_ov002_020ff050[];

    BMD_File* f;
    int isOneUp, isMega;

    mMushroomType = param1 & 0xf;

    isOneUp = (actorID == ACTOR_ONEUPKINOKO);
    if (isOneUp) {
        if ((unsigned int)(mMushroomType - 0xb) <= 1) {
            if (mModel.SetFile(data_ov002_0210d9b8.file, 1, 1) == 0)
                return 0;
        } else {
            f = (BMD_File*)Model::LoadFile(data_ov002_0210d9d8);
            if (mModel.SetFile(f, 1, 1) == 0)
                return 0;
        }
    } else {
        if ((unsigned int)(mMushroomType - 0xb) <= 1) {
            if (mModel.SetFile(data_ov002_0210d9b8.file, 1, 1) == 0)
                return 0;
        } else {
            f = (BMD_File*)Model::LoadFile(data_ov002_0210da30);
            if (mModel.SetFile(f, 1, 1) == 0)
                return 0;
        }
    }

    if (mShadowModel.InitCylinder() == 0)
        return 0;

    if (mMushroomType == 6 || mMushroomType == 8 || (unsigned int)(mMushroomType - 0xb) <= 1) {
        _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, (dActor_c*)this, 0x64000, 0x40000, 0x100002, 0);
        if ((unsigned int)(mMushroomType - 0xb) <= 1) {
            mdCcAc_c.vulnFlags |= CC_VULN_YOSHI_TONGUE;
        }
    } else {
        isMega = (actorID == ACTOR_SCALEUP_KINOKO);
        if (isMega) {
            _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, (dActor_c*)this, 0x41000, 0x41000, 0x100002, 0);
        } else {
            _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, (dActor_c*)this, 0x32000, 0x32000, 0x100002, 0);
        }
    }

    mState = 0;
    if (data_ov002_020ff040[mMushroomType] == 0) {
        mdCcAc_c.flags |= CC_FLAGS_DISABLED;
    }
    if (data_ov002_020ff050[mMushroomType] == 0) {
        mFlags &= ~ACTOR_FLAG_CLIP_TEST;
    }
    if ((unsigned int)(mMushroomType - 0xb) <= 1) {
        mShown = 1;
    } else {
        mShown = 0;
    }
    mBlinkOn = 1;
    mUnlockCount = ((unsigned int)param1 >> 4) & 0xf;
    mSpawnPosX = mPosX;
    mSpawnPosY = mPosY;
    mSpawnPosZ = mPosZ;
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x32000;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(&mWithMeshClsn, (dActor_c*)this, 0x32000, 0x32000, 0, 0);
    mWithMeshClsn.SetLimMovFlag();
    mParticleID = 0;

    if (data_0209f2f8 == 7 && mPosY == 0xdac000 && mPosZ == 0
        && (data_0209f220 == 1 || IsStarCollectedInCurLevel(1) == 0)) {
        MarkForDestruction();
        return 0;
    }

    return 1;
}

/* -------------------------------------------------------------------------- */
/* MEGA_MUSHROOM (277 / SCALEUP_KINOKO). Leaf operator new routes to
   fBase_c::operator new; the implicit constructor inlines the dEnemyBase_c
   base step, vptr store, and the four member constructors. */
// @symbol da1up_c_classInit_SCALEUP_KINOKO
extern "C" da1up_c *da1up_c_classInit_SCALEUP_KINOKO()
{
    return new da1up_c();
}

/* -------------------------------------------------------------------------- */
/* ONE_UP_MUSHROOM (276 / ONEUPKINOKO). Same class, second profile. */
// @symbol da1up_c_classInit_ONEUPKINOKO
extern "C" da1up_c *da1up_c_classInit_ONEUPKINOKO()
{
    return new da1up_c();
}
