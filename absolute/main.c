#include <assert.h>
#include <stdio.h>
#include <string.h>

void extract(char *line) {
	for (int i=0; i<strlen(line); i++) {
		if (line[i] == '^') {
			line[i] = '\0';
			break;
		}
	}
}

int main() {
	char name1[10], line[60], *temp;
	int staddr, length;
	FILE *fin = fopen("input.obj", "r");

	// Get name
	printf("Enter the program name: ");
	scanf("%s", name1);
	// Get name from file
	fscanf(fin, "%s", line);
	assert(strtok(line, "^")[0] == 'H');
	printf("Name from obj.%s\n", strtok(NULL, "^"));
	printf("\nAddress\t\tBytes\n");

	fscanf(fin, "%s", line);
	while (strtok(line, "^")[0] != 'E') {
		sscanf(strtok(NULL, "^"), "%x", &staddr);
		sscanf(strtok(NULL, "^"), "%x", &length);
		for (int i=0; i<length; i++) {
			if (i%3 == 0 && ((temp = strtok(NULL, "^")) == NULL)) {
				break;
			}
			printf("%06x\t\t%c%c\n", staddr++, temp[(i%3)*2], temp[(i%3)*2+1]);
		}

		fscanf(fin, "%s", line);
	}


exit_jump:
	fclose(fin);
	return 0;
}
