#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SPBufferset.h"
#include "main_aux.h"

int main(int argc, char** argv) {
	SP_BUFF_SET();


	if (argc > 1) {
		int seed = atoi(argv[1]);
		srand(seed);
	} else {
		srand(time(NULL));
	}

	while (true) {
		bool shouldExit = runGame();
		if (shouldExit) {
			break;
		}
	}

	return 0;
}
