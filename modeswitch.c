#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv) {
	char *mode;

	if (argc != 2) {
		printf(2, "usage: modeswitch [mode]\nmode: -t or --text: text mode, -g or --graphic: graphic mode\n");
		exit();
	}

	mode = argv[1];

	if (strcmp(mode, "-t") == 0 || strcmp(mode, "--text") == 0) {
		modeswitch(0);
	} else if (strcmp(mode, "-g") == 0 || strcmp(mode, "--graphic") == 0) {
		modeswitch(1);
	} else {
		printf(1, "modeswitch: wrong argument\n");
	}

	exit();
}