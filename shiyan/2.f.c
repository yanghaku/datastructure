#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#define T int
#define MAX_MESION 100
typedef struct {
	int *MAX_dimension;// 每一维的大小
	int total_dimension; //维度
	T* elem;
	int cap;
}multidimensional_array;

void init_array(multidimensional_array* array, int n, int a[]) {
	array->total_dimension = n;
	array->MAX_dimension = (int*)malloc(sizeof(int)*n);
	array->cap = 1;
	for (int i = 0; i < n; ++i) {
		array->MAX_dimension[i] = a[i];
		array->cap *= a[i];
	}
	array->elem = (T*)malloc(sizeof(int)*array->cap);
}
T get(multidimensional_array* array, int a[]) {
	static int mesion[MAX_MESION];
	int k = 1, index=0;
	for (int i = array->total_dimension - 1; i > -1; --i)k *= array->MAX_dimension[i];
	for (int i = 0; i < array->total_dimension; ++i) {
		k /= array->MAX_dimension[i];
		index += a[i] * k;
	}
	return array->elem[index];
}
void set(multidimensional_array* array, int a[], T val) {
	static int mesion[MAX_MESION];
	int k = 1, index = 0;
	for (int i = array->total_dimension - 1; i > -1; --i)k *= array->MAX_dimension[i];
	for (int i = 0; i < array->total_dimension; ++i) {
		k /= array->MAX_dimension[i];
		index += a[i] * k;
	}
	array->elem[index] = val;
}
int check(multidimensional_array* array, int a[]) {
	for (int i = 0; i < array->total_dimension; ++i) {
		if (a[i] < 0 || a[i] >= array->MAX_dimension[i])return -1;
	}
	return 0;
}
/*
 (1)定义整型多维数组的类型，各维的下标是任意整数开始的连续整数；
 (2)下标变量赋值，指向下标范围检查。
 (3)同类型数组赋值。
 (4)子数组赋值，例如，a[1..n]=a[2..n+1]等
 (5)确定数组的大小
*/
int main() {
	multidimensional_array arr;
	int a[] = { 2,3,4 };
	init_array(&arr, 3, a);
	a[0] = 1; a[1] = 2; a[2] = 3;
	set(&arr, a, 10);
	for (int i = 0; i < 24; ++i) {
		if (i % 4 == 0)printf("\n");
		printf("%d ", arr.elem[i]);
	}
	printf("%d\n", get(&arr, a));
	return 0;
}