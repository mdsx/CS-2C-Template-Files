// File FHkruskal.h
// Template definitions for FHgraph.  
#ifndef FHKRUSKAL_H
#define FHKRUSKAL_H
#include "FHgraph.h"
#include <iostream>
#include <queue>
#include <functional>

template <class Object, typename CostType>
class FHkruskal
{
   // internal typedefs to simplify syntax
   typedef FHvertex<Object, CostType> Vertex;
   typedef Vertex* VertPtr;
   typedef map<VertPtr, CostType> EdgePairList;
   typedef set<VertPtr> VertPtrSet;
   typedef vector<VertPtrSet> Forest;
   typedef FHedge<Object, CostType>  Edge;
   typedef Edge * EdgePtr;
   typedef FHgraph<Object, CostType> Graph;

   // how to make priority_queue a min heap rather than max heap
   // it requires > operator is defined for the underlying Edge class
   typedef priority_queue<Edge, vector<Edge>, greater<Edge> > EdgeHeap;

private:
   EdgeHeap edgeHeap;
   const Graph *inGraph;

public:
   FHkruskal (Graph const & grph) { setInGraph(grph); }
   FHkruskal () : inGraph(NULL) {}
   void clear();
   void setInGraph(Graph const & grph)
   { 
      inGraph = &grph;
      clear();
   }

   // algorithms
   bool genKruskal(Graph &outGraph);

private:
   bool buildEdgeHeap();
   static void setUnion(VertPtrSet &a, VertPtrSet &b);
   static bool memberOf(VertPtr v, VertPtrSet &vset);
};

template <class Object, typename CostType>
bool FHkruskal<Object, CostType>::genKruskal(Graph &outGraph)
{
   typename VertPtrSet::iterator iter;
   typename Forest::iterator fIter, fIterSrc, fIterDst;
   Forest vertexSets;
   VertPtrSet vertsInGraph, singleton;
   Edge smallestEdge;
   VertPtr src, dst;
   vector<Edge> newEdges;
   int k, numVertsFound;

   // can't use same Graph for input and output
   if (inGraph == NULL || inGraph == &outGraph)
      return false;

   // get a local list of vertices
   vertsInGraph = inGraph->getVertPtrSet();

   // form a forest of sets, initializing each with only one vertex from the graph
   for (k = 0, iter = vertsInGraph.begin(); 
      iter != vertsInGraph.end(); iter++, k++)
   {
      singleton.clear(); 
      singleton.insert(*iter);
      vertexSets.push_back(singleton);
   }

   // form a binary min heap of edges so we can easily find min costs
   if (!buildEdgeHeap())
      return false;

   // test for empty to avoid inf. loop resulting from disconnected graph
   while (!edgeHeap.empty() && vertexSets.size() > 1)
   {
      // pop smallest edge left in heap
      smallestEdge = edgeHeap.top();
      src = smallestEdge.source;
      dst = smallestEdge.dest;
      edgeHeap.pop();

      // see if src and dst are in different sets.  if so, take union
      for (fIter = vertexSets.begin(), numVertsFound = 0 ; 
         fIter != vertexSets.end()  &&  (numVertsFound < 2) ; 
         fIter++)
      {
         if ( memberOf(src, *fIter) )
         {
            fIterSrc = fIter;
            numVertsFound++;
         }

         if ( memberOf(dst, *fIter) )
         {
            fIterDst = fIter;
            numVertsFound++;
         }
      }
      if (fIterSrc == fIterDst)  // same sets: reject
         continue;

      newEdges.push_back(smallestEdge);
      setUnion( *fIterSrc, *fIterDst );
      vertexSets.erase(fIterDst);
   }

   outGraph.setGraph(newEdges);
   return true;
}

template <class Object, typename CostType>
void FHkruskal<Object, CostType>::clear()
{
   while (!edgeHeap.empty())
      edgeHeap.pop();
}

// puts the union of sets a and b into a.  b unchanged
template <class Object, typename CostType>
void FHkruskal<Object, CostType>::setUnion(
   VertPtrSet &a, VertPtrSet &b)
{
   typename VertPtrSet::iterator iter;
   for (iter = b.begin(); iter != b.end(); iter++)
      a.insert(*iter);
}

template <class Object, typename CostType>
bool FHkruskal<Object, CostType>::memberOf(
   VertPtr v, VertPtrSet &vset)
{
   return ( vset.find(v) != vset.end() );
}

template <class Object, typename CostType>
bool FHkruskal<Object, CostType>::buildEdgeHeap()
{
   VertPtrSet vertsInGraph;
   typename VertPtrSet::iterator vertIter;
   typename EdgePairList::iterator adjListIter;
   VertPtr src, dst;
   CostType cost;
   
   if (inGraph == NULL)
      return false;
   vertsInGraph = inGraph->getVertPtrSet();

   for (vertIter = vertsInGraph.begin(); 
      vertIter != vertsInGraph.end(); vertIter++)
   {
      src =  (*vertIter);
      for (adjListIter = src->adjList.begin(); 
         adjListIter != src->adjList.end(); ++adjListIter)
      {
         dst = (*adjListIter).first;
         cost = (*adjListIter).second;
         edgeHeap.push( Edge(src, dst, cost) );
      }
   }
   return true;
}
#endif
