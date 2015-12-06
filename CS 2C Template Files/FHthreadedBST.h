// File FHthreadedBST.h
// Template definitions for FHthreadedBSTs, which are general trees
#ifndef FHTHREADTREE_H
#define FHTHREADTREE_H

// --------------------- FHthreadedNode Prototype -------------------------
template <class Comparable>
class FHthreadedNode
{
public:
   FHthreadedNode( const Comparable & d = Comparable(), 
      FHthreadedNode *l = NULL, 
      FHthreadedNode *r = NULL,
      bool lftThr = true,
      bool rtThr = true,
      int ht = 0)
         : 
         lftChild(l), rtChild(r), data(d), 
         lftThread(lftThr), rtThread(rtThr), height(ht)
   { }

   FHthreadedNode *lftChild, *rtChild;
   Comparable data;
   bool lftThread, rtThread;
   int height;
}; 

// ---------------------- FHthreadedBST Prototype --------------------------
template <class Comparable>
class FHthreadedBST
{
protected:
   int mSize;
   FHthreadedNode<Comparable> *mRoot;

public:
   FHthreadedBST() { mSize = 0; mRoot = NULL; }
   FHthreadedBST(const FHthreadedBST &rhs) 
      { mRoot = NULL; mSize = 0; *this = rhs; }
   ~FHthreadedBST() { clear(); }

   const Comparable &findMin() const;
   const Comparable &findMax() const;
   const Comparable &find(const Comparable &x) const;

   bool empty() const { return (mSize == 0); }
   int size() const { return mSize; }
   void clear() { makeEmpty(mRoot); }
   const FHthreadedBST & operator=(const FHthreadedBST &rhs);

   bool insert(const Comparable &x);
   bool remove(const Comparable &x);
   bool contains(const Comparable &x) const { return find(mRoot, x) != NULL; }

   template <class Processor>
   void traverse(Processor func) const;
   int showHeight() const { return findHeight(mRoot); }

protected:
   void clone( FHthreadedNode<Comparable> *root, 
      FHthreadedBST<Comparable> &newTree);
   FHthreadedNode<Comparable> *findMin(FHthreadedNode<Comparable> *root) const;
   FHthreadedNode<Comparable> *findMax(FHthreadedNode<Comparable> *root) const;
   FHthreadedNode<Comparable> *find(FHthreadedNode<Comparable> *root,
      const Comparable &x) const;
   bool remove(FHthreadedNode<Comparable> * &root, const Comparable &x);
   void makeEmpty(FHthreadedNode<Comparable> * &subtreeToDelete);
   int findHeight(FHthreadedNode<Comparable> *treeNode, int height = -1) const;
   void redirectThreadsPointingToMe(FHthreadedNode<Comparable> *nodeToRemove);
   void adjustParentThreadFlagsAndUnlink( FHthreadedNode<Comparable> *nodeToRemove );

   static FHthreadedNode<Comparable> 
      *successor(FHthreadedNode<Comparable> *node);
   static FHthreadedNode<Comparable> 
      *predecessor(FHthreadedNode<Comparable> *node);

public:
   // for exception throwing
   class EmptyTreeException {};
   class NotFoundException {};
};

// FHthreadedBST public method definitions -----------------------------
template <class Comparable>
const Comparable & FHthreadedBST<Comparable>::findMin() const
{
   if (mRoot == NULL)
      throw EmptyTreeException();
   return findMin(mRoot)->data;
}

template <class Comparable>
const Comparable & FHthreadedBST<Comparable>::findMax() const
{
   if (mRoot == NULL)
      throw EmptyTreeException();
   return findMax(mRoot)->data;
}

template <class Comparable>
const Comparable &FHthreadedBST<Comparable>::find(
   const Comparable &x) const
{ 
   FHthreadedNode<Comparable> *resultNode;
   
   resultNode = find(mRoot, x);
   if (resultNode == NULL)
      throw NotFoundException();
    return resultNode->data;
}

template <class Comparable>
const FHthreadedBST<Comparable> &FHthreadedBST<Comparable>::operator=
   (const FHthreadedBST &rhs)
{
   if (&rhs != this) 
   {
      clear();
      clone(rhs.mRoot, *this);
   }
   return *this;
}

template <class Comparable>
bool FHthreadedBST<Comparable>::remove(const Comparable &x)
{
   if (remove(mRoot, x))
   {
      mSize--;
      return true;
   }
   return false;
}

template <class Comparable>
template <class Processor>
void FHthreadedBST<Comparable>::traverse( Processor func) const
{
   if (mRoot == NULL)
      return;

   FHthreadedNode<Comparable> *node = findMin(mRoot);
   do
   {
      func(node->data);
      node = successor(node);
   } while (node != NULL);
}


// FHthreadedBST private method definitions -----------------------------
template <class Comparable>
void FHthreadedBST<Comparable>::clone( FHthreadedNode<Comparable> *root,
   FHthreadedBST<Comparable> &newTree )
{
   // to overcome complex threading, simply add node into a new tree
   // and let the insert() algorithm naturally set the threads.
   // nodes will go into new tree root in equivalent order as old

   newTree.insert(root->data);
   if ( !(root->lftThread) )
      clone(root->lftChild, newTree);
   if ( !(root->rtThread) )
      clone(root->rtChild, newTree);
}

template <class Comparable>
FHthreadedNode<Comparable> *FHthreadedBST<Comparable>::findMin(
   FHthreadedNode<Comparable> *root) const
{
   if (root == NULL)
      return NULL;
   while ( !(root->lftThread) )
      root = root->lftChild;

   return root;
}

template <class Comparable>
FHthreadedNode<Comparable> *FHthreadedBST<Comparable>::findMax(
   FHthreadedNode<Comparable> *root) const
{
   if (root == NULL)
      return NULL;
   while ( !(root->rtThread) )
      root = root->rtChild;

  return root;
}

template <class Comparable>
FHthreadedNode<Comparable>* FHthreadedBST<Comparable>::find(
   FHthreadedNode<Comparable> *root, const Comparable &x) const
{
   if (root == NULL)
      return NULL;

   while(true) 
   {
      if (root == NULL)
         return NULL;
      if (x < root->data)
      {
         if(root->lftThread)
            return NULL;
         root = root->lftChild;
      }
      else if (root->data < x)
      {
         if(root->rtThread)
            return NULL;
         root = root->rtChild;
      }
      else 
         break;
   }
   return root;
}

template <class Comparable>
bool FHthreadedBST<Comparable>::insert(const Comparable &x)
{
   if (mRoot == NULL)
   {
      mRoot = new FHthreadedNode<Comparable>(x, NULL, NULL, true, true, 0);
      mSize++;
      return true;
   }

   FHthreadedNode<Comparable> *newNode, *parent;
   parent = mRoot;
   while(true) 
   {
      if (x < parent->data)
      {
         if( !(parent->lftThread) )
            parent = parent->lftChild;
         else
         {
            // place as new left child
            newNode = new FHthreadedNode<Comparable>
               (x, parent->lftChild, parent, true, true, 0);
            parent->lftChild = newNode;
            parent->lftThread = false;
            break;
         }
      }
      else if (parent->data < x)
      {
         if( !(parent->rtThread) )
            parent = parent->rtChild;
         else
         {
            // place as new right child
            newNode = new FHthreadedNode<Comparable>
               (x, parent, parent->rtChild, true, true, 0);
            parent->rtChild = newNode;
            parent->rtThread = false;
            break;
         }
      }
      else 
         return false; // duplicate;
   }

   mSize++;
   return true;
}

// very hard to remove recursion from this, so adjust pred/succ links
template <class Comparable>
bool FHthreadedBST<Comparable>::remove(
   FHthreadedNode<Comparable> * &root, const Comparable &x)
{
   if (root == NULL)
      return false;

   if (x < root->data)
      if (root->lftThread)
         return false;
      else
         return remove(root->lftChild, x);
   if (root->data < x)
      if (root->rtThread)
         return false;
      else
         return remove(root->rtChild, x);

   // found the node
   if ( !(root->lftThread) && !(root->rtThread) )
   {
      // two real children
      FHthreadedNode<Comparable> *minNode = findMin(root->rtChild);
      root->data = minNode->data;
      remove(root->rtChild, minNode->data);
   }
   else
   {
      // one or two "fake" children => at least one thread
      FHthreadedNode<Comparable> *nodeToRemove = root;

      // adjust nodeToRemove's in root's subtree that "thread directly up" to nodeToRemove
      redirectThreadsPointingToMe(nodeToRemove);

      // if a full leaf, we have to modify one of parent's thread flags and unlink
      if (nodeToRemove->lftThread && nodeToRemove->rtThread)
      {
         adjustParentThreadFlagsAndUnlink(nodeToRemove);

         // in case this was final node in tree
         if (nodeToRemove->lftChild == NULL && nodeToRemove->rtChild == NULL)
            mRoot = NULL;
      }
      else
         // at least one real child, so we copy to parent
         root = ( !(nodeToRemove->lftThread) )? 
            nodeToRemove->lftChild : nodeToRemove->rtChild;

      // no completely unlinked and adjusted, so safe to remove
      delete nodeToRemove;
   }
   return true;
}

template <class Comparable>
void FHthreadedBST<Comparable>::redirectThreadsPointingToMe( 
   FHthreadedNode<Comparable> *nodeToRemove )
{
   FHthreadedNode<Comparable>  *minNode, *maxNode, *node;

   // adjust nodes in root's subtree that "thread directly up" to root
   minNode = findMin(nodeToRemove);
   maxNode = findMax(nodeToRemove);
   for (node = minNode; node != NULL; node = successor(node))
      if (node->lftThread && node->lftChild == nodeToRemove)
      {
         node->lftChild = predecessor(nodeToRemove);
         break;  // last of only two possible threads pointing up
      }
      else if (node->rtThread && node->rtChild == nodeToRemove)
      {
         node->rtChild = successor(nodeToRemove);
      }
}

// called when both flags are true, meaning one MUST be parent. find out
// which one, so we can set parent's left of right thread flag to true
template <class Comparable>
void FHthreadedBST<Comparable>::adjustParentThreadFlagsAndUnlink( 
   FHthreadedNode<Comparable> *nodeToRemove )
{
   FHthreadedNode<Comparable> *node;

   node = nodeToRemove->rtChild;  // successor is parent?
   if ( node != NULL )
   {
      if ( node->lftChild == nodeToRemove )
      {
         node->lftThread = true;
         node->lftChild = nodeToRemove->lftChild;
      }
   }

   // test both in case mRoot is leaf
   node = nodeToRemove->lftChild;  // predecessor is parent?
   if ( node != NULL )
   {
      if ( node->rtChild == nodeToRemove )
      {
         node->rtThread = true;
         node->rtChild = nodeToRemove->rtChild;
      }
   }
}

template <class Comparable>
void FHthreadedBST<Comparable>::makeEmpty(
   FHthreadedNode<Comparable> * &subtreeToDelete)
{
   if (subtreeToDelete == NULL)
      return;

   // remove children
   if ( !(subtreeToDelete->lftThread) )
      makeEmpty(subtreeToDelete->lftChild);
   if ( !(subtreeToDelete->rtThread) )
      makeEmpty(subtreeToDelete->rtChild);

   // clear client's pointer
   subtreeToDelete = NULL;
   delete subtreeToDelete;
   --mSize;
}

template <class Comparable>
int FHthreadedBST<Comparable>::findHeight( 
   FHthreadedNode<Comparable> *treeNode, int height ) const
{
   int leftHeight, rightHeight;

   if (treeNode == NULL)
      return height;
   height++;
   leftHeight  = treeNode->lftThread ? height 
      : findHeight(treeNode->lftChild, height);
   rightHeight = treeNode->rtThread ? height 
      : findHeight(treeNode->rtChild, height);
   return (leftHeight > rightHeight)? leftHeight : rightHeight;
}

template <class Comparable>
FHthreadedNode<Comparable> *FHthreadedBST<Comparable>::successor(
   FHthreadedNode<Comparable> *node)
{
   if (node == NULL)
      return NULL;

   if (node->rtThread)
      return node->rtChild;
   node = node->rtChild;
   while ( !(node->lftThread) )
      node = node->lftChild;
   return node;
}

template <class Comparable>
FHthreadedNode<Comparable> *FHthreadedBST<Comparable>::predecessor(
   FHthreadedNode<Comparable> *node)
{
   if (node == NULL)
      return NULL;

   if (node->lftThread)
      return node->lftChild;
   node = node->lftChild;
   while ( !(node->rtThread) )
      node = node->rtChild;
   return node;
}

#endif