/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2020. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CmainPane                       1
#define  CmainPane_message               2
#define  CmainPane_status                3
#define  CmainPane_ALERT                 4
#define  CmainPane_no_of_missile         5
#define  CmainPane_asimuth               6
#define  CmainPane_elevation             7
#define  CmainPane_time                  8
#define  CmainPane_type_of_missile       9
#define  CmainPane_display               10
#define  CmainPane_plot                  11      /* callback function: plotfn */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK plotfn(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
