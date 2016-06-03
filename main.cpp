#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

//global constants
const int GPUPRICE = 400;
const int ROW_SIZE = 5;

struct Info {
    int workDayHours;
    int totalWorkDays; //CHANGE TO 41
    int spareHours;
    int totalAvailableHours;
    double hourlyWage;
    double expectedWorkPercentage;
    double totalHoursWorked;
    time_t lastWorkedDay;
};

void StartMenu(int);
int CalculateRemainingDays();
void ReadLog(string[][ROW_SIZE]);
void AddLog(Info*, string, tm*);

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

    StartMenu(flag);



    return 0;
}

void StartMenu(int flag){

    Info goalInfo;

    //Populate the goalInfo constants

    fstream input;
    input.open("data.txt", fstream::in);

    char data[250];
    input.getline(data, 250); //Get rid of the header line

    input>>goalInfo.workDayHours;
    input>>goalInfo.totalWorkDays;
    input>>goalInfo.expectedWorkPercentage;

    //Populate the log array

    int numObservations = goalInfo.totalWorkDays - CalculateRemainingDays(); //Gets the total number of elapsed days
    string logArray[numObservations][ROW_SIZE];
    ReadLog(logArray);

    //Check if any previous days were missed

    time_t rawCurrent = time(0);
    tm* currentDay = new tm;
    currentDay = localtime(&rawCurrent);

    time_t rawLast = goalInfo.lastWorkedDay + 86400; //Start on the day after the last day
    tm* lastDay = new tm;
    lastDay = localtime(&rawLast);

    while (currentDay->tm_yday != lastDay->tm_yday){ //Checks yday which is the # of days since January 1st

        if (lastDay->tm_wday != 0 && lastDay->tm_wday != 6){

            char initBuffer[80];
            strftime(initBuffer, 80, "%A %B %d", lastDay);
            string answer, buffer(initBuffer);

            cout<<"It looks like you missed work on "<<buffer<<". Is this true? (YES/NO)."<<endl;
            cin>>answer;

            while (answer != "YES" && answer != "YES"){
                cout<<"Please enter YES/NO (case sensitive)."<<endl;
                cin>>answer;
            }

            if (answer == "YES"){
                AddLog(&goalInfo, "MISS", lastDay);
            }
            else{
                AddLog(&goalInfo, "NORMAL", lastDay);
            }
        }

        rawLast += 86400;
        lastDay = localtime(&rawLast);
    }

    delete currentDay;
    delete lastDay;

    //Get basic information to present
    goalInfo.totalHoursWorked = stoi(logArray[numObservations - 1][ROW_SIZE - 1]);
    goalInfo.lastWorkedDay = stoi(logArray[numObservations - 1][1]);

    // Hourly wage info
    goalInfo.totalAvailableHours = CalculateRemainingDays() * goalInfo.workDayHours;
    double totalNecessaryWorkHours = (goalInfo.totalWorkDays * goalInfo.workDayHours * goalInfo.expectedWorkPercentage) - goalInfo.totalHoursWorked;
    double expectedDailyWorkHours = totalNecessaryWorkHours / CalculateRemainingDays();
    goalInfo.hourlyWage = GPUPRICE / (goalInfo.totalWorkDays * goalInfo.workDayHours * goalInfo.expectedWorkPercentage);
    double totalEarned = goalInfo.totalHoursWorked * goalInfo.hourlyWage;
    goalInfo.spareHours = totalAvailableHours - totalNecessaryWorkHours);


    // Get the net number of hours you're behind / above
    int netHours = 0;

    for (int i = 0; i < numObservations; i++){

        if (logArray[i][3] == 1 && logArray[i][4] == 0){
            //Do nothing, we don't want to subtract from the net if we had a valid excuse to miss
        }
        else{
            netHours += (stoi(logArray[i][2]) - expectedDailyWorkHours)
        }
    }

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

        cout<<"The time is "<<buffer<<endl<<endl;

        cout<<"Welcome!"<<endl;
        cout<<"So far you have worked "<<goalInfo.totalHoursWorked<<" hours at a rate of $"<<goalInfo.hourlyWage<<"/hour."<<endl;
        cout<<"You've earned $"<<setprecision(2)<<totalEarned<<" so far towards your goal of $"<<GPUPRICE<<" for the 1070."<<endl<<endl;

        cout<<"You are expected to work "<<setprecision(1)<<expectedDailyWorkHours<<" hours today."<<endl;
        cout<<"Every hour you miss today is an extra "<<setprecision(1)<<60 / (CalculateRemainingDays() - 1)<<" minutes you'll need to work on future days."<<endl;
        cout<<"You have "<<goalInfo.spareHours<<" spare hours remaining";



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

//Going to be used for statistics and such
void ReadLog(string log[][ROW_SIZE]){

    fstream input;
    input.open("log.txt", fstream::in);
    char charTemp[256];
    input.getline(charTemp, 256);
    int row = 0, element = 0;

    while(input.getline(charTemp, 256, ',')){
        string temp = charTemp;

        log[row][element] = temp;

        row += (element / (ROW_SIZE - 1)); // Works since integers round down, so it will only add '1' when the element is the last element in the row (Or ROW_SIZE - 1)
        element = (++element) % ROW_SIZE;

    }

    input.close();
    return;
}

//option: Can be either MISS or NORMAL
//Don't worry about missedDay if NORMAL
void AddLog(Info* goalInfo, string option, tm* missedDay){

    fstream output;
    output.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);

    if(option == "MISS"){
        string answer;

        cout<<"Should you have missed? (YES/NO)"<<endl;
        cin>>answer;

        while (answer != "YES" && answer != "NO"){
            cout<<"Enter YES or NO (case-sensitive)"<<endl;
            cin>>answer;
        }

        //Convert the date to string
        char initBuffer[80];
        strftime(initBuffer, 80, "%x", missedDay);
        string buffer(initBuffer);

        if (answer == "YES"){
            //Write "Good Miss" to log

            goalInfo->totalWorkDays--; //Subtract so that we account for the one less day we have to work

            output<<buffer<<","; //Date
            output<<mktime(missedDay)<<","; //time_t
            output<<"0,"; //Hours worked
            output<<"1,"; //Missed
            output<<"0,"; //Bad Miss
            output<<"0,"; //ADHD
            output<<goalInfo.totalHoursWorked<<endl;

        }
        else{
            //Write "Bad Miss" to log

            output<<buffer<<","; //Date
            output<<mktime(missedDay)<<","; //time_t
            output<<"0,"; //Hours worked
            output<<"1,"; //Missed
            output<<"1,"; //Bad Miss
            output<<"0,"; //ADHD
            output<<goalInfo.totalHoursWorked<<endl;
        }

    }

}






























