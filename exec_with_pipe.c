#include <windows.h>
#include <stdio.h>




#define BUF_SIZE 4096

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Usage: %s program.exe \"args and args \"", argv[0]);
        return 0;
    }


    // parse args
    char* program = argv[1];
    char* args = argv[2];

    // create buffer for cmdline argument
    // //your solution here!
    char* cmdLine = (char*)malloc(strlen(program) + strlen(args) + strlen("C:\\Windows\\system32\\cmd.exe /c ") + 1);
    if (cmdLine == NULL){
        printf("Failed to allocate memory for cmdLine because of %d \n", GetLastError());
        return 0;
    }

    
    // Declare handles for StdOut
    HANDLE hStdOutRead, hStdOutWrite;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // TODO: Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
    // Could be relevant: https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
    // //your solution here!
    si.dwFlags = STARTF_USESTDHANDLES;
    

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!
    sa.bInheritHandle = TRUE;

    // TODO: Create a pipe object and share the handle with a child processes 
    // //your solution here!
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)){
        printf("Failed to create pipe because of %d \n", GetLastError());
        return 0;
    }
    if(!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)){
        printf("Failed to set handle information because of %d \n", GetLastError());
        return 0;
    } // I thought this would be redundant since we set si.hStdInput to NULL, but it was mentioned in the link above as well as in the assignment description so I included it.

    // TODO: Set
    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;

    
    // Create the child Processes and wait for it to terminate!
    // //your solution here!
    sprintf(cmdLine, "C:\\Windows\\system32\\cmd.exe /c %s %s", program, args);
    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("Failed to create process because of %d \n", GetLastError());
        return 0;
    }
    else{
        WaitForSingleObject(pi.hProcess, INFINITE); // Not sure why assignement description mentions that we dont necessarily want child process to terminate but comment above says to wait for it to terminate.
    }

    // TODO: perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutWrite);

    // Finally, print the contents from the pipe!
    char buffer[BUF_SIZE];
    DWORD dwBytesRead = 0;
    if(!PeekNamedPipe(hStdOutRead, buffer, BUF_SIZE, &dwBytesRead, NULL, NULL)){
        printf("Failed to peek named pipe because of %d \n", GetLastError());
        return 0;
    }
    if (dwBytesRead > 0){
        if (!ReadFile(hStdOutRead, buffer, BUF_SIZE, &dwBytesRead, NULL)){
            printf("Failed to read file into buffer because of %d \n", GetLastError());
            return 0;
        }
        buffer[dwBytesRead] = '\0';
        printf("%s", buffer);
    }
    else{
        printf("No data in pipe \n");
    }
    CloseHandle(hStdOutRead);
    free(cmdLine);
    
    return 0;
}