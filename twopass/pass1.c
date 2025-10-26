#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct OPTAB{
	char opcode[10];
	char code[3];
} optab[20];
int totalOpcodes = 0;

void get_optab() {
	FILE *fop = fopen("optab.txt", "r");
	if (!fop) return;
	while (fscanf(fop, "%s%s", optab[totalOpcodes].opcode, optab[totalOpcodes].code) != EOF) totalOpcodes++;
	fclose(fop);
}

_Bool is_in_optab(char *opcode) {
	for (int i=0; i<totalOpcodes; i++) {
		if (strncmp(optab[i].opcode, opcode, 20) == 0) {
			return 1;
		}
	}
	return 0;
}

struct SYMTAB{
	char symbol[20];
	int value;
} symtab[20];
int totalSymbols = 0;

_Bool is_in_symtab(char *label) {
	for (int i=0; i<totalSymbols; i++) {
		if (strncmp(symtab[i].symbol, label, 20) == 0) {
			return 1;
		}
	}
	return 0;
}

int main() {
	char label[20], opcode[20], operand[20];
	int locctr = 0, starting_address = 0;
	int length;
	
	// Open files, read optab
	FILE *fin = fopen("input.txt", "r");
	FILE *fout = fopen("intermediate.txt", "w");
	if (!fin || !fout) {
		printf("Error opening files\n");
		goto exit_jump;
	}
	get_optab();

	// read the first line and check START
	fscanf(fin, "%s%s%s", label, opcode, operand);
	if(strncmp(opcode, "START", 5) == 0) {
		sscanf(operand, "%x", &starting_address);
		locctr = starting_address;
		if (label[0] != '*') {
			strncpy(symtab[0].symbol, label, 20);
			symtab[0].value = starting_address;
			totalSymbols = 1;
		}
		fprintf(fout, "** %s %s %s\n", label, opcode, operand);
		fscanf(fin, "%s%s%s", label, opcode, operand);
	}

	// Read lines until End of File
	do {
		fprintf(fout, "%x %s %s %s\n", locctr, label, opcode, operand);
		if (!strcmp(opcode, "END")) break;

		if (label[0] != '*') {
			if (is_in_symtab(label)) {
				printf("Duplicate symbol '%s'\n", label);
				goto exit_jump;
			}
			strncpy(symtab[totalSymbols].symbol, label, 20);
			symtab[totalSymbols++].value = locctr;
		}
		
		if(is_in_optab(opcode)) {
			locctr += 3;
		} else if (!strcmp(opcode, "WORD")) {
			locctr += 3;
		} else if (!strcmp(opcode, "RESW")) {
			locctr += 3 * atoi(operand);
		} else if (!strcmp(opcode, "RESB")) {
			locctr += atoi(operand);
		} else if (!strcmp(opcode, "BYTE") &&
		operand[0] == 'C' && operand[1] == '\'') {
			locctr += strlen(operand) - 3;
		} else {
			printf("Invalid opcode %s\n", opcode);
			goto exit_jump;
		}
	} while(fscanf(fin, "%s%s%s", label, opcode, operand) != EOF);


	// Write length
	FILE *flength = fopen("length.txt", "w");
	fprintf(flength, "%x", locctr - starting_address);
	fclose(flength);

	// Write symtab
	FILE *fsym = fopen("symtab.txt", "w");
	for (int i=0; i<totalSymbols; i++) {
		fprintf(fsym, "%s %02x\n", symtab[i].symbol, symtab[i].value);
	}
	fclose(fsym);

exit_jump:
	fclose(fout);
	fclose(fin);
	return 0;
}
