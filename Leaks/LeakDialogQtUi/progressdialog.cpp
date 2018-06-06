#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "leakdialogqtui.h"

#include "LeakLib.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    connect(ui->StopPushButton, SIGNAL (released()), this, SLOT(handleStopButton()));

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parent);

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
/*
        std::thread thread = std::thread(AllocationThreadFunc, hDlg, test.GetResourceAllocationType(), test.GetInterations(), test.GetBytesPerIteration());

        std::wstringstream ss;
        ss << thread.get_id();
        uint64_t id = std::stoull(ss.str());

        test.SetThreadId(id);
        test.SetState( ERESOURCE_ALLOCATION_COMPLETED_STATE::STARTED);

        thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
*/
        CLeakLib::LeakNewMemory(iTest.GetInterations(), iTest.GetBytesPerIteration());
    }

    ui->RunTestsProgressBar->setValue(75);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::handleStopButton()
{
    this->close();
}

/*
void AllocationThreadFunc(const HWND hDlg, const ERESOURCE_ALLOCATION_TYPES eAllocationType, const size_t nIterations, const size_t nBytesPerIteration)
{
     DWORD dwStatus = ERROR_SUCCESS;
     ERESOURCE_ALLOCATION_COMPLETED_STATE state = ERESOURCE_ALLOCATION_COMPLETED_STATE::UNDEFINED;
     std::wstringstream ss;
     uint64_t id = 0;

     ss << std::this_thread::get_id();
     id = std::stoull(ss.str());

     state = ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING;

     SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)state);

     std::this_thread::yield();

     for (size_t nIteration = 0; (nIteration < nIterations) && gbThreadContinue && SUCCEEDED(dwStatus); nIteration++)
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING;

          switch (eAllocationType)
          {
          case ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
               dwStatus = CLeakLib::LeakNewMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, (LoadStringFromResourceId(IDS_NEW_OPER_FAILURE)).c_str(), (LoadStringFromResourceId(IDS_FAILURE)).c_str(), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
               dwStatus = CLeakLib::LeakMallocMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, (LoadStringFromResourceId(IDS_MALLOC_FUNC_FAILURE)).c_str(), (LoadStringFromResourceId(IDS_FAILURE)).c_str(), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
               dwStatus = CLeakLib::LeakCallocMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, (LoadStringFromResourceId(IDS_CALLOC_FUNC_FAILURE)).c_str(), (LoadStringFromResourceId(IDS_FAILURE)).c_str(), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
               dwStatus = CLeakLib::LeakHandle(1);
               if (FAILED(dwStatus)) MessageBox(hDlg, (LoadStringFromResourceId(IDS_HANDLE_FUNC_FAILURE)).c_str(), (LoadStringFromResourceId(IDS_FAILURE)).c_str(), (MB_ICONERROR | MB_OK));
               break;
          default:
               MessageBox(hDlg, (LoadStringFromResourceId(IDS_INVALID_ALLOC_TYPE)).c_str(), (LoadStringFromResourceId(IDS_INVALID)).c_str(), (MB_ICONERROR | MB_OK));
               dwStatus = E_INVALIDARG;
               break;
          }

          while (gbThreadPause && gbThreadContinue)
          {
               SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)ERESOURCE_ALLOCATION_COMPLETED_STATE::PAUSED);
               std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });
               SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING);
          }
     }

     if (SUCCEEDED(dwStatus))
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::SUCCESS;
     }
     else
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED;
     }

     if (false == gbThreadContinue)
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::INTERRUPTED;
     }


     SendMessage(hDlg, WM_USER_THREAD_COMPLETE, (WPARAM)id, (LPARAM)state);
}
*/
