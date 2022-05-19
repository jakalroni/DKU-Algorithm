#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10000                 // ������ �� ���� �����͸� � ����Ұ��� ����

int arr_data[MAX_SIZE];                //�����Լ��� ���� �����͸� ������ ���� �迭(original)
int s[MAX_SIZE];                       //�� ���� �˰��򿡼� ����� ������ ���� �迭
int sorted[MAX_SIZE];                  // merge sort���� ����� ������ ���� �迭
clock_t start, end, used_time = 0;    //���� �ð� ������ ���� ����

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
    srand((unsigned)time(NULL));  // ���� �߻��� ���� �õ尪�� ����
    // printf("������ ��(���� ���� ������-���� ��): ");

    for(int i = 0; i < MAX_SIZE; i++) {
        arr_data[i] = rand() % 10000;  // 0 ~ 9999 ���� MAX_SIZE���� ���� ������ ����
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

// ���� �Ϸ�� �迭 ���
void print_output(int s[], int size) {
    printf("���� �� �迭: ");
    for(int i = 0; i < size; i++) {
        printf("%d ", s[i]);
    }
    printf("\n\n");
}

//���� �ð��� ���� �� ����ϴ� �Լ�(ms�ʷ� ���)
void calculate_elapsed_time()
{
    used_time = end-start;
    printf("elapsed time : %.3lf msec\n",(double)used_time);
}

//���� �迭�� �����ϴ� �Լ�
void copy_originalArr()
{
    for(int i=0; i<MAX_SIZE; i++)
        s[i]=arr_data[i];
}

// ��ȯ ����
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

// ���ĵ� �迭 2���� ���� ���� �����ϸ鼭 �ϳ��� �պ�
void merge(int s[], int left, int middle, int right) {
    int i = left;
    int j = middle+1;
    int k = left;

    // ���� ������� �迭�� ����
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
    // ���� �����͵� ����
    if(i > middle) {
        while(j <= right) {
            sorted[k] = s[j];
            j++;
            k++;
        }
    }
    // ���� �����͵� ����
    if (i > middle) { // ���� j�� �� �־���
        for(int t=j; t<=right; t++) {
            sorted[k] = s[t];
            k++;
        }
    }
    else { // ���� i�� �� �־���
        for(int t=i; t<=middle; t++) {
            sorted[k] = s[t];
            k++;
        }
    }
    // ���ĵ� �迭 ����
    for(int t=left; t<=right; t++) {
        s[t] = sorted[t];
    }
}

// �պ� ����
void merge_sort(int s[], int left, int right) {
    if (left < right) {
        int middle = (left+right) / 2; // ������ ����
        merge_sort(s, left, middle);  // ���� ����
        merge_sort(s, middle+1, right); // ������ ����
        merge(s, left, middle, right); // �պ�
    }
}

// �� ������ ����
int partition(int s[], int left, int right) // �߰������� �Ǻ����� �����ϱ�
{
    int pivot = s[left]; // ó�� �Ǻ� ��ġ: ���ʿ������� ����
    int low = left+1;
    int high = right;
 
    while(low <= high) { // low, high�� �����Ҷ����� �ݺ�
        while(low<=right && s[low]<=pivot) // �Ǻ����� ū �� ã��
            low++; // low�� ���������� �̵��Ͽ� ��Ž��
        while(high>=left+1 && s[high]>pivot) // �Ǻ����� ���� �� ã��
            high--; // high�� �������� �̵��Ͽ� ��Ž��
        if(low <= high) // ���� �� �� �����̸� low�� high ��ġ ����
            swap(s, low, high);
    }
    swap(s, left, high); // �Ǻ��� high ��ġ ����
    return high; // �Ǻ��� �ٲ� ��ġ ����
}

// �� ����
void quick_sort(int s[], int left, int right)
{
    if(left < right)
    {
        int pivot = partition(s, left, right); // ����
        quick_sort(s, left, pivot-1); // ���� ���� ���� ����
        quick_sort(s, pivot+1, right); // ���� ���� ������ ����
    }
}

// �� �����ϱ�
void heapify(int s[], int n, int i) {
    int parent = i;
    int left_child = i*2 + 1;
    int right_child = i*2 + 2;

    // ���� �ڽ� ��尡 �����ϴ��� üũ�ϰ� �θ� ���� �� ��
    if (left_child<n && s[parent]<s[left_child]) {
        parent = left_child;
    }
    // ������ �ڽ� ��尡 �����ϴ��� üũ�ϰ� �θ� ���� �� ��
    if (right_child<n && s[parent]<s[right_child]) {
        parent = right_child;
    }
    // �����̳� ������ �ڽ� ��� �� �θ� ��庸�� ū ���� ������ ���
    if (i != parent) {
        swap(s, parent, i);
        heapify(s, n, parent); // �ʱ� �θ� ��尡 �� �� ���� ���� ã�ư� ������ ������
    }
}

// �� ����
void heap_sort(int s[], int n) {
    // �ִ� �� ����
    for (int i=(n/2)-1; i>=0; i--) {
        heapify(s, n, i);
    }
    // root ��忡 ��ġ�� �ִ��� ������ ���� �ٲ�
    // �迭(��)�� ũ�⸦ ���� �ٿ����� ����� root ��忡 ���ؼ� ���� �籸��
    for (int i=n-1; i>0; i--) {
        swap(s, 0, i);
        heapify(s, i, 0);
    }
}

// swap �Լ�
void swap(int s[], int x, int y)
{
	int temp = s[x];
	s[x] = s[y];
	s[y] = temp;
}

// ��� ����(LSD ���)
void radix_sort(int s[], int n) {
    int result[n];
    int max = 0;
    int digit = 1;

    for (int i=0; i<n; i++) { // �ִ� ���ϱ�
        if (s[i] > max) {
            max = s[i];
        }
    }
    while (max/digit > 0) { // 1�� �ڸ������� ���
        int bucket[10] = {0,}; // 10���� ������ ���İ������� ����� ��Ŷ �ʱ�ȭ

        for (int i=0; i<n; i++) {
            bucket[(s[i]/digit) % 10]++; // �ڸ����� �ش��ϴ� ��Ŷ�� �ִ� ���� ���� ���ϱ�
        }
        for (int i=1; i<10; i++) {
            bucket[i] += bucket[i-1]; // ��� �迭�� ����� ���� ��Ŷ ������ ���ϱ�
        }
        for(int i=n-1; i>=0; i--) { 
            result[bucket[(s[i]/digit) % 10] - 1] = s[i]; // ���� ���ġ, ���� �ڸ��������� ���� ����
            bucket[(s[i]/digit) % 10]--;
        }
        for (int i=0; i<n; i++) {
            s[i] = result[i]; // �־��� �迭 ����
        }
        digit *= 10; // ���� �ڸ����� �̵�
    }
}