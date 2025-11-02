// ZiggyLauncher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ZiggyLauncher.h"

using namespace std;

string instrument = "";

int main(int argc, char** argv)
{
    instrument = argv[1];

    int retVal = 0;

    int rep = 0;
    int res=-1;
    
    while (res != 0 && rep < 5)
    {
        printf("%i", rep);
        res = RunExternal(instrument);

        ++rep;
    }
    if (rep == 5) retVal = -1;
    return retVal;
}


int RunExternal(std::string param)
{
    DWORD exitCode = 0;
    SHELLEXECUTEINFO ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = _T("open");
    ShExecInfo.lpFile = _T("ZiggyAnalProc.exe");
    ShExecInfo.lpParameters = param.c_str();
    ShExecInfo.lpDirectory = NULL;  // strEXEPath.c_str();
    //ShExecInfo.lpDirectory = "C:\\Users\\dmorr\\Dropbox\\Indicators\\Dev\\ZiggyAnal\\Release";  // strEXEPath.c_str();
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);

    if (WaitForSingleObject(ShExecInfo.hProcess, INFINITE) == 0) {
        GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);
        if (exitCode != 0) {
            return exitCode;
        }
        else {
            return 0;
        }
    }
    else {
        return -99;
    }
}

/*
bool ClassName::ExecuteExternalExeFileNGetReturnValue(Parameter ...)
{
    DWORD exitCode = 0;
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = _T("open");
    ShExecInfo.lpFile = _T("XXX.exe");
    ShExecInfo.lpParameters = strParameter.c_str();
    ShExecInfo.lpDirectory = strEXEPath.c_str();
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);

    if(WaitForSingleObject(ShExecInfo.hProcess,INFINITE) == 0){
        GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);
        if(exitCode != 0){
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}


res = CreateProcess(
        NULL,                //  pointer to name of executable module
        commandLine,         //  pointer to command line string
        NULL,                //  pointer to process security attributes
        NULL,                //  pointer to thread security attributes
        TRUE,                //  handle inheritance flag
        0,                   //  creation flags
        NULL,                //  pointer to new environment block
        NULL,                //  pointer to current directory name
        &StartupInfo,        //  pointer to STARTUPINFO
        &ProcessInfo         //  pointer to PROCESS_INFORMATION
      );

    if (!res)
      //  process creation failed!
      {
        showError(commandLine);
        retVal = 1;
      }
    else if (waitForCompletion)
      {
        res = WaitForSingleObject(
                ProcessInfo.hProcess,
                INFINITE      // time-out interval in milliseconds
                );
        GetExitCodeProcess(ProcessInfo.hProcess, &exitCode);

        retVal = (int)exitCode;
      }

      
      
      
      
      */