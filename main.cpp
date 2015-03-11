/*************************************************************************
    > File Name: ChinaUnicom.cpp
    > Author: ic2y
    > Mail: ic2y@qq.com
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define DEBUG false
CURL *curl;
char *findPosition;
bool onlineFlag = false;
int sleepTime = 10;
int phoneOffset = 89;
int passwordOffset = 110;
char targetURL[] = "http://www.baidu.com";
char keyword[] = "200";
char loginURL[] = "http://202.106.46.37/login.do?callback=jQuery17104011246492154896_1426003928431&username=00000000000&password=000000&passwordType=6&wlanuserip=&userOpenAddress=bj&checkbox=0&basname=&setUserOnline=&sap=&macAddr=&bandMacAuth=0&isMacAuth=&basPushUrl=http%253A%252F%252F202.106.46.37%252F&passwordkey=&_=1426003931839";



void autoLogin();
void initCurl();
void debug(char *msg);
void log(char *msg);
void chinaUnicomIsGood();
bool loadConfig(int argc,char *argv[]);


void debug(char *msg){
    if(DEBUG){
        log(msg);
    }
}

void log(char *msg){
    time_t now;
    time(&now);
    printf("[TIME]:%s[DEBUG]:%s\n",ctime(&now),msg);
}

void autoLogin()
{
    CURL *loginCURL = curl_easy_init();
    if(loginCURL){
        curl_easy_setopt(loginCURL, CURLOPT_READFUNCTION,NULL);
        curl_easy_setopt(loginCURL,CURLOPT_URL,loginURL);
        curl_easy_perform(loginCURL);
        curl_easy_cleanup(loginCURL);
        log("send login done");
    }else{
        log("send login error");
    }
}
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    findPosition = strstr(ptr,keyword);
    if(findPosition){
        onlineFlag = true;
    }
    return 0;
}


bool loadConfig(int argc,char *argv[])
{
    if(argc < 3){
        log("input argv...like: *.out phone password");
        return false;
    }
    strncpy(loginURL + phoneOffset,argv[1],11);
    strncpy(loginURL + passwordOffset,argv[2],6);
    if(argc > 3){
        sleepTime = atoi(argv[3]);
    }
    return true;
}


void chinaUnicomIsGood()
{
    curl_easy_perform(curl);
}

void initCurl(){
    curl = curl_easy_init();
    if(!curl){
        debug("init curl error");
    }else{
        curl_easy_setopt(curl, CURLOPT_URL, targetURL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_callback);
    }
}

int main(int argc,char *argv[]){

    if(!loadConfig(argc,argv)){
        return -1;
    }
    initCurl();

    while(true){
        onlineFlag = false;
        //test ChinaUnicom is good?If disconnect,auto login
        chinaUnicomIsGood();

        if(!onlineFlag)
        {
            autoLogin();
        }else{
            debug("net ok");
        }
        sleep(sleepTime);
    }

    return 0;
}
