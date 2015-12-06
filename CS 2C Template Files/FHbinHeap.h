// File FHbinHeap.h
// Template definitions for FHbinHeap.  
// Binary heaps for priority queues and heap sorts
#ifndef FHBINHEAP_H
#define FHBINHEAP_H
#include "FHvector.h"
using namespace std;

// ---------------------- FHbinHeap Prototype --------------------------
template <class Comparable>
class FHbinHeap
{
   static const int INIT_CAPACITY = 64; // perfect tree of size 63

private:
   FHvector<Comparable> mArray;
   int mSize;
   int mCapacity;

public:
   FHbinHeap(int capacity = INIT_CAPACITY);
   FHbinHeap(const FHvector<Comparable> & items );
   bool empty() const { return mSize == 0; }
   void makeEmpty() { mSize = 0; };
   void insert(const Comparable & x);
   Comparable & remove();

   int size() const { return mSize; }

   // for exception throwing
   class HeapEmptyException { };

private:
   void orderHeap();
   void percolateDown( int hole );
};

// FHbinHeap method definitions -------------------
template <class Comparable>
FHbinHeap<Comparable>::FHbinHeap(int capacity)
{
   // choose a capacity that is exactly 2^N - 1 for some N (esthetic)
   // which leads to mCapacity 2^N, internally -- user asks for 127, we
   // reserve 128, if they want 128, we have to reserve 256.
   for (mCapacity = INIT_CAPACITY; 
      mCapacity <= capacity;
      mCapacity = 2 * mCapacity
      )
   {
      if (mCapacity < 0)
      {
         mCapacity = INIT_CAPACITY; // give up - overflow
         break;
      }
   }
   mArray.resize(mCapacity);
   makeEmpty();
}

template <class Comparable>
FHbinHeap<Comparable>::FHbinHeap( const FHvector<Comparable> & items )
: mSize(items.size())
{
   int k;

   // find the first perfect tree size > items.size() and INIT_CAPACITY
   for (mCapacity = INIT_CAPACITY; 
      mCapacity <= mSize;
      mCapacity = 2 * mCapacity
      )
   {
      if (mCapacity < 0)
         mCapacity = mSize + 1; // give up - overflow, and not perfect
   }
   mArray.resize(mCapacity);

   // copy starting with position 1 - no ordering yet
   for(k = 0; k < mSize; k++ )
      mArray[k+1] = items[k];

   // order the heap
   orderHeap( );
}

template <class Comparable>
void FHbinHeap<Comparable>::insert(const Comparable & x)
{
   int hole;

   if( mSize == mCapacity - 1 )
   {
      mCapacity = 2 * mCapacity;
      mArray.resize(mCapacity);
   }

   // percolate up
   hole = ++mSize;
   for( ; hole > 1 && x < mArray[hole/2]; hole /= 2 )
      mArray[hole] = mArray[hole/2];
   mArray[hole] = x;
}

template <class Comparable>
Comparable & FHbinHeap<Comparable>::remove()
{
   static Comparable minObject;

   if( empty() )
      throw HeapEmptyException();

   minObject = mArray[1];

   mArray[1] = mArray[mSize--];
   percolateDown(1);

   return minObject;
}

template <class Comparable>
void FHbinHeap<Comparable>::percolateDown(int hole)
{ 
   int child;
   Comparable tmp;

   for( tmp = mArray[hole]; 2 * hole <= mSize; hole = child )
   {
      child = 2 * hole;
      // if 2 children, get the lesser of the two
      if( child < mSize && mArray[child + 1] < mArray[child] )
         child++;
      if( mArray[child] < tmp )
         mArray[hole] = mArray[child];
      else
         break;
   }
   mArray[hole] = tmp;
}

template <class Comparable>
void FHbinHeap<Comparable>::orderHeap()
{
   int k;

   for(k = mSize/2; k > 0; k-- )
      percolateDown(k);
}

#endif