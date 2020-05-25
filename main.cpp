#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <curl/curl.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <jsoncpp/json/json.h>

#define API_KEY "5f00619da00d5e9c4972c673e406e6fe"
#define URL "http://api.openweathermap.org/data/2.5/weather?appid="
#define VWIDTH 6
#define SFILL ' '

//http://api.openweathermap.org/data/2.5/weather?appid=5f00619da00d5e9c4972c673e406e6fe&zip=80234,us&units=imperial


struct memory {
    char *response;
    size_t size;
};


static std::string resp;

static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
    
    std::string tmp((char *) data,nmemb);
    resp.append(tmp);
    size_t realsize = size * nmemb;
    //struct memory *mem = (struct memory *)userp;
    //printf("%.*s=========\n", nmemb, (char*) data);
    // printf("%.*s=========\n", mem->size, (char*) mem->response);
    // char *ptr = realloc(mem->response, mem->size + realsize + 1);
    // if(ptr == NULL)
    //     return 0;  /* out of memory! */

    // mem->response = ptr;
    // memcpy(&(mem->response[mem->size]), data, realsize);
    // mem->size += realsize;
    // mem->response[mem->size] = 0;

    return realsize;
}


int main(int argc, char** argv)
{
  CURL *curl;
  CURLcode res;
  int zipcode=0;

  if(argc < 2 ){
    std::cout<<"Wrong parameters"<<std::endl;
    std::cout<<"Usage: a.out <zipcode>"<<std::endl;
    return 1;
  }else 
  {
    try{
      zipcode = std::stoi(argv[1]);
    }catch (std::exception e){
      std::cout<< e.what() << std::endl;
      return 2;
    }

    if(strlen(argv[1]) < 5){
      std::cout<<"ZIPCODE sould be an intiger of 5 digits"<<std::endl;
      std::cout<<"Usage: a.out <zipcode>"<<std::endl;
      return 3;
    }
  }

  std::string url;
  url.append(URL);
  url.append(API_KEY);
  url.append("&zip=");
  url.append( argv[1]);
  url.append(",us");
  url.append("&units=imperial");
  

  //std::cout<<url<<std::endl;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    struct memory chunk;
 
    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);

    //std::cout<<resp<<std::endl;

    Json::Value root;
    Json::Value * ptr;
    Json::Reader reader;
    
    reader.parse(resp, root);
    
    
    if(root["cod"].asString().compare("200") != 0){
      std::cout<< "City Not Found"<<std::endl;
      return 4;
    }
    const std::time_t x= root["dt"].asUInt();
    std::cout<< std::asctime(std::localtime(&x))<<std::endl;

    std::cout<<root["name"].asString()<<", LAT:"<<root["coord"]["lat"].asFloat()<<",LNG:"<<root["coord"]["lat"].asFloat()<<std::endl;

    ptr= &root["weather"][0];
    std::cout<<(*ptr)["main"].asString()<<": "<<(*ptr)["description"].asString()<<std::endl;
    
    ptr= &root["main"];
    std::cout<< std::setw(10) << std::setfill(SFILL) << (*ptr)["temp"].asFloat()<<" °f"<<std::endl;
    std::cout<< (*ptr)["temp_min"].asFloat()<<" °f";
    std::cout<< std::setw(12) << std::setfill(SFILL) << (*ptr)["temp_max"].asFloat()<<" °f"<<std::endl;
    std::cout<< "Feels Like " << std::setw(VWIDTH) << std::setfill(SFILL) << (*ptr)["feels_like"].asFloat()<<" °f"<<std::endl;
    std::cout<< "Pressure   " << std::setw(VWIDTH) << std::setfill(SFILL) << (*ptr)["pressure"].asFloat()<<" hPa"<<std::endl;
    std::cout<< "Visibility " << std::setw(VWIDTH) << std::setfill(SFILL) << root["visibility"].asFloat()<<" m"<<std::endl;

    std::cout<< "Wind:Speed " << std::setw(VWIDTH) << std::setfill(SFILL) << root["wind"]["speed"].asFloat()<<" miles/hr";
    std::cout<< ", Direction"<< std::setw(VWIDTH) << std::setfill(SFILL) << root["wind"]["deg"].asInt()<<" °"<<std::endl;
    std::cout<< "Cloudy     "<< std::setw(VWIDTH) << std::setfill(SFILL) << root["clouds"]["all"].asInt()<<" %"<<std::endl;


    

  }
  return 0;
}
