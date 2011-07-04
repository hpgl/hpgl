

//---------------------------------------------------------------------
//
//                    Last modifed: Dec. 2, 2000
//
//The program is part of the term project of class Advanced Programming
//in 2000 Fall. The advisor is Prof. David Musser.
//
//The purpose of program is to search the approximate nearest neighbor 
//of given query points in refernce points.
//
//The program modifies part of C++ source code of Sunil Arya and David Mount,
//and rewrite into STL style.
//
//Here are their declaration.
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//      Programmer:     Sunil Arya and David Mount
//      Last modified:  03/04/98 (Release 0.1)
//      Description:    test program for ANN (approximate nearest neighbors)
//
//----------------------------------------------------------------------
// Copyright (c) 1997-1998 University of Maryland and Sunil Arya and David
// Mount.  All Rights Reserved.
// 
// This software and related documentation is part of the 
// Approximate Nearest Neighbor Library (ANN).
// 
// Permission to use, copy, and distribute this software and its 
// documentation is hereby granted free of charge, provided that 
// (1) it is not a component of a commercial product, and 
// (2) this notice appears in all copies of the software and
//     related documentation. 
// 
// The University of Maryland (U.M.) and the authors make no representations
// about the suitability or fitness of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
//----------------------------------------------------------------------

#ifndef __ANN_KD_LIBRARY_H__
#define __ANN_KD_LIBRARY_H__

#include <stdlib.h>                     // standard libs
#include <stdio.h>                      // standard I/O (for NULL)
#include <iostream>                   // I/O streams
#include <cmath>                       // math includes
#include <iomanip>                    // I/O manipulators

#define INFDISTANCE 1e100         //define infinity distance.

int     PQ_NULL_INFO = -1;              // nonexistent info value

// ANN boolean type
enum ANNbool {ANNfalse = 0, ANNtrue = 1};

enum    {LO=0, HI=1};                   // splitting indices
enum    {IN=0, OUT=1};                  // shrinking indices

enum DISTANCE_norm {
        EUCLIDEAN,
        MANHATTAN,
        MAX} norm;

// standard 2-d indirect indexing
#define PA(i,d)         (pa[pidx[(i)]][(d)])
// accessing a single point
#define PP(i)           (pa[pidx[(i)]])

// swap two points in pa array
#define PASWAP(a,b) { int tmp = pidx[a];\
                    pidx[a] = pidx[b];\
                    pidx[b] = tmp; }

static int IDX_TRIVIAL[] = {0}; // trivial point index


enum ANNerr {ANNwarn = 0, ANNabort = 1};

void annError(char *msg, ANNerr level)
{
    if (level == ANNabort) {
        std::cerr << "ANN: ERROR------->" << msg 
                << "<-------------ERROR\n";
        exit(1);
    }
    else {
        std::cerr << "ANN: WARNING----->" << msg 
                << "<-------------WARNING\n";
    }
}


template <typename ANNcoord,typename ANNdist,typename ANNidx> class ANNpointSet;
template <typename ANNcoord,typename ANNdist,typename ANNidx> class ANNkd_node;
template <typename ANNcoord,typename ANNdist,typename ANNidx> class ANNkd_split;
template <typename ANNcoord,typename ANNdist,typename ANNidx> class ANNkd_leaf;
template <typename ANNcoord> class ANNorthRect;
template <typename ANNcoord,typename ANNdist,typename ANNidx> class ANNkd_tree;


template <typename ANNdist> class ANNmin_k;


// recursive construction of kd-tree
template <typename ANNcoord,typename ANNidx,typename ANNdist,
        typename SPLIT_FUNCTION_OBJECT>
ANNkd_node<ANNcoord,ANNdist,ANNidx>* rkd_tree(
    ANNcoord**  pa,     // point array (unaltered)
    ANNidx*     pidx,   // point indices to store in subtree
    int         n,      // number of points
    int         dim,    // dimension of space
    int         bsp,    // bucket space
    ANNorthRect<ANNcoord> &bnd_box, // bounding box for current node
    SPLIT_FUNCTION_OBJECT splitter);    // splitting routine

template <typename ANNcoord,typename ANNidx>
void annEnclRect( // compute smallest enclosing rectangle
    ANNcoord**  pa,             // point array
    ANNidx*     pidx,           // point indices
    int         n,              // number of points
    int         dim,            // dimension
    ANNorthRect<ANNcoord> &bnds); // bounding cube (returned)

template <typename ANNcoord,typename ANNidx>
ANNcoord annSpread(     // compute point spread along dimension
    ANNcoord**  pa,     // point array
    ANNidx*     pidx,   // point indices
    int         n,      // number of points
    int         d);     // dimension to check

template <typename ANNcoord,typename ANNidx>
int annMaxSpread( // compute dimension of max spread
    ANNcoord**  pa,     // point array
    ANNidx*     pidx,   // point indices
    int         n,      // number of points
    int         dim);   // dimension of space

template <typename ANNcoord,typename ANNidx>
void annMedianSplit(    // split points along median value
    ANNcoord**  pa,     // points to split
    ANNidx*     pidx,   // point indices
    int         n,      // number of points
    int         d,      // dimension along which to split
    ANNcoord    &cv,    // cutting value
    int         n_lo);  // split into n_lo and n-n_lo


template <typename ANNcoord,typename ANNdist>
ANNdist annBoxDistance( // compute distance from point to box
    const ANNcoord*     q,      // the point
    const ANNcoord*     lo,     // low point of box
    const ANNcoord*     hi,     // high point of box
    int                 dim);   // dimension of space


template<typename ANNcoord> ANNcoord ANN_POW(ANNcoord v)
{
switch (norm) {
case EUCLIDEAN:                         
        return (v*v);
        break;
case MANHATTAN:                         
        return fabs(v);
        break;
case MAX:                               
        return fabs(v);
        break;
default:
        annError("INTERNAL ERROR: Unknown distance norm", ANNabort);
        return 0.0;
        break;
}
}

template<typename ANNcoord> ANNcoord ANN_ROOT(ANNcoord x)
{
switch (norm) {
case EUCLIDEAN:                         
        return sqrt(x);
        break;
case MANHATTAN:                         
        return (x);
        break;
case MAX:                               
        return (x);
        break;
default:
        annError("INTERNAL ERROR: Unknown distance norm", ANNabort);
        return 0.0;
        break;
}
}

template<typename ANNcoord>
ANNcoord ANN_SUM(ANNcoord x, ANNcoord y)
{
switch (norm) {
case EUCLIDEAN:                         
        return ((x) + (y));
        break;
case MANHATTAN:                         
        return ((x) + (y));
        break;
case MAX:                               
        return ((x) > (y) ? (x) : (y));
        break;
default:
        annError("INTERNAL ERROR: Unknown distance norm", ANNabort);
        return 0.0;
        break;
}
}

template<typename ANNcoord>
ANNcoord ANN_DIFF(ANNcoord x, ANNcoord y)
{
switch (norm) {
case EUCLIDEAN:                         
        return ((y) - (x));
        break;
case MANHATTAN:                         
        return ((y) - (x));
        break;
case MAX:                               
        return (y);
        break;
default:
        annError("INTERNAL ERROR: Unknown distance norm", ANNabort);
        return 0.0;
        break;
}
}


template <typename ANNcoord,typename ANNidx>
struct kd_split
{
void operator()(
    ANNcoord**  pa,             // point array (permuted on return)
    ANNidx*             pidx,           // point indices
    const ANNorthRect<ANNcoord> &bnds,  // bounding rectangle for cell
    int                 n,              // number of points
    int                 dim,            // dimension of space
    int                 &cut_dim,       // cutting dimension (returned)
    ANNcoord            &cut_val,       // cutting value (returned)
    int                 &n_lo)  // num of points on low side (returned)
{
    // find dimension of maximum spread
    cut_dim = annMaxSpread(pa, pidx, n, dim);
    
    n_lo = n/2; // median rank
    
    // split about median
    annMedianSplit(pa, pidx, n, cut_dim, cut_val, n_lo);
}
};



template <typename ANNcoord,typename ANNdist,typename ANNidx>
class ANNpointSet {
public:
    virtual ~ANNpointSet() {}   // virtual distroyer

    virtual void annkSearch(    // approx k near neighbor search
        ANNcoord*       q,      // query point
        int             k,      // number of near neighbors to return
        ANNidx* nn_idx,         // nearest neighbor array (returned)
        ANNdist*        dd,     // dist to near neighbors (returned)
        double          eps=0.0 // error bound
        ) = 0;                  // pure virtual (defined elsewhere)
};


template <typename ANNcoord,typename ANNdist,typename ANNidx>
class ANNkd_node{               // generic kd-tree node (empty shell)
public:
    virtual ~ANNkd_node() {}    // virtual distroyer

    virtual void ann_search(int, ANNcoord*, ANNcoord**, 
        ANNdist, ANNmin_k<ANNdist>*, ANNdist) = 0; // tree search

    // allow kd-tree to access us
    friend class ANNkd_tree<ANNcoord,ANNdist,ANNidx>;
};


template <typename ANNcoord,typename ANNdist,typename ANNidx>
class ANNkd_split : public ANNkd_node<ANNcoord,ANNdist,ANNidx>
// splitting node of a kd-tree
{
    int         cut_dim;        // dim orthogonal to cutting plane
    ANNcoord    cut_val;        // location of cutting plane
    ANNcoord    cd_bnds[2];     // lower and upper bounds of
                                // rectangle along cut_dim
                                
    // left and right children
    ANNkd_node<ANNcoord,ANNdist,ANNidx>* child[2];
public:
    ANNkd_split(                        // constructor
        int cd,                         // cutting dimension
        ANNcoord cv,                    // cutting value
        ANNcoord lv, ANNcoord hv,               // low and high values
        ANNkd_node<ANNcoord,ANNdist,ANNidx>* lc=NULL, 
        ANNkd_node<ANNcoord,ANNdist,ANNidx>* hc=NULL)   // children
        {
            cut_dim     = cd;   // cutting dimension
            cut_val     = cv;   // cutting value
            cd_bnds[LO] = lv;   // lower bound for rectangle
            cd_bnds[HI] = hv;   // upper bound for rectangle
            child[LO]   = lc;   // left child
            child[HI]   = hc;   // right child
        }

    ~ANNkd_split()              // destructor
        {
            if (child[LO]!= NULL) delete child[LO];
            if (child[HI]!= NULL) delete child[HI];
        }
        
    // standard search routine
    virtual void ann_search(int, ANNcoord*, ANNcoord**, 
        ANNdist, ANNmin_k<ANNdist>*, ANNdist);
};


template <typename ANNcoord,typename ANNdist,typename ANNidx>
class ANNkd_leaf: public ANNkd_node<ANNcoord,ANNdist,ANNidx>
// leaf node for kd-tree
{
    int         n_pts;          // no. points in bucket
    ANNidx*     bkt;            // bucket of points
public:
    ANNkd_leaf(                 // constructor
        int     n,              // number of points
        ANNidx* b)              // bucket
        {
            n_pts = n;          // number of points in bucket
            bkt   = b;          // the bucket
        }

    ~ANNkd_leaf() { }           // destructor (none)

    virtual void ann_search(int, ANNcoord*, ANNcoord**, ANNdist, 
        ANNmin_k<ANNdist>*, ANNdist);   // standard search routine
};


template <typename ANNcoord>
class ANNorthRect {
public:
    ANNcoord*   lo;     // rectangle lower bounds
    ANNcoord*   hi;     // rectangle upper bounds

    ANNorthRect(        // basic constructor
        int dd,         // dimension of space
        ANNcoord l=0,   // default is empty
        ANNcoord h=0)
    {   lo = annAllocPt(dd, l);  
        hi = annAllocPt(dd, h);  }

    ANNorthRect(        // (almost a) copy constructor
        int dd,         // dimension
        const ANNorthRect<ANNcoord> &r) // rectangle to copy
    {  lo = annCopyPt(dd, r.lo);  hi = annCopyPt(dd, r.hi);  }

    ANNorthRect(        // construct from points
        int dd,         // dimension
        ANNcoord* l,    // low point
        ANNcoord* h)    // hight point
    {  lo = annCopyPt(dd, l);  hi = annCopyPt(dd, h);  }

    ~ANNorthRect()      // destructor
    {  delete lo;  delete hi;  lo = hi = NULL;  }

    // is point p inside rectangle?
    ANNbool inside(int dim, ANNcoord* p);
};


template <typename ANNcoord,typename ANNdist,typename ANNidx>
class ANNkd_tree: public ANNpointSet<ANNcoord,ANNdist,ANNidx> {
protected:
    int                 dim;            // dimension of space
    int                 n_pts;          // number of points in tree
    int                 bkt_size;       // bucket size
    ANNcoord**  pts;            // the points
    ANNidx*             pidx;           // point indices (to pts)
    ANNkd_node<ANNcoord,ANNdist,ANNidx>*  root; // root of kd-tree
    ANNcoord*           bnd_box_lo;     // bounding box low point
    ANNcoord*           bnd_box_hi;     // bounding box high point
    
    void SkeletonTree(                  // construct skeleton tree
        int n,                          // number of points
        int dd,                         // dimension
        int bs);                        // bucket size

public:
    ANNkd_tree(                         // build from point array
        ANNcoord**      pa,             // point array
        int             n,              // number of points
        int             dd,             // dimension
        int             bs = 1,         // bucket size
        DISTANCE_norm   dis_norm = EUCLIDEAN);

    ~ANNkd_tree();              // tree destructor

    virtual void annkSearch(    // approx k near neighbor search
        ANNcoord*       q,      // query point
        int             k,      // number of near neighbors to return
        ANNidx* nn_idx,         // nearest neighbor array (returned)
        ANNdist*        dd,     // dist to near neighbors (returned)
        double          eps=0.0); // error bound
};


// construct from point array
template <typename ANNcoord,typename ANNdist,typename ANNidx>
ANNkd_tree<ANNcoord,ANNdist,ANNidx>::ANNkd_tree(
    ANNcoord**  pa,     // point array (with at least n pts)
    int                 n,      // number of points
    int                 dd,     // dimension
    int                 bs,     // bucket size
    DISTANCE_norm       dis_norm)
{
    if (dis_norm !=EUCLIDEAN && dis_norm!=MANHATTAN && dis_norm!=MAX)
        annError("Unknown Distance Norm!", ANNabort);
    
    norm = dis_norm;
    SkeletonTree(n, dd, bs);            // set up the basic stuff
    pts = pa;                           // where the points are
    if (n == 0) return;                 // no points--no sweat

    ANNorthRect<ANNcoord> bnd_box(dd);  // bounding box for points
    
    // construct bounding rectangle
    annEnclRect(pa, pidx, n, dd, bnd_box);
    
    // copy to tree structure
    bnd_box_lo = annCopyPt(dd, bnd_box.lo);
    bnd_box_hi = annCopyPt(dd, bnd_box.hi);

    root = rkd_tree<ANNcoord,ANNdist,ANNidx, 
        kd_split<ANNcoord,ANNidx> >(pa, pidx, n, dd, bs, bnd_box, 
        kd_split<ANNcoord,ANNidx>());
}

template <typename ANNcoord,typename ANNdist,typename ANNidx>
ANNkd_tree<ANNcoord,ANNdist,ANNidx>::~ANNkd_tree() // tree destructor
{
    if (root != NULL) delete root;
    if (pidx != NULL) delete [] pidx;
    if (bnd_box_lo != NULL) annDeallocPt(bnd_box_lo);
    if (bnd_box_hi != NULL) annDeallocPt(bnd_box_hi);
}


// construct skeleton tree
template <typename ANNcoord,typename ANNdist,typename ANNidx>
void ANNkd_tree<ANNcoord,ANNdist,ANNidx>::SkeletonTree( 
        int n,          // number of points
        int dd,         // dimension
        int bs)         // bucket size
{
    dim = dd;           // initialize basic elements
    n_pts = n;
    bkt_size = bs;
    root = NULL;        // no associated tree yet
    pts = NULL;         // no associated points yet

    pidx = new int[n];  // allocate space for point indices
    for (int i = 0; i < n; i++) {
        pidx[i] = i;    // initially identity
    }
    bnd_box_lo = bnd_box_hi = NULL; // bounding box is nonexistent
}


template <typename ANNcoord>
ANNcoord* annAllocPt(int dim, ANNcoord c) // allocate 1 point
{
    ANNcoord* p = new ANNcoord[dim];
    for (int i = 0; i < dim; i++) p[i] = c;
    return p;
}

template <typename ANNcoord>
void annDeallocPt(ANNcoord* &p)         // deallocate 1 point
{
    delete [] p;
    p = NULL;
}

template <typename ANNcoord>   
ANNcoord* annCopyPt(int dim, ANNcoord* source)  // copy point
{
    ANNcoord* p = new ANNcoord[dim];
    for (int i = 0; i < dim; i++) p[i] = source[i];
    return p;
}


template <typename ANNcoord,typename ANNidx>
void annEnclRect(
    ANNcoord**  pa,             // point array
    ANNidx*             pidx,   // point indices
    int                 n,      // number of points
    int                 dim,    // dimension
    ANNorthRect<ANNcoord> &bnds) // bounding cube (returned)
{
    // find smallest enclosing rectangle
    for (int d = 0; d < dim; d++) {
        ANNcoord lo_bnd = PA(0,d); // lower bound on dimension d
        ANNcoord hi_bnd = PA(0,d); // upper bound on dimension d
        for (int i = 0; i < n; i++) {
            if (PA(i,d) < lo_bnd) lo_bnd = PA(i,d);
            else if (PA(i,d) > hi_bnd) hi_bnd = PA(i,d);
        }
        bnds.lo[d] = lo_bnd;
        bnds.hi[d] = hi_bnd;
    }
}


template <typename ANNcoord,typename ANNidx>
ANNcoord annSpread(     // compute point spread along dimension
    ANNcoord**  pa,     // point array
    ANNidx*             pidx,   // point indices
    int                 n,      // number of points
    int                 d)      // dimension to check
{
    ANNcoord min = PA(0,d);     // compute max and min coords
    ANNcoord max = PA(0,d);
    for (int i = 1; i < n; i++) {
        ANNcoord c = PA(i,d);
        if (c < min) min = c;
        else if (c > max) max = c;
    }
    return (max - min);         // total spread is difference
}


template <typename ANNcoord,typename ANNidx>
int annMaxSpread(               // compute dimension of max spread
    ANNcoord**  pa,             // point array
    ANNidx*             pidx,   // point indices
    int                 n,      // number of points
    int                 dim)    // dimension of space
{
    int max_dim = 0;            // dimension of max spread
    ANNcoord max_spr = 0;       // amount of max spread

    if (n == 0) return max_dim; // no points, who cares?

    for (int d = 0; d < dim; d++) { // compute spread along each dim
        ANNcoord spr = annSpread(pa, pidx, n, d);
        if (spr > max_spr) {    // bigger than current max
            max_spr = spr;
            max_dim = d;
        }
    }
    return max_dim;
}


template <typename ANNcoord,typename ANNidx>
void annMedianSplit(
    ANNcoord**  pa,     // points to split
    ANNidx*     pidx,   // point indices
    int         n,      // number of points
    int         d,      // dimension along which to split
    ANNcoord    &cv,    // cutting value
    int         n_lo)   // split into n_lo and n-n_lo
{
    int l = 0;          // left end of current subarray
    int r = n-1;        // right end of current subarray
    while (l < r) {
        register int i = (r+l)/2; // select middle as pivot
        register int k;

        if (PA(i,d) > PA(r,d))  // make sure last > pivot
            PASWAP(i,r)
        PASWAP(l,i);    // move pivot to first position

        ANNcoord c = PA(l,d);   // pivot value
        i = l;
        k = r;
        for(;;) {               // pivot about c
            while (PA(++i,d) < c) ;
            while (PA(--k,d) > c) ;
            if (i < k) PASWAP(i,k) else break;
        }
        PASWAP(l,k);            // pivot winds up in location k

        if (k > n_lo)      r = k-1; // recurse on proper subarray
        else if (k < n_lo) l = k+1;
        else break;             // got the median exactly
    }
    if (n_lo > 0) {             // search for next smaller item
        ANNcoord c = PA(0,d);   // candidate for max
        int k = 0;              // candidate's index
        for (int i = 1; i < n_lo; i++) {
            if (PA(i,d) > c) {
                c = PA(i,d);
                k = i;
            }
        }
        PASWAP(n_lo-1, k); // max among pa[0..n_lo-1] to pa[n_lo-1]
    }
    // cut value is midpoint value
    cv = (PA(n_lo-1,d) + PA(n_lo,d))/2.0;
}


// recursive construction of kd-tree
template <typename ANNcoord,typename ANNdist,
typename ANNidx,typename SPLIT_FUNCTION_OBJECT>
ANNkd_node<ANNcoord,ANNdist,ANNidx>* rkd_tree(
    ANNcoord**  pa,     // point array
    ANNidx*     pidx,   // point indices to store in subtree
    int         n,      // number of points
    int         dim,    // dimension of space
    int         bsp,    // bucket space
    ANNorthRect<ANNcoord> &bnd_box, // bounding box for current node
    SPLIT_FUNCTION_OBJECT splitter) // splitting routine
{
    if (n <= bsp) {     // n small, make a leaf node
        if (n == 0)     // empty leaf node
                // return empty leaf
            return new ANNkd_leaf<ANNcoord,ANNdist,ANNidx>(0, IDX_TRIVIAL);
        else    // construct the node and return
            return new ANNkd_leaf<ANNcoord,ANNdist,ANNidx>(n, pidx); 
    }
    else {              // n large, make a splitting node
        int cd;         // cutting dimension
        ANNcoord cv;    // cutting value
        int n_lo;       // number on low side of cut
        // low and high children
        ANNkd_node<ANNcoord,ANNdist,ANNidx> *lo, *hi;

        // invoke splitting function object
        splitter(pa, pidx, bnd_box, n, dim, cd, cv, n_lo);

        // save bounds for cutting dimension
        ANNcoord lv = bnd_box.lo[cd];
        ANNcoord hv = bnd_box.hi[cd];

        bnd_box.hi[cd] = cv;    // modify bounds for left subtree
        
        // build left subtree
        lo = rkd_tree<ANNcoord,ANNdist,ANNidx, SPLIT_FUNCTION_OBJECT>(
                pa, pidx, n_lo, // ...from pidx[0..n_lo-1]
                dim, bsp, bnd_box, splitter);
        bnd_box.hi[cd] = hv;    // restore bounds

        bnd_box.lo[cd] = cv;    // modify bounds for right subtree
        
        // build right subtree
        hi = rkd_tree<ANNcoord,ANNdist,ANNidx, SPLIT_FUNCTION_OBJECT>(
                pa, pidx + n_lo, n-n_lo,// ...from pidx[n_lo..n-1]
                dim, bsp, bnd_box, splitter);
        bnd_box.lo[cd] = lv;    // restore bounds

        // create the splitting node
        ANNkd_split<ANNcoord,ANNdist,ANNidx>
                *ptr = new ANNkd_split<ANNcoord,
                        ANNdist,ANNidx>(cd, cv, lv, hv, lo, hi);

        return ptr;     // return pointer to this node
    }
} 



template <typename ANNdist>
class ANNmin_k {

    struct mk_node {            // node in min_k structure
        ANNdist         key;    // key value
        int             info;   // info field (user defined)
    };

    int         k;      // max number of keys to store
    int         n;      // number of keys currently active
    mk_node     *mk;    // the list itself

public:

    ANNmin_k(int max)   // constructor (given max size)
        {
            n = 0;      // initially no items
            k = max;    // maximum number of items
            mk = new mk_node[max+1];    // sorted array of keys
        }

    ~ANNmin_k()                 // destructor
        { delete [] mk; }
    
    ANNdist ANNmin_key()        // return minimum key
        { return (n > 0 ? mk[0].key : (ANNdist)INFDISTANCE); }
    
    ANNdist max_key()           // return maximum key
        { return (n == k ? mk[k-1].key : (ANNdist)INFDISTANCE); }
    
    ANNdist ith_smallest_key(int i) // ith smallest key (i in [0..n-1])
        { return (i < n ? mk[i].key : (ANNdist)INFDISTANCE); }
    
    int ith_smallest_info(int i) // info for ith smallest (i in [0..n-1])
        { return (i < n ? mk[i].info : PQ_NULL_INFO); }

    inline void insert( // insert item (inlined for speed)
        ANNdist kv,     // key value
        int inf)        // item info
        {
            register int i;
                // slide larger values up
            for (i = n; i > 0; i--) {
                if (mk[i-1].key > kv)
                    mk[i] = mk[i-1];
                else
                    break;
            }
            mk[i].key = kv;     // store element here
            mk[i].info = inf;
            if (n < k) n++;     // increment number of items
        }
};



template <typename ANNcoord,typename ANNdist,typename ANNidx>
void ANNkd_tree<ANNcoord,ANNdist,ANNidx>::annkSearch(
    ANNcoord*           q,      // the query point
    int                 k,      // number of near neighbors to return
    ANNidx*             nn_idx, // nearest neighbor indices (returned)
    ANNdist*    dd,             // the approximate nearest neighbor
    double              eps)    // the error bound
{
    if (k > n_pts) {            // too many near neighbors?
        annError("Requesting more near neighbors than data points",
                ANNabort);
    }

    // create set for closest k points
    ANNmin_k<ANNdist>* ANNkdPointMK = new ANNmin_k<ANNdist>(k);
    
    // search starting at the root
    root->ann_search(dim, q, pts, ANN_POW(1.0 + eps), 
        ANNkdPointMK, annBoxDistance<ANNcoord,ANNdist>(q, 
        bnd_box_lo, bnd_box_hi, dim));

    for (int i = 0; i < k; i++) { // extract the k-th closest points
        dd[i] = ANNkdPointMK->ith_smallest_key(i);
        nn_idx[i] = ANNkdPointMK->ith_smallest_info(i);
    }
    delete ANNkdPointMK; // deallocate closest point set
}


template <typename ANNcoord,typename ANNdist,typename ANNidx>
void ANNkd_split<ANNcoord,ANNdist,ANNidx>::ann_search(int ANNkdDim,
        ANNcoord* ANNkdQ, ANNcoord** ANNkdPts, ANNdist ANNkdMaxErr,
        ANNmin_k<ANNdist>* ANNkdPointMK, ANNdist box_dist)
{
    // distance to cutting plane
    ANNcoord cut_diff = ANNkdQ[cut_dim] - cut_val;

    if (cut_diff < 0) {         // left of cutting plane
        // visit closer child first
        child[LO]->ann_search(ANNkdDim, ANNkdQ, ANNkdPts, 
        ANNkdMaxErr, ANNkdPointMK, box_dist);

        ANNcoord box_diff = cd_bnds[LO] - ANNkdQ[cut_dim];
        if (box_diff < 0)       // within bounds - ignore
            box_diff = 0;
                                // distance to further box
        box_dist = (ANNdist) ANN_SUM(box_dist,
                ANN_DIFF(ANN_POW(box_diff), ANN_POW(cut_diff)));

        // visit further child if close enough
        if (box_dist * ANNkdMaxErr < ANNkdPointMK->max_key())
                child[HI]->ann_search(ANNkdDim, ANNkdQ, ANNkdPts,
                        ANNkdMaxErr, ANNkdPointMK, box_dist);
    }
    else {              // right of cutting plane
        // visit closer child first
        child[HI]->ann_search(ANNkdDim, ANNkdQ, ANNkdPts, 
                ANNkdMaxErr, ANNkdPointMK, box_dist);

        ANNcoord box_diff = ANNkdQ[cut_dim] - cd_bnds[HI];
        if (box_diff < 0)       // within bounds - ignore
            box_diff = 0;
                                // distance to further box
        box_dist = (ANNdist) ANN_SUM(box_dist,
                ANN_DIFF(ANN_POW(box_diff), ANN_POW(cut_diff)));

        // visit further child if close enough
        if (box_dist * ANNkdMaxErr < ANNkdPointMK->max_key())
                child[LO]->ann_search(ANNkdDim, ANNkdQ, ANNkdPts, 
                        ANNkdMaxErr, ANNkdPointMK, box_dist);
    }
}


template <typename ANNcoord,typename ANNdist,typename ANNidx>
void ANNkd_leaf<ANNcoord,ANNdist,ANNidx>::ann_search(int ANNkdDim,
        ANNcoord* ANNkdQ, ANNcoord** ANNkdPts, ANNdist ANNksMaxErr,
        ANNmin_k<ANNdist>* ANNkdPointMK, ANNdist box_dist)
{
    register ANNdist dist;      // distance to data point
    register ANNcoord* pp;      // data coordinate pointer
    register ANNcoord* qq;      // query coordinate pointer
    register ANNdist min_dist;  // distance to k-th closest point
    register ANNcoord t;
    register int d;

    // k-th smallest distance so far
    min_dist = ANNkdPointMK->max_key();

    for (int i = 0; i < n_pts; i++) {   // check points in bucket

        pp = ANNkdPts[bkt[i]];  // first coord of next data point
        qq = ANNkdQ;            // first coord of query point
        dist = 0;

        for(d = 0; d < ANNkdDim; d++) {
            // compute length and adv coordinate
            t = *(qq++) - *(pp++);
            
            // exceeds dist to k-th smallest?
            if( (dist = ANN_SUM(dist, ANN_POW(t))) > min_dist) {
                break;
            }
        }

        if (d >= ANNkdDim){     // among the k best?
            // add it to the list
            ANNkdPointMK->insert(dist, bkt[i]);
            min_dist = ANNkdPointMK->max_key();
        }
    }
}


template <typename ANNcoord,typename ANNdist>
ANNdist annBoxDistance(  // compute distance from point to box
    const ANNcoord*     q,      // the point
    const ANNcoord*     lo,     // low point of box
    const ANNcoord*     hi,     // high point of box
    int                 dim)    // dimension of space
{
    register ANNdist dist = 0.0; // sum of squared distances
    register ANNdist t;

    for (register int d = 0; d < dim; d++) {
        if (q[d] < lo[d]) {             // q is left of box
            t = ANNdist(lo[d]) - ANNdist(q[d]);
            dist = ANN_SUM(dist, ANN_POW(t));
        }
        else if (q[d] > hi[d]) {        // q is right of box
            t = ANNdist(q[d]) - ANNdist(hi[d]);
            dist = ANN_SUM(dist, ANN_POW(t));
        }
    }
    return dist;
}


#endif
