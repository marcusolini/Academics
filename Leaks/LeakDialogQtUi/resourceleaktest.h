//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef CRESOURCELEAKTEST_H
#define CRESOURCELEAKTEST_H

class CResourceLeakTest
{
public:
     enum class ERESOURCE_ALLOCATION_TYPES
     { UNDEFINED = 0, NEW_OPERATOR, MALLOC_FUNCTION, CALLOC_FUNCTION, HANDLE_FUNCTION };

     enum class ERESOURCE_ALLOCATION_COMPLETED_STATE
     { UNDEFINED = 0, STARTED, RUNNING, PAUSED, INTERRUPTED, SUCCESS, FAILED };

     CResourceLeakTest(const ERESOURCE_ALLOCATION_TYPES eResourceAllocationType, const size_t nIterations, const size_t nBytesPerIteration)
          : m_eResourceAllocationType(eResourceAllocationType), m_nIterations(nIterations), m_nBytesPerIteration(nBytesPerIteration){}

     virtual ~CResourceLeakTest() {}

     ERESOURCE_ALLOCATION_TYPES GetResourceAllocationType() { return m_eResourceAllocationType; }
     size_t GetInterations() { return m_nIterations; }
     size_t GetBytesPerIteration() { return m_nBytesPerIteration; }
     void SetIsComplete() { m_bComplete = true; }
     bool IsComplete() { return m_bComplete; }
     void SetState(const ERESOURCE_ALLOCATION_COMPLETED_STATE state) { m_state = state; }
     ERESOURCE_ALLOCATION_COMPLETED_STATE GetState() { return m_state; }
     void SetThreadId(uint64_t id) { m_threadId = id; }
     uint64_t GetThreadId() { return m_threadId; }


private:
     ERESOURCE_ALLOCATION_TYPES m_eResourceAllocationType = ERESOURCE_ALLOCATION_TYPES::UNDEFINED;
     size_t m_nIterations = 0;
     size_t m_nBytesPerIteration = 0;
     bool m_bComplete = false;
     ERESOURCE_ALLOCATION_COMPLETED_STATE m_state = ERESOURCE_ALLOCATION_COMPLETED_STATE::UNDEFINED;
     uint64_t m_threadId = 0;
};

#endif // CRESOURCELEAKTEST_H
