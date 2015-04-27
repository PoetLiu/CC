#include <stdio.h>
#include <string.h>
#include "queue.h"

int main()
{
	Queue *q = init_queue();
	int i;

	for (i = 0; i < 10; i++) {
		en_queue(q, &i, sizeof(i));
	}


	while (1) {
		en_queue(q, &i, sizeof(i));
		de_queue(q, NULL, NULL, 0); 
	}

	destroy_queue(q);
	return 0;
}
