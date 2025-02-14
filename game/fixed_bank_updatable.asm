;
; First quarters of the fixed bank.
;
; No caveat there, unlike in the last quarter. See "static_bank.asm".
;

#echo
#echo ===== FIXED-BANK (updatable) =====

* = $c000 ; $c000 is where the PRG fixed bank rom is mapped in CPU space, so code position is relative to it

; Note - interruption handlers and main loop first, their location being constant is critical (impacts static bank)
fixed_bank_main_begin:
#include "nine/main.asm"
#echo
#echo FIXED-bank (updatable) main loop and interrupts size:
#print *-fixed_bank_main_begin

; Note - data before code, some address changes in data may require server update (notably anim_invisible)
fixed_bank_data_begin:
#include "game/data/fixed-bank-data.asm"
#echo
#echo FIXED-bank (updatable) data size:
#print *-fixed_bank_data_begin

fixed_bank_code_begin:

fixed_bank_code_begin_stnp_lib:
#include "game/logic/stnp_lib.asm"
#echo
#echo FIXED-bank (updatable) code size (stnp_lib)
#print *-fixed_bank_code_begin_stnp_lib

fixed_bank_code_begin_animation_extra:
#include "game/logic/animation_extra.asm"
#echo
#echo FIXED-bank (updatable) code size (animation_extra)
#print *-fixed_bank_code_begin_animation_extra

fixed_bank_code_begin_logic:
#include "game/logic/logic.asm"
#echo
#echo FIXED-bank (updatable) code size (logic)
#print *-fixed_bank_code_begin_logic

fixed_bank_code_begin_utils:
#include "game/logic/utils.asm"
#echo
#echo FIXED-bank (updatable) code size (utils)
#print *-fixed_bank_code_begin_utils

fixed_bank_code_begin_fixed_bank:
#include "nine/fixed_bank.asm"
#echo
#echo FIXED-bank (updatable) code size (fixed_bank)
#print *-fixed_bank_code_begin_fixed_bank

#echo
#echo FIXED-bank (updatable) code size:
#print *-fixed_bank_code_begin

#echo
#echo FIXED-bank (updatable) total space:
#print $f000-$c000
#echo
#echo FIXED-BANK (updatable) free space:
#print $f000-*

;
; Credits in the rom
;

#if $f000-* < 0
#error updatable fixed bank occupies too much space
#endif
.dsb $f000-*, 0
