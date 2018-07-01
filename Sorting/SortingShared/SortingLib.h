// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#ifndef SORTING_LIB_H
#define SORTING_LIB_H

#ifdef _WIN32
	//#define WIN32_LEAN_AND_MEAN 
	//#include <SDKDDKVer.h>
	//#include <stdio.h>
	#include <windows.h>
	//#include <minwindef.h>
#else

#endif // _WIN32

#include <string>
#include <random>
#include <functional>
#include <chrono>
#include <errno.h>
#include <iostream>

#include "../../Error_Checks/ERROR_CHECKS.H"

using STATUS = long;

enum class ESortingTypes : int { QuickSort, MergeSort, BubbleSort };
ESortingTypes operator++(ESortingTypes& eSortingType, int);
std::wostream& operator << (std::wostream& str, ESortingTypes eSortingType);


template <class T, class U>
class CSorting
{
public:
     //enum class ESortType { QuickSort, MergeSort, BubbleSort };
     
     static STATUS Sort(IN const ESortingTypes eSortType, IN OUT T* array, IN const U arraySize, OUT OPTIONAL std::size_t* pnNumberOfSorts = nullptr, OUT OPTIONAL std::chrono::duration<double>* pDuration = nullptr);
     static STATUS VerifySort(IN const T* array, IN const U arraySize, OUT bool& bSorted);

private:
     static STATUS QuickSort(IN OUT T* array, IN const U arraySize, OUT std::size_t& numberOfSorts);
     static STATUS MergeSort(IN OUT T* array, IN const U arraySize, IN OUT T* mergedArray, OUT std::size_t& numberOfSorts);
     static STATUS BubbleSort(IN OUT T* array, IN const U arraySize, OUT std::size_t& numberOfSorts);
     
     static U RandomArrayReference(IN const U arraySize);

     static void MergeSplit(IN T* array, IN const U size, OUT T** lArray, OUT U& lArraySize, OUT T** rArray, OUT U& rArraySize);
};


inline ESortingTypes operator++(ESortingTypes& eSortingType, int)
{
     eSortingType = ESortingTypes(int(eSortingType) + 1);
     return eSortingType;
}

inline std::wostream& operator << (std::wostream& str, ESortingTypes eSortingType)
{
     switch (eSortingType)
     {
     case ESortingTypes::QuickSort: str << L"QuickSort"; break;
     case ESortingTypes::MergeSort: str << L"MergeSort"; break;
     case ESortingTypes::BubbleSort: str << L"BubbleSort"; break;
     }
     return str;
}


// QUICK SORT

template<class T, class U>
STATUS CSorting<T, U>::Sort(IN const ESortingTypes eSortType, IN OUT T* array, IN const U arraySize, OUT OPTIONAL std::size_t* pnNumberOfSorts /*=nullptr*/, OUT OPTIONAL std::chrono::duration<double>* pDuration /*=nullptr*/)
{
     if ((0 == array) || (0 == arraySize)) return EINVAL;

     STATUS nStatus = 0;
     std::size_t nNumberOfSorts = 0;

     try
     {
          auto startSort = std::chrono::high_resolution_clock::now();

          switch (eSortType)
          {
          case ESortingTypes::QuickSort:
               CHECK_SUCCEEDED_LOG_THROW( CSorting::QuickSort(array, arraySize, nNumberOfSorts) );
               break;
          case ESortingTypes::MergeSort:
          {
               T* pMergedArray = new T[arraySize];

               for (U nIndex = 0; nIndex < arraySize; nIndex++)  // initialize merged array with original array
                    pMergedArray[nIndex] = array[nIndex];

               CHECK_SUCCEEDED_LOG_THROW( CSorting::MergeSort(array, arraySize, pMergedArray, nNumberOfSorts) );

               for (U nIndex = 0; nIndex < arraySize; nIndex++) // copy merged array into original array
                    array[nIndex] = pMergedArray[nIndex];

               if (pMergedArray) delete[] pMergedArray;
          }
               break;
          case ESortingTypes::BubbleSort:
               CHECK_SUCCEEDED_LOG_THROW( CSorting::BubbleSort(array, arraySize, nNumberOfSorts) );
               break;
          default:
               CHECK_SUCCEEDED_LOG_THROW(EINVAL);
               break;
          }

          auto endSort = std::chrono::high_resolution_clock::now();
          auto deltaSort = endSort - startSort;

          if (nullptr != pDuration)  *pDuration = deltaSort;

          if (nullptr != pnNumberOfSorts) *pnNumberOfSorts = nNumberOfSorts;
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

     return nStatus;
}


template<class T, class U>
STATUS CSorting<T, U>::QuickSort(IN OUT T* array, IN const U arraySize, OUT std::size_t& numberOfSorts)
{
    T* left = array;
    T* right = array + (arraySize - 1);
    U pivot = *(array + (CSorting::RandomArrayReference(arraySize) - 1));

    STATUS nStatus = 0;

     try
     {
	     while (left < right)
	     {
                     if ((U)(*left) < pivot)
			     left++;
                     else if ((U)(*right) > pivot)
			     right--;
		     else if (*left == *right) // Handles duplicates
			     left++;
		     else
			     std::iter_swap(left, right);
	     }

          if (left != array)	// Not begining of array (left)
          {
               numberOfSorts += 1;
               CHECK_SUCCEEDED_LOG_THROW( QuickSort(array, (U)(left - array), numberOfSorts) );
          }

          if (right != array + (arraySize - 1))	// Not end of array (right)
          {
               numberOfSorts += 1;
               CHECK_SUCCEEDED_LOG_THROW( QuickSort(right + 1, (U)((array + (arraySize - 1)) - right), numberOfSorts) );
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

	return nStatus;
}

template<class T, class U>
U CSorting<T,U>::RandomArrayReference(IN const U arraySize)
{
	U randomNumber = -1;
	std::uniform_int_distribution<U> distribution(1, arraySize);
	std::mt19937 engine;
	auto generator = std::bind(distribution, engine);
	randomNumber = generator();
	return randomNumber;
}



// MERGE SORT

template<class T, class U>
STATUS CSorting<T,U>::MergeSort(IN OUT T* array, IN const U arraySize, IN OUT T* mergedArray, OUT std::size_t& numberOfSorts)
{
     STATUS nStatus = 0;

     try
     {
          if (arraySize > 1)
          {
               T* lArray = 0;
               T* rArray = 0;
               U lArraySize = 0;
               U rArraySize = 0;

               MergeSplit(array, arraySize, &lArray, lArraySize, &rArray, rArraySize);

               CHECK_SUCCEEDED_LOG_THROW( MergeSort(lArray, lArraySize, mergedArray, numberOfSorts) );
               numberOfSorts += 1;

               CHECK_SUCCEEDED_LOG_THROW( MergeSort(rArray, rArraySize, mergedArray, numberOfSorts) );
               numberOfSorts += 1;

               // Perform merge sort
               U lIndex = 0, rIndex = 0, mIndex = 0;

               while (lIndex < lArraySize && rIndex < rArraySize)
               {
                    if (lArray[lIndex] < rArray[rIndex])
                    {
                         mergedArray[mIndex] = lArray[lIndex];
                         lIndex++;
                    }
                    else
                    {
                         mergedArray[mIndex] = rArray[rIndex];
                         rIndex++;
                    }
                    mIndex++;
               }

               // Copy the remaining
               for (; lIndex < lArraySize; lIndex++, mIndex++)
                    mergedArray[mIndex] = lArray[lIndex];
               for (; rIndex < rArraySize; rIndex++, mIndex++)
                    mergedArray[mIndex] = rArray[rIndex];

               // Copy sorted and merged array back into original array
               for (U nIndex = 0; nIndex < arraySize; nIndex++)
                    array[nIndex] = mergedArray[nIndex];
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

     return nStatus;
}


template <class T, class U>
void CSorting<T, U>::MergeSplit(IN T* array, IN const U size, OUT T** lArray, OUT U& lArraySize, OUT T** rArray, OUT U& rArraySize)
{
     lArraySize = size / 2;
     *lArray = array;

     rArraySize = size - lArraySize;
     *rArray = &array[lArraySize];
}



// BUBBLE SORT

template<class T, class U>
STATUS CSorting<T,U>::BubbleSort(IN OUT T* array, IN const U arraySize, OUT std::size_t& numberOfSorts)
{
     STATUS nStatus = 0;
     
     bool bSorted = true;

     try
     {
          for (U nIndex = 0; nIndex < arraySize - 1; nIndex++)
          {
               if (array[nIndex] > array[nIndex + 1])
               {
                    std::swap(array[nIndex], array[nIndex + 1]);
                    bSorted = false;
               }
          }

          if (false == bSorted)
          {
               numberOfSorts += 1;
               CHECK_SUCCEEDED_LOG_THROW( BubbleSort(array, arraySize - 1, numberOfSorts) );  // Optimize array size one less since last elemented will be sorted.
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

     return nStatus;
}



// VERIFY SORT

template<class T, class U>
STATUS CSorting<T,U>::VerifySort(IN const T* array, IN const U arraySize, OUT bool& bSorted)
{
     STATUS nStatus = 0;
     bSorted = true;

     if ((0 == array) || (0 == arraySize))
     { bSorted = false;  return EINVAL; }

     try
     {
          for (U iIndex = 0; iIndex < (arraySize - 1) && (true == bSorted); iIndex++)
          {
               bSorted = (array[iIndex] <= array[iIndex + 1]);
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

	return nStatus;
}


#endif // SORTING_LIB_H
