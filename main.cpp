#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;


const int GOALPRICE = 400;

void StartMenu(int);
int CalculateRemainingDays();

struct Info {
    int workDayHours = 7;
    int totalWorkDays = 41;
    int daysLeft;
    double expectedWorkPercentage = 0.8;
    double totalHoursWorked;
};



int main()
{




    // Begin the menu, determine if we are starting or ending the day

    int flag;
    cout<<"Welcome! Enter 0 if you are starting the day or 1 if it's quitting time"<<endl;
    cin>>flag;
    cout<<endl;

    while (flag != 1 && flag != 0){
        cout<<"Please enter 1 or 0: "<<endl;
        cin>>flag;
    }

    CalculateRemainingDays();

    StartMenu(flag);



    return 0;
}

void StartMenu(int flag){

    // Start either the starting or ending menu

    if (flag == 0){

        // Welcome statement with the starting time HH:MM

        time_t rawTime;
        tm* timeInfo = new tm;
        char initBuffer[80];

        time (&rawTime);
        timeInfo = localtime(&rawTime);

        strftime(initBuffer, 80, "%I:%M %p", timeInfo);

        string buffer(initBuffer);
        cout<<"You've started work at "<<buffer<<endl<<endl;




        delete timeInfo;
    }
    else{

    }

}

int CalculateRemainingDays(){

    // Get the current date and August 1st as the ctime struct tm * as time_t

    tm* august1st = new tm;
    august1st->tm_sec = 0;
    august1st->tm_min = 0;
    august1st->tm_hour = 0;
    august1st->tm_year = 116;
    august1st->tm_wday = 1;
    august1st->tm_mday = 1;
    august1st->tm_mon = 7;
    august1st->tm_yday = 152;

    time_t rawAugust1st = mktime(august1st);

    time_t rawCurrent;
    rawCurrent = time(0);
    tm* currentDay = new tm;
    currentDay = localtime(&rawCurrent);

    int daysLeft = 0;

    //Iterate through each day (including the current) to August 1st. Include the day if it is a weekday

    while (rawCurrent < rawAugust1st){ //Is the date before August 1st

        if (currentDay->tm_wday != 0 && currentDay->tm_wday != 6) //If the date is neither a Sunday or Saturday
            daysLeft++;

        rawCurrent += 86400; //86400 seconds in a day, how we're iterating through the days
        currentDay = localtime(&rawCurrent);
    }

    delete august1st;
    delete currentDay;

    return daysLeft;
}
































