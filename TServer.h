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

#define  SmainPane                       1
#define  SmainPane_no_of_missiles        2
#define  SmainPane_asimuth               3
#define  SmainPane_elevation             4
#define  SmainPane_time_impact           5
#define  SmainPane_transmit              6       /* callback function: transmitfn */
#define  SmainPane_QUITBUTTON            7       /* callback function: QuitCallback */
#define  SmainPane_status_message        8
#define  SmainPane_heading               9
#define  SmainPane_type_missile          10


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK transmitfn(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
