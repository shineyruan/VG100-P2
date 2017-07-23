# VG100-P2

Gesture-Based Mouse Cursor Control System  
This system can only work on windows system. If your operating system is IOS or Linux, sorry, I don't know how to handle it.  

## Compile Executive Program

Now we have three programs to compile

 - slide
 - move
 - drag

To compile them,  
`cd program`  
`g++ -o slide.exe slide-UARTTest.cpp Com.cpp JY901.cpp stdafx.cpp`  
`g++ -o move.exe move-UARTTest.cpp Com.cpp JY901.cpp stdafx.cpp`  
`g++ -o drag.exe drag-UARTTest.cpp Com.cpp JY901.cpp stdafx.cpp`  


## AutoHotKey

Download AutoHotKey and install (only in windows system)  
Use this software to compile all .ahk files  

## Final step

Executive the .exe file you want to use and execute corresponding .bat file.  
Turn on your sensor.  
Now you can use your gesture to control the mouse cursor.  
