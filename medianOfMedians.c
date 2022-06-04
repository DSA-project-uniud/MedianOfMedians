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

int minn (int a, int b){
		return a>b?b:a;
}

void insertionSort (int *a, int len){
	for (int j=1; j<len; j++){
		int key = a[j];
		int i = j-1;
		while (i >= 0 && a[i] > key){
			a[i+1] = a[i];
			i--;
		}
		a[i+1] = key;
	}
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
			//non faccio niente
		}
	}
	return k;
}

void quick3(int *a, int i, int j){ //vettore in 3 parti: a e' il vettore stesso, j fine scansione, i inizio scansione
	if (i < j){
		int c = partition3(a, i, j);
			quick3(a, i, c);
			quick3(a, c+1, j);
	}
}

int medianOfMedians (int *a, int s, int f){
	printf("\nArrivo: ");
	for (int z=s; z<=f; z++){
		printf("%d ", a[z]);
	}
	if (f-s > 1){
		int suppl = (f-s)/5+1; //quanti pezzi
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
		printf("finale: s:%d, suppl:%d\n", s, suppl);
		return medianOfMedians(a, s, s+suppl-1);
	} else {
		if (f-s == 1 && a[s] < a[f]){
			return a[f];
		} else {
			return a[s];
		}
	}
}

int partition (int arr[], int pivot, int low, int high)
{
    int i = low; // Index of smaller element and indicates the right position of pivot found so far
 
    for (int j = low; j <= high; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
			swapp(&arr[i], &arr[j]);
            i++; // increment index of smaller element
        }
    }
    //swapp(&arr[i + 1], &arr[high]);
    return i;
}


int main() {
	int a[1000];
	int len = scanArray(a); //7 3 11 5
	//int a[]={21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	//int len=sizeof(a)/sizeof(int);
	int nth; //2
	scanf ("%d", &nth);
	
	int mMedian;
	int p=0;
	int q=len-1;
	int pvt=-1;
	
	//printf("%d\n", mMedian);
	/*for (int i=0; i<len; i++){
		printf("Array %d: ", i+1);
		mMedian=medianOfMedians(a, i, q);
		//pvt=partition(a, i+1, p, q);
		for (int z=i; z<len; z++){
			//printf ("%d ",a[z]);
		}
		printf(" pvt:%d\n", mMedian);
	}*/
	//for (nth=1; nth<=len; nth++){
	while (nth-1 != pvt){ //3-0>2-1
	//int polo=10;
	//while (polo>0){
		mMedian=medianOfMedians(a, p, q);
		pvt=partition(a, mMedian, p, q);//dalla posizione pvt in poi sono >=
		printf("%d %d\n", pvt, nth-1);
		for (int z=p; z<=q; z++)
			printf("%d ", a[z]);
		printf("\n");
		if (nth-1 < pvt){ //7<3
			q=pvt-1;
		} else {
			p=pvt;
		}
		//polo--;
	}
	printf("%d ", a[nth-1]);


}
