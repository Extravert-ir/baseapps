/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS net command
 * FILE:            base/applications/network/net/cmdHelpMsg.c
 * PURPOSE:
 *
 * PROGRAMMERS:     Aleksandar Andrejevic <theflash AT sdf DOT lonestar DOT org>
 */

#include "net.h"

#include <stdlib.h>

INT cmdHelpMsg(INT argc, WCHAR **argv)
{
    WCHAR szBuffer[MAX_PATH];
    HMODULE hMsgDll = NULL;
    INT i;
    LONG errNum;
    LPWSTR endptr;
    LPWSTR pBuffer;

    if (argc < 3)
    {
        ConResPuts(StdOut, IDS_GENERIC_SYNTAX);
        ConResPuts(StdOut, IDS_HELPMSG_SYNTAX);
        return 1;
    }

    for (i = 2; i < argc; i++)
    {
        if (_wcsicmp(argv[i], L"/help") == 0)
        {
            ConResPuts(StdOut, IDS_GENERIC_SYNTAX);
            ConResPuts(StdOut, IDS_HELPMSG_SYNTAX);
            ConResPuts(StdOut, IDS_HELPMSG_HELP_1);
            ConResPuts(StdOut, IDS_HELPMSG_HELP_2);
            return 1;
        }
    }

    errNum = wcstol(argv[2], &endptr, 10);
    if (*endptr != 0)
    {
        ConResPuts(StdOut, IDS_GENERIC_SYNTAX);
        ConResPuts(StdOut, IDS_HELPMSG_SYNTAX);
        return 1;
    }

    if (errNum >= MIN_LANMAN_MESSAGE_ID && errNum <= MAX_LANMAN_MESSAGE_ID)
    {
        /* Load netmsg.dll */
        GetSystemDirectoryW(szBuffer, ARRAYSIZE(szBuffer));
        wcscat(szBuffer, L"\\netmsg.dll");

        hMsgDll = LoadLibrary(szBuffer);
        if (hMsgDll == NULL)
        {
            ConPrintf(StdOut, L"Failed to load netmsg.dll\n");
            return 0;
        }

        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE |
                       FORMAT_MESSAGE_IGNORE_INSERTS,
                       hMsgDll,
                       errNum,
                       LANG_USER_DEFAULT,
                       (LPWSTR)&pBuffer,
                       0,
                       NULL);
        if (pBuffer)
        {
            ConPrintf(StdOut, L"\n%s\n", pBuffer);
            LocalFree(pBuffer);
        }

        FreeLibrary(hMsgDll);
    }
    else
    {
        /* Retrieve the message string without appending extra newlines */
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                       FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL,
                       errNum,
                       LANG_USER_DEFAULT,
                       (LPWSTR)&pBuffer,
                       0,
                       NULL);
        if (pBuffer)
        {
            ConPrintf(StdOut, L"\n%s\n", pBuffer);
            LocalFree(pBuffer);
        }
    }

    return 0;
}

/* EOF */

