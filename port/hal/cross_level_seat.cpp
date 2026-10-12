/* THE PER-LOADED-LEVEL CROSS-POINTER SEAT (run hunt4, lane CROSSPATCH1).
 *
 * THE JOB. port/tools/ovdata.py's --cross pass rebases every pointer that
 * leaves its own mount onto the host copy of its target, except one class it
 * must drop at build time: a target inside a DS window that several mounted
 * overlays share. Every level overlay is linked at 0x021111a0 and the object
 * overlays stack at fixed addresses above it, so a word such as ov073's
 * 0x0211153c (a CLPS collision-property table in Wario's key arena, ov057)
 * or ov102's 0x021136f4 (one in Whomp's Fortress, ov015) names whichever
 * overlay is loaded there. The pass left those words holding the raw DS
 * address, and its own comment asked for "a seat that re-patches per loaded
 * level". This is that seat.
 *
 * WHAT A RAW WORD COST. On an ordinary PC ntr/io.cpp reserves DS main RAM as
 * zeroed pages, so the read succeeds and returns zeros: dBgW_Kc::GetSurfaceInfo
 * (func_020381cc) takes the default surface where the cartridge reads a real
 * CLPS row -- a quiet difference in surface type, sound and slipperiness. On a
 * PC where something else already holds 0x02000000 (one player's machine does,
 * every launch: "[io] LOST 02000000..02400000") the same read faults: levels
 * 48 / 49 and the ending's Whomp's Fortress scene.
 *
 * WHAT THE RIGHT VALUE IS. On the DS the word always holds its ROM address and
 * the read lands on whatever is loaded there. So the faithful host value is the
 * host address of THE SAME BYTE in the copy of the overlay that is RESIDENT for
 * the loaded level, and which overlays are resident is the ROM's own answer:
 *
 *   LoadLevelOverlays(level)  -> data_020758c8[level], the level overlay
 *   LoadOrUnloadObjectOverlays(fn, level)
 *                             -> that level's object overlays out of
 *                                data_02075998 / data_02075804, then ov060 on
 *                                levels 0x24/0x26/0x28, else ov098 (not in VS)
 *                                and ov102
 *
 * The second is the linked matched TU (src/_Z26LoadOrUnloadObjectOverlaysPFviEi
 * .cpp, port/slice_gate213.txt), CALLED here with a collecting callback, so the
 * resident set is the cartridge's code over the cartridge's tables and not a
 * restatement of them. ov001 (resident from boot) and ov002 (the in-level scene
 * the level and object overlays stack on, see ovdata.py's Residency rule 3)
 * are always resident while a level is.
 *
 * Per contested word, from the generated table in ov_cross.c:
 *   its own overlay not resident          -> the raw ROM address (nothing
 *                                            loaded can read it; this is the
 *                                            value a fresh load would carry)
 *   exactly one resident candidate copy   -> the host address of that byte
 *   none                                  -> the raw ROM address, as before
 *   more than one                         -> the raw ROM address, and a line
 *                                            (the ROM tables never load two
 *                                            overlapping overlays, so this
 *                                            would mean the table is wrong)
 * A target inside a candidate's .text is never a candidate (ovdata.py's
 * level_seat_table): DS code bytes are no host answer to a pointer.
 *
 * NOT OWNED HERE. A word an existing hand seat in port/hal rewrites stays that
 * seat's: the generator scans hal/*.cpp for seat rows and leaves those words
 * out of the table (ovdata.py hal_seated). Today that is hal/ttc_level_data_seat
 * .cpp's eight ov065 words, hal/actor_classes_ov074.cpp's twenty ov074 words
 * (a seat that runs once and aborts unless the word still holds its ROM
 * address, so binding them first on level 45 killed the boot) and
 * hal/intro_ov002_seat.cpp's three ov002 words, plus ov089's six key-model
 * words (port_ov089_keymodels_fixup, ovdata.py's HAND_SEATED).
 * The STATIC_ROCK CLPS word data_ov102_0214e190+0 IS in the table: the
 * one-directional seat in hal/level_boot.cpp writes port_ov016_at() on the
 * first Jolly Roger Bay mount and never releases it; this seat runs after it
 * on every mount and accepts that value as a known alternate, so on Jolly Roger
 * Bay the word keeps naming ov016's bytes and elsewhere it follows the loaded
 * level like its neighbours.
 *
 * NO GUARD, ON PURPOSE, the TTC seat's argument: the words are .dsstate (every
 * mounted byte is), the value written is a pure function of the level id, and
 * the seat accepts the ROM address or any host copy it could itself have
 * written. A save-state restore that rolls the words back is answered by the
 * next mount; nothing host-side describes them a second time.
 *
 * CALLED FROM port_level_mount_at() on both paths (patch and cache hit), after
 * port_ttc_level_data_seat, so like that seat the SECOND mount call of a level
 * change (the incoming level) is the one that decides.
 *
 * SM64DS_TRACE_XLSEAT=1 prints one line per word per mount with the value
 * written and, for a bound CLPS table, the magic and first row it now reads.
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
extern const unsigned port_xl_want_count;
extern const unsigned port_xl_cand_count;
extern unsigned char *const port_xl_site[];
extern const unsigned port_xl_rom[];
extern const unsigned short port_xl_owner[];
extern const unsigned short port_xl_cand0[];
extern const unsigned short port_xl_ncand[];
extern const unsigned short port_xl_cand_ov[];
extern unsigned char *const port_xl_cand_host[];
extern unsigned char *const port_xl_cand_alt[];

extern int data_020758c8[];                 /* level -> level overlay id */
extern int overlay_60, overlay_98, overlay_102;   /* hal/scene_boot.cpp */

void port_cross_level_seat(int level_id);
}

/* The ROM's own object-overlay walk (C++ linkage, the matched TU). */
void LoadOrUnloadObjectOverlays(void (*fn)(int), int idx);

namespace {

const int LEVEL_COUNT = 52;     /* data_020758c8 / data_02075998 rows */
const int OV_LIMIT = 128;

unsigned char g_resident[OV_LIMIT];
int g_bad_id;

/* The DS passes the overlay id; the three the TU names by symbol arrive as
   host addresses of hal/scene_boot.cpp's placeholder ints (the ADDRESS is the
   id on the DS), so they are mapped back here. */
void collect(int id)
{
    if (id == (int)(size_t)&overlay_60)
        id = 60;
    else if (id == (int)(size_t)&overlay_98)
        id = 98;
    else if (id == (int)(size_t)&overlay_102)
        id = 102;
    if (id >= 0 && id < OV_LIMIT)
        g_resident[id] = 1;
    else
        g_bad_id = id;
}

unsigned rd32(const unsigned char *p)
{
    return (unsigned)p[0] | ((unsigned)p[1] << 8) |
           ((unsigned)p[2] << 16) | ((unsigned)p[3] << 24);
}

void wr32(unsigned char *p, unsigned v)
{
    p[0] = (unsigned char)(v & 0xff);
    p[1] = (unsigned char)((v >> 8) & 0xff);
    p[2] = (unsigned char)((v >> 16) & 0xff);
    p[3] = (unsigned char)((v >> 24) & 0xff);
}

int trace_on(void)
{
    static int on = -1;
    if (on < 0)
        on = std::getenv("SM64DS_TRACE_XLSEAT") != 0;
    return on;
}

}  /* namespace */

extern "C" void port_cross_level_seat(int level_id)
{
    std::memset(g_resident, 0, sizeof g_resident);
    g_bad_id = -1;
    const bool in_range = level_id >= 0 && level_id < LEVEL_COUNT;
    if (in_range) {
        g_resident[1] = 1;
        g_resident[2] = 1;
        int lov = data_020758c8[level_id];
        if (lov >= 0 && lov < OV_LIMIT)
            g_resident[lov] = 1;
        LoadOrUnloadObjectOverlays(collect, level_id);
    }
    if (g_bad_id != -1)
        std::fprintf(stderr, "  [xl-seat] level %d: the object-overlay walk "
                     "passed id %d, outside 0..%d -- ignored\n", level_id,
                     g_bad_id, OV_LIMIT - 1);

    int bound = 0, absent = 0, nocopy = 0, twice = 0, wrote = 0, odd = 0;
    for (unsigned i = 0; i < port_xl_want_count; ++i) {
        unsigned char *site = port_xl_site[i];
        const unsigned rom = port_xl_rom[i];
        const unsigned c0 = port_xl_cand0[i], cn = port_xl_ncand[i];
        unsigned want = rom;
        int pick = -1, hits = 0;
        const char *why;
        if (!g_resident[port_xl_owner[i]]) {
            ++absent;
            why = "owner not loaded";
        } else {
            for (unsigned j = c0; j < c0 + cn; ++j)
                if (g_resident[port_xl_cand_ov[j]]) {
                    pick = (int)j;
                    ++hits;
                }
            if (hits == 1) {
                want = (unsigned)(size_t)port_xl_cand_host[pick];
                ++bound;
                why = "bound";
            } else if (hits == 0) {
                ++nocopy;
                why = "no resident copy";
            } else {
                ++twice;
                pick = -1;
                why = "TWO resident copies, left raw";
            }
        }
        /* What this word may hold before the write: its ROM address or a copy
           this seat (or the STATIC_ROCK seat, the alternate) could have
           written. Anything else is another writer nobody predicted. It is
           reported, not fatal: the value below is a pure function of the
           level and replaces it either way. */
        const unsigned have = rd32(site);
        bool known = have == rom;
        for (unsigned j = c0; !known && j < c0 + cn; ++j)
            known = have == (unsigned)(size_t)port_xl_cand_host[j] ||
                    (port_xl_cand_alt[j] &&
                     have == (unsigned)(size_t)port_xl_cand_alt[j]);
        if (!known) {
            ++odd;
            std::fprintf(stderr, "  [xl-seat] level %d: word %u (rom %08x) "
                         "held %08x, not the ROM address or a copy of it\n",
                         level_id, i, rom, have);
        }
        /* The STATIC_ROCK alternate: already naming the whole image of the
           one resident candidate is the same bytes; keep it. */
        if (pick >= 0 && port_xl_cand_alt[pick] &&
            have == (unsigned)(size_t)port_xl_cand_alt[pick])
            want = have;
        if (have != want) {
            wr32(site, want);
            ++wrote;
        }
        if (trace_on()) {
            char what[64] = "";
            if (pick >= 0) {
                const unsigned char *p = (const unsigned char *)(size_t)want;
                if (rd32(p) == 0x53504c43u)            /* 'CLPS' */
                    std::snprintf(what, sizeof what, " CLPS n=%u row0=%08x:%08x",
                                  rd32(p + 4) >> 16, rd32(p + 8), rd32(p + 12));
                else
                    std::snprintf(what, sizeof what, " reads %08x %08x",
                                  rd32(p), rd32(p + 4));
            }
            std::fprintf(stderr, "  [xl-seat] level %d word %u rom %08x owner "
                         "ov%03u -> %08x (%s, copy ov%03u)%s\n", level_id, i,
                         rom, (unsigned)port_xl_owner[i], want, why,
                         pick >= 0 ? (unsigned)port_xl_cand_ov[pick] : 0u,
                         what);
        }
    }
    std::fprintf(stderr, "  [xl-seat] level %d: %u contested cross words, %d "
                 "bound to the resident copy, %d owner not loaded, %d no "
                 "resident copy, %d doubly covered (%d written)%s\n",
                 level_id, port_xl_want_count, bound, absent, nocopy, twice,
                 wrote, odd ? ", UNEXPECTED VALUES SEEN" : "");
}
