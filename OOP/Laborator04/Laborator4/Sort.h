class Sort {
private:
    void swap(int&, int&);
    void reverseArray();
    void quickSort(int, int);
    int partition(int, int);

public:
    int *v, n;

    Sort(int, int, int);
    Sort(int* a, int b);
    Sort(int, ...);
    Sort(const char*);

    void InsertSort(bool reverse = false);
    void QuickSort(bool reverse = false);
    void BubbleSort(bool reverse = false);

    void Print();
    int  GetElementsCount();
    int  GetElementFromIndex(int index);
};