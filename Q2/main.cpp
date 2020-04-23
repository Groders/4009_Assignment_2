
#include <cstdlib>
#include <iostream>
#include <cilk/cilk.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "hwtimer.h"

#define BASE_CASE 5
void P_Merge (int *C, int *A, int *B, int na, int nb);
void P_Mergesort (int *B, int *A, int size);
int BinSearch (int a, int *B, int nb);
int compare (const void * a, const void * b);

using namespace std;

void P_Mergesort (int *B, int *A, int size){
	//printf("MERGESORT: size = %d\n", size);
	if (size <= BASE_CASE){
		for (int i = 0; i < size; i++){
			B[i] = A[i];
		}
	}else{
		//merge sort recursivly
		int *C;
		C = (int*) malloc ((size)*sizeof(int));
		P_Mergesort (C, A, size/2);
		P_Mergesort (C + size/2, A + size/2, size - size/2);
		P_Merge (B, C, C + size/2, size/2, size - size/2);
	}
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void print_arr (int *a, int na){
		for (int i = 0; i < na; i ++){
			printf("%d, ", a[i]);
		}
		printf("\n");
}

void P_Merge (int *C, int *A, int *B, int na, int nb){
	//printf("MERGE: na = %d, nb = %d\n", na, nb);
	if (na < nb){
		P_Merge (C, B, A, nb, na);
	} else if (na <= BASE_CASE){
		for (int i = 0; i < na; i++){
			C[i] = A[i];
		}
		for (int i = 0; i < nb; i++){
			C[i + na] = B[i];
		}
		qsort(C, na + nb, sizeof(int), compare);
	} else {
		int ma = na/2;
		int mb = BinSearch(A[ma], B, nb);
		P_Merge(C, A, B, ma, mb);
		P_Merge(C + ma + mb, A + ma, B + mb, na - ma, nb - mb);
	}
}

int BinSearch(int x, int* arr, int n){
    int l = 0;
    int r = n-1;
    int m = l+(r-l)/2;
    //~ printf("start - x: %d, l:%d, m:%d, r:%d\n",x, l,m,r);
    while(l <= r){
        if (arr[m] < x){
            l = m + 1;
        } 
        else if (arr[m] > x){
            r = m - 1;
        }else{
            //~ printf("breaking - l:%d, m:%d, r:%d\n",l,m,r);
            break;
        }

        m = l+(r-l)/2;
        //~ printf("l:%d, m:%d, r:%d\n",l,m,r);
    }
    return m;
}



int main(int argc, char* argv[]){
	int num_ints; //number of ints to sort
	
	
	if (argc != 3) {
		cout << "Usage: q1 <in> <out>" << endl;
		return 1;
	}
		
	std::ifstream  fn(argv[1]);
	std::string line1;
	std::getline(fn, line1);
	istringstream inputstream(line1);
	inputstream >> num_ints;
	
	
	int *in_array = new int[num_ints];
	int *out_array = new int[num_ints];

	for (int i = 0; i < num_ints; i++){
		std::string l;
		std::getline(fn, l);
		istringstream instream(l);
		instream >>  in_array[i];
		//printf("%d\n", in_array[i]);
	}

	

	hwtimer_t timer;
	initTimer(&timer);
	startTimer(&timer);
		
		
	cout << "START MERGE" << endl;
	P_Mergesort (out_array, in_array, num_ints);

		
	stopTimer(&timer);
	double fibTime = (double) getTimerNs(&timer);
	
	cout << "Total time: " << fibTime/1000000000 << "s" << endl;
	cout << "STOP MERGE" << endl;

	ofstream outfile(argv[2]);
	outfile << num_ints;
	outfile << '\n';
	for (int i = 0; i < num_ints; i++){
		//printf("%d\n", out_array[i]);
		outfile << out_array[i];
		outfile << '\n';
	}
	
	
	return 0;
};
