
#include <stdio.h>

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main() {
	int n, ct[20], bt[20], rt[20], at[20], tat[20], wt[20], pid[20];
	int time = 0, completed = 0, safe = 1;
	_Bool finished[20];

	printf("Enter the no. of processes: ");
	scanf("%d", &n);

	printf("Enter the burst time and arrival time of\n");
	for (int i=0; i<n; i++) {
		printf("P%d: ", i);
		scanf(" %d%d", at+i, bt+i);
		pid[i] = i;
		finished[i] = 0;
		rt[i] = 0;
	}

	for (int i=0; i<n-1; i++) {
		for (int j=0; j<n-1-i; j++) {
			if (bt[j] > bt[j+1]) {
				swap(at+j, at+j+1);
				swap(bt+j, bt+j+1);
				swap(pid+j, pid+j+1);
			}
		}
	}

	printf("\nPID\tBT\tAT\tCT\tWT\tTAT\n");
	while (completed < n) {
		safe = 0;
		for (int i=0; i<n; i++) {
			if (finished[i]) continue;
			if (at[i] > time) continue;
			if (rt[i] == 0) {
				rt[i] = bt[i];
			}
			safe = 1;
			wt[i] = time - at[i];
			time += bt[i];
			ct[i] = time;
			tat[i] = ct[i] - at[i];
			finished[i] = 1;
			completed++;
			printf("%d\t%d\t%d\t%d\t%d\t%d\n",
					pid[i], bt[i], at[i], ct[i], wt[i], tat[i]);
		}
		if (!safe) {
			time++;
		}
	}

	return 0;
}
