//cpp
/**
 * dScDSMT_c -- the DS Multi-Play (download-play host) scene, ov007. TEN of
 * the class's twenty functions; see PARTIAL FOLD below for why the other
 * ten stay in their own files.
 *
 * The scene pumps ov007's session state machine every frame, answers the
 * command codes the client sends (sound mode, backlight, erase/copy save
 * files, minigame records) and leaves for a save file, the minigame
 * overlays or the minigame menu when the session ends. What is written
 * here is the destructor pair, the nested graphCallback_c (which hands the
 * sub screen to ov007's own draw routine), Render, the empty
 * OnPendingDestroy, and the four free helpers that keep the per-file
 * summary the client displays. .text 0x020cc028..0x020cc2cc, one object,
 * byte-identical to retail through the real link.
 *
 * PARTIAL FOLD, and exactly where the line falls. The class occupies one
 * contiguous linker run, 0x020cc028..0x020ccb54, twenty functions:
 * tu_map's two units 0x020cc028..0x020cc600 and 0x020cc600..0x020ccb54 are
 * one TU -- the command dispatcher func_ov007_020cc600 calls this file's
 * func_ov007_020cc0cc/0e4/118/168, and the factory dScDSMT_c_classInit at
 * the run's end installs this class's vtable. All twenty were written as
 * one file and byte-match 20/20 (tools/tubuild.py verify, objisolate and
 * relocation destinations clean, ROM-ascending emission with the
 * destructor inline in the class body). They do not LINK:
 *
 * - Behavior (0x020cc2cc) takes (int)&overlay_64 / &overlay_66 and
 *   InitResources (0x020cc4c0) takes &overlay_100 / &overlay_102 -- the
 *   NitroSDK FS_OVERLAY_ID idiom, an absolute linker symbol whose value is
 *   the overlay number. The cartridge's pool words there are 0x40/0x42 and
 *   0x64/0x66. Writing the numbers as literals is not the same source: 64
 *   and 100 are immediate-encodable, so a literal compiles to `mov`, not
 *   the pool load the cartridge has (measured: InitResources with literal
 *   ids builds 0x138 bytes against the cartridge's 0x140). Nothing in the tree's link defines an
 *   overlay_N symbol (overlay_64/66 have no symbols.txt row at all, and
 *   overlay_100/102 are two of the nine pre-existing `dsd check symbols`
 *   errors), so mwldarm reports all four Undefined -- measured with both
 *   shards marked complete on a stock tree, before any of this file. Every
 *   other overlay_N user in the tree (func_0201a2f8, func_0201a798,
 *   func_02034fbc, ...) is unenrolled for the same reason.
 * - Both sit in the MIDDLE of the run, and a delinks entry carries exactly
 *   one .text range, so the licensed range cannot skip them. Below
 *   Behavior: ten functions (this file). Above: CleanupResources alone,
 *   then InitResources, then seven more. This file is the larger side.
 *
 * Still in their own files: Behavior, CleanupResources, InitResources,
 * func_ov007_020cc600, the five wrappers func_ov007_020cca68..020ccab4, and
 * the factory dScDSMT_c_classInit (src/d_s_dsmt.c).
 *
 * DESTRUCTOR FORM. The cartridge places D1 (0x020cc028) BELOW D0
 * (0x020cc070). With the destructor inline in the class body (the
 * dScTitle_c form) mwccarm emits D1, D0 in that order, but only from the TU
 * that defines the key function InitResources -- which cannot come along.
 * So the header keeps the out-of-line declaration, the destructor is the
 * key function and is defined here, and `#pragma defer_codegen off` makes
 * mwccarm emit D1, D0 and then a base-object D2 the cartridge never carried
 * (licensed as a deadstrip). defer_codegen off also turns emission order
 * ROM-ascending for the whole file, so this file is written in ROM order.
 * Do not reorder.
 *
 * deslop leftovers:
 * - The four func_ov007_* helpers are written free, and those names are
 *   address-derived analysis labels, not recovered spellings.
 * - data_0209b33c (the three-file buffer), data_0209b340 (the summary block
 *   ov007's session library reads; decl_common.h spells it int[], and
 *   func_ov007_020cc168 writes it bytewise) and its byte views
 *   data_0209b34b/4e/3d8 are arm9 .bss this TU does not own.
 * - GetSoundMode, IsStarCollected and func_ov007_020b6eb4 have no header.
 */

#include "dScDSMT_c.h"
#include "SaveData.h"
#include "Sound.h"
#include "decl_common.h"

/* One save file, the 0x44-byte unit SaveData::ReadFileData and
 * SaveData::EraseSaveFile move; include/SaveData.h only forward-declares
 * it. Layout as src/func_02013c84.c spells it. */
struct FileSaveData {
    u32 magic8000;
    u32 flags1;
    u32 flags2;
    u32 minigameRabbits;
    u32 cannonUnlocked;
    u8  stars[30];
    u8  coinRecords[15];
    u8  currentCharacter;
    u8  controllerMode;
    u8  unk43;
};

extern "C" {
/* All three save files, one buffer, which func_ov007_020cc600 allocates
 * and CleanupResources frees. */
extern FileSaveData *data_0209b33c;
extern u8 data_0209b34b[];
extern u8 data_0209b34e[];
extern u8 data_0209b3d8[];
extern u8 data_0208ee3c[];
extern int data_0209caa0[];

void func_ov007_020b6eb4(void *callback);
int GetSoundMode(void);
int IsStarCollected(int course, int star);
}

#pragma defer_codegen off

/* The key function: defining the destructor out of line here emits the
 * vtable. The empty body reproduces D1 and D0 -- the inlined vptr stores,
 * the fader member's ~dFdDummy_c at +0x54, fBase_c::~fBase_c, and D0's
 * operator delete. */
// @symbol _ZN9dScDSMT_cD1Ev
dScDSMT_c::~dScDSMT_c()
{
}

// @symbol func_ov007_020cc0cc
extern "C" FileSaveData *func_ov007_020cc0cc(int idx)
{
    return &data_0209b33c[idx];
}

// @symbol func_ov007_020cc0e4
extern "C" void func_ov007_020cc0e4(FileSaveData *files)
{
    data_0209b33c = files;
}

/* dGraph_c::callback_c slot 2: hands this callback to ov007's sub-screen
 * draw routine and declines the slot. */
// @symbol _ZN9dScDSMT_c15graphCallback_c14GraphCallback2Ev
int dScDSMT_c::graphCallback_c::GraphCallback2()
{
    func_ov007_020b6eb4(this);
    return 0;
}

/* dGraph_c::callback_c slot 0: declines, against the base's default of 1. */
// @symbol _ZN9dScDSMT_c15graphCallback_c14GraphCallback0Ev
int dScDSMT_c::graphCallback_c::GraphCallback0()
{
    return 0;
}

/* Queues a music cue after a delay unless one is already pending. */
// @symbol func_ov007_020cc118
extern "C" int func_ov007_020cc118(int musicID, unsigned int delay)
{
    if (data_ov007_02103260 >= 0) {
        return 0;
    }
    data_ov007_02103260 = musicID;
    data_ov007_02104c28 = (unsigned short)delay;
    Sound::StopLoadedMusic_Layer1(delay);
    return 1;
}

/* Fills the download-play summary of save file idx: sound mode, the
 * current character's unlock state, per-course stars and coin records,
 * and the castle star count. */
// @symbol func_ov007_020cc168
extern "C" void func_ov007_020cc168(u32 idx)
{
    int chr;
    int i;
    int j;
    u8 *rec;

    if (GetSoundMode() == 0) {
        ((u8 *)data_0209b340)[8] = 1;
    } else if (GetSoundMode() == 1) {
        ((u8 *)data_0209b340)[8] = 2;
    } else if (GetSoundMode() == 2) {
        ((u8 *)data_0209b340)[8] = 0;
    }

    ((u8 *)data_0209b340)[9] = data_0208ee3c[0];
    data_0209b34b[idx] = (data_0209caa0[1] & 1) ? 1 : 0;

    chr = 3;
    i = 2;
    do {
        if (SaveData::IsCharacterUnlocked((u32)i)) break;
        chr--;
        i--;
    } while (i >= 0);
    data_0209b34e[idx] = chr;

    rec = &((u8 *)data_0209b340)[idx * 0xf];
    for (j = 0; j < 0xf; j++) {
        rec[0x6b] = IsStarCollected(j, 0) ? 1 : 0;
        rec[0x11] = CountStarsCollectedInLevelToDisplay(j) - rec[0x6b];
        rec[0x3e] = SaveData::GetCoinRecord((u32)j);
        rec++;
    }

    data_0209b3d8[idx] = CountStarsCollectedInLevelToDisplay(0x1d);
}

/* vtable slot 12. */
// @symbol _ZN9dScDSMT_c16OnPendingDestroyEv
void dScDSMT_c::OnPendingDestroy()
{
}

/* vtable slot 9. */
// @symbol _ZN9dScDSMT_c6RenderEv
s32 dScDSMT_c::Render()
{
    func_ov007_020b7040(this);
    return 1;
}
