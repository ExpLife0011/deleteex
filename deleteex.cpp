//
// Copyright 2017 OSR Open Systems Resources, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE
// 
#include "stdafx.h"


int wmain(int argc, wchar_t *argv[])
{

    HANDLE hFile = INVALID_HANDLE_VALUE;
    FILE_DISPOSITION_INFO dispoInfo;
    FILE_DISPOSITION_INFO_EX dispoInfoEx;

    //
    // Create and open link to the running EXE
    //
    if (!CreateHardLink(L"OsrLink.exe", argv[0], NULL)) {
        printf("Create link failed %d\n", GetLastError());
        goto Exit;
    }

    hFile = CreateFile(L"OsrLink.exe",
                       DELETE, 
                       FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Create failed %d\n", GetLastError());
        goto Exit;
    }

    //
    // Traditional delete, which fails because there's an active image section
    //
    dispoInfo.DeleteFile = TRUE;

    if (!SetFileInformationByHandle(hFile, 
                                    FileDispositionInfo,
                                    &dispoInfo,
                                    sizeof(FILE_DISPOSITION_INFO))) {
        printf("FileDispositionInfo failed as expected! %d\n", GetLastError());
    } else {
        printf("FileDispositionInfo worked??!\n");
    }

    //
    // New delete, which works because we're not deleting the last link to the
    // file!
    //
    dispoInfoEx.Flags = FILE_DISPOSITION_FLAG_DELETE;

    if (!SetFileInformationByHandle(hFile, 
                                    FileDispositionInfoEx,
                                    &dispoInfoEx,
                                    sizeof(FILE_DISPOSITION_INFO_EX))) {
        printf("FileDispositionInfoEx failed!? %d\n", GetLastError());
        goto Exit;
    }

    printf("Link deleted...\n");

Exit:
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
    }
    return 0;
}

