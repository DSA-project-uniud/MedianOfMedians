#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAX_LINE_SIZE 10000   // maximum size of a line of input

int scanArray(int *a) {
    // scan line of text
    char line[MAX_LINE_SIZE];
    scanf("%[^\n]", line);

    // convert text into array
    int size = 0, offset = 0, numFilled, n;
    do {
        numFilled = sscanf(line + offset, "%d%n", &(a[size]), &n);
        if (numFilled > 0) {
            size++;
            offset += n;
        }
    } while (numFilled > 0);

    return size;
}

void swapp (int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

int partition3(int *a, int i, int j){
	int pivot=a[i];
	int k = i;
	for (int l=i+1; l<=j; l++){
		if (a[l] < pivot) {
			swapp (&(a[k]), &(a[l]));
			swapp (&(a[k+1]), &(a[l]));
			k++;
		} else {//l > pivot
			//nothing to do
		}
	}
	return k;
}

void quick3(int *a, int i, int j){
	if (i < j){
		int c = partition3(a, i, j);
			quick3(a, i, c);
			quick3(a, c+1, j);
	}
}

int medianOfMedians (int *a, int s, int f){
	if (f-s > 1){
		int suppl = (f-s)/5+1; //how many pieces
		int i;
		
		for (i=s; i+4<=f; i+=5){
			quick3(a, i, i+4);
		}
		int rest = (f-s+1)%5;
		int check=suppl;
		int rem;
		if (rest > 0){
			rem=s+(suppl-1)*5;
			quick3(a, rem, f);
			check--;
		}
		for (int i=0; i<check; i++){
			swapp(&a[s+i], &a[s+i*5+2]);
		}
		if (check != suppl){
			swapp(&a[s+suppl-1], &a[(rem+f+1)/2]);
		}
		return medianOfMedians(a, s, s+suppl-1);
	} else {
		if (f-s == 1 && a[s] < a[f]){
			return a[f];
		} else {
			return a[s];
		}
	}
}

int partition(int *a, int pivot, int i, int j){
	int k = i;
	for (int l=i+1; l<=j; l++){
		if (a[l] < pivot) {
			swapp (&(a[k]), &(a[l]));
			swapp (&(a[k+1]), &(a[l]));
			k++;
		} else {//l > pivot
			//nothing to do
		}
	}
	return k+1;
}


int main() {
	//int a[10000];
	int* a = (int *) malloc(sizeof(int) * 10000);
	int len = scanArray(a);
	int nth; //2
	scanf ("%d", &nth);
	
	int mMedian;
	int p=0;
	int q=len-1;
	int pvt=-1;

	while (q-p>0){
		mMedian=medianOfMedians(a, p, q);
		pvt=partition(a, mMedian, p, q);//from pvt ahead, numbers are greater or equal
		if (nth-1 < pvt){
			q=pvt-1;
		} else{
			p=pvt;
		}
	}
	printf("%d ", a[nth-1]);


}
