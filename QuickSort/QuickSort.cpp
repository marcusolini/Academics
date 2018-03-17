// QuickSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <random>
#include <functional>
#include <iostream>

// Declarations

using PIntArray = int*;
using PAlphaArray = wchar_t*;
template < class T > struct IntArraysStruct;

template < class T > int QuickSort(T array, int size);
template < class T > bool VerifySort(const T array, const int arraySize);
template < class T > void PrintArray(const T array, const int arraySize);

int RandomArrayReference(const int size);

int TestInts();
int TestAlpas();


// Definitions

template < class T >
struct IntArraysStruct
{
     T pIntArray;
     int nIntArraySize;
};

int RandomArrayReference(const int size)
{
     int randomNumber = -1;
     std::uniform_int_distribution<int> distribution(1, size);
     std::mt19937 engine;
     auto generator = std::bind(distribution, engine);
     randomNumber = generator();
     return randomNumber;
}

template < class T >
int QuickSort(T array, int size)
{
     T left = array;
     T right = array + (size - 1);
     int pivot = *(array + (RandomArrayReference(size)-1));
     int numberOfSorts = 1;

    while (left < right)
    {
         if (*left < pivot)
              left++;
         else if (*right > pivot)
              right--;
         else if (*left == *right) // Handles duplicates
              left++;
         else 
              std::iter_swap(left, right);
    }

     if (left != array)	// Not begining of array (left)
         numberOfSorts += QuickSort(array, (left - array));

     if (right != array + (size - 1))	// Not end of array (right)
          numberOfSorts += QuickSort(right + 1, (array + (size - 1)) - right);

     return numberOfSorts;
}


template < class T >
bool VerifySort(const T array, const int arraySize )
{
     bool bCorrect = true;

     for (int iIndex = 0; iIndex < (arraySize - 1) && true == bCorrect; iIndex++)
     {
          bCorrect = (array[iIndex] <= array[iIndex+1]);
     }

     std::wcout << L"Result: " << (bCorrect ? L"Correct" : L"Incorrect") << std::endl << std::endl;

     return bCorrect;
}

template < class T >
void PrintArray(const T array, const int arraySize)
{
     for (int iIndex = 0; iIndex < arraySize;  iIndex++)
     {
          std::wcout << array[iIndex] << L" ";
     }
     std::wcout << std::endl;
}


// Testing

int TestInts()
{
     bool bSortCorrect = true;
     int nNumberOfSorts = 0;
     int nStatus = EXIT_SUCCESS;

     int a1[] = { 15, 3, 9, 8, 2, 7, 1, 6 };
     int a2[] = { 5, 3, 10, 1, 2 };
     int a3[] = { 5, 3, 10, 11, 2, 4 };
     int a4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
     int a5[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
     int a6[] = { 10, 10, 9, 9, 8, 8 };
     int a7[] = { 1, 0 };
     int a8[] = { 0, 1, 0 };
     int a9[] = { 0, 0, 1, 0, 0 };
     int a10[] = { 1 };

     IntArraysStruct<PIntArray> testArrays[] = { 
     { a1, sizeof(a1) / sizeof(int) },
     { a2, sizeof(a2) / sizeof(int) },
     { a3, sizeof(a3) / sizeof(int) },
     { a4, sizeof(a4) / sizeof(int) },
     { a5, sizeof(a5) / sizeof(int) },
     { a6, sizeof(a6) / sizeof(int) },
     { a7, sizeof(a7) / sizeof(int) },
     { a8, sizeof(a8) / sizeof(int) },
     { a9, sizeof(a9) / sizeof(int) },
     { a10, sizeof(a10) / sizeof(int) },
     };

     for (auto& iTestArrays : testArrays)
     {
          PrintArray<PIntArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          nNumberOfSorts = QuickSort<PIntArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          //std::wcout << L"Number of Sorts: " << nNumberOfSorts << std::endl;
          PrintArray<PIntArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          bSortCorrect = VerifySort<PIntArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);

          if (false == bSortCorrect)
          {
               nStatus = EXIT_FAILURE;
          }
     }

     return nStatus;
}

int TestAlpas()
{
     bool bSortCorrect = true;
     int nNumberOfSorts = 0;
     int nStatus = EXIT_SUCCESS;

     wchar_t a1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' } ;
     wchar_t a2[] = { 'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a' };
     wchar_t a3[] = { 'x', 'y', 'z', 'c', 'b', 'a' };
     wchar_t a4[] = { 'w', 'x', 'y', 'z', 'c', 'b', 'a' };
     wchar_t a5[] = { 'a' };
     wchar_t a6[] = { 'b', 'b', 'a', 'a' };
     wchar_t a7[] = { 'b', 'b', 'c', 'c', 'a', 'a' };
     wchar_t a8[] = { 'b', 'a', 'b' };

     IntArraysStruct<PAlphaArray> testArrays[] = {
     { a1, sizeof(a1) / sizeof(wchar_t) },
     { a2, sizeof(a2) / sizeof(wchar_t) },
     { a3, sizeof(a3) / sizeof(wchar_t) },
     { a4, sizeof(a4) / sizeof(wchar_t) },
     { a5, sizeof(a5) / sizeof(wchar_t) },
     { a6, sizeof(a6) / sizeof(wchar_t) },
     { a7, sizeof(a7) / sizeof(wchar_t) },
     { a8, sizeof(a8) / sizeof(wchar_t) },
     };

     for (auto& iTestArrays : testArrays)
     {
          PrintArray<PAlphaArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          nNumberOfSorts = QuickSort<PAlphaArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          //std::wcout << L"Number of Sorts: " << nNumberOfSorts << std::endl;
          PrintArray<PAlphaArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);
          bSortCorrect = VerifySort<PAlphaArray>(iTestArrays.pIntArray, iTestArrays.nIntArraySize);

          if (false == bSortCorrect)
          {
               nStatus = EXIT_FAILURE;
          }
     }

     return nStatus;
}


// main

int wmain()
{
     int nStatus = EXIT_SUCCESS;

     nStatus += TestInts();
     nStatus += TestAlpas();

     std::wcout << L"All RESULTS: " << (nStatus ? L"FAILURE" : L"SUCCESSFUL") << std::endl << std::endl;

     return nStatus;
}


