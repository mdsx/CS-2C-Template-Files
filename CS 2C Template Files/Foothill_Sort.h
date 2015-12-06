// File Foothill_Sort.h
// Template definitions for sorting.  Specifically, include this file
// to use for any array sort when the class overloads the < operator
// Invoke:    arraySort(yourArray, arraySize);
 

// #include "FHvector.h"

// prototypes ----------------------------------------------------
template <typename Comparable>
void mySwap(Comparable &a, Comparable &b);

template <typename Comparable>
bool floatLargestToTop(Comparable data[], int top);

// definitions ---------------------------------------------------
template <typename Comparable>
void arraySort(Comparable array[], int arraySize)
{
    for (int k = 0; k < arraySize; k++)
       if (!floatLargestToTop(array, arraySize-1-k))
          return;
}
// returns true if a modification was made to the array
template <typename Comparable>
bool floatLargestToTop(Comparable data[], int top)
{
   bool changed = false;
   
   // notice we stop at length -2 because of expr. k+1 in loop
   for (int k =0; k < top; k++)
      if (data[k+1] < data[k] )
      {
         mySwap(data[k], data[k+1]);
         changed = true;
      }
   return changed;
}

template <typename Comparable>
void mySwap(Comparable &a, Comparable &b)
{
   Comparable temp;

   temp = a;
   a = b;
   b = temp;
}

template <typename Comparable>
void insertionSort(Comparable array[], int arraySize)
{
   int k, pos;
   Comparable tmp;

   for(pos = 1; pos < arraySize; pos++ )
   {
      tmp = array[pos];
      for(k = pos; k > 0 && tmp < array[k-1]; k-- )
         array[k] = array[k-1];
      array[k] = tmp;
   }
}

// shellsort #1 -- using shell's outer loop
template <typename Comparable>
void shellSort1( Comparable array[], int arraySize )
{
   int k, pos, gap;
   Comparable tmp;
    
   for (gap = arraySize/2;  gap > 0;  gap /= 2)
      for(pos = gap ; pos < arraySize; pos++ )
      {
         tmp = array[pos];
         for(k = pos; k >= gap && tmp < array[k - gap]; k -= gap )
            array[k] = array[k - gap];
         array[k] = tmp;
   }
}

// forward declarations
template <typename Comparable>
void mergeSort(Comparable array[], Comparable workingArray[], int arraySize);

template <typename Comparable>
void merge(Comparable client[], Comparable working[], 
   int rightPos, int arraySize);

template <typename Comparable>
void mergeSort(Comparable array[], int arraySize)
{
   if (arraySize < 2)
      return;

   Comparable *working = new Comparable[arraySize];
   mergeSort(array, working, arraySize);
   delete[] working;
}

// mergeSort internal function
template <typename Comparable>
void mergeSort(Comparable array[], Comparable workingArray[], int arraySize)
{
   int rightStart;

   if (arraySize < 2)
      return;

   rightStart = arraySize/2;
   mergeSort(array, workingArray, rightStart);
   mergeSort(array + rightStart, workingArray, arraySize - rightStart);
   merge(array, workingArray, rightStart, arraySize);
}

// input array 1:  client[0] ... client[rightPos-1]
// input array 2:  client[rightPos] ... client[arraySize - 1]
// working[] array supplied by client to avoid local allocation
template <typename Comparable>
void merge(Comparable client[], Comparable working[], 
   int rightPos, int arraySize)
{
   int leftPos, leftStop, rightStop, workingPos;

   workingPos = leftPos = 0;
   leftStop = rightPos - 1;
   rightStop = arraySize - 1;

   // as soon as we reach the end of either input array, stop
   while(leftPos <= leftStop && rightPos <= rightStop)
      if(client[leftPos] <= client[rightPos])
         working[workingPos++] = client[leftPos++];
      else
         working[workingPos++] = client[rightPos++];

   // merge is over; copy the remainder of one or the other input array
   while(leftPos <= leftStop)
      working[workingPos++] = client[leftPos++];
   while( rightPos <= rightStop )
      working[workingPos++] = client[rightPos++];

   // copy back into client array
   for( ; rightStop >= 0; rightStop-- )
      client[rightStop] = working[rightStop];
}
