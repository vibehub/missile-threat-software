#include <ansi_c.h>
#include <tcpsupp.h>
#include <cvirte.h>		
#include <userint.h>
#include "TServer.h"

static int g_hmainPanel=0;
#define tcpChk(f) if ((g_TCPError=(f)) < 0) {ReportTCPError(); goto Done;} else


int CVICALLBACK ServerTCPCB (unsigned handle, int event, int error, void *callbackData);
static void ReportTCPError (void);


static unsigned int g_hconversation;
static int g_TCPError = 0;

int main (int argc, char *argv[])
{
	int      portNum;
    char     tempBuf[256] = {0};
    int      registered = 0;
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	
	if ((g_hmainPanel = LoadPanel (0, "TServer.uir", SmainPane)) < 0)
		return -1;
	PromptPopup ("Port Number?",
                 "Type the port number on which you would like to register "
                 "this server application.\n\n(example: 10000)",
                 tempBuf, 31);
    portNum = atoi (tempBuf);

    if (RegisterTCPServer (portNum, ServerTCPCB, 0) < 0)
        MessagePopup("TCP Server", "Server registration failed!\n");
    else
        {
        registered = 1;
        
       
        SetCtrlVal (g_hmainPanel, SmainPane_status_message, "server online\n");
        DisplayPanel (g_hmainPanel);
		
        RunUserInterface ();
        }
    
Done:
    if (registered)
        UnregisterTCPServer (portNum);
        DiscardPanel (g_hmainPanel);
        CloseCVIRTE ();
        return 0;
}




int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK transmitfn (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char transmitBuf[512] = {0};
	char asi[40]={0};
	char ele[40]={0};
	char ele1[40]={0};
	char t[40]={0};
	char t1[40]={0}; 
	char type[40]={0};
	char* nn;
	char* ee;
	char* tt;
	int e;
	int time;
	int no;
	int flag=0;
	switch (event)
	{
		case EVENT_COMMIT:
			 SetCtrlVal (g_hmainPanel, SmainPane_status_message, "transmitting info.....\n");
			 GetCtrlVal (g_hmainPanel, SmainPane_no_of_missiles, transmitBuf);
			  nn=strtok(transmitBuf,",");
			  while(nn!=NULL){
			  no=atoi(nn);
			  if(no<=0|| no>=9){
				   flag=1;
				   MessagePopup ("input error", "please enter valid number of missiles (less than 9)");
				   SetCtrlVal (g_hmainPanel, SmainPane_status_message, "input error occured!\n");
			  }
			  nn=strtok(NULL,",");
			  }
			  if(flag==1)
				  break;
			  
			 GetCtrlVal (g_hmainPanel, SmainPane_asimuth, asi);
			 GetCtrlVal (g_hmainPanel, SmainPane_elevation, ele);
			 strcpy(ele1,ele);
			 flag=0;
			 ee=strtok(ele,",");
			  while(ee!=NULL){
			    e = atof (ee) ;
			  if((e>90)|| (e<-90)){
				 	 flag=1;
				  MessagePopup ("input error", "please enter valid elevation angle (-90<=d<=90)");
				  SetCtrlVal (g_hmainPanel, SmainPane_status_message, "input error occured!\n"); 
			 }
			  ee=strtok(NULL,",");
			  }
			  if(flag==1)
				  break;
			  
			  //flag=0;
			 GetCtrlVal (g_hmainPanel, SmainPane_time_impact, t);
			   strcpy(t1,t);  
			   tt=strtok(t,",");
			  while(tt!=NULL){
			    time=atoi(tt);
			  if(time>30) {
				  	   flag=1;
				  MessagePopup ("input error", "please enter valid time to impact(1<t<=30)");
				  SetCtrlVal (g_hmainPanel, SmainPane_status_message, "input error occured!\n"); 
			  }
			  tt=strtok(NULL,",");
			  }
			  if(flag==1)
				  break;
			  
			 GetCtrlVal (g_hmainPanel, SmainPane_type_missile, type);
             strcat (transmitBuf, "#");
			 strcat (transmitBuf, asi);
			 strcat (transmitBuf, "#");
			 strcat (transmitBuf, ele1);
			 strcat (transmitBuf, "#");
			 strcat (transmitBuf, t1);
			 strcat (transmitBuf, "#");
			 strcat (transmitBuf, type);
		   	 strcat (transmitBuf, "#");
			
             if (ServerTCPWrite (g_hconversation, transmitBuf,strlen (transmitBuf), 1000) < 0)
				SetCtrlVal (g_hmainPanel, SmainPane_status_message, "transmission failed!\n");
			
			 else
				SetCtrlVal (g_hmainPanel, SmainPane_status_message, "transmission complete!\n");
			 
			 break;
	}
	return 0;
}

int CVICALLBACK ServerTCPCB (unsigned handle, int event, int error,
                             void *callbackData)
{
    char receiveBuf[256] = {0};
    int  dataSize = sizeof (receiveBuf) - 1;
    char addrBuf[31];

    switch (event)
        {
        case TCP_CONNECT:
            if (g_hconversation)
                {
               
                tcpChk (GetTCPPeerAddr (handle, addrBuf, 31));
                sprintf (receiveBuf, "-- Refusing conection request from ""%s --\n", addrBuf); 
				SetCtrlVal (g_hmainPanel, SmainPane_status_message, receiveBuf);
                tcpChk (DisconnectTCPClient (handle));
                }
            else
                {
                
                g_hconversation = handle;
              
                tcpChk (GetTCPPeerAddr (g_hconversation, addrBuf, 31));
                
                tcpChk (GetTCPPeerName (g_hconversation, receiveBuf, 256));
                
                sprintf (receiveBuf, "-- New connection from %s --\n",addrBuf); 
				SetCtrlVal (g_hmainPanel, SmainPane_status_message, receiveBuf);
               
                tcpChk (SetTCPDisconnectMode (g_hconversation, TCP_DISCONNECT_AUTO));
                }
            break;
      
        case TCP_DISCONNECT:
            if (handle == g_hconversation)
                {
               
                g_hconversation = 0;
                SetCtrlVal (g_hmainPanel, SmainPane_status_message, "-- Client disconnected --\n");
               
                }
        break;
    }
    
Done:    
    return 0;
}

static void ReportTCPError (void)
{
    if (g_TCPError < 0)
        {
        char messageBuffer[1024];
        sprintf(messageBuffer, "TCP library error message: %s\nSystem error message: %s",GetTCPErrorString (g_TCPError), GetTCPSystemErrorString());
        MessagePopup ("Error", messageBuffer);
        }
}
