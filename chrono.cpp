#include <sys/time.h>
#include <iostream>
#include <iomanip>

struct timeval timeStart,timeEnd;

void setStartMark(){
    gettimeofday(&timeStart, NULL); 
}

void setEndMark(){
    gettimeofday(&timeEnd,NULL);
}

void displayDuration(){
  std::cout << "duration : " 
	    << std::setprecision(6)
	    << (float)(timeEnd.tv_sec - timeStart.tv_sec) + (float)(timeEnd.tv_usec - timeStart.tv_usec)/1000000 
	    << " sec"
	    << std::endl
	    << std::endl;
} 
