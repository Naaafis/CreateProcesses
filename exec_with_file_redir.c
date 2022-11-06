#include "printfile.h"




int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    char* program = argv[1];
    //printf("program: %s ", program);
    char* args = argv[2];
    //printf("args: %s ", args);
    char* outfile = argv[3];
    //printf("outfile: %s ", outfile);

    // TODO: Make cmd line from program, args, and outfile
    // //your solution here!
    char* cmdLine = (char*)malloc(strlen(program) + strlen(args) + strlen(outfile) + strlen("C:\\Windows\\system32\\cmd.exe /c ") + 4);
    // +3 for the 3 spaces and the redirect operator
    if (cmdLine == NULL){
        printf("Failed to allocate memory for cmdLine because of %d \n", GetLastError());
        return 0;
    }

    sprintf(cmdLine, "C:\\Windows\\system32\\cmd.exe /c %s %s > %s", program, args, outfile);
    // printf("cmdLine is %s \n", cmdLine);

    // Values needed for CreateProcessA
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Dead squirrels
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Dead squirrels 
    ZeroMemory(&pi, sizeof(pi));

    // //your solution here!
    // Make sure to spawn the process in such a way that you can get a handle to it! 

    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
        printf("Failed to create process because of %d \n", GetLastError());
    }
    else{
        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);
        // Close process and thread handles. 
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // if (!CreateProcessA("C:\\Windows\\system32\\cmd.exe", cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
    //     printf("Failed to create process because of %d \n", GetLastError());
    //     return 0;
    // }


    // TODO: Wait for processes to exit 
    // //your solution here!
    // WaitForSingleObject(pi.hProcess, INFINITE);


    // TODO: Cleanup
    // //your solution here!
    // CloseHandle(pi.hProcess);
    // CloseHandle(pi.hThread);
    free(cmdLine);
    
    
    //PrintFileContents(outfile);
    if(!PrintFileContents(outfile)){
        printf("Failed to print file contents because of %d \n", GetLastError());
        return 0;
    }
    // else{
    //     // printf("Successfully printed file contents \n");
    //     continue;
    // }

    return 0;

}