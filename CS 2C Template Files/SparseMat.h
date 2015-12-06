#ifndef FHSPARSE_MAT_H
#define FHSPARSE_MAT_H

#include <iostream>
#include <iomanip>
using namespace std;

#include "FHvector.h"
#include "FHlist.h"

template <class Object>
class MatNode
{
protected:
   int col;

public:
   Object data;
   // we need a default constructor for lists
   MatNode(int cl = 0, Object dt = Object()) : col(cl), data(dt) {}
   int getCol() const { return col; }

   // not optimized yet for set() = defaultVal;  partial solution to
   // extra credit that does not correctly handle that one case
   const Object & operator=(const Object &x) { return (data = x); }
};

template <class Object>
class SparseMat
{
protected:
   typedef FHlist<MatNode<Object> > MatRow;
   typedef FHvector<MatRow> MasterCol;
   MasterCol rows;
   Object defaultVal;
   int rowSize, colSize;

public:
   static const int MIN_SIZE = 1;
   SparseMat<Object>(int numRows, int numCols, const Object & defaultVal);
   const Object &get(int r, int c) const;
   bool set(int r, int c, const Object &x);
   MatNode<Object> & set(int r, int c);
   void clear();
   int getRowSize() { return rowSize; }
   int getColSize() { return colSize; }

   // show submatrix, square upper left (start, start)
   void showSubSquare(int start, int size);
private:
   bool valid(int r, int c) const;

public:
   class MatConstructorException { };
   class MatBoundsException { };
};

// constructor
template <class Object>
SparseMat<Object>::SparseMat(int numRows, int numCols,
   const Object & defaultVal)
{
   if (numRows < MIN_SIZE || numCols < MIN_SIZE)
      throw MatConstructorException();
   rowSize = numRows;
   colSize = numCols;
   this->defaultVal = defaultVal;
   // let FHvector resize itself efficiently
   rows.resize(numRows);
}

// basic set():
template <class Object>
bool SparseMat<Object>::set(int r, int c, const Object &x)
{
   typename MatRow::iterator iter, iterStart, iterEnd;
   MatRow *thisRow;

   if (!valid(r, c))
      return false;

   // store into locals for fast access
   thisRow = &rows[r];
   iterStart = thisRow->begin();
   iterEnd = thisRow->end();

   // single loop covers all possibilities
   for (iter = iterStart; iter != iterEnd; iter++)
      if ((*iter).getCol() == c)
      {
         if (x == defaultVal)
            thisRow->erase(iter);
         else
            (*iter).data = x;
         return true;
      }

   // not found
   if (x != defaultVal)
      thisRow->push_back(MatNode<Object>(c, x));
   return true;
}

// extra credit set() - does not provide true sparse mat functionality
template <class Object>
MatNode<Object> & SparseMat<Object>::set(int r, int c)
{
   typename MatRow::iterator iter, iterStart, iterEnd;
   MatRow *thisRow;

   if (!valid(r, c))
      throw MatBoundsException();

   // store into locals for fast access
   thisRow = &rows[r];
   iterStart = thisRow->begin();
   iterEnd = thisRow->end();

   for (iter = iterStart; iter != iterEnd; iter++)
      if ((*iter).getCol() == c)
         return *iter;

   // this is a mutator to add a default element anticipating a quick mutation
   thisRow->push_front(MatNode<Object>(c, defaultVal));
   iter = thisRow->begin();
   return *iter;
}

template <class Object>
const Object &SparseMat<Object>::get(int r, int c) const
{
   typename MatRow::const_iterator iter, iterStart, iterEnd;
   const MatRow *thisRow;

   if (!valid(r, c))
      throw MatBoundsException();

   // store into locals for fast access
   thisRow = &rows[r];
   iterStart = thisRow->begin();
   iterEnd = thisRow->end();

   for (iter = iterStart; iter != iterEnd; iter++)
      if ((*iter).getCol() == c)
         return (*iter).data;
   // not found
   return defaultVal;
}

template <class Object>
void SparseMat<Object>::clear()
{
   int r;

   for (r = 0; r < rowSize; r++)
      rows[r].clear();
}

template <class Object>
bool SparseMat<Object>::valid(int r, int c) const
{
   if (r >= 0 && r < rowSize && c >= 0 && c < colSize)
      return true;
   return false;
}

template <class Object>
void SparseMat<Object>::showSubSquare(int start, int size)
{
   int r, c;

   if (start < 0 || size < 0
      || start + size > rowSize
      || start + size > colSize)
      return;

   for (r = start; r < start + size; r++)
   {
      for (c = start; c < start + size; c++)
      {
         cout.width(3);
         cout.precision(2);
         cout << get(r, c) << " ";
      }
      cout << endl;
   }
   cout << endl << endl;
}

#endif