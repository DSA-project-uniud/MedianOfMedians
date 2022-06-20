#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
	N_MEZZI,
	ULTIMO_ELEMENTO,
	RANDOM, 
	CRESCENTE
} CHOOSE_OF_K;

double duration(struct timespec start, struct timespec end);

double getResolution(void);

double calculateStandardDeviation(double *a, double mean, int len);

double calculateMean(double *a, int len);

/*
Maps the value form [iStart, iStop] to [oStart, oStop]
*/
double mapValue(double value, double iStart, double iStop, double oStart, double oStop);

/*
estimateTime :
	func: funzione
	inputLength: lunghezza input
	meanLength: quante iterazioni con lo stesso input fare
	randomInput: indica se il vettore dell'input deve essere generato casualmente (true -> casuale; false -> crescente)
	chooseOfK: scelta del k-esimo elemto tra {N_MEZZI, ULTIMO_ELEMENTO, RANDOM, CRESCENTE}
*/
void estimateTime(int func(int*, int, int), int inputLength, int meanLength, bool randomInput, int chooseOfK, int fd);

#endif

#define E 0.001
#define A 100
#define B 0.157673137

double duration(struct timespec start, struct timespec end) {
	return end.tv_sec - start.tv_sec  + ((end.tv_nsec - start.tv_nsec ) / (double) 1000000000.0);
}
double getResolution(){
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		clock_gettime(CLOCK_MONOTONIC, &end);
	} while (duration(start, end) == 0.0);
	return duration(start, end);
}
double calculateStandardDeviation(double *a, double mean, int len){
	double som = 0;
	for(int i = 0; i < len; i++){
		som += pow(a[i]-mean, 2);
	}
	return sqrt(som/len);
}
double calculateMean(double *a, int len){
	double som = 0;
	for(int i = 0; i < len; i++)
		som += a[i];
	return som/len;
}
double mapValue(double value, double iStart, double iStop, double oStart, double oStop){
	return oStart+(oStop-oStart)*((value-iStart)/(iStop-iStart));
}

void estimateTime(int func(int*, int, int), int inputLength, int meanLength, bool randomInput, int chooseOfK, int fd){
	srand(time(0));
	struct timespec start, end;
	int n[inputLength];
	for(int i=0; i < inputLength; i++)
		n[i] = A*pow(2, B*i);
	int lower, upper;
	int *input = NULL;
	double tmin = 0;
	double *means = (double *)malloc(meanLength*sizeof(double));
	for(int i = 0; i < inputLength; i++){
		input = (int *)malloc(n[i]*sizeof(int));
		lower = -(n[i]/2);
		upper = n[i]/2;
		int randomPos;
		if(chooseOfK == N_MEZZI)
			randomPos = (n[i]/2)-1;
		else if(chooseOfK == ULTIMO_ELEMENTO)
			randomPos = n[i]-1;
		else if(chooseOfK == CRESCENTE)
			randomPos = (int)mapValue(i, 0, inputLength, 0, n[i]);
		
		// Ciclo per calcolare media e deviazione standard
		for(int dev_idx = 0; dev_idx < meanLength; dev_idx++){
			for(int j = 0; j < n[i]; j++){
				if(randomInput)
					input[j] = (rand() % (upper - lower + 1)) + lower;
				else
					input[j] = j;
			}
			if(chooseOfK == RANDOM)
				randomPos = (rand() % n[i]);

			tmin = getResolution() * ((1/E) + 1); 
			int k = 0;
			clock_gettime(CLOCK_MONOTONIC, &start);
			do{
				func(input, n[i], randomPos);
				clock_gettime(CLOCK_MONOTONIC, &end);
				k++;
			}while(duration(start, end) < tmin);

			means[dev_idx] = (duration(start, end)/k)*1000.0; //Trasformo in ms
		}
		free(input); 
		double mean = calculateMean(means, meanLength);
		double standardDeviation = calculateStandardDeviation(means, mean, meanLength);
		char buffer[50];
		snprintf(buffer, sizeof(buffer),"%d;%.6f;%.6f\n", n[i], mean, standardDeviation);
		int len = strlen(buffer);
		char newBuff[len];
		for(int w = 0; w < len; w++){
			if(buffer[w] == '.')
				buffer[w] = ',';
			newBuff[w] = buffer[w];
		}
		if (write(fd, newBuff, (size_t) len) != len) {perror("errore in scrittura");}
	}
	free(means);
}

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
