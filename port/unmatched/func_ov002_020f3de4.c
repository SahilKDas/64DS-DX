/* PORT-UNMATCHED character-icon state 8 of the ending's cast screen
 * (func_ov002_020f3de4, 0x318 bytes; data_ov002_02110eec[8], the last of the
 * nine per-slot states func_ov002_020f562c dispatches for CUTSCENE_OBJECT's
 * four 0x4c-byte icon records). Adopted from the banked near-miss draft
 * (nearmiss/db.jsonl, source log_attempt, 44 codegen divergences vs ROM,
 * floor class "ordering"). Compiled with the pinned mwccarm 2004/b56 it emits
 * the ROM's 198 instructions in the ROM's order with the same opcodes,
 * immediates, branch shape, calls (cstd::atan2 at 0x020f3f48,
 * func_ov002_020f5a94 at 0x020f4064) and pool words (data_02082214,
 * data_ov002_021000c0); the 44 divergences are register choice in the
 * timer prologue and the orbit block plus one swapped pair of independent
 * instructions (the pool load and the +0x10 base at 0x020f3fc8). Semantics
 * only; retires when the function matches for real.
 *
 * The twin of src/func_ov002_020f43cc.c (state 6) and src/func_ov002_020f5010.c
 * (state 2): burn the hold timer (+0x30); until arrival (+0x48) step along the
 * heading (+0x2e) at speed (+8), bleed speed above 0x2000, snap onto the
 * target (+0x1c/+0x20) within 2 units or turn toward it 0x100 a step; once
 * arrived, orbit (0x80000,0x60000) at radius +0x10. When the last unlocked
 * character's slot has orbited past 0x10000, every live slot (+0x44) goes
 * back to state 1 with its heading from data_ov002_021000c0[j]. */

extern short _ZN4cstd5atan2E5Fix12IiES1_(int y, int x);
extern int func_ov002_020f5a94(void);
extern short data_02082214[];
extern unsigned short data_ov002_021000c0[];

void func_ov002_020f3de4(char *c, int i)
{
    int off = i * 0x4c;
    int idx, dx, dy;
    unsigned short ang;
    unsigned short target;
    unsigned short v;
    int j;

    v = *(unsigned short *)(c + 0x30 + off);
    if (v != 0) {
        *(unsigned short *)(c + 0x30 + off) = v - 1;
        return;
    }

    if (*(unsigned char *)(c + 0x48 + off) == 0) {
        idx = *(unsigned short *)(c + 0x2e + off) >> 4;
        {
            short tv = data_02082214[idx * 2 + 1];
            int spd = *(int *)(c + 8 + off);
            *(int *)(c + 0 + off) += (int)(((long long)tv * spd + 0x800) >> 12);
        }

        idx = *(unsigned short *)(c + 0x2e + off) >> 4;
        {
            short tv = data_02082214[idx * 2];
            int spd = *(int *)(c + 8 + off);
            *(int *)(c + 4 + off) += (int)(((long long)tv * spd + 0x800) >> 12);
        }

        if (*(int *)(c + 8 + off) >= 0x2000)
            *(int *)(c + 8 + off) -= 0x120;

        dx = (*(int *)(c + 0x1c + off) - *(int *)(c + 0 + off)) >> 12;
        dy = (*(int *)(c + 0x20 + off) - *(int *)(c + 4 + off)) >> 12;

        if (dx >= -2 && dx <= 2 && dy >= -2 && dy <= 2) {
            *(unsigned char *)(c + 0x48 + off) += 1;
            *(int *)(c + 0x24 + off) = 0;
            *(unsigned short *)(c + 0x2e + off) = 0xc000;
            return;
        }

        target = _ZN4cstd5atan2E5Fix12IiES1_(dy, dx);
        ang = *(unsigned short *)(c + 0x2e + off);
        if (target > ang) {
            *(unsigned short *)(c + 0x2e + off) += 0x100;
            if (target <= *(unsigned short *)(c + 0x2e + off))
                *(unsigned short *)(c + 0x2e + off) = target;
        } else {
            if (ang <= target)
                return;
            *(unsigned short *)(c + 0x2e + off) -= 0x100;
            if (*(unsigned short *)(c + 0x2e + off) <= target)
                *(unsigned short *)(c + 0x2e + off) = target;
        }
        return;
    }

    idx = *(unsigned short *)(c + 0x2e + off) >> 4;
    {
        short tv = data_02082214[idx * 2 + 1];
        int spd = *(int *)(c + 0x10 + off);
        *(int *)(c + 0 + off) = (int)(((long long)tv * spd + 0x800) >> 12) + 0x80000;
    }
    idx = *(unsigned short *)(c + 0x2e + off) >> 4;
    {
        short tv = data_02082214[idx * 2];
        int spd = *(int *)(c + 0x10 + off);
        *(int *)(c + 4 + off) = (int)(((long long)tv * spd + 0x800) >> 12) + 0x60000;
    }
    *(unsigned short *)(c + 0x2e + off) += *(unsigned short *)(c + 0x42 + off);
    *(int *)(c + 0x24 + off) += *(unsigned short *)(c + 0x42 + off);

    if (i != func_ov002_020f5a94() - 1)
        return;
    if ((unsigned int)*(int *)(c + 0x24) < 0x10000)
        return;

    for (j = 0; j < 4; j++) {
        if (*(unsigned char *)(c + 0x44) != 0) {
            *(unsigned char *)(c + 0x47) = 1;
            *(unsigned short *)(c + 0x3c) = 0;
            *(unsigned char *)(c + 0x48) = 0;
            *(int *)(c + 0x10) = 0x38000;
            *(unsigned short *)(c + 0x2e) = data_ov002_021000c0[j];
            *(int *)(c + 0x24) = 0;
            *(unsigned short *)(c + 0x42) = 0x200;
        }
        c += 0x4c;
    }
}
