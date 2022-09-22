#include <cstb.h>

///////////////////////////////////////////
// Dependencies from destination game state
///////////////////////////////////////////

void superpose_earth_sprites();
extern uint8_t ONLINE_MODE_SCREEN_EXTRA_BANK_NUMBER; // Actually a label, use its address or "state_bank()"

static uint8_t state_bank() {
	return ptr_lsb(&ONLINE_MODE_SCREEN_EXTRA_BANK_NUMBER);
}

///////////////////////////////////////////
// Transition implementation
///////////////////////////////////////////

void online_mode_screen_fadein_extra() {
	// Clear bottom nametable with sky color
	*PPUSTATUS;
	*PPUADDR = 0x28;
	*PPUADDR = 0x00;
	for (uint16_t i = 0; i < 0x400; ++i) {
		*PPUDATA = 0;
	}

	// Hide sprites
	uint8_t orig_tile_pos[256];
	for (uint16_t i = 0; i < 256; i += 4) {
		orig_tile_pos[i] = oam_mirror[i];
		oam_mirror[i] = 0xfe;
	}

	// Reactivate NMI, and wait one frame (to process nt_buffers from init)
	*ppuctrl_val = 0x92;
	*PPUCTRL = *ppuctrl_val;
	long_sleep_frame();

	// Change palettes, to have transition colors from "sky" to "space banner"
	*PPUADDR = 0x3f;
	*PPUADDR = 0x07;
	*PPUDATA = 0x0f;

	*PPUADDR = 0x3f;
	*PPUADDR = 0x0b;
	*PPUDATA = 0x02;

	// Reactivate rendering
	*scroll_y = 0;
	long_sleep_frame();
	*PPUMASK = 0x1e;

	// Animation primitives
#define ATT(br, bl, tr, tl) ((br << 6) + (bl << 4) + (tr << 2) + tl)
#define COMMA ,
#define NT_BUFF(addr, data) {\
	static uint8_t const buff[] = { data };\
	static uint8_t const header[] = {(addr) >> 8, (addr) & 0xff, sizeof(buff)};\
	wrap_construct_nt_buffer(header, buff);\
}
#define SET_SCROLL(nametable, y) *ppuctrl_val = 0x90 + (nametable); *scroll_y = y;
#define FRAME for (uint8_t i =0; i < 1 ; ++i) {long_sleep_frame();}

	// Cut sky to create space banner
	NT_BUFF(
		0x2bc8,
		ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(2,2,2,2) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(2,2,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(2,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(2,1,2,2) COMMA ATT(2,2,0,1) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(2,1,2,2) COMMA ATT(2,2,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(2,2,2,1) COMMA ATT(0,2,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(2,2,2,1) COMMA ATT(0,2,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(1,1,0,0) COMMA ATT(2,2,0,0) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,2,1) COMMA ATT(2,2,2,2) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,2,1) COMMA ATT(2,2,2,2) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,1,1) COMMA ATT(2,2,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(2,1,1,1) COMMA ATT(2,2,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,1,1) COMMA ATT(0,0,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(2,1,0,0) COMMA ATT(2,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,2,2) COMMA ATT(2,2,0,1) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,2,2) COMMA ATT(2,2,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,2,2,1) COMMA ATT(0,2,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,2,2,1) COMMA ATT(0,2,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,2,1) COMMA ATT(0,0,2,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(2,2,0,0) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,2,1) COMMA ATT(2,2,2,2) COMMA ATT(0,2,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,2,1) COMMA ATT(2,2,2,2) COMMA ATT(2,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,1,1) COMMA ATT(2,2,2,2) COMMA ATT(0,0,0,2) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,1,1) COMMA ATT(2,2,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA
		ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,2,2) COMMA ATT(0,0,0,2) COMMA ATT(0,0,0,0) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(2,1,0,0) COMMA ATT(2,2,0,0) COMMA ATT(0,0,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,1,2,2) COMMA ATT(2,2,0,2) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,2,2,2) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,2,2,1) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(2,2,2,1) COMMA ATT(0,2,2,2) COMMA
		ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,2,1) COMMA ATT(0,0,2,2) COMMA ATT(0,0,0,0) COMMA
	)
	FRAME
	NT_BUFF(
		0x2bc8,
		ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(1,1,0,0) COMMA ATT(2,2,0,0) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,2,1) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA
		ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(1,1,1,1) COMMA ATT(0,1,1,1) COMMA
		ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,1,1) COMMA ATT(0,0,2,2) COMMA
	)
	FRAME

	// Return to top nametable, tweak palettes so space banner's contents is hidden
	SET_SCROLL(0, 0)
	NT_BUFF(
		0x3f00,
		0x0f COMMA 0x0f COMMA 0x0f COMMA 0x21 COMMA  0x0f COMMA 0x0f COMMA 0x0f COMMA 0x0f COMMA  0x0f COMMA 0x0f COMMA 0x0f COMMA 0x0f COMMA  0x0f COMMA 0x28 COMMA 0x20 COMMA 0x21 COMMA
		0x0f COMMA 0x29 COMMA 0x02 COMMA 0x0f COMMA  0x0f COMMA 0x28 COMMA 0x0f COMMA 0x20 COMMA  0x0f COMMA 0x00 COMMA 0x00 COMMA 0x00 COMMA  0x0f COMMA 0x00 COMMA 0x00 COMMA 0x00
	)
	FRAME

	// Earth fade-in
	for (uint16_t i = 0; i <= 255; i += 4) {
		oam_mirror[i] = orig_tile_pos[i];
	}
	NT_BUFF(0x3f05, 0x0f COMMA 0x0f COMMA 0x20) // stars
	NT_BUFF(0x3f09, 0x09 COMMA 0x0f COMMA 0x01) // bg
	NT_BUFF(0x3f11, 0x09 COMMA 0x0f COMMA 0x0f) // sprites
	FRAME
	NT_BUFF(0x3f09, 0x19 COMMA 0x02 COMMA 0x11) // bg
	NT_BUFF(0x3f11, 0x19 COMMA 0x02 COMMA 0x0f) // sprites
	FRAME
	NT_BUFF(0x3f09, 0x29 COMMA 0x02 COMMA 0x21) // bg
	NT_BUFF(0x3f11, 0x29 COMMA 0x02 COMMA 0x0f) // sprites
	FRAME

	// Boxes fade-in
	wrap_trampoline(state_bank(), code_bank(), &superpose_earth_sprites);
	NT_BUFF(0x3f01, 0x0f COMMA 0x02 COMMA 0x21 COMMA  0x0f COMMA 0x0f COMMA 0x08 COMMA 0x20)
	FRAME
	NT_BUFF(0x3f01, 0x02 COMMA 0x01 COMMA 0x21 COMMA  0x0f COMMA 0x08 COMMA 0x18 COMMA 0x20)
	FRAME
	NT_BUFF(0x3f01, 0x02 COMMA 0x11 COMMA 0x21 COMMA  0x0f COMMA 0x08 COMMA 0x28 COMMA 0x20)
	FRAME

	// Fully restore screen as expected by gamestate
	NT_BUFF(
		0x3f00,
		0x0f COMMA 0x02 COMMA 0x11 COMMA 0x21 COMMA  0x0f COMMA 0x08 COMMA 0x28 COMMA 0x20 COMMA  0x0f COMMA 0x29 COMMA 0x02 COMMA 0x21 COMMA  0x0f COMMA 0x28 COMMA 0x20 COMMA 0x21 COMMA
		0x0f COMMA 0x29 COMMA 0x02 COMMA 0x0f COMMA  0x0f COMMA 0x28 COMMA 0x0f COMMA 0x20 COMMA  0x0f COMMA 0x00 COMMA 0x00 COMMA 0x00 COMMA  0x0f COMMA 0x00 COMMA 0x00 COMMA 0x00
	)
	FRAME
}
