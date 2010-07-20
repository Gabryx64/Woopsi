#include "tinyfont.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 TinyFont_glyphdata[189] = {
/* 'A' */	0x5400,
/* 'B' */	0x7800,
/* 'C' */	0xBE80,
/* 'D' */	0xFB80,
/* 'E' */	0xA500,
/* 'F' */	0x7180,
/* 'G' */	0x2800,
/* 'H' */	0x6800,
/* 'I' */	0x4480,
/* 'J' */	0x1500,
/* 'K' */	0x0B80,
/* 'L' */	0x0000,
/* 'M' */	0x0380,
/* 'N' */	0x0000,
/* 'O' */	0x2500,
/* 'P' */	0x5680,
/* 'Q' */	0xD400,
/* 'R' */	0xC700,
/* 'S' */	0xC700,
/* 'T' */	0xB780,
/* 'U' */	0xF380,
/* 'V' */	0xF380,
/* 'W' */	0xE500,
/* 'X' */	0xF780,
/* 'Y' */	0xF780,
/* 'Z' */	0x1000,
/* '[' */	0x1000,
/* '\' */	0x2A00,
/* ']' */	0x1C00,
/* '^' */	0x8880,
/* '_' */	0xE500,
/* '`' */	0x0000,
/* 'a' */	0x1000,
/* 'b' */	0x0000,
/* 'c' */	0xF400,
/* 'd' */	0x7C00,
/* 'e' */	0x9400,
/* 'f' */	0xAC00,
/* 'g' */	0x0000,
/* 'h' */	0x9000,
/* 'i' */	0x2800,
/* 'j' */	0xA000,
/* 'k' */	0x4000,
/* 'l' */	0x6000,
/* 'm' */	0x0000,
/* 'n' */	0x1000,
/* 'o' */	0xC000,
/* 'p' */	0xA800,
/* 'q' */	0x5000,
/* 'r' */	0x9C00,
/* 's' */	0x3800,
/* 't' */	0x2400,
/* 'u' */	0x3800,
/* 'v' */	0xBC00,
/* 'w' */	0x5000,
/* 'x' */	0xBC00,
/* 'y' */	0x2400,
/* 'z' */	0x4000,
/* '{' */	0x6000,
/* '|' */	0x4400,
/* '}' */	0xE000,
/* '~' */	0x6000,
/* 0x7F */	0x1000,
/* 0x80 */	0xF780,
/* 0x81 */	0xF780,
/* 0x82 */	0xD700,
/* 0x83 */	0xF200,
/* 0x84 */	0xD680,
/* 0x85 */	0xF300,
/* 0x86 */	0xF300,
/* 0x87 */	0xF200,
/* 0x88 */	0xB780,
/* 0x89 */	0x5400,
/* 0x8A */	0xE900,
/* 0x8B */	0xB700,
/* 0x8C */	0xE000,
/* 0x8D */	0xFF80,
/* 0x8E */	0xF680,
/* 0x8F */	0xF680,
/* 0x90 */	0xF780,
/* 0x91 */	0xF680,
/* 0x92 */	0xF700,
/* 0x93 */	0xF380,
/* 0x94 */	0xE900,
/* 0x95 */	0xB680,
/* 0x96 */	0xB680,
/* 0x97 */	0xB780,
/* 0x98 */	0xB500,
/* 0x99 */	0xB500,
/* 0x9A */	0xE500,
/* 0x9B */	0xE800,
/* 0x9C */	0xA400,
/* 0x9D */	0xD400,
/* 0x9E */	0x5400,
/* 0x9F */	0x0000,
/* 0xA0 */	0x9C00,
/* 0xA1 */	0xB400,
/* 0xA2 */	0xB800,
/* 0xA3 */	0x9C00,
/* 0xA4 */	0xB800,
/* 0xA5 */	0x9C00,
/* 0xA6 */	0xC000,
/* 0xA7 */	0xBC00,
/* 0xA8 */	0xB400,
/* 0xA9 */	0x5000,
/* 0xAA */	0x7000,
/* 0xAB */	0xB400,
/* 0xAC */	0x9C00,
/* 0xAD */	0xB400,
/* 0xAE */	0xB400,
/* 0xAF */	0xBC00,
/* 0xB0 */	0xC000,
/* 0xB1 */	0xFC00,
/* 0xB2 */	0xB400,
/* 0xB3 */	0x3C00,
/* 0xB4 */	0x5000,
/* 0xB5 */	0xBC00,
/* 0xB6 */	0xA800,
/* 0xB7 */	0xFC00,
/* 0xB8 */	0xB400,
/* 0xB9 */	0x5000,
/* 0xBA */	0x9C00,
/* 0xBB */	0xB000,
/* 0xBC */	0x2400,
/* 0xBD */	0x7000,
/* 0xBE */	0x0000,
/* 0xBF */	0x1C00,
/* 0xC0 */	0x9000,
/* 0xC1 */	0xF780,
/* 0xC2 */	0xD700,
/* 0xC3 */	0xF200,
/* 0xC4 */	0xD680,
/* 0xC5 */	0xF300,
/* 0xC6 */	0xF300,
/* 0xC7 */	0xF200,
/* 0xC8 */	0xB780,
/* 0xC9 */	0x5400,
/* 0xCA */	0xE900,
/* 0xCB */	0xB700,
/* 0xCC */	0xE000,
/* 0xCD */	0xFF80,
/* 0xCE */	0xF680,
/* 0xCF */	0xF680,
/* 0xD0 */	0xF780,
/* 0xD1 */	0xF680,
/* 0xD2 */	0xF700,
/* 0xD3 */	0xF380,
/* 0xD4 */	0xE900,
/* 0xD5 */	0xB680,
/* 0xD6 */	0xB680,
/* 0xD7 */	0xB780,
/* 0xD8 */	0xB500,
/* 0xD9 */	0xB500,
/* 0xDA */	0xE500,
/* 0xDB */	0x2B00,
/* 0xDC */	0x5400,
/* 0xDD */	0x8980,
/* 0xDE */	0x7800,
/* 0xDF */	0x0000,
/* 0xE0 */	0x0000,
/* 0xE1 */	0xB400,
/* 0xE2 */	0xB800,
/* 0xE3 */	0x9C00,
/* 0xE4 */	0xB800,
/* 0xE5 */	0x9C00,
/* 0xE6 */	0xC000,
/* 0xE7 */	0xBC00,
/* 0xE8 */	0xB400,
/* 0xE9 */	0x5000,
/* 0xEA */	0x7000,
/* 0xEB */	0xB400,
/* 0xEC */	0x9C00,
/* 0xED */	0xB400,
/* 0xEE */	0xB400,
/* 0xEF */	0xBC00,
/* 0xF0 */	0xC000,
/* 0xF1 */	0xFC00,
/* 0xF2 */	0xB400,
/* 0xF3 */	0x3C00,
/* 0xF4 */	0x5000,
/* 0xF5 */	0xBC00,
/* 0xF6 */	0xA800,
/* 0xF7 */	0xFC00,
/* 0xF8 */	0xB400,
/* 0xF9 */	0x5000,
/* 0xFA */	0x9C00,
/* 0xFB */	0x4400,
/* 0xFC */	0x5000,
/* 0xFD */	0x6000,
};

static const u16 TinyFont_offset[189] = {
    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
   96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
  112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
  128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
  160,  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
  176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  186,  187,  188
};

static const u8 TinyFont_width[189] = {
 2, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 0, 3, 0, 3, 3,
 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 0,
 2, 0, 3, 3, 3, 3, 0, 2, 3, 3, 2, 2, 0, 2, 1, 3,
 2, 3, 3, 3, 3, 3, 2, 3, 3, 2, 2, 3, 3, 2, 2, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 1, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 3, 0, 3,
 3, 3, 3, 3, 3, 1, 3, 3, 2, 2, 3, 3, 3, 3, 3, 1,
 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 2, 3, 2, 0, 3, 2,
 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 1, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 0, 0,
 3, 3, 3, 3, 3, 1, 3, 3, 2, 2, 3, 3, 3, 3, 3, 1,
 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 2, 2
};

TinyFont::TinyFont(u8 fixedWidth) : PackedFont1 (
	65,
	253,
	TinyFont_glyphdata,
	TinyFont_offset,
	TinyFont_width,
	3,
	1,
	3) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
