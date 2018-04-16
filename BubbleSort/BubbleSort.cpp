// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// BubbleSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

// Declarations

using PIntArray = int*;

template < class T, class U > struct ArrayStruct;
template < class T, class U> int BubbleSort(T array, U size);
template < class T, class U> void PrintArray(T array, U size);
template < class T, class U> bool VerifyArray(T array, U size);


// Definitions

template < class T, class U>
int BubbleSort(T array, U size)
{
     int nNumberOfSorts = 1;
     bool bSorted = true;

     for (auto nIndex = 0; nIndex < size-1; nIndex++)
     {
          if (array[nIndex] > array[nIndex + 1])
          {
               std::swap(array[nIndex], array[nIndex+1]);
               bSorted = false;
          }
     }

     if (false == bSorted)
          nNumberOfSorts += BubbleSort(array, size-1);  // Optimize array size one less since last elemented will be sorted.
          
     return nNumberOfSorts;
}

template < class T, class U >
struct ArrayStruct
{
     T array;
     U size;
};

template < class T, class U>
void PrintArray(T array, U size)
{
     for (auto nIndex = 0; nIndex < size; nIndex++)
     {
          std::wcout << array[nIndex] << L" ";
     }
     std::wcout << std::endl;
}

template < class T, class U>
bool VerifyArray(T array, U size)
{
     bool bSorted = true;

     for (auto nIndex = 0; nIndex < size-1 && true == bSorted; nIndex++)
     {
          if (array[nIndex] > array[nIndex+1])
               bSorted = false;
     }

     std::wcout << L"Result: " << (bSorted ? "Sorted" : "Not Sorted") << std::endl;
     return bSorted;
}


int wmain()
{
     int nStatus = EXIT_SUCCESS;
     int nNumberOfSorts = 0;
     bool bSorted = false;

     int a1[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
     int a2[] = { 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0 };
     int a3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
     int a4[] = { 0 };

     ArrayStruct<PIntArray, int> arrays[] = {
          {a1, sizeof(a1) / sizeof(int)},
          {a2, sizeof(a2) / sizeof(int)},
          {a3, sizeof(a3) / sizeof(int)},
          {a4, sizeof(a4) / sizeof(int)}
     };

     for (auto& iArray : arrays)
     {
          PrintArray<PIntArray>(iArray.array, iArray.size);
          nNumberOfSorts = BubbleSort(iArray.array, iArray.size);
          PrintArray<PIntArray>(iArray.array, iArray.size);
          bSorted = VerifyArray<PIntArray>(iArray.array, iArray.size);

          if (false == bSorted)
          {
               nStatus = EXIT_FAILURE;
          }
     }
     
     return nStatus;
}

