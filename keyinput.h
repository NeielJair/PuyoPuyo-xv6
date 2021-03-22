//Para kbmap latam
#ifndef KEYINPUT_H
#define KEYINPUT_H

//region Keys
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_E 0x12
#define KEY_R 0x13
#define KEY_T 0x14
#define KEY_Y 0x15
#define KEY_U 0x16
#define KEY_I 0x17
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_INVCOMMA 0x1A //'
#define KEY_PLUS 0x1B //+, no estoy seguro de este
#define KEY_CCURLY 0x2B //}

#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_NTILDE 0x27 //Ñ
#define KEY_OCURLY 0x28 //{

#define KEY_Z 0x2C
#define KEY_X 0x2D
#define KEY_C 0x2E
#define KEY_V 0x2F
#define KEY_B 0x30
#define KEY_N 0x31
#define KEY_M 0x32
#define KEY_COMMA 0x33 //,
#define KEY_DOT 0x34 //.
#define KEY_DASH 0x35 //-
//endregion

//region Special Characters
#define KEY_CONTROL 0x1D
#define KEY_LSHIFT 0x2A
#define KEY_RSHIFT 0x36

#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D

#define KEY_ESCAPE 1
//endregion

//region Released Keys
#define KEY_RELEASE_Q 0x90
#define KEY_RELEASE_W 0x91
#define KEY_RELEASE_E 0x92
#define KEY_RELEASE_R 0x93
#define KEY_RELEASE_T 0x94
#define KEY_RELEASE_Y 0x95
#define KEY_RELEASE_U 0x96
//...
//endregion

//extern int *input_key;
#endif
