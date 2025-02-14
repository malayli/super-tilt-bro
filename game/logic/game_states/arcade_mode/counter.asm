.(
; Increment counter
; Overwrites A, X
+arcade_mode_inc_counter
.(
	; Avoid going over 9min 59sec, so minutes are always in one digit
	ldx system_index

	lda arcade_mode_counter_minutes
	cmp #9
	bne ok
		lda arcade_mode_counter_seconds
		cmp #59
		bne ok
			lda arcade_mode_counter_frames
			clc
			adc #1
			cmp framerate, x
			beq end
	ok:

	; Increment counter
	inc arcade_mode_counter_frames
	lda arcade_mode_counter_frames
	;ldx system_index ; useless, done above
	cmp framerate, x
	bne end

		lda #0
		sta arcade_mode_counter_frames

		inc arcade_mode_counter_seconds
		lda arcade_mode_counter_seconds
		cmp #60
		bne end

			lda #0
			sta arcade_mode_counter_seconds

			inc arcade_mode_counter_minutes

	end:
	rts

	framerate:
	.byt 50, 60
.)

+arcade_mode_display_counter:
.(
	+ARCADE_COLON_TILE = 256 - 25

	POSITION_X = 3
	POSITION_Y = 3
	FIRST_DIGIT_PPU_ADDR = $2000+POSITION_Y*32+POSITION_X

	lda #ARCADE_COLON_TILE
	sta tmpfield1
	lda #<FIRST_DIGIT_PPU_ADDR
	sta tmpfield2
	lda #>FIRST_DIGIT_PPU_ADDR
	sta tmpfield3

	; Fallthrough to arcade_mode_display_counter_params
.)

; Push a nametable buffer displaying the counter
;  tmpfield1 - Separator tile
;  tmpfield2,tmpfield3 - PPU address
;
; Overwrites A, X
+arcade_mode_display_counter_params:
.(
	separator_tile = tmpfield1
	ppu_addr = tmpfield2
	;ppu_addr_msb = tmpfield3

	; Write buffer header
	LAST_NT_BUFFER

	lda #1 ; continuation byte
	sta nametable_buffers, x
	inx

	lda ppu_addr+1 ; PPU address
	sta nametable_buffers, x
	inx
	lda ppu_addr
	sta nametable_buffers, x
	inx

	lda #7 ; Tile count
	sta nametable_buffers, x
	inx

	; Write counter value
	ldy arcade_mode_counter_minutes
	lda base10_to_tile_low, y
	jsr set_one_tile

	lda separator_tile
	jsr set_one_tile

	ldy arcade_mode_counter_seconds
	lda base10_to_tile_high, y
	jsr set_one_tile
	lda base10_to_tile_low, y
	jsr set_one_tile

	lda separator_tile
	jsr set_one_tile

	ldy arcade_mode_counter_frames
	lda base10_to_tile_high, y
	jsr set_one_tile
	lda base10_to_tile_low, y
	jsr set_one_tile

	; Buffer stop byte
	lda #0
	sta nametable_buffers, x
	stx nt_buffers_end

	rts

	set_one_tile:
	.(
		sta nametable_buffers, x
		inx
		rts
	.)

	base10_to_tile_low:
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9
	.byt TILE_CHAR_0+0, TILE_CHAR_0+1, TILE_CHAR_0+2, TILE_CHAR_0+3, TILE_CHAR_0+4, TILE_CHAR_0+5, TILE_CHAR_0+6, TILE_CHAR_0+7, TILE_CHAR_0+8, TILE_CHAR_0+9

	base10_to_tile_high:
	.byt TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0, TILE_CHAR_0+0
	.byt TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1, TILE_CHAR_0+1
	.byt TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2, TILE_CHAR_0+2
	.byt TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3, TILE_CHAR_0+3
	.byt TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4, TILE_CHAR_0+4
	.byt TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5, TILE_CHAR_0+5
	.byt TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6, TILE_CHAR_0+6
	.byt TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7, TILE_CHAR_0+7
	.byt TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8, TILE_CHAR_0+8
	.byt TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9, TILE_CHAR_0+9

.)
.)
