﻿/*
 * PROJECT:         ReactOS On-Screen Keyboard
 * LICENSE:         GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:         Settings file for warning dialog on startup
 * COPYRIGHT:       Copyright 2018 Bișoc George (fraizeraust99 at gmail dot com)
 */

/* INCLUDES *******************************************************************/

#include "osk.h"
#include "settings.h"

/* FUNCTIONS *******************************************************************/

BOOL LoadDataFromRegistry()
{
    HKEY hKey;
    LONG lResult;
    DWORD dwShowWarningData, dwLayout;
    DWORD cbData = sizeof(DWORD);

    /* Set the structure members to TRUE */
    Globals.bShowWarning = TRUE;
    Globals.bIsEnhancedKeyboard = TRUE;

    /* Open the key, so that we can query it */
    lResult = RegOpenKeyExW(HKEY_CURRENT_USER,
                            L"Software\\Microsoft\\osk",
                            0,
                            KEY_READ,
                            &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        return FALSE;
    }

    /* Query the key */
    lResult = RegQueryValueExW(hKey,
                               L"ShowWarning",
                               0,
                               0,
                               (BYTE *)&dwShowWarningData,
                               &cbData);

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        RegCloseKey(hKey);
        return FALSE;
    }

    /* Load the data value (it can be either FALSE or TRUE depending on the data itself) */
    Globals.bShowWarning = (dwShowWarningData != 0);

    /* Query the key */
    lResult = RegQueryValueExW(hKey,
                               L"IsEnhancedKeyboard",
                               0,
                               0,
                               (BYTE *)&dwLayout,
                               &cbData);

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        RegCloseKey(hKey);
        return FALSE;
    }

    /* Load the dialog layout value */
    Globals.bIsEnhancedKeyboard = (dwLayout != 0);
    
    /* If we're here then we succeed, close the key and return TRUE */
    RegCloseKey(hKey);
    return TRUE;
}

BOOL SaveDataToRegistry()
{
    HKEY hKey;
    LONG lResult;
    DWORD dwShowWarningData, dwLayout;

    /* If no key has been made, create one */
    lResult = RegCreateKeyExW(HKEY_CURRENT_USER,
                              L"Software\\Microsoft\\osk",
                              0,
                              NULL,
                              0,
                              KEY_WRITE,
                              NULL,
                              &hKey,
                              NULL);

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        return FALSE;
    }

    /* The data value of the subkey will be appended to the warning dialog switch */
    dwShowWarningData = Globals.bShowWarning;

    lResult = RegSetValueExW(hKey,
                             L"ShowWarning",
                             0,
                             REG_DWORD,
                             (BYTE *)&dwShowWarningData,
                             sizeof(dwShowWarningData));

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        RegCloseKey(hKey);
        return FALSE;
    }

    /* The value will be appended to the layout dialog */
    dwLayout = Globals.bIsEnhancedKeyboard;

    lResult = RegSetValueExW(hKey,
                             L"IsEnhancedKeyboard",
                             0,
                             REG_DWORD,
                             (BYTE *)&dwLayout,
                             sizeof(dwLayout));

    if (lResult != ERROR_SUCCESS)
    {
        /* Bail out and return FALSE if we fail */
        RegCloseKey(hKey);
        return FALSE;
    }

    /* If we're here then we succeed, close the key and return TRUE */
    RegCloseKey(hKey);
    return TRUE;
}
