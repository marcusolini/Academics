// Copyright(c) 2018 Mark Nicolini

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