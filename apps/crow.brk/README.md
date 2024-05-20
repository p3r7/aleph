

issue w/ libm not linked!

https://www.avrfreaks.net/s/topic/a5C3l000000U5lsEAC/t035984

```
LN      aleph-crow.elf
../../apps/crow/../../lua/src/lvm.o: In function `luaV_tointeger':
/root/code/aleph/apps/crow/../../apps/crow/../../lua/src/lvm.c:671: undefined reference to `floor'
../../apps/crow/../../lua/src/lvm.o: In function `luaV_mod':
/root/code/aleph/apps/crow/../../apps/crow/../../lua/src/lvm.c:786: undefined reference to `floor'
/root/code/aleph/apps/crow/../../apps/crow/../../lua/src/lvm.c:786: undefined reference to `pow'
/root/code/aleph/apps/crow/../../apps/crow/../../lua/src/lvm.c:786: undefined reference to `fmod'
collect2: ld returned 1 exit status
make: *** [../../libavr32/asf/avr32/utils/make/Makefile.avr32.in:460: aleph-crow.elf] Error 1
```
