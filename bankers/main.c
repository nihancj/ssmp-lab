#include <stdatomic.h>
#include <stdio.h>

int need[20][5], max[20][5], alloc[20][5], avail[5], finished[20];
int n, m, safe, can_complete, completed = 0;
int seq[20];

void print_mat() {
	printf("PID\t\tMAX\t\tALLOC\t\tNEED\n");
	for (int i=0; i<n; i++) {
		printf("%d\t\t", seq[i]);
		for (int j=0; j<m; j++) {
			printf("%d ", max[i][j]);
		}
		printf("\t\t");
		for (int j=0; j<m; j++) {
			printf("%d ", alloc[i][j]);
		}
		printf("\t\t");
		for (int j=0; j<m; j++) {
			printf("%d ", need[i][j]);
		}
		printf("\n");
	}
}

int main() {
	printf("Enter the no. processes: ");
	scanf("%d", &n);
	printf("Enter the no. of resouces: ");
	scanf(" %d", &m);
	printf("Enter the max matrix:\n");
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			scanf(" %d", &max[i][j]);
		}
		finished[i] = 0;
	}
	printf("Enter the need matrix:\n");
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			scanf(" %d", &alloc[i][j]);
			need[i][j] = max[i][j] - alloc[i][j];
		}
	}
	printf("Enter the available resources:\n");
	for (int j=0; j<m; j++) {
		scanf(" %d", &avail[j]);
	}

	while (completed < n) {
		safe = 0;
		for (int i=0; i<n; i++) {
			if (finished[i]) continue;
			for (int j=0; j<n; j++) {
				can_complete = 1;
				if (avail[j] < need[i][j]) {
					can_complete = 0;
				}
			}
			if (can_complete) {
				safe = 1;
				for (int j=0; j<n; j++) {
					avail[j] += alloc[i][j];
				}
				finished[i] = 1;
				seq[completed++] = i;
			}
		}

		if (!safe) {
			printf("The system is not safe!\n");
			break;
		}
	}

	if (safe) {
		print_mat();
	}

	return 0;
}
