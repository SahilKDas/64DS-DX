//cpp
/* Production translation unit for ov064/daWater_Ring_c.
 * 15 function(s), .text 0x02119a58..0x0211a200. The last is the registry
 * factory daWater_Ring_c_classInit (0x0211a1b0), `new daWater_Ring_c()`.
 * One out-of-line destructor emits D1 then D0;
 * `#pragma defer_codegen off` lays .text down in source order.
 *
 * The seven func_ov064_* helpers are the state bodies daWater_Ring_c::State
 * points at, the state installer, and the per-frame matrix update. Their ROM
 * names are unrecovered, so they stay extern "C" free functions taking the
 * ring explicitly; a member spelling would mangle to a symbol that does not
 * exist.
 */

#pragma defer_codegen off

#include "common.h"
#include "daWater_Ring_c.h"
#include "SharedFilePtr.h"
#include "TextureTransformer.h"
#include "Player.h"

extern "C" {
/* decl_common.h types both state tables as plain data; they are cast where
   they are installed. */
extern int data_ov064_0211c944;
extern char data_ov064_0211c954[];
extern Vector3 data_ov064_0211c3d0;         /* the collider offset */
extern s16 data_02082214[];                 /* sine/cosine table */
extern char data_ov002_0210d6dc[];          /* the ring's BTA */
extern Matrix4x3 data_020a0e68;             /* the shared scratch matrix */

s16 Vec3_VertAngle(const Vector3 *v1, const Vector3 *v0);
int AngleDiff(int a, int b);
unsigned short DecIfAbove0_Short(unsigned short *p);
void Vec3_Asr(struct Vector3 *d, struct Vector3 *s, int sh);
void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
void Matrix4x3_ApplyInPlaceToRotationXYZExt(void *m, int x, int y, int z);

/* local extern: the header spellings take Fix12<int> by value, and Fix12
   has no int constructor, so a call through them cannot be written with
   these literals. */
void _ZN18TextureTransformer7SetFileER8BTA_Filei5Fix12IiEj(TextureTransformer *self, BTA_File *animFile, int flags, int speed, unsigned short startFrame);
void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(dCcAcPos_c *self, dActor_c *actor, const Vector3 *offset, int radius, int height, u32 flags, u32 vulnFlags);

int func_ov064_02119ecc(daWater_Ring_c *self, const daWater_Ring_c::State *state);
}

extern SharedFilePtr data_ov002_0210da10;   /* the ring's BMD */

void ApproachLinear(int &value, int target, int step);

// @symbol _ZN14daWater_Ring_cD1Ev
// @symbol _ZN14daWater_Ring_cD0Ev
daWater_Ring_c::~daWater_Ring_c()
{
}

/* Checks whether the player swam through the ring this frame. */
// @symbol func_ov064_02119afc
extern "C" void func_ov064_02119afc(daWater_Ring_c *self)
{
    Vector3 otherPos;
    Vector3 offset;
    dActor_c *other;
    int isPlayer;
    u32 id;

    self->mTextureTransformer.speed = 0x1000;
    offset = data_ov064_0211c3d0;
    self->mdCcAcPos_c.SetPosRelativeToActor(offset);
    id = self->mdCcAcPos_c.otherOwner;
    if (id == 0) return;
    other = dActor_c::FindWithID(id);
    if (other == 0) return;
    isPlayer = (other->actorID == 0xbf);
    if (isPlayer == 0) return;
    {
        const Vector3 &pos = *(const Vector3 *)&other->mPosX;
        otherPos.x = pos.x;
        otherPos.y = pos.y;
        otherPos.z = pos.z;
    }
    if (AngleDiff(self->mAngleX, Vec3_VertAngle((Vector3 *)&self->mPosX, &otherPos)) >= 0x3000)
        return;
    if ((self->unk_37c == 1 && ((self->unk_388 >> 16) & 1) != ((self->HorzAngleToCPlayer() >> 16) & 1))
        || self->unk_37c != 1) {
        if (self->unk_37c == 1)
            ((Player *)other)->Heal(0x100);
        self->mTextureTransformer.speed = 0x4000;
        func_ov064_02119ecc(self, (const daWater_Ring_c::State *)&data_ov064_0211c944);
    } else {
        self->unk_388 = self->HorzAngleToCPlayer();
    }
}

/* Fading out: grows and fades until it is gone. */
// @symbol func_ov064_02119c60
extern "C" int func_ov064_02119c60(daWater_Ring_c *self)
{
    self->unk_380 -= 2;
    if (self->unk_380 < 2)
        self->unk_380 = 2;
    ApproachLinear(self->mScaleX, 0x3000, 0x199);
    self->mScaleZ = self->mScaleX;
    self->mScaleY = self->mScaleZ;
    if ((u16)self->mStateTimer == 0 || self->mScaleX >= 0x2ffd)
        self->MarkForDestruction();
    return 1;
}

/* Init of the passed state: tells the spawner which ring was hit. */
// @symbol func_ov064_02119ce4
extern "C" int func_ov064_02119ce4(daWater_Ring_c *self) {
    self->unk_380 = 0x1f;
    self->mStateTimer = 0x64;
    int type = self->unk_37c;
    if (type == 1 || type == 2) {
        char *spawner = self->unk_38c;
        if (spawner != 0) {
            /* The spawner's "ring that was passed" slot; the two spawning
               classes keep it at different offsets. */
            if (type == 1) {
                *(daWater_Ring_c **)(spawner + 0x3a8) = self;
            } else {
                *(daWater_Ring_c **)(spawner + 0x31c) = self;
            }
        }
    }
    return 1;
}

/* Main of the active state: wobbles, spins and waits for the player. */
// @symbol func_ov064_02119d28
extern "C" int func_ov064_02119d28(daWater_Ring_c *self) {
    int spd;
    func_ov064_02119afc(self);
    if ((u16)self->mStateTimer < 0x1e) {
        if (self->unk_380 >= 2)
            self->unk_380--;
    }
    spd = (self->unk_37c == 1) ? 0x333 : 0x199;
    self->unk_374 += 0x1000;
    {
        s16 phase = self->unk_374;
        int idx = ((u16)phase >> 4) * 2;
        int wobble = (int)((((s64)data_02082214[idx] << 10) + 0x800) >> 12);
        int target = wobble + self->unk_384;
        ApproachLinear(self->unk_384, 0x2000, spd);
        ApproachLinear(self->mScaleX, target, spd);
    }
    self->unk_378 += 0x800;
    {
        s16 phase = self->unk_378;
        int idx = ((u16)phase >> 4) * 2;
        self->mPrevAngleX += (int)((((s64)data_02082214[idx] << 8) + 0x800) >> 12);
    }
    self->mScaleZ = self->mScaleX;
    self->mScaleY = self->mScaleZ;
    if ((u16)self->mStateTimer == 0 || self->unk_380 <= 1)
        self->MarkForDestruction();
    return 1;
}

/* Init of the active state. */
// @symbol func_ov064_02119ea0
extern "C" s32 func_ov064_02119ea0(daWater_Ring_c *self) {
    self->mStateTimer = 0xc8;
    s32 angle = self->HorzAngleToCPlayer();
    self->unk_388 = (s16)angle;
    return 1;
}

/* Installs a state and runs its init. */
// @symbol func_ov064_02119ecc
extern "C" int func_ov064_02119ecc(daWater_Ring_c *self, const daWater_Ring_c::State *state)
{
    self->mState = state;
    const daWater_Ring_c::State *installed = self->mState;
    if (installed->init == 0)
        return 1;
    return (self->*installed->init)();
}

/* Poses the model: position, rotation and opacity. */
// @symbol func_ov064_02119f1c
extern "C" void func_ov064_02119f1c(daWater_Ring_c *self) {
    Vector3 v;
    Vec3_Asr(&v, (Vector3 *)&self->mPosX, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68, self->mAngleX, self->mAngleY, self->mAngleZ);
    self->mModel.ApplyOpacity(self->unk_380, 1);
    self->mModel.mat4x3 = data_020a0e68;
}

// @symbol _ZN14daWater_Ring_c16CleanupResourcesEv
int daWater_Ring_c::CleanupResources()
{
    data_ov002_0210da10.Release();
    return 1;
}

// @symbol _ZN14daWater_Ring_c16OnPendingDestroyEv
void daWater_Ring_c::OnPendingDestroy()
{
}

// @symbol _ZN14daWater_Ring_c6RenderEv
int daWater_Ring_c::Render()
{
    mTextureTransformer.Update(mModel.data);
    mModel.Render((const Vector3 *)&mScaleX);
    return 1;
}

// @symbol _ZN14daWater_Ring_c8BehaviorEv
int daWater_Ring_c::Behavior()
{
    DecIfAbove0_Short((unsigned short *)&mStateTimer);
    if (mState->execute)
        (this->*mState->execute)();
    UpdatePos(&mdCcAcPos_c);
    mAngleX = mPrevAngleX;
    mAngleY = mPrevAngleY;
    mAngleZ = mPrevAngleZ;
    func_ov064_02119f1c(this);
    mdCcAcPos_c.Clear();
    mdCcAcPos_c.Update();
    mTextureTransformer.Advance();
    return 1;
}

// @symbol _ZN14daWater_Ring_c13InitResourcesEv
int daWater_Ring_c::InitResources()
{
    BMD_File *f = (BMD_File *)Model::LoadFile(data_ov002_0210da10);
    if (mModel.SetFile(f, 1, -1) == 0)
        return 0;

    unk_37c = param1 & 0xff;
    if (unk_37c > 2 || unk_37c == 0xff)
    {
        unk_37c = 0;
    }

    TextureTransformer::Prepare(*((BMD_File **)&data_ov002_0210da10)[1], *(BTA_File *)data_ov002_0210d6dc);
    _ZN18TextureTransformer7SetFileER8BTA_Filei5Fix12IiEj(&mTextureTransformer, (BTA_File *)data_ov002_0210d6dc, 0, 0x1000, 0);

    mTextureTransformer.speed = 0x1000;
    mScaleX = 0x1000;
    mScaleY = 0x1000;
    mScaleZ = 0x1000;

    Vector3 v;
    v.x = data_ov064_0211c3d0.x;
    v.y = data_ov064_0211c3d0.y;
    v.z = data_ov064_0211c3d0.z;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(&mdCcAcPos_c, this, &v, 0x88000, 0xe8000, 0x800006, 0);

    unk_380 = 0x1f;
    func_ov064_02119ecc(this, (const daWater_Ring_c::State *)data_ov064_0211c954);
    return 1;
}

/* Reconstructed source-style name: SM64DS proves daWater_Ring_c through
 * RTTI, allocation size, vtable identity, and the WATER_RING registry
 * profile; later EAD lineage supplies classInit. Exact original spelling is
 * not preserved. Historical alias: WaterRing_Spawn. */
// @symbol daWater_Ring_c_classInit
extern "C" daWater_Ring_c *daWater_Ring_c_classInit()
{
    return new daWater_Ring_c();
}
