CHARSET_SYMBOLS_BANK_NUMBER = CURRENT_BANK_NUMBER

charset_symbols:

; Charset's size in tiles (zero means 256)
.byt (charset_symbols_end-charset_symbols_tiles)/8

charset_symbols_tiles:
.byt %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000
.byt %00000000, %00011000, %00011000, %00111000, %00110000, %00000000, %01100000, %01100000
.byt %00000000, %00100100, %01101100, %11111110, %01101100, %11111110, %01101100, %01001000
.byt %00000000, %00001100, %00011110, %00111000, %00011100, %01001110, %00111100, %00011000
.byt %00000000, %01000110, %10101100, %10111000, %01110100, %00111010, %01101010, %11000100
.byt %00000000, %00011000, %00011000, %00010000, %00000000, %00000000, %00000000, %00000000
.byt %00000000, %00001100, %00011000, %00110000, %00110000, %00110000, %00011000, %00001100
.byt %00000000, %00110000, %00011000, %00001100, %00001100, %00001100, %00011000, %00110000
.byt %00000000, %00000000, %00011000, %00011000, %01111110, %01111110, %00011000, %00011000
.byt %00000000, %00000000, %00000000, %00000000, %00000000, %00110000, %00110000, %01100000
.byt %00000000, %00000000, %00000000, %00111100, %01111110, %00000000, %00000000, %00000000
.byt %00000000, %00000000, %00000000, %00000000, %00000000, %00011000, %00011000, %00000000
.byt %00000000, %00000000, %00011000, %00011000, %00000000, %00011000, %00011000, %00000000
.byt %00000000, %00000000, %00000000, %00111100, %00000000, %00111100, %00000000, %00000000
.byt %00000000, %00111100, %01100110, %00101100, %00011000, %00110000, %00000000, %00110000
.byt %00000000, %00000000, %00111100, %01000110, %11010110, %11011100, %01100000, %00111000
charset_symbols_end:
