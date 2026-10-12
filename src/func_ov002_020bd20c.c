/* func_ov002_020bd20c at 0x020bd20c
 *
 * Matched byte-for-byte with mwccarm 1.2/sp2p3 (ov002).
 */

#ifdef _MSC_VER
/* THE NEW STATE IS WIDENED AT THE CALL, AS THE ROM DOES IT. func_ov002_020bd06c
   (src/func_ov002_020bd06c.c) takes it as a full u32 and switches on the whole
   word. On ARM the byte arrives zero-extended, because the ROM loads it with
   ldrb r1,[r4] at 0x020bd21c. MSVC passes an unsigned char argument by pushing
   the whole register after a byte load (mov al,[edi] / push eax), so the callee
   switched on the upper 24 bits of whatever eax held before (0x0052c005 for 5
   in the ending cutscene) and every case missed. Declared unsigned int, the call
   zero-extends the byte here the way ldrb does. */
extern void func_ov002_020bd06c(unsigned char *a, unsigned int val);
#else
extern void func_ov002_020bd06c(unsigned char *a, unsigned char val);
#endif

int func_ov002_020bd20c(unsigned char *a, unsigned char *b)
{
    if (a[0x727] != *b) {
        func_ov002_020bd06c(a, *b);
        a[0x727] = *b;
        a[0x728] = 0;
    }
    return 1;
}
