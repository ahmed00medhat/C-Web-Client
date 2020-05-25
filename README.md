# C-Web-Client
main.cpp is the program to get the weather of a certain zipcode.
the zipcode should be a valid 5 digit number.
I am using the http://api.openweathermap.org/ APIs to query the weather
the weather details are then prined to standard output.

The application depends on the libcurl and libjsoncpp
Compilation command: g++ main.cpp -lcurl -ljsoncpp  -g

Usage: ./a.out \<zip-code\>
