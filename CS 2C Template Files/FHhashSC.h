// File FHhashSC.h
// Template definitions for FHhashSC.  
// Separate Chaining Hash Table
#ifndef FHHASHSC_H
#define FHHASHSC_H
#include "FHvector.h"
#include "FHlist.h"
#include <cmath>
using namespace std;

// ---------------------- FHhashSC Prototype --------------------------
template <class Object>
class FHhashSC
{
   static const int INIT_TABLE_SIZE = 97;
   static const float INIT_MAX_LAMBDA;
private:
   FHvector<FHlist<Object> > mLists;
   int mSize;
   int mTableSize;
   float mMaxLambda;

public:
   FHhashSC(int tableSize = INIT_TABLE_SIZE);
   bool contains(const Object & x) const;
   void makeEmpty();
   bool insert(const Object & x);
   bool remove(const Object & x);
   static long nextPrime(long n);
   int size() const { return mSize; }
   bool setMaxLambda( float lm ); 

private:
   void rehash();
   int myHash(const Object & x) const;
};

template <class Object>
const float FHhashSC<Object>::INIT_MAX_LAMBDA = 1.5;

// FHhashSC method definitions -------------------
template <class Object>
FHhashSC<Object>::FHhashSC(int tableSize) : mSize(0)
{
   if (tableSize < INIT_TABLE_SIZE)
      mTableSize = INIT_TABLE_SIZE;
   else
      mTableSize = nextPrime(tableSize);

   mLists.resize(mTableSize);
   mMaxLambda = INIT_MAX_LAMBDA;
}

template <class Object>
int FHhashSC<Object>::myHash(const Object & x) const
{
   int hashVal;

   hashVal = Hash(x) % mTableSize;
   if(hashVal < 0)
      hashVal += mTableSize;

   return hashVal;
}

template <class Object>
void FHhashSC<Object>::makeEmpty()
{
   int k, size = mLists.size();

   for(k = 0; k < size; k++)
      mLists[k].clear();
   mSize = 0;
}

template <class Object>
bool FHhashSC<Object>::contains(const Object & x) const
{
   const FHlist<Object> & theList = mLists[myHash(x)];
   typename FHlist<Object>::const_iterator iter;

   for (iter = theList.begin(); iter != theList.end(); iter++)
      if (*iter == x)
         return true;

   // not found
   return false;
}

template <class Object>
bool FHhashSC<Object>::remove(const Object & x)
{
   FHlist<Object> &theList = mLists[myHash(x)];
   typename FHlist<Object>::iterator iter;

   for (iter = theList.begin(); iter != theList.end(); iter++)
      if (*iter == x)
      {
         theList.erase(iter);
         mSize--;
         return true;
      }

   // not found
   return false;
}

template <class Object>
bool FHhashSC<Object>::insert(const Object & x)
{
   typename FHlist<Object>::iterator iter; 
   FHlist<Object> &theList = mLists[myHash(x)];

   for (iter = theList.begin(); iter != theList.end(); iter++)
      if (*iter == x)
         return false;

   // not found so we insert
   theList.push_back(x);

   // check load factor
   if( ++mSize > mMaxLambda * mTableSize )
      rehash();

   return true;
}

template <class Object>
void FHhashSC<Object>::rehash()
{
   FHvector< FHlist<Object> > oldLists = mLists;
   typename FHlist<Object>::iterator iter;
   int k, oldTableSize = mTableSize;

   mTableSize = nextPrime(2*oldTableSize);
   mLists.resize( mTableSize );

   // only the first old_size lists need be cleared
   for(k = 0; k < oldTableSize; k++ )
      mLists[k].clear();

   mSize = 0;
   for(k = 0; k < oldTableSize; k++)
      for(iter = oldLists[k].begin(); iter != oldLists[k].end(); iter++)
         insert(*iter);
}
template <class Object>
bool FHhashSC<Object>::setMaxLambda(float lam)
{ 
   if (lam < .1 || lam > 100)
      return false;
   mMaxLambda = lam;
   return true;
}

template <class Object>
long FHhashSC<Object>::nextPrime(long n)
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