.(
#echo
#echo ====== DATA-16-BANK =====
* = $8000

bank_data16_begin:

.(
bank_data_begin:
#include "game/logic/game_states/arcade_mode/congratz.asm"
#echo
#echo Arcade congratz screen logic
#print *-bank_data_begin
.)

bank_data16_end:

#echo
#echo DATA-16-bank used size:
#print bank_data16_end-bank_data16_begin
#echo
#echo DATA-16-bank free space:
#print $c000-*

#if $c000-* < 0
#error Data bank occupies too much space
#else
.dsb $c000-*, CURRENT_BANK_NUMBER
#endif
.)
