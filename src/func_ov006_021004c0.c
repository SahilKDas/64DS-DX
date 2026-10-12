struct Obj { char pad[0x5660]; int f; /* 0x5660 */ };

extern int func_ov004_020adc1c(void);
#ifdef _MSC_VER
/* func_ov004_020b19f0 draws the number it is given. On the cartridge
   func_ov004_020adc1c's result is still in r0 at the bl that follows
   (ov006 0x021004e0 / 0x021004e4), so the PC build passes it. */
extern int func_ov004_020b19f0(int value);
#else
extern void func_ov004_020b19f0(void);
#endif

void func_ov006_021004c0(struct Obj *o)
{
    if (o->f < 2)
        return;
#ifdef _MSC_VER
    func_ov004_020b19f0(func_ov004_020adc1c());
#else
    func_ov004_020adc1c();
    func_ov004_020b19f0();
#endif
}
