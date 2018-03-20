// Copyright(c) 2018 Mark Nicolini
// MergeSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>

// Declarations 

using INT = int;
using INTARRAY = int*;
using PPINTARRAY = int**;

template <class T, class U> struct ARRAY_STRUCT;

template <class T, class U> void MergeSort(T array, U size, T mergedArray);
template <class T, class U> void MergeSort(T array, U size);
template <class T, class U, class V> void MergeSplit(T array, U size, V lArray, U& lArraySize, V rArray, U&rArraySize);


// Definitions

template <class T, class U> 
void MergeSort(T array, U size, T mergedArray)  
{
     if (size > 1)
     {
          T lArray = 0, rArray = 0;
          int lArraySize = 0, rArraySize = 0;

          MergeSplit<T, U, PPINTARRAY>(array, size, &lArray, lArraySize, &rArray, rArraySize);

          auto lMergedArray = std::make_shared<U>(lArraySize);
          auto rMergedArray = std::make_shared<U>(rArraySize);

          MergeSort<T, U>(lArray, lArraySize, lMergedArray.get());
          MergeSort<T, U>(rArray, rArraySize, rMergedArray.get());

          int lIndex = 0, rIndex = 0, mIndex = 0;               // indices

          for (lIndex = 0, rIndex = 0, mIndex = 0;              // initial
               lIndex < lArraySize && rIndex < rArraySize;      // bounds
               lIndex++, rIndex++, mIndex+2                     // increments
               )
          {
               if (lArray[lIndex] < rArray[rIndex])
               {
                    mergedArray[mIndex] = lArray[lIndex];
                    mergedArray[mIndex + 1] = rArray[rIndex];
               }
               else
               {
                    mergedArray[mIndex] = rArray[rIndex];
                    mergedArray[mIndex + 1] = lArray[lIndex];
               }
          }

          // Deplete the remaining
          for (; lIndex < lArraySize; lIndex++, mIndex++)
               mergedArray[mIndex] = lArray[lIndex];
          for (; rIndex < rArraySize; rIndex++, mIndex++)
               mergedArray[mIndex] = lArray[lIndex];
     }
}


template <class T, class U, class V>
void MergeSplit(T array, U size, V lArray, U& lArraySize, V rArray, U& rArraySize)
{    // TODO: Add parameter validation.

     // Split the array
     lArraySize = size / 2;
     *lArray = array;

     rArraySize = size - lArraySize;
     *rArray = &array[lArraySize];

     //rArray = array+((size/2)-1);
     //rSizeArray = array + ((size - 1) - rArray) + 1);
}

template <class T, class U>
void MergeSort(T array, U size)
{
     auto mergedArray = std::make_shared<U>(size);
     //std::shared_ptr<int> mergedA = new int(size);

     MergeSort<T, U>(array, size, mergedArray.get());

     auto pMergedArray = mergedArray.get();

     for ( auto nIndex = 0; nIndex < size; nIndex++)
     {
          array[nIndex] = pMergedArray[nIndex];
     }
}


template <class T, class U>
struct ARRAY_STRUCT
{
     T array;
     U size;
};


int main()
{
     int nStatus = EXIT_SUCCESS;

     int a1[] = { 1, 2 };
     int a2[] = { 2, 4, 5 };

     ARRAY_STRUCT<INTARRAY, INT> arrays[] = {
           { a1, sizeof(a1) / sizeof(int) }
          ,{ a2, sizeof(a2) / sizeof(int) }

     };

     for (auto& array : arrays)
     {
          MergeSort<INTARRAY, INT>(array.array, array.size);
     }
     
     return nStatus;
}