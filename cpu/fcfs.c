#include <stdio.h>

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main() {
	int n, ct[20], bt[20], at[20], tat[20], wt[20], pid[20];
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
	}

	for (int i=0; i<n-1; i++) {
		for (int j=0; j<n-1-i; j++) {
			if (at[j] > at[j+1]) {
				swap(at+j, at+j+1);
				swap(bt+j, bt+j+1);
				swap(pid+j, pid+j+1);
			}
		}
	}

	while (completed < n) {
		for (int i=0; i<n; i++) {
			if (finished[i]) continue;
			if (at[i] > time) {
				i=-1;
				time++;continue;
			}
			wt[i] = time - at[i];
			time += bt[i];
			ct[i] = time;
			tat[i] = ct[i] - at[i];
			finished[i] = 1;
			completed++;
		}
	}

	printf("PID\tBT\tAT\tCT\tWT\tTAT\n");
	for (int i=0; i<n; i++) {
		printf("%d\t%d\t%d\t%d\t%d\t%d\n",
				pid[i], bt[i], at[i], ct[i], wt[i], tat[i]);
	}

	return 0;
}
