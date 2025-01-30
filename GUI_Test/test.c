#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("test\n");
	int* test = (int*)malloc(sizeof(int));
	*test = 42;
	printf("%d",*test);
	free(test);
}
