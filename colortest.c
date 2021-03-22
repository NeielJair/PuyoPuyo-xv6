#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv) {
	modeswitch(1);

	for (int i = 0; i < 320; i++) {
		for (int j = 0; j < 200 - 100 * ((int)(i/5) % 8 == 0); j++) {
			plotpixel(i, j, (int)i/5);
		}
		for (int j = 200 - 100 * ((int)(i/5) % 8 == 0); j < 200; j++) {
			plotpixel(i, j,63);
		}
	}

	exit();
}
