;TODO re-check that none of these constants are used directly
duration_table(PLAYER_RESPAWN_INVISIBLE_DURATION, player_respawn_invisible_duration)
duration_table(PLAYER_RESPAWN_MAX_DURATION, player_respawn_max_duration)
duration_table(PLAYER_DOWN_TAP_MAX_DURATION, player_down_tap_max_duration)

;tech_max_frames_before_collision:
;	.byt TECH_MAX_FRAMES_BEFORE_COLLISION_PAL, TECH_MAX_FRAMES_BEFORE_COLLISION_NTSC
tech_nb_forbidden_frames:
	.byt TECH_NB_FORBIDDEN_FRAMES_PAL, TECH_NB_FORBIDDEN_FRAMES_NTSC
tech_window:
	.byt TECH_MAX_FRAMES_BEFORE_COLLISION_PAL + TECH_NB_FORBIDDEN_FRAMES_PAL
	.byt TECH_MAX_FRAMES_BEFORE_COLLISION_NTSC + TECH_NB_FORBIDDEN_FRAMES_NTSC
