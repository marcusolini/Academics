// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

class Guard;
class CriticalSection;

class CriticalSection
{
public:
     CriticalSection()
     {
          InitializeCriticalSectionAndSpinCount(&m_criticalSection, 1000);
     }

     virtual ~CriticalSection()
     {
          DeleteCriticalSection(&m_criticalSection);
     }

     void Lock()
     {
          EnterCriticalSection(&m_criticalSection);
          bLocked = true;
     }

     void Unlock()
     {
          if (true == bLocked)
          {
               bLocked = false;
               LeaveCriticalSection(&m_criticalSection);
          }
     }

     CriticalSection(const CriticalSection&) = delete;
     CriticalSection& operator=(const CriticalSection&) = delete;

private:
     CRITICAL_SECTION m_criticalSection;
     bool bLocked = false;
};


class CriticalSectionGuard
{
public:
     explicit CriticalSectionGuard(CriticalSection& criticalSection) : m_criticalSection(criticalSection)
     {
          m_criticalSection.Lock();
     }

     virtual ~CriticalSectionGuard()
     {
          m_criticalSection.Unlock();
     }

     CriticalSectionGuard(CriticalSectionGuard const&) = delete;
     CriticalSectionGuard& operator= (CriticalSectionGuard const&) = delete;

private:

     CriticalSection& m_criticalSection;
};
