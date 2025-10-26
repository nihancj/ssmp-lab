#include <stdio.h>
#include <string.h>

struct OPTAB {
	char opcode[10];
	int code;
} optab[20];
int totalOpcodes = 0;

void getOpcodes() {
	FILE *fop = fopen("optab.txt", "r");
	while (fscanf(fop, "%s%x", optab[totalOpcodes].opcode, &optab[totalOpcodes].code) != EOF) {
		totalOpcodes++;
	}
	fclose(fop);
}

int is_in_optab(char *opcode) {
	for (int i=0; i<totalOpcodes; i++) {
		if (strcmp(optab[i].opcode, opcode) == 0) {
			return (i+1);
		}
	}
	return 0;
}

struct SYMTAB {
	char symbol[20];
	int value;
} symtab[20];
int totalSymbols = 0;

void getSymbols() {
	FILE *fsym = fopen("symtab.txt", "r");
	while (fscanf(fsym, "%s%x", symtab[totalSymbols].symbol, &symtab[totalSymbols].value) != EOF) totalSymbols++;
	fclose(fsym);
}

int is_in_symtab(char *symbol) {
	for (int i=0; i<totalSymbols; i++) {
		if (strcmp(symtab[i].symbol, symbol) == 0) {
			return i+1;
		}
	}
	return 0;
}

unsigned int record[20];
int totalRecords = 0;
void write_text(FILE *fobj, int staddr) {
	fprintf(fobj, "T^%06x^%02x", staddr, totalRecords*3);
	for (int i=0; i<totalRecords; i++) {
		fprintf(fobj, "^%06x", record[i]);
	}
	fprintf(fobj, "\n");
	totalRecords = 0;
}

int main() {
	char addr[6], label[20], opcode[20], operand[20];
	int length;
	int opidx, symidx, staddr = 0;

	FILE *fin = fopen("intermediate.txt", "r");
	FILE *fobj = fopen("output.obj", "w");
	FILE *fout = fopen("output.txt", "w");
	if (!fin || !fout || !fobj) {
		printf("Error opening files\n");
		goto exit_jump;
	}
	FILE *flength = fopen("length.txt", "r");
	fscanf(flength, "%x", &length);
	fclose(flength);
	getOpcodes();
	getSymbols();

	fscanf(fin, "%s%s%s%s", addr, label, opcode, operand);
	fprintf(fobj, "H^%s^00%s^%04x\n", label, operand, length);

	while (fscanf(fin, "%s%s%s%s", addr, label, opcode, operand) != EOF) {
		if (!strcmp(opcode, "END")) break;
		
		opidx = is_in_optab(opcode);
		symidx = is_in_symtab(operand);
		if (!staddr) sscanf(addr, "%x", &staddr);

		if (opidx--) {
			if (symidx--) {
				record[totalRecords++] = optab[opidx].code*0x10000 + symtab[symidx].value;
			} else {
				record[totalRecords++] = optab[opidx].code*0x10000;
			}
			fprintf(fout, "%s %s %s %s %06x\n", addr, label, opcode, operand, record[totalRecords-1]);
		}
		else if (!strcmp(opcode, "WORD")) {
			if (totalRecords) {
				write_text(fobj, staddr);
			}
			sscanf(addr, "%x", &staddr);
			sscanf(operand, "%x", &record[totalRecords++]);
			fprintf(fout, "%s %s %s %s %06x\n", addr, label, opcode, operand, record[totalRecords-1]);
			printf("%s %s %s %s %06x\n", addr, label, opcode, operand, record[totalRecords-1]);
			write_text(fobj, staddr);
			staddr = 0;
		} 
		else if (!strcmp(opcode, "BYTE")) {
			if (totalRecords) {
				write_text(fobj, staddr);
				sscanf(addr, "%x", &staddr);
			}
			record[totalRecords++] = operand[2];
			fprintf(fout, "%s %s %s %s %02x\n", addr, label, opcode, operand, record[totalRecords-1]);
			fprintf(fobj, "T^%06x^%02x", staddr, totalRecords);
			for (int i=0; i<totalRecords; i++) {
				fprintf(fobj, "^%x", record[i]);
			}
			fprintf(fobj, "\n");
			totalRecords = 0;
			staddr = 0;

		} else {
			fprintf(fout, "%s %s %s %s\n", addr, label, opcode, operand);
			continue;
		}

		if (totalRecords >= 4) {
			write_text(fobj, staddr);
			staddr = 0;
		}
	}

	if (totalRecords) {
		write_text(fobj, staddr);
		staddr = 0;
	}
	fprintf(fobj, "E^00%s", operand);
	fprintf(fout, "%s %s %s %s\n", addr, label, opcode, operand);


exit_jump:
	fclose(fobj);
	fclose(fin);
	fclose(fout);
	return 0;
}
