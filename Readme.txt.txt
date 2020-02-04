This is a missile warning system which runs on a typical Server-Client architecture.
This uses TCP/IP Service.

Here are directions on how to use the software.

HOW TO RUN SERVER SIDE:
1. the Server side must always be run first
2. once the application is running, a pop appears.
3. In this popup box enter the port number you want to run this application in.(eg:5000)
4. once the port number is specified the Server is running.
5. GUI pops up which is an interface to enter values against the specified labels.
6. on this interface the user will have to input the values like,
     a) number of missiles(integer values only)
     b) asimuth entered in degrees. For multiple missiles, use comma to separate values for each missile. eg: 23.2,56,78 for three missiles respectively.
     c) angle of elevation in degrees. For multiple missiles use comma to separate values of each missile. 
     d)time to impact in seconds. For multiple missiles use comma to separate values of each missile. 
     e) type of missile. select it from the list. eg: AAM,SAM or MANPAD 
8.BEFORE TRANSMISSION, RUN CLIENT SIDE APPLICATION.

HOW TO RUN CLIENT SIDE:
1. once the application is running, apop up appears.
2. This popup asks for the IP address of the Server to which you wish to connect. (give 127.0.0.1 if both server and application run on your PC)
3. once you enter the IP address, press enter.
4. A second popup appears to connect to a port number of the server.
5. note that this port number should match with the number you gave at Server application. Press enter.(in this case 5000)
6. on successful connection establishment, the client side GUI open on the screen.

HOW TO TRANSMIT AND RECIEVE DATA:
1. now once all the information is filled in Step 6 of "HOW TO RUN SERVER SIDE", press transmit button.
2. the information must now appear on the Client side GUI.
3. on Client side, display button is now enabled. press on display button.
4. the missile approach diagram must be displayed on a balck screen on left of Client GUI.
5. this is the required simulation.
6. you can only quit the application when the Server quits first.
7. to quit go to Server GUI , press quit button to terminate the connection.

     