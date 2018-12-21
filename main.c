#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SPBufferset.h"
#include "main_aux.h"

int main(int argc, char** argv) {
	SP_BUFF_SET();

	if (argc > 0) {
		int seed = atoi(argv[0]);
		srand(seed); /*TODO: check validity of doing this.*/
	}

	while (true) {
		bool shouldExit = runGame();
		if (shouldExit) {
			break;
		}
	}

	return 0;
}
