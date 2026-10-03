//cpp
/* daKirai_c: the spike bomb (ov060, .text 0x02118438..0x02118cbc;
 * daKirai_c_classInit stays out).
 * Every live bomb registers with the spike-bomb slot table
 * (AddSpikeBomb) and keeps the uniqueIDs of its siblings (actor 0x11c) so that,
 * once they have all gone off, the one nearest the player can re-arm.
 *
 * The nine func_ov060_* helpers are daKirai_c members in the original source;
 * the ROM gives them no name, so they keep their unnamed C symbols here and
 * take the bomb as an explicit `self`. data_ov060_0211b1d8 is the state table
 * Behavior dispatches through, filled at static-init time with helpers below.
 */

#include "daKirai_c.h"
#include "common.h"
#include "SharedFilePtr.h"
#include "Player.h"

/* Per-member opt brackets bind only when codegen is not deferred, and that
 * also lays .text down in source order. The file is therefore ROM-ascending. */
#pragma defer_codegen off

typedef void (daKirai_c::*KiraiState)();

extern "C" {
extern void ClearSpikeBomb(int idx);
extern int AddSpikeBomb(void *p);
extern int Vec3_HorzLen(const Vector3 *v);
extern int Vec3_Dist(const Vector3 *a, const Vector3 *b);
extern short Vec3_HorzAngle(const Vector3 *v0, const Vector3 *v1);
extern void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
extern void func_02012694(int a, void *b);
/* local extern: Particle::System::NewSimple, dActor_c::Earthquake and
   Player::Hurt take Fix12<int> by value and none of them is declared in
   include/; the sibling promoted TUs (daDkk_c, daDgr_c) keep the same
   mangled spelling for the same reason. */
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
extern void _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(void *self, Vector3 *v, int f);
extern void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *self, Vector3 *v, unsigned int b, int c, unsigned int d, unsigned int e, unsigned int f);
/* local extern: dCcAcPos_c::Init also takes its radius and height as
   Fix12<int> by value; spelt through the header, the two aggregate
   temporaries grow the frame by 8 and InitResources by 0x10. */
extern void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
    dCcAcPos_c *self, dActor_c *actor, const Vector3 &v, int radius, int height, unsigned flags, unsigned vulnFlags);
/* local extern: the model file and the state table keep the spellings
   include/decl_common.h and the ov060 sinit already give them (a byte, and a
   word array); retyping either here opens a new declaration-agreement
   disagreement, so each is cast at its one use instead. */
extern char data_ov060_0211b1c4;
extern int data_ov060_0211b1d8[];
}

// @symbol _ZN9daKirai_cD1Ev
// @symbol _ZN9daKirai_cD0Ev
daKirai_c::~daKirai_c()
{
}

extern "C" {
/* Disarm: give up the spike-bomb slot, stop colliding, and remember every
   other bomb still in the level. */
// @symbol func_ov060_021184bc
void func_ov060_021184bc(daKirai_c *self)
{
    int i;
    int j;
    unsigned int id;
    dActor_c *a;

    ClearSpikeBomb(self->mSlotIndex);
    self->mdCcAcPos_c.flags |= 1;
    self->mStateIndex = 3;
    a = 0;
    for (i = 0; i < 8; i++)
        self->mOtherBombIDs[i] = 0;
    j = 0;
    id = 0x11c;
    while (1) {
        a = dActor_c::FindWithActorID(id, a);
        if (a == 0)
            break;
        if (a != self) {
            self->mOtherBombIDs[j] = a->uniqueID;
            j++;
            if (j == 8)
                break;
        }
    }
}

/* Is `pos` close enough to set this bomb off? */
// @symbol func_ov060_02118544
int func_ov060_02118544(daKirai_c *self, Vector3 *pos)
{
    int horz, homeHorz;
    if (self->mStateIndex != 0) return 0;
    horz = Vec3_HorzLen(pos);
    homeHorz = self->mHomeHorzDist;
    if (horz >= homeHorz - 0x12c000 && horz <= homeHorz + 0x12c000) {
        if (Vec3_Dist((Vector3 *)&self->mHomePosX, pos) < self->mHomeYOffset) return 1;
    }
    return 0;
}

/* Explode. */
// @symbol func_ov060_021185c4
void func_ov060_021185c4(daKirai_c *self)
{
    Vector3 v;
    self->mStateIndex = 1;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xa8, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xa9, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xaa, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xab, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xac, self->mPosX, self->mPosY, self->mPosZ);
    func_02012694(0x2f, &self->mCamSpacePosX);
    v.x = self->mPosX;
    v.y = self->mPosY;
    v.z = self->mPosZ;
    _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(self, &v, 0x7d0000);
    self->mTimer = 0;
    func_ov060_021184bc(self);
}

/* Place the model and fade it. */
// @symbol func_ov060_02118690
void func_ov060_02118690(daKirai_c *self)
{
    Matrix4x3_FromTranslation(&self->mModel.mat4x3, self->mPosX >> 3, self->mPosY >> 3, self->mPosZ >> 3);
    self->mModel.ApplyOpacity((u8)(self->mOpacity >> 3), 1);
}

/* Re-arm. */
// @symbol func_ov060_021186d8
void func_ov060_021186d8(daKirai_c *self)
{
    self->mScaleX = 0x1000;
    self->mScaleY = 0x1000;
    self->mScaleZ = 0x1000;
    self->mOpacity = 0xff;
    self->mStateIndex = 0;
    self->mdCcAcPos_c.flags &= ~1;
    self->mTimer = 0;
    self->mSlotIndex = AddSpikeBomb(self);
}

/* Spent: wait until every sibling has gone off, then re-arm whichever bomb
   (this one or a sibling) is nearest the player. */
#pragma opt_strength_reduction off
#pragma opt_common_subs off
// @symbol func_ov060_02118728
void func_ov060_02118728(daKirai_c *self)
{
    Vector3 v;
    Player *player;
    daKirai_c *bestActor;
    daKirai_c *actor;
    int best;
    int i;

    player = self->ClosestPlayer();
    if (((self->mFlags & 8) ? 1 : 0) == 0) return;
    if (player == 0) return;

    {
        Vector3 *pp = (Vector3 *)&player->mPosX;
        v.x = pp->x;
        v.y = pp->y;
        v.z = pp->z;
    }
    best = Vec3_Dist((Vector3 *)&self->mPosX, &v);
    bestActor = self;

    for (i = 0; i < 8; i++) {
        int id = self->mOtherBombIDs[i];
        if (id == 0) continue;
        actor = (daKirai_c *)dActor_c::FindWithID(id);
        if (actor != 0) {
            if (actor->mStateIndex != 3) return;
            if (((actor->mFlags & 8) ? 1 : 0) == 0) continue;
            {
                int d = Vec3_Dist(&v, (Vector3 *)&actor->mPosX);
                if (d < best) {
                    best = d;
                    bestActor = actor;
                }
            }
        } else {
            self->mOtherBombIDs[i] = 0;
        }
    }
    func_ov060_021186d8(bestActor);
}
#pragma opt_common_subs on
#pragma opt_strength_reduction on

/* Exploding: swell, fade and rise for 0x1c frames, then disarm. */
// @symbol func_ov060_02118834
void func_ov060_02118834(daKirai_c *self)
{
    int scale = (self->mTimer * 9 << 12) / 14 + 0x1000;
    self->mScaleX = scale;
    self->mScaleY = scale;
    self->mScaleZ = scale;
    self->mOpacity -= 0xa;
    if (self->mOpacity < 0xa) self->mOpacity = 0;
    self->mPosY += self->mVertSpeed;
    if (self->mTimer == 0x1c) func_ov060_021184bc(self);
    self->mTimer++;
}

/* Swell for 0x1c frames without colliding, then disarm. */
// @symbol func_ov060_021188e8
void func_ov060_021188e8(daKirai_c *self)
{
    int scale;
    self->mdCcAcPos_c.flags |= 1;
    scale = (self->mTimer * 9 << 12) / 14 + 0x1000;
    self->mScaleX = scale;
    self->mScaleY = scale;
    self->mScaleZ = scale;
    if (self->mTimer == 0x1c)
        func_ov060_021184bc(self);
    self->mTimer++;
}

/* Armed: if the thing that touched us is a player (actor 0xbf), blow up in
   its face. */
// @symbol func_ov060_02118970
void func_ov060_02118970(daKirai_c *self)
{
    dActor_c *a;
    Vector3 quakePos, hurtPos;
    int isPlayer; /* int, not bool: measured, a bool local misses */
    unsigned int id;
    id = self->mdCcAcPos_c.otherOwner;
    if (id == 0) return;
    a = dActor_c::FindWithID(id);
    if (a == 0) return;
    isPlayer = (a->actorID == 0xbf);
    if (!isPlayer) return;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xa8, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xa9, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xaa, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xab, self->mPosX, self->mPosY, self->mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xac, self->mPosX, self->mPosY, self->mPosZ);
    func_02012694(0x2f, &self->mCamSpacePosX);
    quakePos.x = self->mPosX;
    quakePos.y = self->mPosY;
    quakePos.z = self->mPosZ;
    _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(self, &quakePos, 0x7d0000);
    hurtPos.x = self->mPosX;
    hurtPos.y = self->mPosY;
    hurtPos.z = self->mPosZ;
    _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(a, &hurtPos, 2, 0xc000, 1, 0, 1);
    func_ov060_021184bc(self);
}
}

// @symbol _ZN9daKirai_c16CleanupResourcesEv
int daKirai_c::CleanupResources()
{
    ((SharedFilePtr *)&data_ov060_0211b1c4)->Release();
    return 1;
}

// @symbol _ZN9daKirai_c6RenderEv
int daKirai_c::Render()
{
    if (mStateIndex != 0) return 1;
    if (mOpacity < 8) return 1;
    mModel.Render(0);
    return 1;
}

// @symbol _ZN9daKirai_c8BehaviorEv
int daKirai_c::Behavior()
{
    (this->*((KiraiState *)data_ov060_0211b1d8)[mStateIndex])();
    func_ov060_02118690(this);
    mdCcAcPos_c.Clear();
    Vector3 v;
    v.x = 0;
    v.y = -0x96000;
    v.z = 0;
    mdCcAcPos_c.SetPosRelativeToActor(v);
    mdCcAcPos_c.Update();
    return 1;
}

// @symbol _ZN9daKirai_c13InitResourcesEv
int daKirai_c::InitResources()
{
    Vector3 v;
    Vector3 z;

    mModel.SetFile((BMD_File *)Model::LoadFile(*(SharedFilePtr *)&data_ov060_0211b1c4), 1, -1);
    v.x = 0;
    v.y = -0x96000;
    v.z = 0;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
        &mdCcAcPos_c, this, v, 0x96000, 0x12c000, 0x204004, 0);
    mScaleX = 0x1000;
    mScaleY = 0x1000;
    mScaleZ = 0x1000;
    mOpacity = 0xff;
    z.x = 0;
    z.y = 0;
    z.z = 0;
    Vec3_HorzAngle(&z, (const Vector3 *)&mPosX);
    mHomeYOffset = 0x2ee000;
    mHomePosX = mPosX;
    mHomePosY = mPosY;
    mHomePosZ = mPosZ;
    mHomePosY += mHomeYOffset >> 3;
    mHomeHorzDist = Vec3_HorzLen((const Vector3 *)&mPosX);
    mStateIndex = 0;
    mSlotIndex = AddSpikeBomb(this);
    return 1;
}
