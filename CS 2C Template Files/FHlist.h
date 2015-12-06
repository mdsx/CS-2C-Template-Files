// File FHlist (FHlist_XC.h)
// Xcode Safe: iterator classes defined in-line, not forward/external
// Template definitions for FHlists.  Specifically, include this file
// to create FHlist classes in a manner similar to STD lists
#ifndef FHLIST_H
#define FHLIST_H
#include <stdlib.h>
// ---------------------- FHlist Prototype --------------------------
template <class Object>
class FHlist
{
private:
   // Node prototype - this nested template is defined outside
   class Node;

   // private data for FHlist
   int mSize;
   Node *mHead;
   Node *mTail;

public:
   FHlist()
   {
      init();
   }
   ~FHlist()
   {
      clear(); delete mHead; delete mTail;
   }
   bool empty() const
   {
      return (mSize == 0);
   }
   int size() const
   {
      return mSize;
   }
   void clear();
   void pop_front();
   void pop_back();
   void push_front( const Object &x );
   void push_back( const Object &x );
   Object & front()
   {
      return mHead->next->data;
   }
   const Object & front() const
   {
      return mHead->next->data;
   }
   Object & back()
   {
      return mTail->prev->data;
   }
   const Object & back() const
   {
      return mTail->prev->data;
   }

   // const_iterator nested class -------------------------------------
   class const_iterator
   {
      friend class FHlist;

   protected:
      // protected member data
      Node *mCurrent;
      const FHlist *mMyList;  // needed to test for certain errors

      // protected constructor for use only by derived iterator and friends
      const_iterator( Node *p, const FHlist<Object> &lst ) : mCurrent( p ), mMyList( &lst )
      {}

   public:
      const_iterator() : mCurrent( NULL ), mMyList( NULL )
      {}

      const Object &operator*() const
      {
         if ( !mCurrent )
            throw NullIteratorException();
         return mCurrent->data;
      }

      const_iterator & operator++()
      {
         if ( mCurrent->next != NULL )
            mCurrent = mCurrent->next;
         return *this;
      }

      const_iterator & operator--(int)
      {
         static const_iterator old;
         old = *this;
         if ( mCurrent->prev != NULL )
            mCurrent = mCurrent->prev;
         return old;
      }

      const_iterator & operator--()
      {
         if ( mCurrent->prev != NULL )
            mCurrent = mCurrent->prev;
         return *this;
      }

      const_iterator & operator++(int)
      {
         static const_iterator old;
         old = *this;
         if ( mCurrent->next != NULL )
            mCurrent = mCurrent->next;
         return old;
      }

      bool operator==(const const_iterator &rhs) const
      {
         return mCurrent == rhs.mCurrent;
      }
      bool operator!=(const const_iterator &rhs) const
      {
         return !(*this == rhs);
      }
   };
   // ----------------------------------------------------------

   // iterator nested class -------------------------------------
   class iterator : public FHlist<Object>::const_iterator
   {
      friend class FHlist;
   protected:
      // chain to base class
      iterator( Node *p, const FHlist<Object> & lst ) : const_iterator( p, lst )
      {}

   public:
      iterator()
      {}
      const Object &operator*() const
      {
         return const_iterator::operator*();
      }
      Object &operator*()
      {
         if ( !this->mCurrent )
            throw NullIteratorException();
         return this->mCurrent->data;
      }
      iterator & operator++()
      {
         if ( this->mCurrent->next != NULL )
            this->mCurrent = this->mCurrent->next;
         return *this;
      }
      iterator & operator++(int)
      {
         static iterator old;
         old = *this;
         if ( this->mCurrent->next != NULL )
            this->mCurrent = this->mCurrent->next;
         return old;
      }
      iterator & operator--()
      {
         if ( this->mCurrent->prev != NULL )
            this->mCurrent = this->mCurrent->prev;
         return *this;
      }
      iterator & operator--(int)
      {
         static iterator old;
         old = *this;
         if ( this->mCurrent->prev != NULL )
            this->mCurrent = this->mCurrent->prev;
         return old;
      }
   };
   // ----------------------------------------------------------

   // for exception throwing
   class NullIteratorException
   {};
   class IteratorMismatchException
   {};

   const_iterator begin() const
   {
      return const_iterator( mHead->next, *this );
   }
   const_iterator end() const
   {
      return const_iterator( mTail, *this );
   }
   iterator begin()
   {
      return iterator( mHead->next, *this );
   }
   iterator end()
   {
      return iterator( mTail, *this );
   }

   const FHlist & operator=(const FHlist & rhs);
   FHlist( const FHlist &rhs )
   {
      init(); *this = rhs;
   }

   // syntax too difficult to define outside
   iterator insert( iterator iter, const Object &x )
   {
      if ( iter.mMyList != this )
         throw IteratorMismatchException();

      Node *p = iter.mCurrent;
      if ( p == NULL )
         throw NullIteratorException();
      if ( p->prev == NULL )
         throw NullIteratorException();

      // build a node around x and link it up
      Node *newNode = new Node( x, p->prev, p );
      p->prev->next = newNode;
      p->prev = newNode;
      iterator newIter( newNode, *this );
      mSize++;
      return newIter;
   }

   iterator erase( iterator iter )
   {
      if ( iter.mMyList != this )
         throw IteratorMismatchException();

      Node *p = iter.mCurrent;
      if ( p == NULL )
         throw NullIteratorException();
      if ( p->prev == NULL || p->next == NULL )
         throw NullIteratorException();

      iterator retVal( p->next, *this );
      p->prev->next = p->next;
      p->next->prev = p->prev;
      delete p;
      mSize--;

      return retVal;
   }

   iterator erase( iterator start, iterator stop )
   {
      if ( start.mMyList != this || stop.mMyList != this )
         throw IteratorMismatchException();
      if ( start.mCurrent == NULL || stop.mCurrent == NULL )
         throw NullIteratorException();
      if ( start.mCurrent->prev == NULL || start.mCurrent->next == NULL
         || stop.mCurrent->prev == NULL )
         throw NullIteratorException();

      for ( iterator iter = start; iter != stop; )
         iter = erase( iter );  // if this throws, then mCurrent erase() will throw, too

      return stop;
   }

private:
   void init();
};

// FHlist method definitions -------------------
// private utilities for member methods
template <class Object>
void FHlist<Object>::init()
{
   mSize = 0;
   mHead = new Node;
   mTail = new Node;
   mHead->next = mTail;
   mTail->prev = mHead;
}

// public interface
template <class Object>
void FHlist<Object>::clear()
{
   while ( mSize > 0 )
      pop_front();
}

template <class Object>
void FHlist<Object>::pop_front()
{
   Node *p;

   // safer, but a little slower with this test
   if ( mSize == 0 )
      return;

   p = mHead->next;
   mHead->next = p->next;
   mHead->next->prev = mHead;
   delete p;
   mSize--;
}

template <class Object>
void FHlist<Object>::pop_back()
{
   Node *p;

   // safer, but a little slower with this test
   if ( mSize == 0 )
      return;

   p = mTail->prev;
   mTail->prev = p->prev;
   mTail->prev->next = mTail;
   delete p;
   mSize--;
}

template <class Object>
void FHlist<Object>::push_front( const Object &x )
{
   Node *p = new Node( x, mHead, mHead->next );
   mHead->next->prev = p;
   mHead->next = p;
   mSize++;
}

template <class Object>
void FHlist<Object>::push_back( const Object &x )
{
   Node *p = new Node( x, mTail->prev, mTail );
   mTail->prev->next = p;
   mTail->prev = p;
   mSize++;
}

template <class Object>
const FHlist<Object> & FHlist<Object>::operator=(const FHlist & rhs)
{
   const_iterator iter;
   if ( &rhs == this )
      return *this;
   clear();
   for ( iter = rhs.begin(); iter != rhs.end(); ++iter )
      this->push_back( *iter );
   return *this;
}

// definition of nested FHlist<Object>::Node class ----------------------
template <class Object>
class FHlist<Object>::Node
{
public:
   Node *prev, *next;
   Object data;

   Node( const Object & d = Object(), Node *prv = NULL, Node *nxt = NULL )
      : prev( prv ), next( nxt ), data( d )
   {}
};

#endif
