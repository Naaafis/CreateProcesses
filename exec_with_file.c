#include "printfile.h"




int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    // arse args
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];

    // create buffer for cmdline argument
    // //your solution here!

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // TODO: Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
    
    // //your solution here!
    si.dwFlags = STARTF_USESTDHANDLES;


    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!
    sa.bInheritHandle = TRUE;

    // TODO: Create a file  object and share the handle with a child processes 
    // //your solution here!
    HANDLE hFile = CreateFileA(outfile, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        printf("Failed to create file because of %d \n", GetLastError());
        return 0;
    }

    // TODO: Set
    // set startupinfo handles
    // //your solution here!
    si.hStdInput = NULL;
    si.hStdError = hFile;
    si.hStdOutput = hFile;

    
    // Create the child Processes and wait for it to terminate!
    // //your solution here!
    
    char* cmdline = (char*)malloc(strlen(program) + strlen(args) + strlen("C:\\Windows\\system32\\cmd.exe /c") + 1);
    if (cmdline == NULL){
        printf("Failed to allocate memory for cmdLine because of %d \n", GetLastError());
        return 0;
    }
    sprintf(cmdline, "C:\\Windows\\system32\\cmd.exe /c %s %s", program, args);
    
    if(!CreateProcessA(NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("CreateProcess failed because of %d\n", GetLastError());
    }
    else{
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hFile);
        // printf("Child process terminated\n");
    }
    // printf("Done\n");

    // TODO: perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    free(cmdline);
    // done above 

    // Finally, print the contents of the file!
    if(!PrintFileContents(outfile)){
        printf("Failed to print file contents because of %d \n", GetLastError());
        return 0;
    }
    else{
       //  printf("Successfully printed file contents \n");
    }
    return 0;
}