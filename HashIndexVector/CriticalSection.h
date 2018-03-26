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

     CriticalSection(const CriticalSection&) = delete;
     CriticalSection& operator=(const CriticalSection&) = delete;

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

private:
     CRITICAL_SECTION m_criticalSection;
     bool bLocked = false;
};


class Guard
{
public:
     Guard(CriticalSection* criticalSection) : m_criticalSection(criticalSection)
     {
          m_criticalSection->Lock();
     }

     virtual ~Guard()
     {
          m_criticalSection->Unlock();
     }

private:

     CriticalSection * m_criticalSection;
};
