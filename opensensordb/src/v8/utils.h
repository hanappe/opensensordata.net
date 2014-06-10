#ifndef JSUTILS_H
#define JSUTILS_H

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

namespace {

  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  }
  
  int strtoi(const std::string& str) {
                
    
    const char* str_beg = str.c_str();
    const char* str_end = str_beg + str.size();
    char* p = NULL;
    
    const int result = std::strtol(str.c_str(), &p, 10);
    
    if (p == str_beg) { 
      return 0;
    }
                
    for (; p != str_end && std::isspace(*p); ++p) {
      // eat trailing spaces
    }
    
    if (p != str_end || errno != 0) {
      return 0;
    }
                
    return result;
  }

  time_t ConvertToTime(const std::string& s) {
    
    time_t result = 0;
    std::vector<std::string> both = split(s, ' ');
    if (both.size() == 2) {
      time_t t;
      struct tm * tinfo;
      
      time(&t); // get current timeinfo and modify it
      tinfo = localtime(&t);
      
      bool has_date = false;
      bool has_time = false;
      // Parse date
      std::vector<std::string> date = split(both[0], '-');
      if (date.size() == 3) {
	tinfo->tm_year = strtoi(date[0]) - 1900;
	tinfo->tm_mon = strtoi(date[1]) - 1;
	tinfo->tm_mday = strtoi(date[2]);
	has_date = true;
      }
      
      // Parse time
      std::vector<std::string> time = split(both[1], ':');
      if (date.size() == 3) {
	tinfo->tm_hour = strtoi(time[0]);
	tinfo->tm_min = strtoi(time[1]);
	tinfo->tm_sec = strtoi(time[2]);
	has_time = true;
      }
      
      result = (has_time && has_date) ? mktime(tinfo) : 0;
      //std::cout << "Converted Date: " << date[0] << " " << std::endl;

    }
    return result;
  }

  time_t ConvertToTimeSinceEpoch(const std::string& s) {
    
    time_t result = 0;
    std::vector<std::string> both = split(s, ' ');
    if (both.size() == 2) {
      time_t t;
      struct tm * tinfo;
      
      time(&t); // get current timeinfo and modify it
      tinfo = localtime(&t);
      
      bool has_date = false;
      bool has_time = false;
      // Parse date
      std::vector<std::string> date = split(both[0], '-');
      if (date.size() == 3) {
	tinfo->tm_year = strtoi(date[0]) - 1900;
	tinfo->tm_mon = strtoi(date[1]) - 1;
	tinfo->tm_mday = strtoi(date[2]);
	
        tinfo->tm_year *= 1000;
	tinfo->tm_mon *= 1000;
	tinfo->tm_mday *= 1000;
	
        has_date = true;
      }
      
      // Parse time
      std::vector<std::string> time = split(both[1], ':');
      if (date.size() == 3) {
              char* p = NULL;
              //tinfo->tm_hour = std::strtol(time[0].c_str(), &p, 10);
              //tinfo->tm_min = std::strtol(time[1].c_str(), &p, 10);
              //tinfo->tm_sec = std::strtol(time[2].c_str(), &p, 10);
              tinfo->tm_hour = atoi(time[0].c_str());
              tinfo->tm_min = atoi(time[1].c_str());
              tinfo->tm_sec = atoi(time[2].c_str());
	
              has_time = true;
      }
      

      if (has_time && has_date) {
              result = mktime(tinfo);
              //result = (mktime(tinfo) * 1000) + (tinfo->tm_hour * 60 * 60 + tinfo->tm_min * 60 + tinfo->tm_sec);
              std::cout << "TESTEUH A: " << tinfo->tm_hour << "_" <<  tinfo->tm_min << "_" <<  tinfo->tm_sec << "_" << std::endl;
              std::cout << "TESTEUH B: " << time[0] << "_" <<  time[1] << "_" <<  time[2] << "_" << std::endl;
              std::cout << "TESTEUH C: " << both[1] << std::endl;
              
              //result = (mktime(tinfo) * 1000) + (tinfo->tm_sec * 1000);
      } else {
              result = 0;
      }
      //std::cout << "Converted Date: " << date[0] << " " << std::endl;

    }

    return result;
  }



};

#endif // JSUTILS_H
