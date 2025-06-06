/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Authors:
 *
 *     dmex    2017
 *
 */

// NOTE: Copied from processhacker2\ProcessHacker\procprp.h

#ifndef PV_PRP_H
#define PV_PRP_H

#define PV_PROPCONTEXT_MAXPAGES 20

typedef struct _PV_PROPSHEETCONTEXT
{
    BOOLEAN LayoutInitialized;
    WNDPROC DefaultWindowProc;
    PH_LAYOUT_MANAGER LayoutManager;
    PPH_LAYOUT_ITEM TabPageItem;

    WNDPROC OldOptionsButtonWndProc;
    HWND OptionsButtonWindowHandle;
    HWND OptionsSearchWindowHandle;
    PWSTR PositionSettingName;
    PWSTR SizeSettingName;
} PV_PROPSHEETCONTEXT, *PPV_PROPSHEETCONTEXT;

typedef struct _PV_PROPCONTEXT
{
    PROPSHEETHEADER PropSheetHeader;
    HPROPSHEETPAGE *PropSheetPages;
    BOOLEAN EnableControlButtons;
    PWSTR PositionSettingName;
    PWSTR SizeSettingName;
} PV_PROPCONTEXT, *PPV_PROPCONTEXT;

typedef struct _PV_PROPPAGECONTEXT
{
    PPV_PROPCONTEXT PropContext;
    PVOID Context;
    PROPSHEETPAGE PropSheetPage;

    BOOLEAN LayoutInitialized;
} PV_PROPPAGECONTEXT, *PPV_PROPPAGECONTEXT;

PPV_PROPCONTEXT HdCreatePropContext(
    _In_ PCWSTR Caption
    );

BOOLEAN PvAddPropPage(
    _Inout_ PPV_PROPCONTEXT PropContext,
    _In_ _Assume_refs_(1) PPV_PROPPAGECONTEXT PropPageContext
    );

BOOLEAN PvAddPropPage2(
    _Inout_ PPV_PROPCONTEXT PropContext,
    _In_ HPROPSHEETPAGE PropSheetPageHandle
    );

PPV_PROPPAGECONTEXT PvCreatePropPageContext(
    _In_ LPCWSTR Template,
    _In_ DLGPROC DlgProc,
    _In_opt_ PVOID Context
    );

PPV_PROPPAGECONTEXT PvCreatePropPageContextEx(
    _In_opt_ PVOID InstanceHandle,
    _In_ LPCWSTR Template,
    _In_ DLGPROC DlgProc,
    _In_opt_ PVOID Context
    );

#define PH_PROP_PAGE_TAB_CONTROL_PARENT ((PPH_LAYOUT_ITEM)0x1)

PPH_LAYOUT_ITEM PvAddPropPageLayoutItem(
    _In_ HWND hwnd,
    _In_ HWND Handle,
    _In_ PPH_LAYOUT_ITEM ParentItem,
    _In_ ULONG Anchor
    );

PPH_LAYOUT_ITEM PvAddPropPageLayoutItemEx(
    _In_ HWND hwnd,
    _In_ HWND Handle,
    _In_ PPH_LAYOUT_ITEM ParentItem,
    _In_ ULONG Anchor,
    _In_ BOOLEAN EnableControlButtons,
    _In_opt_ PWSTR PositionSettingName,
    _In_opt_ PWSTR SizeSettingName
    );

VOID PvDoPropPageLayout(
    _In_ HWND hwnd
    );

_Success_(return)
FORCEINLINE
BOOLEAN
PvPropPageDlgProcHeader(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ LPARAM lParam,
    _Out_ LPPROPSHEETPAGE *PropSheetPage,
    _Out_ PPV_PROPPAGECONTEXT *PropPageContext
    )
{
    LPPROPSHEETPAGE propSheetPage;
    PPV_PROPPAGECONTEXT propPageContext;

    if (uMsg == WM_INITDIALOG)
    {
        // Save the context.
        propSheetPage = (LPPROPSHEETPAGE)lParam;
        propPageContext = (PPV_PROPPAGECONTEXT)propSheetPage->lParam;

        PhSetWindowContext(hwndDlg, 0xfff, propSheetPage);
    }
    else
    {
        propSheetPage = (LPPROPSHEETPAGE)PhGetWindowContext(hwndDlg, 0xfff);

        if (!propSheetPage)
            return FALSE;

        propPageContext = (PPV_PROPPAGECONTEXT)propSheetPage->lParam;

        if (uMsg == WM_NCDESTROY)
        {
            PhRemoveWindowContext(hwndDlg, 0xfff);
        }
    }

    *PropSheetPage = propSheetPage;
    *PropPageContext = propPageContext;

    return TRUE;
}

#endif
