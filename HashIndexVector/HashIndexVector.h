// Copyright(c) 2018 marcusolini@outlook.com

#pragma once

#include <exception>
#include "CriticalSection.h"

// TODO
// Test Expand
// Test ReIndex
// Add Remove
// Add Shrink?


// Declarations

template <class KEY, class DATA> class IndexVector;
template <class KEY, class DATA> class VectorElement;

// Definitions

template <class KEY, class DATA> class VectorElement
{
public:

     VectorElement(){}

     VectorElement(KEY key, DATA data, size_t hash, size_t hashIndex, size_t hashAlgVer) : m_key(key), m_data(data), m_hash(hash), m_hashIndex(hashIndex), m_hashAlgorithmVersion(hashAlgVer) {}

     virtual ~VectorElement() {}

     KEY GetKey() { return m_key;  }
     DATA GetData() { return m_data;  }
     size_t GetHash() { return m_hash;  }
     size_t GetHashIndex() { return m_hashIndex;  }
     size_t GetHashAlgorithmVersion() { return m_hashAlgorithmVersion; }

private:
     KEY m_key = 0;
     DATA m_data = 0;

     size_t m_hashAlgorithmVersion = 0;
     size_t m_hash = 0;
     size_t m_hashIndex = 0;
};

template <class KEY, class DATA> class VectorNode
{
public:
     VectorNode() {}

     void Insert(VectorElement<KEY, DATA>& v)
     {
          if (nullptr == m_vectorElement)
          {
               VectorElement<KEY, DATA>* pVe = new VectorElement<KEY, DATA>(v.GetKey(), v.GetData(), v.GetHash(), v.GetHashIndex(), v.GetHashAlgorithmVersion());

               m_vectorElement = pVe;
          }
          else
          {
               if (nullptr == m_pNextVectorNode)
               {
                    m_pNextVectorNode = new VectorNode<KEY, DATA>();
               }

               m_pNextVectorNode->Insert(v);
          }
     }

     // TODO GetFirst, GetNext
     /*VectorElement<KEY, DATA>& GetFirst()
     {
          if (key == m_vectorElement->GetKey())

     VectorElement<KEY, DATA>& Get(const KEY key)
     {
          if (key == m_vectorElement->GetKey())
          {
               return *m_vectorElement;
          }
          else
          {
               m_pNextVectorNode->Get(key);
          }
     }
     */

private:
     VectorElement<KEY, DATA>* m_vectorElement = nullptr;
     VectorNode<KEY, DATA>* m_pNextVectorNode = nullptr;
};


template <class KEY, class DATA> class IndexVector
{
public:
     IndexVector()
     {
          m_indexVector = new VectorNode<KEY, DATA>[m_count];
          m_size = m_count * sizeof(VectorNode<KEY, DATA>);
     }

     virtual ~IndexVector()
     {
          if (m_indexVector)
          {
               delete[] m_indexVector;
          }

          m_indexVector = 0;
          m_size = 0;
          m_count = 0;
     }

     void Insert(const KEY key, const DATA data)
     {
          CriticalSectionGuard guard(m_criticalSection);

          size_t hash = CalculateHash(key);
          size_t hashIndex = CalculateHashIndex(hash);
          size_t hashAlgVer = m_hashAlgorithmVersion;

          VectorElement<KEY, DATA> vectorElement = { key, data, hash, hashIndex, hashAlgVer };

          Insert(hashIndex, vectorElement);

     }

     void Insert(const size_t hashIndex, VectorElement<KEY, DATA>& vectorElement)
     {
          CriticalSectionGuard guard(m_criticalSection);

          if (hashIndex > m_count-1)
          {
               Expand(hashIndex);
          }

          m_indexVector[hashIndex].Insert(vectorElement);
          
          /*
          if (nullptr == m_indexVector[hashIndex])
          {
               m_indexVector->Insert(vectorElement);
          }
          else
          {
               // TODO: Handle colisions


               throw std::invalid_argument("index IS occupied");
          }
          */
     }

     size_t Count()
     {
          CriticalSectionGuard guard(m_criticalSection);
          return m_count;
     }

     DATA Get(const KEY key)
     {
          CriticalSectionGuard guard(m_criticalSection);

          size_t hash = CalculateHash(key);
          size_t hashIndex = CalculateHashIndex(hash);

          if (hashIndex > m_count - 1)
          {
               throw std::out_of_range("input index out of range");
          }
          else
          {
               /*
               if (0 == m_indexVector[hashIndex])
                    throw std::invalid_argument("index NOT occupied");
               else
                    return m_indexVector[hashIndex].Get(key);
                    */
               return m_indexVector[hashIndex].Get(key).GetData();
          }
     }

     
     DATA Get(size_t index)
     {
          CriticalSectionGuard guard(m_criticalSection);

          if (index > m_count - 1)
          {
               throw std::out_of_range("input index out of range");
          }
          else
          {
               if (nullptr == m_indexVector[index])
                    throw std::invalid_argument("index NOT occupied");
               else
                    return m_indexVector[index].GetData();
          }
     }
     

     void Remove(const KEY key)
     {
          CriticalSectionGuard guard(m_criticalSection);

          size_t hash = CalculateHash(key);
          size_t hashIndex = CalculateHashIndex(hash);

          //return Get(hashIndex);
     }

     /* Do not see the benefit size index is hidden
     DATA operator[] (size_t index)
     {
          CriticalSectionGuard guard(m_criticalSection);
          if (index > m_count - 1)
          {
               throw std::out_of_range("input index out of range");
          }
          else
          {
               if (0 == m_indexVector[index].GetKey())
                    throw std::invalid_argument("index NOT occupied");
               else
                    return m_indexVector[index].GetData();
          }
     }
     */

private:
     //size_t m_count = 1031; // prime number after 1024
     size_t m_count = 7; // Testing only
     size_t  m_size = 0;
     VectorNode<KEY, DATA>* m_indexVector = 0;

     std::hash<KEY> m_hashFunc;
     size_t m_hashAlgorithmVersion = 1; // std::hash

     CriticalSection m_criticalSection;

     size_t CalculateHash(const KEY key)
     {
          size_t hash = m_hashFunc(key);
          return hash;
     }

     size_t CalculateHashIndex(const size_t hash)
     {
          size_t index = hash % Count();
          return index;
     }

     void Expand(const size_t hashIndex)
     {
          Guard guard(&m_criticalSection);

          // TODO: Validate parameters

          size_t count = m_count;
          size_t size = m_size;

          while (hashIndex > count-1)
          {
               count *= 2;
          }

          count = GetNextPrime(count);

          VectorNode<KEY, DATA>* newIndexVector = new VectorNode<KEY, DATA>[count];
          size = count * sizeof(VectorElement<KEY, DATA>);

          RecalculateHashIndices(newIndexVector, count);

          delete[] m_indexVector;

          m_indexVector = newIndexVector;
          m_count = count;
          m_size = size;
     }

     void RecalculateHashIndices(VectorNode<KEY, DATA>* newIndexVector, const size_t count)
     {
          Guard guard(&m_criticalSection);

          // TODO: Validate parameters

          for (size_t index = 0; index < m_count; index++)
          {
               VectorElement<KEY, DATA>* pVe = nullptr;

               pVe = m_indexVector[index].Get(key);

               KEY key = (m_indexVector[index].Get(key)).GetKey();
               DATA data = m_indexVector[index].Get(key).GetData();

               size_t hash = CalculateHash(key);
               size_t hashIndex = CalculateHashIndex(hash);
               size_t hashAlgVer = m_hashAlgorithmVersion;

               VectorElement<KEY, DATA> newVectorElement( key, data, hash, hashIndex, hashAlgVer );

               newIndexVector[hashIndex] = newVectorElement;
          }
     }


     size_t GetNextPrime(size_t num)
     {
          num++;
          for (int i = 2; i < num; i++)
          {
               if (num%i == 0)
               {
                    num++;
                    i = 2;
               }
          }
          return num;
     }

};

