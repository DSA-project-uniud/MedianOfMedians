#include "functions.h"

#define ELARR 7
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
			//non faccio niente
		}
	}
	return k;
}
int minn (int a, int b){
		return a>b?b:a;
}
void quick(int *a, int i, int j){
	if (i < j){
		int c = partition3(a, i, j);
			quick(a, i, c);
			quick(a, c+1, j);
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
int medianOfMediansRec(int *a, int s, int f){
	if (f-s > 1){
		int suppl = (f-s)/ELARR+1; //how many pieces
		int i;
		
		for (i=s; i+ELARR-1<=f; i+=ELARR){
			quick(a, i, i+ELARR-1);
		}
		int rest = (f-s+1)%ELARR;
		int check=suppl;
		int rem;
		if (rest > 0){
			rem=s+(suppl-1)*ELARR;
			quick(a, rem, f);
			check--;
		}
		for (int i=0; i<check; i++){
			swapp(&a[s+i], &a[s+i*ELARR+2]);
		}
		if (check != suppl){
			swapp(&a[s+suppl-1], &a[(rem+f+1)/2]);
		}
		return medianOfMediansRec(a, s, s+suppl-1);
	} else {
		if (f-s == 1 && a[s] < a[f]){
			return a[f];
		} else {
			return a[s];
		}
	}
}
int medianOfMedians(int *a, int len, int nth){
	int mMedian;
	int p=0;
	int q=len-1;
	int pvt=-1;
	while (q-p>0){
		mMedian=medianOfMediansRec(a, p, q);
		pvt=partition(a, mMedian, p, q);//from pvt ahead, numbers are greater or equal
		if (nth-1 < pvt){
			q=pvt-1;
		} else{
			p=pvt;
		}
	}
	return a[nth-1];
}

int main(){
	int fd;

	//Average case
	printf("Inizio medianOfMedians-randomInput-KRandom\n");
	fd = open("medianOfMedians-randomInput-KRandom.csv", O_CREAT | O_WRONLY, 0666);
	estimateTime(medianOfMedians, 100, 50, true, RANDOM, fd);
	close(fd);

	//Worst case?
	printf("Inizio medianOfMedians-InputOrdinato-KUltimo\n");
	fd = open("medianOfMedians-InputOrdinato-KUltimo.csv", O_CREAT | O_WRONLY, 0666);
	estimateTime(medianOfMedians, 100, 50, false, ULTIMO_ELEMENTO, fd);
	close(fd);

	printf("Fine medianOfMedians\n");
}
