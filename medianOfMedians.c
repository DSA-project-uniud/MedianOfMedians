#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAX_LINE_SIZE 1000   // maximum size of a line of input

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

void quick3(int *a, int i, int j){ //vettore in 3 parti: a e' il vettore stesso, j fine scansione, i inizio scansione
	if (i < j){
		int c = partition3(a, i, j);
			quick3(a, c+1, j);
			quick3(a, i, c);
	}
}

int medianOfMedians (int *a, int s, int f){
	printf("Array:");
	for (int o=s;o<=f; o++){
		printf("%d ", a[o]);
	}
	printf("\nf:%d, s:%d", f, s);
	if (f-s > 1){
		int suppl = (f-s)/5+1; //quanti pezzi
		int b[suppl];
		int toSort[5];
		int i;
		for (i=s; i+4<=f; i+=5){
			for (int j=0; j<5; j++){
				toSort[j] = a[i+j];
			}
			quick3(toSort, 0, 4);
			b[i/5] = toSort[2];
		}
		int rest = (f-s+1)%5;
		
		if (rest > 0){
			int didd=(suppl-1)*5;
			for (int j=didd; j<=f; j++){
				toSort[j-didd] = a[s+j];
			}
			printf("Valorriies:%d %d %d, ora array:",didd, suppl, a[s]);
			for (int o=0; o<rest; o++)
				printf("%d ", toSort[o]);
			printf("\n");
			quick3(toSort, 0, f-didd);
			b[suppl-1] = toSort[(rest)/2];
		}
		return medianOfMedians(b, 0, suppl-1);
	} else {
		printf ("Candidates:%d %d\n", a[0], a[1]);
		//coppia tour;
		//tour.mc1=a[0];
		if (f-s == 1 && a[0] < a[1]){
			return a[1];
		} else {
			return a[0];
		}
	}
}

int partition (int arr[], int pivot, int low, int high)
{
    int i = (low-1); // Index of smaller element and indicates the right position of pivot found so far
 
    for (int j = low; j <= high-1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swapp(&arr[i], &arr[j]);
        }
    }
    swapp(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/*int partition(int *a, int pivot, int i, int j){
	
	int s=i-1;
	int h=j;
	int l;
	for (l=i; l<j; l++){
		if (a[l]<pivot){
			s++;
			swapp(&(a[h]), &(a[l]));
			//h--;
		}
	}
	swapp(&(a[s+1]), &(a[j]));
	
	
	
	//int k = i;
	/*int h = j;
	for (int l=i; l<h; l++){
		while (a[l] > pivot && l < h){
			for (int pol=i; pol<=j; pol++)
				printf("%d ", a[pol]);
			printf ("L=%d H=%d J=%d\n", l, h, j);
			swapp(&(a[h]), &(a[l]));
			h--;
		}
		/*else if (a[l] < pivot) {
			swapp (&(a[k]), &(a[l]));
			swapp (&(a[h]), &(a[l]));
			k++;
			h++;
		} else {//l > pivot
			//non faccio niente
		}
	}
	return s+2;
}*/

int main() {
	int a[1000];
	int len = scanArray(a); //7 3 11 5
	//int a[]={90,20,10,40,50,00,30};
	//int len=sizeof(a)/sizeof(int);
	int nth; //2
	scanf ("%d", &nth);
	
	//Medians
	int mMedian;
	int p=0;
	int q=len-1;
	int pvt=0;
	
	for (int i=0; i<len; i++)
		printf("%d ", a[i]);
	
	//printf(" mMedian=%d\n", mMedian);
	//pvt = partition(a, mMedian, 0, len-1);
	//printf(" Pivot=%d\n", pvt);
	
	/*while (a[nth-1] != a[pvt]){
		for (int i=0; i<len; i++)
			printf("%d ", a[i]);
		printf("\n");
		if (nth < pvt){
			mMedian = medianOfMedians(a, p, pvt-1);
			q = pvt-1;
			pvt = partition(a, mMedian, p, pvt-1);
		} else {
			mMedian = medianOfMedians(a, pvt, q);
			p = pvt;
			pvt = partition(a, mMedian, pvt, q);
		}
	}*/
	
	//printf("%d\n", mMedian);
	int polo=10;
	//while (pvt != nth+1){ //3-0>2-1
	while (polo>0){
		mMedian=medianOfMedians(a, p, q);
		printf("\nmMedian: %d Primma: ", mMedian);
		for (int z=p; z<=q; z++)
			printf("%d ",a[z]);
		pvt=partition(a, mMedian, p, q);//fino alla posizione pvt sono minori, poi sono >=
		printf("\npvt: %d Doppio: ", pvt);
		for (int z=p; z<=q; z++)
			printf("%d ",a[z]);
		printf("p:%d q:%d\n", p, q);
		if (pvt+p < nth-1){ //7<3
			p=pvt+p+1;
		} else {
			q=pvt+p;
		}
		polo--;
		printf("\nPOSSIBILE:%d\n", a[nth-1]);
	}

}
