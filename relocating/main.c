#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int records[30];
int totalRecords = 0;

int main() {
	char line[60], *addr_str, *name;
	int new, old, diff, staddr, length, mbits;
	int byte, nameLen;
	FILE *fin = fopen("input.obj", "r");
	FILE *fout = fopen("output.obj", "w");

	printf("Enter the actual starting address: ");
	scanf("%x", &new);

	fscanf(fin, "%s", line);
	assert(strtok(line, "^")[0] == 'H');	// H
	name = strtok(NULL, "^");				// <NAME>
	addr_str = strtok(NULL, "^");			// start_address
	sscanf(addr_str, "%x", &old);
	diff = new - old;

	fprintf(fout, "H^%s^%06x^%s\n", name, new, strtok(NULL, "^"));

	fscanf(fin, "%s", line);
	while (strtok(line, "^")[0] == 'T') {
		sscanf(strtok(NULL, "^"), "%x", &staddr);
		sscanf(strtok(NULL, "^"), "%x", &length);
		sscanf(strtok(NULL, "^"), "%x", &mbits);
		staddr += diff;

		for (int addr=staddr, i=11; i>(11-length/3); i--, addr+=3) {
			sscanf(strtok(NULL, "^"), "%x", &byte);
			if (mbits & (1u << i)) {
				byte += diff;
			}
			records[totalRecords++] = byte;
			printf("%06x\t\t%06x\n", addr, byte);
		}
		
		fprintf(fout, "T^%06x^%02x", staddr, length);
		for (int i=0; i<totalRecords; i++) {
			fprintf(fout, "^%06x", records[i]);
		}
		fprintf(fout, "\n"); 
		totalRecords = 0;

		fscanf(fin, "%s", line);
	}

	if (line[3] != '*') {
		sscanf(line+3, "%x", &old);
		fprintf(fout, "E^%06x\n", old+diff);
	} else {
		fprintf(fout, "%s", line);
	}

	
exit_jump:
	fclose(fin);
	fclose(fout);
	return 0;
}
