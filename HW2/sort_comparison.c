#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10000                 // 생성할 총 난수 데이터를 몇개 사용할건지 선언

int arr_data[MAX_SIZE];                //랜덤함수로 만든 데이터를 저장할 원본 배열(original)
int s[MAX_SIZE];                       //각 정렬 알고리즘에서 사용할 데이터 저장 배열
int sorted[MAX_SIZE];                  // merge sort에서 사용할 데이터 저장 배열
clock_t start, end, used_time = 0;    //실행 시간 측정을 위한 변수

void print_output(int arr[], int size);
void calculate_elapsed_time();
void copy_originalArr();

void exchange_sort(int s[], int n);
void merge(int s[], int left, int middle, int right);
void merge_sort(int s[], int left, int right);
int partition(int s[], int left, int right);
void quick_sort(int s[], int left, int right);
void heapify(int s[], int n, int i);
void heap_sort(int s[], int n);
void swap(int s[], int x, int y);
void radix_sort(int s[], int n);


int main() {
    srand((unsigned)time(NULL));  // 난수 발생을 위한 시드값을 설정
    // printf("생성된 값(원본 랜덤 데이터-정렬 전): ");

    for(int i = 0; i < MAX_SIZE; i++) {
        arr_data[i] = rand() % 10000;  // 0 ~ 9999 사이 MAX_SIZE개의 랜덤 데이터 생성
        // printf("%d ", arr_data[i]);
    }
    printf("\n\n");
    
    copy_originalArr();
    printf("exchange sort - ");
    start=clock();
    exchange_sort(s, MAX_SIZE);
    end=clock();
    calculate_elapsed_time();
    // print_output(s, MAX_SIZE);

    copy_originalArr();
    printf("merge sort - ");
    start=clock();
    merge_sort(s, 0, MAX_SIZE-1);
    end=clock();
    calculate_elapsed_time();
    // print_output(s, MAX_SIZE);


    copy_originalArr();
    printf("quick sort - ");
    start=clock();
    quick_sort(s, 0, MAX_SIZE-1);
    end=clock();
    calculate_elapsed_time();
    // print_output(s, MAX_SIZE);

    copy_originalArr();
    printf("heap sort - ");
    start=clock();
    heap_sort(s, MAX_SIZE);
    end=clock();
    calculate_elapsed_time();
    // print_output(s, MAX_SIZE);

    copy_originalArr();
    printf("radix sort - ");
    start=clock();
    radix_sort(s, MAX_SIZE);
    end=clock();
    calculate_elapsed_time();
    // print_output(s, MAX_SIZE);

    return 0;
}

// 정렬 완료된 배열 출력
void print_output(int s[], int size) {
    printf("정렬 후 배열: ");
    for(int i = 0; i < size; i++) {
        printf("%d ", s[i]);
    }
    printf("\n\n");
}

//실행 시간을 측정 및 출력하는 함수(ms초로 계산)
void calculate_elapsed_time()
{
    used_time = end-start;
    printf("elapsed time : %.3lf msec\n",(double)used_time);
}

//원본 배열을 복사하는 함수
void copy_originalArr()
{
    for(int i=0; i<MAX_SIZE; i++)
        s[i]=arr_data[i];
}

// 교환 정렬
void exchange_sort(int s[], int n) {
    int i,j,temp;
    for(i=0; i<n; i++) {
        for(j=i+1; j<n; j++) {
            if(s[i] > s[j]) {
                temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }
}

// 정렬된 배열 2개를 정렬 상태 유지하면서 하나로 합병
void merge(int s[], int left, int middle, int right) {
    int i = left;
    int j = middle+1;
    int k = left;

    // 작은 순서대로 배열에 삽입
    while(i <= middle && j <= right) {
        if(s[i] < s[j]) {
            sorted[k] = s[i];
            i++;
        } else {
            sorted[k] = s[j];
            j++;
        }
        k++;
    }
    // 남은 데이터도 삽입
    if(i > middle) {
        while(j <= right) {
            sorted[k] = s[j];
            j++;
            k++;
        }
    }
    // 남은 데이터도 삽입
    if (i > middle) { // 남은 j값 다 넣어줌
        for(int t=j; t<=right; t++) {
            sorted[k] = s[t];
            k++;
        }
    }
    else { // 남은 i값 다 넣어줌
        for(int t=i; t<=middle; t++) {
            sorted[k] = s[t];
            k++;
        }
    }
    // 정렬된 배열 삽입
    for(int t=left; t<=right; t++) {
        s[t] = sorted[t];
    }
}

// 합병 정렬
void merge_sort(int s[], int left, int right) {
    if (left < right) {
        int middle = (left+right) / 2; // 반으로 분할
        merge_sort(s, left, middle);  // 왼쪽 정렬
        merge_sort(s, middle+1, right); // 오른쪽 정렬
        merge(s, left, middle, right); // 합병
    }
}

// 퀵 정렬의 분할
int partition(int s[], int left, int right) // 중간지점을 피봇으로 설정하기
{
    int pivot = s[left]; // 처음 피봇 위치: 왼쪽에서부터 시작
    int low = left+1;
    int high = right;
 
    while(low <= high) { // low, high가 교차할때까지 반복
        while(low<=right && s[low]<=pivot) // 피봇보다 큰 값 찾기
            low++; // low를 오른쪽으로 이동하여 재탐색
        while(high>=left+1 && s[high]>pivot) // 피봇보다 작은 값 찾기
            high--; // high를 왼쪽으로 이동하여 재탐색
        if(low <= high) // 교차 안 된 상태이면 low와 high 위치 스왑
            swap(s, low, high);
    }
    swap(s, left, high); // 피봇과 high 위치 스왑
    return high; // 피봇의 바뀐 위치 리턴
}

// 퀵 정렬
void quick_sort(int s[], int left, int right)
{
    if(left < right)
    {
        int pivot = partition(s, left, right); // 분할
        quick_sort(s, left, pivot-1); // 피폿 기준 왼쪽 정렬
        quick_sort(s, pivot+1, right); // 피폿 기준 오른쪽 정렬
    }
}

// 힙 구성하기
void heapify(int s[], int n, int i) {
    int parent = i;
    int left_child = i*2 + 1;
    int right_child = i*2 + 2;

    // 왼쪽 자식 노드가 존재하는지 체크하고 부모 노드와 값 비교
    if (left_child<n && s[parent]<s[left_child]) {
        parent = left_child;
    }
    // 오른쪽 자식 노드가 존재하는지 체크하고 부모 노드와 값 비교
    if (right_child<n && s[parent]<s[right_child]) {
        parent = right_child;
    }
    // 왼쪽이나 오른쪽 자식 노드 중 부모 노드보다 큰 값이 존재한 경우
    if (i != parent) {
        swap(s, parent, i);
        heapify(s, n, parent); // 초기 부모 노드가 맨 끝 리프 노드로 찾아갈 때까지 내려감
    }
}

// 힙 정렬
void heap_sort(int s[], int n) {
    // 최대 힙 구성
    for (int i=(n/2)-1; i>=0; i--) {
        heapify(s, n, i);
    }
    // root 노드에 위치한 최댓값을 마지막 노드와 바꿈
    // 배열(힙)의 크기를 점점 줄여가며 변경된 root 노드에 대해서 힙을 재구성
    for (int i=n-1; i>0; i--) {
        swap(s, 0, i);
        heapify(s, i, 0);
    }
}

// swap 함수
void swap(int s[], int x, int y)
{
	int temp = s[x];
	s[x] = s[y];
	s[y] = temp;
}

// 기수 정렬(LSD 방식)
void radix_sort(int s[], int n) {
    int result[n];
    int max = 0;
    int digit = 1;

    for (int i=0; i<n; i++) { // 최댓값 구하기
        if (s[i] > max) {
            max = s[i];
        }
    }
    while (max/digit > 0) { // 1의 자릿수부터 계산
        int bucket[10] = {0,}; // 10진수 정수의 정렬과정에서 사용할 버킷 초기화

        for (int i=0; i<n; i++) {
            bucket[(s[i]/digit) % 10]++; // 자릿수에 해당하는 버킷에 있는 원소 갯수 구하기
        }
        for (int i=1; i<10; i++) {
            bucket[i] += bucket[i-1]; // 결과 배열을 만들기 위해 버킷 누적수 구하기
        }
        for(int i=n-1; i>=0; i--) { 
            result[bucket[(s[i]/digit) % 10] - 1] = s[i]; // 순서 재배치, 같은 자릿수끼리는 순서 유지
            bucket[(s[i]/digit) % 10]--;
        }
        for (int i=0; i<n; i++) {
            s[i] = result[i]; // 주어진 배열 갱신
        }
        digit *= 10; // 다음 자릿수로 이동
    }
}