// File FHsort.h
// Template definitions for vector sorting.  Specifically, include this file
// to use for any array sort when the class overloads the < operator

#include "FHvector.h"

// version that takes vector
template <typename Comparable>
void insertionSort( FHvector<Comparable> & a )
{
    int k, pos, arraySize;
    Comparable tmp;
    
    arraySize = a.size();
    for(pos = 1; pos < arraySize; pos++ )
    {
        tmp = a[pos];
        for(k = pos; k > 0 && tmp < a[k - 1]; k--)
            a[k] = a[k - 1];
        a[k] = tmp;
    }
}

// internal ugly version
template <typename Iterator, typename Comparable>
void insertionSort(Iterator begin, Iterator end, Comparable x)
{
   Iterator pos, k;
   Comparable tmp;
    
   for(pos = begin; pos != end; pos++ )
   {
      tmp = *pos;
      for(k = pos; k != begin && tmp < *(k-1); k-- )
         *k = *(k-1);
      *k = tmp;
   }
}

// version that takes two iterators, compatible with STL iterators
template <typename Iterator>
void insertionSort(Iterator begin, Iterator end)
{
   if (begin != end)
      insertionSort(begin, end, *begin);
}

// alternative that allows client to pass type, but requires instantiation
template <class Comparable>
class insertionSortOfType
{
public:
   template <typename Iterator>
   insertionSortOfType(Iterator begin, Iterator end)
   {
      Iterator pos, k;
      Comparable tmp;

      for(pos = begin; pos != end; pos++ )
      {
         tmp = *pos;
         for(k = pos; k != begin && tmp < *(k-1); k-- )
            *k = *(k-1);
         *k = tmp;
      }
   }
};

// version that takes vector and range
template <typename Comparable>
void insertionSort(FHvector<Comparable> & a, int left, int right)
{
    int k, pos;
    Comparable tmp;
    
    for(pos = left + 1; pos <= right; pos++ )
    {
        tmp = a[pos];
        for(k = pos; k > left && tmp < a[k - 1]; k--)
            a[k] = a[k - 1];
        a[k] = tmp;
    }
}

// shellSort #1 -- using shell's outer loop
template <typename Comparable>
void shellSort1( FHvector<Comparable> & a )
{
   int k, pos, arraySize, gap;
   Comparable tmp;
    
   arraySize = a.size();
   for (gap = arraySize/2;  gap > 0;  gap /= 2)  // outer gap loop
      for(pos = gap ; pos < arraySize; pos++ )   // middle loop (outer of "insertion-sort")
      {
         tmp = a[pos];
         for(k = pos; k >= gap && tmp < a[k - gap]; k -= gap )   // inner loop
            a[k] = a[k - gap];
         a[k] = tmp;
   }
}

// mergesort helper, merge
template <typename Comparable>
void merge(FHvector<Comparable> & client, FHvector<Comparable> & working,
   int leftPos, int rightPos, int rightStop)
{
   int leftStop, workingPos, arraySize;

   workingPos = leftPos;
   leftStop = rightPos - 1;
   arraySize = rightStop - leftPos + 1;

   // as soon as we reach the end of either input array, stop
   while(leftPos <= leftStop && rightPos <= rightStop)
      if(client[leftPos] < client[rightPos])
         working[workingPos++] = client[leftPos++];
      else
         working[workingPos++] = client[rightPos++];

   // merge is over; copy the remainder of one or the other input array
   while(leftPos <= leftStop)
      working[workingPos++] = client[leftPos++];
   while( rightPos <= rightStop )
      working[workingPos++] = client[rightPos++];

   // copy back into client array
   for( ; arraySize > 0; arraySize--, rightStop-- )
      client[rightStop] = working[rightStop];
}

// mergesort internal
template <typename Comparable>
void mergeSort(FHvector<Comparable> & a, FHvector<Comparable> & working,
   int start, int stop)
{
   int rightStart;

   if (stop - start < 1)
      return;

   rightStart = (start + stop)/2 + 1;
   mergeSort(a, working, start, rightStart - 1);
   mergeSort(a, working, rightStart, stop);
   merge(a, working, start, rightStart, stop);
}

// mergesort public driver 
template <typename Comparable>
void mergeSort(FHvector<Comparable> & a)
{
   if (a.size() < 2)
      return;

   FHvector<Comparable> working(a.size());
   mergeSort(a, working, 0, a.size() - 1);
}

template <typename Comparable>
void percolateDown(FHvector<Comparable> & inArray, int hole, int arraySize)
{ 
   int child;
   Comparable tmp;

   for( tmp = inArray[hole]; 2 * hole + 1 < arraySize; hole = child )
   {
      child = 2 * hole + 1;
      // if 2 children, get the GREATER of the two (because MAX heap)
      if( child < arraySize - 1 && inArray[child] < inArray[child + 1])
         child++;
      if( tmp < inArray[child] )   // MAX heap, not min heap
         inArray[hole] = inArray[child];
      else
         break;
   }
   inArray[hole] = tmp;
}

template <typename Comparable>
inline void mySwapFH(Comparable &x, Comparable &y)
{
   Comparable tmp = x;
   x = y;
   y = tmp;
}

template <typename Comparable>
void heapSort(FHvector<Comparable> & inArray)
{
   int k, arraySize;

   // order the array using percolate down
   arraySize = inArray.size();
   for(k = arraySize/2; k >= 0; k-- )
      percolateDown(inArray, k, arraySize);

   // now remove the max element (root) and place at end of array
   for(k = arraySize - 1; k > 0; k-- )
   {
      mySwapFH(inArray[0], inArray[k]); // "remove" by placing at end of array
      percolateDown( inArray, 0, k );  // k represents the shrinking array size
   }
}

// quickSort helper
// median3 sorts a[left], a[center] and a[right].
// it leaves the smallest in a[left], the largest in a[right]
// and median (the pivot) is moved "out-of-the-way" in a[right-1].
// (a[center] has what used to be in a[right-1])
template <typename Comparable>
const Comparable & median3(FHvector<Comparable> & a, int left, int right)
{
   int center;

   center = (left + right) / 2;
   if(a[center] < a[left])
      mySwapFH(a[left], a[center]);
   if(a[right] < a[left])
     mySwapFH( a[left], a[right] );
   if(a[right] < a[center])
      mySwapFH(a[center], a[right]);

   mySwapFH(a[center], a[right - 1]);
   return a[right - 1];
}

#define QS_RECURSION_LIMIT 15

// quickSort internal
template <typename Comparable>
void quickSort(FHvector<Comparable> & a, int left, int right)
{
   Comparable pivot;
   int i, j;

   if( left + QS_RECURSION_LIMIT <= right )
   {
      pivot = median3(a, left, right);
      for(i = left, j = right - 1; ; )
      {
         while( a[++i] < pivot )
            ;
         while( pivot < a[--j])
            ;
         if(i < j)
            mySwapFH(a[i], a[j]);
         else
            break;
      }

      mySwapFH(a[i], a[right - 1]);  // restore pivot

      // recursive calls on smaller sub-groups
      quickSort(a, left, i - 1);     
      quickSort(a, i + 1, right);    
   }
   else
      // non-recursive escape valve - insertionSort
      insertionSort(a, left, right);
}

// quickSort public driver
template <typename Comparable>
void quickSort( FHvector<Comparable> & a )
{
    quickSort(a, 0, a.size() - 1);
}

// smart pointer for indirect sort
template <typename Comparable>
class SmartPointer
{
private:
   Comparable *pointee;

public:
   Comparable * operator=(Comparable *rhs);
   SmartPointer<Comparable> & operator=(const SmartPointer<Comparable>  &rhs);
   bool operator<( const SmartPointer<Comparable> & rhs ) const;
   bool operator!=(const SmartPointer<Comparable> & rhs) const;
   bool operator!=(const Comparable *rhs) const;
   int operator-(const Comparable *rhs) const;
   Comparable operator*() const  { return *pointee; }
};

// SmartPointer method definitions ----------------
template <typename Comparable>
Comparable * SmartPointer<Comparable>::operator=(Comparable *rhs)
{
   return (pointee = rhs);
}

template <typename Comparable>
SmartPointer<Comparable> & SmartPointer<Comparable>::operator=(
   const SmartPointer<Comparable>  &rhs)
{
   pointee = rhs.pointee;
   return *this;
}

template <typename Comparable>
bool SmartPointer<Comparable>::operator<( const SmartPointer<Comparable> & rhs ) const
{
   return *pointee < *rhs.pointee;
}

template <typename Comparable>
bool SmartPointer<Comparable>::operator!=(const SmartPointer<Comparable> & rhs) const
{
   return pointee != rhs.pointee;
}

template <typename Comparable>
bool SmartPointer<Comparable>::operator!=(const Comparable *rhs) const
{
   return pointee != rhs;
}

template <typename Comparable>
int SmartPointer<Comparable>::operator-(const Comparable *rhs) const
{
   return pointee - rhs;
}

// indirect sort - uses SmartPointer as intermediate type
template <typename Comparable>
void indirectSort( FHvector<Comparable> & a )
{
   int k, j, nextJ, arraySize = a.size();
   Comparable tmp;
   FHvector< SmartPointer<Comparable> > p(arraySize);

   // copy smart pointer to the client array
   for( k = 0; k < arraySize; k++ )
      p[k] = &a[k];

   // do the sort - it only changes the smart pointer order
   quickSort(p);

   // untangle the client elements so they match the pointer order
   for(k = 0; k < a.size( ); k++ )
      if( p[k] != &a[k] )
      {
         tmp = a[k];
         for(j = k; p[j] != &a[k]; j = nextJ)
         {
            nextJ = p[j] - &a[0];
            a[j] = *p[j];
            p[j] = &a[j];
         }
         a[j] = tmp;
         p[j] = &a[j];
      }
}
