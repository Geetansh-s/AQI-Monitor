This project was made in a very tight deadline, with the intention of gluing up sensors and just making it work and is not intended to be a representative of production grade code. There are a lot of things that are overlooked for the sake of getting things together and running.
There is extensive use of Global Variables that is completely against safe embedded prectices, but the live expression view in cubeide can only handle global variables, so for the sake of easy debugging, global variables are used.
There are multiple copies of variables, confusing variable names, impractical and non optimized code that needs to be refactored, there were no unit tests, no integration/blackbox testing as we were severely time bound.
End product is something that works, and works Accurately. 

The Hardware Block diagram of the project

![Block diag](https://github.com/user-attachments/assets/0a40971f-e9f5-4831-8b69-b0c1240ff776)

Final Device 

![20240311_162527](https://github.com/user-attachments/assets/e8bb8695-a684-4833-8dac-48fa80328b4b)

The device runs on an infinite while loop, it gathers raw data from sensors, calculates the sub index of individual gas according to CPCB conversion tables, and calculates the final AQI.
The Pollutant concentration measurements and AQI is then displayed onto OLED Display and simultaneously logged on Thingspeak, using ESP8266 Module, over HTTP requests sent over UART, with timestamps so we can analyse the data later.
The results were found to be satisfactory when compared with outdoor AQI, when room was fully ventilated with outdoor air. Subsequently, in indoor testing with incense stick , unventillated room full of CO2, burned residual of matchstick containing CO gas and Butane gas, the results were satisfactory 
