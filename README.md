# HomeSecuritySystem
Embedded C

Idea of this project is to develop a system which would alert the user through an alarm. The home security system would have three main parts 1. Fire Detection 2. gas leakage detection 3. theft control. For ﬁre detection we would interface a temperature detector with the STM32 board. So whenever the sensor gives a positive output the board will actuate a signal which would be used by the alarm to alert the user. Similarly for gas leakage we will use a gas detector with whose output the controller would actuate the alarm to alert the user. Also for the theft control part of the security system we will use a proximity sensor which, on having someone around, would give a signal to the controller. Based on this signal the controller would turn on the alarm. 

##Components :
1. STM32F4 Discovery Board 
2. Sensors : 

• Temperature Sensor (MCP9700A-E/TO) for ﬁre detection. (Datasheet — https://www.sparkfun.com/datasheets/DevTools/LilyPad/MCP9700.pdf ) 
• Gas Sensor (MQ5) for gas detection. (Datasheet — https://www.seeedstudio.com/depot/datasheet/MQ-5.pdf ) 
• Proximity Sensor (GP2Y0A21YK0F Sharp IR). (Datasheet — http://www.sharpsma.com/Webfm_Send/1208 )
