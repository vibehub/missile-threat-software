#include <utility.h>
#include <ansi_c.h>
#include <tcpsupp.h>
#include <cvirte.h>		
#include <userint.h>
#include "tClient.h"

#define tcpChk(f) if ((g_TCPError=(f)) < 0) {ReportTCPError(); goto Done;} else

int CVICALLBACK ClientTCPCB (unsigned handle, int event, int error,void *callbackData); //require in every tcp connection tp handle events of tcp connection 
static void ReportTCPError (void);


static unsigned int g_hconversation;
static int          g_hmainPanel;
static int          g_connected = 0;
static int          g_TCPError = 0;
static char         aval[40];// to read asimuth value
int                 xx,yy;
int                 f=0;
static char         tt[40];
static char         elev[50];

int main (int argc, char *argv[])
{
	int  portNum;
    char tempBuf[256] = {0};
    char portNumStr[32];
    
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	
	if ((g_hmainPanel = LoadPanel (0, "tClient.uir", CmainPane)) < 0)
		goto Done;
    DisableBreakOnLibraryErrors();
	
	PromptPopup ("Server Name?",
                 "Type the IP Address of server "
                 "application.\n\n(example: xxx.xxx.xxx.xxx)",
                 tempBuf, 255);
	
    PromptPopup ("Port Number?",
                 "Type the port number that was set when starting the sample "
                 "server application.\n\n(example: 10000)",
                 portNumStr, 31);
    portNum = atoi (portNumStr);

   
    if (ConnectToTCPServer (&g_hconversation, portNum, tempBuf, ClientTCPCB,
                            NULL, 5000) < 0)
        MessagePopup("TCP Client", "Connection to server failed !");
    else
        {
        g_connected = 1;
		SetCtrlVal (g_hmainPanel, CmainPane_status, "connected to server!\n");
        DisplayPanel (g_hmainPanel);
        RunUserInterface ();
        }
    
Done:
   
    if (g_connected)
    DisconnectFromTCPServer (g_hconversation);
    DiscardPanel (g_hmainPanel);
    CloseCVIRTE ();
    return 0;
	
}
int CVICALLBACK MainPanelCB (int panel, int event, void *callbackData,
                             int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

 CVICALLBACK ClientTCPCB (unsigned handle, int event, int error,
                             void *callbackData)
{
    char rBuf[256] = {0};
	char* ch;
    int  dataSize = sizeof (rBuf) - 1;
	int e;
    switch (event)
        {
        case TCP_DATAREADY:							// case when data is recieved
			
            if ((dataSize = ClientTCPRead (g_hconversation, rBuf,dataSize, 1000))< 0)
                {
				SetCtrlVal (g_hmainPanel, CmainPane_status, "Receive Error\n");  // I have passed the entire information from server to client in a singles string separated by '#'
                }
            else
                {		SetCtrlVal (g_hmainPanel, CmainPane_status, "RECIEVING DATA.....\n");
				  		ch=strtok(rBuf,"#"); //strtok is string tokenizer function. breaks a string in tokens based on delimeter. here delimiter is "#"
						if(ch!=NULL)
							SetCtrlVal (g_hmainPanel, CmainPane_no_of_missile, ch);
				        ch=strtok(NULL,"#");   // to get the next token this is the function call.
						
						if(ch!=NULL)
							SetCtrlVal (g_hmainPanel, CmainPane_asimuth, ch);
						    strcpy(aval,ch);
							
						ch=strtok(NULL,"#");
						
						if(ch!=NULL) 
							SetCtrlVal (g_hmainPanel, CmainPane_elevation, ch);
						     strcpy(elev,ch);
							
						
						  ch=strtok(NULL,"#");
					    if(ch!=NULL)
							SetCtrlVal (g_hmainPanel, CmainPane_time, ch);
						strcpy(tt,ch);
						  ch=strtok(NULL,"#");
						  
						if(ch!=NULL)
							SetCtrlVal (g_hmainPanel, CmainPane_type_of_missile, ch);
						  ch=strtok(NULL,"#"); 
						  
						SetCtrlVal (g_hmainPanel, CmainPane_status, "Data recieved successfully!\n"); 
						
					  SetCtrlAttribute (g_hmainPanel, CmainPane_ALERT, ATTR_VISIBLE, 1);
					  SetCtrlAttribute (g_hmainPanel, CmainPane_plot, ATTR_DIMMED, 0); // the display button is now enabled as data is accquired successfully!
						f++;
					
                
                }
            break;
			
																					 // case for when a disconnection occcurs
        case TCP_DISCONNECT:
			
            MessagePopup ("TCP Client", "Server has closed connection!");
			SetCtrlVal (g_hmainPanel, CmainPane_status, "connection terminated\n");
            g_connected = 0;
            MainPanelCB (0, EVENT_CLOSE, 0, 0, 0);
            break;
			
			
    }
    return 0;
}

static void ReportTCPError(void)						   // function to report any errors that occcured
{
    if (g_TCPError < 0)
        {
        char    messageBuffer[1024];
        sprintf(messageBuffer,"TCP library error message: %s\nSystem error message: %s", GetTCPErrorString (g_TCPError), GetTCPSystemErrorString());
        MessagePopup ("Error", messageBuffer);
        g_TCPError = 0;
        }
}



int CVICALLBACK plotfn (int panel, int control, int event,			   //callack function associated with the display button, to display the map view/ radar view
		void *callbackData, int eventData1, int eventData2)

{
		  int x,y;
		  int r=90;
		  int rr,rx,ry;
		  float z;
		  float rad,res;
		  char* tok;
		  char* tok2;
		  char* tim;
		  int t;  

	switch (event)
	{
		case EVENT_COMMIT:
			  // 300 x 326 is the size of the canvas. I found this out using GetCtrlAttribute of this canvas and used "height" and "width"attribute.
			  // these values were displayed on terminal for my convinience. I have ommited that part of the code for simplicity.
			CanvasClear (g_hmainPanel, CmainPane_display, VAL_ENTIRE_OBJECT);  // clear the canvas
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PICT_BGCOLOR, VAL_BLACK);   //setting background to black
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_GREEN);	
			
			if(f==1){	 //Origin change must happen only once. function maybe called many times. each function call should not change origin.
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_XCOORD_AT_ORIGIN, -163.0);   // change of origin to centre of display
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_YCOORD_AT_ORIGIN, -150.0);
			}
			
			CanvasDrawOval (g_hmainPanel, CmainPane_display, MakeRect(-150,-163,300,326), VAL_DRAW_FRAME);
			CanvasDrawOval (g_hmainPanel, CmainPane_display, MakeRect(-100,-108,200,216), VAL_DRAW_FRAME);
			CanvasDrawOval (g_hmainPanel, CmainPane_display, MakeRect(-50,-53,100,106), VAL_DRAW_FRAME); // drawing the inner green circle
			 
			
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_BLUE);
			CanvasDrawLine (g_hmainPanel, CmainPane_display, MakePoint(0,-150), MakePoint(0,150)); // drawing axis lines in blue color
			CanvasDrawLine (g_hmainPanel, CmainPane_display, MakePoint(-163,0), MakePoint(163,0));
			
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_GREEN);
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_WIDTH, 10);		 // marking the origin by a green point
			CanvasDrawPoint (g_hmainPanel, CmainPane_display, MakePoint(0,0));
		
			tim=strtok(tt,",");
			
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_RED);
			CanvasDrawTextAtPoint (g_hmainPanel, CmainPane_display, tim, VAL_APP_META_FONT, MakePoint(10,10), VAL_CENTER);
			tok=strtok(aval,",");
		
			while(tok!=NULL){
			r=90;	
			z=atof(tok); //atof function is used to convert a string value into float representation.   
			rad= (z*3.141)/180;			// conversion of degrees to rad, I asked user input in degrees
			res=cos(rad);
			yy=100*res;		// calculation of y coordinate of missile by 100cos(x) formula,x in radians, 100 is arbutiary lenght. this length doesnt matter as long as it lies within the canvas
			res=sin(rad);
			xx=100*res;		//calculation of x coordinate of missile by 100cos(x) formula, x in radians.
			
			
			
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_RED); 
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_WIDTH, 15);
			CanvasDrawPoint (g_hmainPanel, CmainPane_display, MakePoint(xx,-yy));   // marking the position of missile. note the coordinate systems are different from our usual 
			// coordinate system after shifting origin. 
			x=xx;
			y=yy;
				SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_WIDTH, 1);  
			SetCtrlAttribute (g_hmainPanel, CmainPane_display, ATTR_PEN_COLOR, VAL_RED);
			
			while(((x!=0)&&(y!=0))|| r>0){			  //to make dotted lines as an animation
			
				x=r*sin(rad);
				y= r*cos(rad);
				if(r%2==0){
				      rr=r-5;
					  rx=rr*sin(rad);
					  ry=rr*cos(rad);
					  CanvasDrawLine (g_hmainPanel, CmainPane_display,MakePoint(x,-y) ,MakePoint(rx,-ry));
					  Delay (0.1);
				}
				r=r-7;
			}
		   tok=strtok(NULL,",");
			}	

			break;
	}
	return 0;
}


