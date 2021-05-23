#include <iostream>
#include <cstdlib>
#include <vector> 

using namespace std;

template <typename T>
class CDA{
   public:
	   CDA();
	   CDA(int s);
      CDA(const CDA<T> &other);
      CDA& operator=(const CDA<T> &c);
      ~CDA();
      T& operator[](int index);
      void AddEnd(T v);
      void AddFront(T v);
      void DelEnd();
      void DelFront();
      int Length();
      int Capacity();
      void Clear();
      T Select(int k);
      void Sort();
      int Search(T e);
      int BinSearch(T e);
      void Reverse();
      void print();
   private:
      int size;
      int capacity;
      int front;
      int back;
      int dir;
      int mid;
      T dummy;
      T *arr;
      void grow();
      void shrink();
      void swap(int i, int j);
      void sortHelper(int l, int r);
      void merge(int l, int m, int r);
      int binarySearch(int l, int r, T e); 
      T selectHelper(int k, vector<T> *A = nullptr);
};

template <typename T>
CDA<T>::CDA(){
	arr = new T[1];
   dummy = arr[0];
   size = 0;
   capacity = 1;
   front = 0;
   back = 0;
   dir = 1;
}

template <typename T>
CDA<T>::CDA(int s){
	arr = new T[s];
   dummy = arr[0];
   size = s;
   capacity = s;
   front = 0;
   back = size-1;
   dir = 1;
}

template <typename T>
CDA<T>::~CDA(){
   delete[] arr;
   arr = nullptr;
}

template <typename T>
CDA<T>::CDA(const CDA<T> &other){
   size = other.size;
   capacity = other.capacity;
   arr = new T[capacity];
   dummy = other.dummy;
   front = other.front;
   back = other.back;
   dir = other.dir;
   for(int i = 0; i < capacity; i++) arr[i] = other.arr[i];
   
}

template <typename T>
T& CDA<T>::operator[](int index){
   if(index >= size || index < 0){
      cout << "Index out of bounds" << endl;
      return dummy;
   }
   if(dir == 1) return arr[(front + index) % capacity];
   else{
      int i = front - index;
      if(i < 0) i += capacity;
      return arr[i];
   }
}

template <typename T>
CDA<T>& CDA<T>::operator=(const CDA<T>& other){
   if(this != &other){
      delete[] arr;
      size = other.size;
      capacity = other.capacity;
      arr = new T[capacity];
      dummy = other.dummy;
      dir = other.dir;
      front = other.front;
      back = other.back;
      for(int i = 0; i < capacity; i++) arr[i] = other.arr[i];
   }
   return *this;
}

template <typename T>
int CDA<T>::Length(){
   return size;
}

template <typename T>
int CDA<T>::Capacity(){
   return capacity;
}

template <typename T>
void CDA<T>::Clear(){
   delete[] arr;
   arr = new T[4];
   size = 0;
   capacity = 4;
   front = 0;
   back = 0;
   dir = 1;

}

template <typename T>
void CDA<T>::grow(){
   int newCap = capacity * 2;
   T *temp = new T[newCap];
   for(int i = 0; i < size; i++) temp[i] = (*this)[i];
   delete[] arr;
   capacity = newCap;
   arr = temp;
   front = dir == 1 ? 0 : size-1;
   back = dir == 1 ? size-1 : 0;
}

template <typename T>
void CDA<T>::shrink(){
   int newCap = capacity / 2;
   T *temp = new T[newCap];
   for(int i = 0; i < size; i++) temp[i] = (*this)[i];
   delete[] arr;
   capacity = newCap;
   arr = temp;
   front = dir == 1 ? 0 : size-1;
   back = dir == 1 ? size-1 : 0;
}

template <typename T>
void CDA<T>::AddEnd(T v){
   if(size == capacity) grow();
   if(dir == 1){
      back = (back + 1) % capacity;
      arr[back] = v;
   }
   else{
      back--;
      if(back < 0) back += capacity;
      arr[back] = v;
   }
   size++;
}

template <typename T>
void CDA<T>::AddFront(T v){
   if(size == capacity) grow();

   if(size == 0){
      AddEnd(v);
      return;
   }
   if(dir == 1){
      front--;
      if (front < 0){
         front += capacity;
      }
      arr[front] = v;
   }
   else{
      front = (front + 1) % capacity;
      arr[front] = v;
   }
   size++;
}

template <typename T>
void CDA<T>::DelEnd(){
   if(size == 0) return;
   size--;
   if(dir == 1){
      back--;
      if(back < 0) back += capacity;

   }
   else{
      back = (back + 1) % capacity;
   }
   int newCap = capacity/2;
   if(newCap >= 4 && size*4 <= capacity) shrink();
}

template <typename T>
void CDA<T>::DelFront(){
   if(size == 0) return;
   if(size == 1){
      DelEnd();
      return;
   }
   size--;
   if(dir == 1){
      front = (front + 1) % capacity;
   }
   else{
      front--;
      if(front < 0) front += capacity;
   }
   int newCap = capacity/2;
   if(newCap >= 4 && size*4 <= capacity) shrink();
   
}


template <typename T>
void CDA<T>::swap(int i, int j){
   T temp;
   temp = (*this)[i];
   (*this)[i] = (*this)[j];
   (*this)[j] = temp;
}

template <typename T>
void CDA<T>::Sort(){
   sortHelper(0, size-1);
}

template <typename T>
void CDA<T>::merge(int l, int m, int r){
   int firstHalf = m - l + 1;
   int secondHalf = r - m;
   int i, j, k;

   int *left = new T[firstHalf], *right = new T[secondHalf];
   for(i = 0; i < firstHalf; i++) left[i] = (*this)[l+i];
   for(j = 0; j < secondHalf; j++) right[j] = (*this)[m+1+j];

   i = j = 0;
   k = l;
   while(i < firstHalf && j < secondHalf){
      if(left[i] <= right[j]){
         (*this)[k] = left[i];
         i++;
      }
      else{
         (*this)[k] = right[j];
         j++;
      }
      k++;
   }
   while(i < firstHalf){
      (*this)[k] = left[i];
      i++;
      k++;
   }
   while(j < secondHalf){
      (*this)[k] = right[j];
      j++;
      k++;
   }
   delete[] left;
   delete[] right;
}

template <typename T>
void CDA<T>::sortHelper(int l, int r){
   if(l < r){
      int m = l + (r - l) / 2;
      sortHelper(l, m);
      sortHelper(m+1, r);

      merge(l, m, r);
   }
}


template <typename T>
int CDA<T>::Search(T e){
   for(int i = 0; i < size; i++){
      if((*this)[i] == e) return i;
   }
   
   return -1;   
}

template <typename T>
int CDA<T>::binarySearch(int l, int r, T e){
   if(r >= l){
      mid = l + (r - l) / 2;
      if((*this)[mid] == e) return mid;
      if((*this)[mid] > e){
         return binarySearch(l, mid-1, e);
      }

      return binarySearch(mid+1, r, e);
   }
   if((mid + 1) < size) return ~(mid+1);
   else return ~size;
}

template <typename T>
int CDA<T>::BinSearch(T e){
   return binarySearch(0, size-1, e);
}

template <typename T> 
void CDA<T>::Reverse(){
   if(size < 2) return;
   
   int temp = front;
   front = back;
   back = temp;
   dir *= -1;

}

template <typename T>
T CDA<T>::Select(int k){
   srand(time(NULL));
   selectHelper(k);
}

template <typename T>
T CDA<T>::selectHelper(int k, vector<T> *A){
   T pivot;
   int n = 0;
   if(A == nullptr){
      pivot = (*this)[rand() % size];
      n = size;
   } 
   else{
      pivot = A->at(rand() % A->size());
      n = A->size();
   }
   vector<T> *L = new vector<T>();vector<T> *E = new vector<T>();vector<T> *G = new vector<T>();

   for(int i = 0; i < n; i++){
      T x;
      if(A == nullptr) x = (*this)[i];
      else x = A->at(i);

      if(x < pivot) L->push_back(x);
      else if (x == pivot) E->push_back(x);
      else G->push_back(x);
   }

   if (k <= L->size()) return selectHelper(k, L);
   else if (k <= L->size() + E->size()) return pivot;
   else return selectHelper(k - L->size() - E->size(), G);
   
   
}

template <typename T>
void CDA<T>::print(){
   for(int i = 0; i < size; i++){
      cout << (*this)[i] << " ";
   }
   cout << endl;

}
