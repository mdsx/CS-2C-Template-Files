// File FHhashQP.h
// Template definitions for FHhashQP.  
// Quadratic Probing Hash Table
#ifndef FHHASHQP_H
#define FHHASHQP_H
#include "FHvector.h"
#include <cmath>
using namespace std;

// ---------------------- FHhashQP Prototype --------------------------
template <class Object>
class FHhashQP
{
protected:
   static const int INIT_TABLE_SIZE = 7;
   static const float INIT_MAX_LAMBDA;

   enum ElementState { ACTIVE, EMPTY, DELETED };
   class HashEntry;

   FHvector<HashEntry> mArray;
   int mSize;
   int mLoadSize;
   int mTableSize;
   float mMaxLambda;

public:
   FHhashQP(int tableSize = INIT_TABLE_SIZE);
   bool contains(const Object & x) const;
   void makeEmpty();
   bool insert(const Object & x);
   bool remove(const Object & x);
   static long nextPrime(long n);
   int size() const { return mSize; }
   bool setMaxLambda( float lm ); 

protected:
   void rehash();
   int myHash(const Object & x) const;
   int findPos( const Object & x ) const;
};

template <class Object>
const float FHhashQP<Object>::INIT_MAX_LAMBDA = 0.49F;

// definition of nested FHhashQP<Object>::HashEntry class ----------------
template <class Object>
class FHhashQP<Object>::HashEntry
{
public:
   Object data;
   ElementState state;
   HashEntry( const Object & d = Object(), ElementState st = EMPTY )
      : data(d), state(st)
   { }
};

// FHhashQP method definitions -------------------
template <class Object>
FHhashQP<Object>::FHhashQP(int tableSize) : mSize(0), mLoadSize(0)
{
   if (tableSize < INIT_TABLE_SIZE)
      mTableSize = INIT_TABLE_SIZE;
   else
      mTableSize = nextPrime(tableSize);
   mArray.resize(mTableSize);
   makeEmpty();
   mMaxLambda = INIT_MAX_LAMBDA;
}

template <class Object>
int FHhashQP<Object>::myHash(const Object & x) const
{
   int hashVal;

   hashVal = Hash(x) % mTableSize;
   if(hashVal < 0)
      hashVal += mTableSize;

   return hashVal;
}

template <class Object>
void FHhashQP<Object>::makeEmpty()
{
   int k, size = mArray.size();

   for(k = 0; k < size; k++)
      mArray[k].state = EMPTY;
   mSize = mLoadSize = 0;
}

template <class Object>
bool FHhashQP<Object>::contains(const Object & x) const
{
   return mArray[findPos(x)].state == ACTIVE;
}

template <class Object>
bool FHhashQP<Object>::remove(const Object & x)
{
   int bucket = findPos(x);

   if ( mArray[bucket].state != ACTIVE )
      return false;

   mArray[bucket].state = DELETED;
   mSize--;      // mLoadSize not dec'd because it counts any non-EMP location
   return true;
}

template <class Object>
bool FHhashQP<Object>::insert(const Object & x)
{
   int bucket = findPos(x);

   if ( mArray[bucket].state == ACTIVE )
      return false;

   mArray[bucket].data = x;
   mArray[bucket].state = ACTIVE;
   mSize++;

   // check load factor 
   if( ++mLoadSize > mMaxLambda * mTableSize )
      rehash();

   return true;
}
template <class Object>
int FHhashQP<Object>::findPos( const Object & x ) const
{
   int kthOddNum = 1;
   int index = myHash(x);

   while ( mArray[index].state != EMPTY
      && mArray[index].data != x )
   {
      index += kthOddNum;  // k squared = (k-1) squared + kth odd #
      kthOddNum += 2;   // compute next odd #
      if ( index >= mTableSize )
         index -= mTableSize;
   }

   return index;
}

template <class Object>
void FHhashQP<Object>::rehash()
{
   FHvector<HashEntry> oldArray = mArray;
   int k, oldTableSize = mTableSize;

   mTableSize = nextPrime(2*oldTableSize);
   mArray.resize( mTableSize );
   for(k = 0; k < mTableSize; k++)
      mArray[k].state = EMPTY;

   mSize = mLoadSize = 0;
   for(k = 0; k < oldTableSize; k++)
      if (oldArray[k].state == ACTIVE)
         insert( oldArray[k].data );
}
template <class Object>
bool FHhashQP<Object>::setMaxLambda(float lam)
{ 
   if (lam < .1 || lam > .49)
      return false;
   mMaxLambda = lam;
   return true;
}

template <class Object>
long FHhashQP<Object>::nextPrime(long n)
{
   long k, candidate, loopLim;

   // loop doesn't work for 2 or 3
   if (n <= 2 )
      return 2;
   else if (n == 3)
      return 3;

   for (candidate = (n%2 == 0)? n+1 : n ; true ; candidate += 2)
   {
      // all primes > 3 are of the form 6k +/- 1
      loopLim = (long)( (sqrt((float)candidate) + 1)/6 );

      // we know it is odd.  check for divisibility by 3
      if (candidate%3 == 0)
         continue;

      // now we can check for divisibility of 6k +/- 1 up to sqrt
      for (k = 1; k <= loopLim; k++)
      {
         if (candidate % (6*k - 1) == 0)
            break;
         if (candidate % (6*k + 1) == 0)
            break;
      }
      if (k > loopLim)
         return candidate;
   }
}

#endif