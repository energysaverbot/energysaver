

#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 200

double battery_capacity = 10000;
double remain_battery_cap = battery_capacity;

vector<int> energyTimestamp;

int Time;
double totalProfit = 0.0; // returns the totalprofit
double phase1profit = 0.0;

vector<vector<int>> allocationmatrix;
vector<int> taskscheduled; // flag whethere task is scheduled or not
vector<double> remain_util;
vector<double> consumed_util;
vector<double> initial_util;
vector<double> batteryconsumed;
vector<int>taskscheuletime;
struct job
{
    int id;
    int arrival;
    int deadline;
    double util;
    double profit;
    job(int i, int a, int d, double u, double p)
    {
        id = i;
        arrival = a;
        deadline = d;
        util = u;
        profit = p;
    }
};

vector<job> Task;

void taskPopulate(vector<vector<string>> &task_dump)

{
    int id;
    int arrival;
    int deadline;
    double util;
    double profit;
    int row = task_dump.size();
    for (int i = 1; i < row; i++)
    {

        id = stoi(task_dump[i][0]);
        arrival = stoi(task_dump[i][1]);
        deadline = stoi(task_dump[i][2]);
        util = stod(task_dump[i][3]);
        profit = stod(task_dump[i][4]);
        // profit = (double)(util*1.0f/deadline)*(util*(1.000f)/deadline);
        job t(id, arrival, deadline, util, profit);
        Task.push_back(t);
    }
    taskscheduled.assign(Task.size(), 0);
}

void printTask()
{
    for (int i = 0; i < Task.size(); i++)
    {

        cout << "id " << Task[i].id << " arrival " << Task[i].arrival << " deadline " << Task[i].deadline << " util " << Task[i].util << " Profit " << Task[i].profit << "\n";
    }
}

double cuberoot(double x)
{
    return cbrt(x);
}

/*
Input : Power Pi
Output : Utilisation U
*/
double calculateU(double P)
{
    if(P==0.0)return 0;
    
    double num = cuberoot((((P * 1.0) / (P_max * 1.0)) - 0.000001) / (0.7 * 1.0)) * (U_max * 1.0);
    return num;
}

// Function to parse a CSV line and return a vector of strings
vector<string> parseCSVLine(const string &line)
{
    vector<string> tokens;
    // cout<<"called";
    char *token = strtok(const_cast<char *>(line.c_str()), ",");

    while (token != nullptr)
    {
        tokens.push_back(token);
        token = strtok(nullptr, ",");
    }

    return tokens;
}
void csv2vector(FILE *f_task, vector<vector<string>> &task_dump)
{
    char line[1024]; // Adjust the size based on your needs

    // Read and parse each line in the CSV file
    while (fscanf(f_task, "%1023[^\n]%*c", line) == 1)
    {
        // Call the function to parse each line and store it in the 2D vector
        std::vector<std::string> row = parseCSVLine(line);
        task_dump.push_back(row);
    }
}
long double profitbyid(int taskid){
    long double pf;
    for(auto &a : Task){
        if(a.id==taskid){
            pf = a.profit;
            break;
        }
    }
    return pf;
}
void populateUtilatTime()
{

    cout << "\n time is " << Time << endl;
    for (int i = 0; i < Time; i++)
    {
        double p = energyTimestamp[i];
        // cout<<p<<" ";
        double getu = calculateU(p);
        initial_util.push_back(getu);
    }
    remain_util = initial_util;
    consumed_util.assign(Time, 0.0); // nothing is consumed intially
}

void show_consumed_util()
{

    cout << "\n &&&&&&&&&&&&&&&&&&&&&& Displaying consumed status at every time stamp &&&&&&&&&&&&&&&&&&&&&&&\n";
    for (auto &a : consumed_util)
    {
        cout << a << " ";
    }
    cout << "\n &&&&&&&&&&&&&&&&&&&&& End of consumed util &&&&&&&&&&&&&&&&&&&\n";
}

// this function will update the consumed util at every time stamp
void update_consumed_util()
{

    for (int i = 0; i < Time; i++)
    {
        consumed_util[i] = initial_util[i] - remain_util[i];
    }
}
void showutil()
{
    cout << " ********** Displaying  Initial Util Data *********** \n";
    for (auto &a : remain_util)
    {
        cout << a << " ";
    }
    cout << "\n *************** Util data end ****************** \n";
}
void storeenergytimestamp(vector<vector<string>> &energy_dump)
{

    int count = 0;
    for (int i = 1; i < energy_dump.size(); i++)
    {

        energyTimestamp.push_back(stoi(energy_dump[i][1]));
    }

    Time = energyTimestamp.size();
    allocationmatrix.assign(Time, vector<int>());
    batteryconsumed.assign(Time,0);
    populateUtilatTime();
    showutil();
}

void filereader()
{
    FILE *f_pow = fopen("../../../Dataset/Multiple Day/Power/power_predicted.csv", "r");
    FILE *f_task = fopen("../../../Dataset/Multiple Day/Task/rho 0.1/task_predicted.csv", "r");
    // Check if the file was successfully opened
    if (f_task == nullptr)
    {
        cerr << "Error opening file: "
             << "task.csv" << endl;
        //  return 1;
    }
    if (f_pow == nullptr)
    {
        cerr << "Error opening file: "
             << "pow.csv" << endl;
        // return 1;
    }

    vector<vector<string>> task_dump;
    vector<vector<string>> energy_dump;

    // fseek(f_task, 0, SEEK_SET);
    csv2vector(f_task, task_dump);
    csv2vector(f_pow, energy_dump);
    cout << "\n energy dump size is " << energy_dump.size() << endl;
    storeenergytimestamp(energy_dump);

    // read from vector to struct task
    taskPopulate(task_dump);
    // printTask();
}

bool comp(struct job &a, struct job &b)
{
    return a.deadline > b.deadline;
}

bool ismoreprocessallowed(int util, int timestamp)
{
    int r_util = (int)remain_util[timestamp - 1];
    return (int)util <= r_util;
}

void printAllocation()
{
    cout << "\n &&&&&&&&&&&&&&&& Printing allocation matrix &&&&&&&&&&&&&&&&&\n";
    for (int i = 0; i < Time; i++)
    {

        for (int j = 0; j < allocationmatrix[i].size(); j++)
        {
            cout << allocationmatrix[i][j] << " ";
        }
        cout << endl;
        // cout<<allocationmatrix[i].size()<<" ";
    }
    cout << " &&&&&&&&&&&&&&& allocation matrix ends &&&&&&&&&&&&&&&&&&\n";
}
// function to check whether the task is available at current timestamp or not
bool istaskavailable(int arrival, int deadline, int curr_time)
{
    int cur_time = curr_time + 1;
    return ((arrival <= cur_time) && (cur_time <= deadline));
}
void phase1()
{
    int count = 0;

    for (int i = Time; i > 0; i--)
    {

        int lookup = i - 1;
        priority_queue<pair<double, pair<int, int>>> maxheap;

        for (int j = 0; j < Task.size(); j++)
        {
            if (Task[j].deadline == i)
            {
                maxheap.push({Task[j].profit, {Task[j].id, Task[j].util}});
            }
        }

        while (!maxheap.empty())
        {
            int id = maxheap.top().second.first;
            int util = maxheap.top().second.second;
            double profit = maxheap.top().first;
            maxheap.pop();
            if (!taskscheduled[id])
            {
                if (ismoreprocessallowed(util, i) && (taskscheduled[id] == 0))
                {
                    taskscheduled[id] = 1;
                    // cout<<" ** Task "<<id<<" scheduled\n";
                    allocationmatrix[lookup].push_back(id);
                    remain_util[lookup] -= util;
                    totalProfit += profit;
                    count++;
                }
            }
        }
        // now scheduled all the task which are available in this time slot
        for (int k = 0; k < Task.size(); k++)
        {
            int arvl = Task[k].arrival;
            int deadlin = Task[k].deadline;
            double prft = Task[k].profit;
            int tid = Task[k].id;
            double utill = Task[k].util;
            if (istaskavailable(arvl, deadlin, i-1))
            {
                maxheap.push({prft, {tid, utill}});
            }
        }
        while (!maxheap.empty())
        {
            double prft = maxheap.top().first;
            int tid = maxheap.top().second.first;
            double utill = maxheap.top().second.second;
            maxheap.pop();
            if (ismoreprocessallowed(utill, i) && (taskscheduled[tid] == 0))
            {
                taskscheduled[tid] = 1;
                // cout<<" ** Task "<<id<<" scheduled\n";
                allocationmatrix[lookup].push_back(tid);
                remain_util[lookup] -= utill;
                totalProfit += prft;
                count++;
            }
        }
    }
    cout << "\n Total task scheduled are " << count << endl;
    // printAllocation();
    cout << "\n $$$$$$$$$$$ Total Profit after Phase 1 " << totalProfit << endl;
    phase1profit = totalProfit;
    update_consumed_util();
    show_consumed_util();
}

/*

Input : U
Output : P

*/
double UtoPOW(int U)
{

    double Pi = P_max * (0.000001 + 1.0 * 0.7 * pow((U * 1.0) / U_max, 3));
    return Pi;
}

/*
from current time slot find the next minimum consumed time slot
note : it returns the index of time stamp
when actually using the time stamp increment the index by 1
*/
int nextMin_consumed_timeslot(int t, double avg_pow)
{
    if (t == Time - 1)
        return -1; // no next time slot as it is the last slot
    int mn = t;
    int val = consumed_util[t];
    double diff = 0;
    for (int i = t + 1; i < Time; i++)
    {
        double diff_temp = avg_pow - UtoPOW(consumed_util[i]);
        if (diff_temp > diff)
        {
            diff = diff_temp;
            mn = i;
            break;
        }
    }
    if (mn == t)
        return -1; // we didnt find any timeslot below average
    return mn;     // return index of next minimum consumed time slot
}

// Returns utility of task given Task ID
pair<double,double> findutilityProfit_byid(int Tid)
{
    double u = 0.0;
    double proft = 0.0;
    for (auto &a : Task)
    {
        if (a.id == Tid)
        {
            u = a.util;
            proft = a.profit;
            break;
        }
    }
    return {u,proft};
}
/*
this function will check removing how many task from the current time slot will reduce that much amount of power
we remove the task from last
*/


bool isremove_equals_to_powReduce(int &timeslot, double &pow_to_reduce, int no_task_Toremove)
{

    int no_task_allocated = allocationmatrix[timeslot].size();
    int task_no = no_task_allocated - 1; // to iterate on array and 0 based indexing
    double total_utility_toRemove = 0.0;
    int tid;
    double oldutil, diffutil;
    double pow_oldutil, pow_newutil;
    double diffpow;

    oldutil = consumed_util[timeslot];
    while (no_task_Toremove)
    {
        tid = allocationmatrix[timeslot][task_no--];
        pair<double,double>p = findutilityProfit_byid(tid);
        total_utility_toRemove += p.first;

        no_task_Toremove--;
    }
    diffutil = oldutil - total_utility_toRemove;
    pow_oldutil = UtoPOW(oldutil);
    pow_newutil = UtoPOW(diffutil);
    diffpow = pow_oldutil - pow_newutil;

    
    return diffpow >pow_to_reduce;
}

double remove_num_of_task(int &timeslot , int &count){
    int col = allocationmatrix[timeslot].size();
    int toremove = count;
    double totalutil = 0.0;
    double profit_remove = 0;
    for(int i=col-1;i>=0;i--){
        if(toremove==0)break;
        int tid = allocationmatrix[timeslot][i];
        taskscheduled[tid]=0;
        allocationmatrix[timeslot].pop_back();
        pair<double,double> p = findutilityProfit_byid(tid);
        totalutil +=p.first;
        profit_remove+=p.second;
        toremove--;
    }
    double oldutil, newutil;
    double pow_oldutil, pow_newutil;
    double diffpow;
    oldutil = consumed_util[timeslot];
    newutil = oldutil - totalutil;
    pow_oldutil = UtoPOW(oldutil);
    pow_newutil = UtoPOW(newutil);
    diffpow = pow_oldutil - pow_newutil;
    energyTimestamp[timeslot] = pow_newutil;
    consumed_util[timeslot] = newutil;
    initial_util[timeslot]-=totalutil;
    remain_util[timeslot] = initial_util[timeslot] - consumed_util[timeslot];
    totalProfit-=profit_remove;
    return diffpow;



}
double removejobs(int &timeslot, double pow_to_reduce)
{
    int num_task_toRemove = 0;//how many task to remove of current time slot
    int col = allocationmatrix[timeslot].size();
    int count = 1;
    while(count < col){
        if(isremove_equals_to_powReduce(timeslot,pow_to_reduce,count)){
            num_task_toRemove = count;
            break;
        }
        count++;
    }
    count--;
    if(count==0)count++;
    cout<<" count "<<count<<endl;
    return remove_num_of_task(timeslot,count);

}

pair<int, pair<int, double>> findmostProfitable_unfinishedjob(int timeslot)
{
    int jobid = -1;
    double proft = 0;
    double available_util = remain_util[timeslot];
    int required_util = 0;
    for (int i = 0; i < Task.size(); i++)
    {
        // check if that task is available at that time slot or not
        if (istaskavailable(Task[i].arrival, Task[i].deadline, timeslot))
        {
            // check if the task is not yet scheduled and required util for that task is available
            if ((taskscheduled[Task[i].id] == 0) && (Task[i].util <= available_util))
            {
                // means task is not scheduled
                if (Task[i].profit > proft)
                {

                    proft = Task[i].profit;
                    jobid = Task[i].id;
                    required_util = Task[i].util;
                }
            }
        }
    }

    return {jobid, {required_util, proft}};
}
void addjobs(int timeslot, double power_added)
{
    // update the power at that time stamp
    double current_pow = UtoPOW(initial_util[timeslot]);
    double newpow = current_pow + power_added;
    double new_total_util = calculateU(newpow);
    double diff_util = new_total_util - initial_util[timeslot];
    energyTimestamp[timeslot] = newpow;
    initial_util[timeslot] = new_total_util;
    remain_util[timeslot] = initial_util[timeslot] - consumed_util[timeslot];

    double tempui = 0.0;

    int itr = 0;
    while (itr < 100)
    {
        pair<int, pair<int, double>> data = findmostProfitable_unfinishedjob(timeslot);
        int newjobid = data.first;
        int required_util = data.second.first;
        double newprofit = data.second.second;

        if (newjobid == -1)
            return; // no job satisfy our condition
        // we have a job
        cout << "job added is " << newjobid << endl;
        tempui += required_util;
        taskscheduled[newjobid] = 1;
        allocationmatrix[timeslot].push_back(newjobid);
        consumed_util[timeslot] += required_util;
        remain_util[timeslot] = initial_util[timeslot] - consumed_util[timeslot];
        totalProfit += newprofit;
        itr++;
    }
    cout << "\n total util required was " << tempui << endl;
}

double avg_pow_consumption()
{
    double avg = 0.0;
    vector<double> pow_consumed;
    for (auto &a : consumed_util)
    {
        double poww = UtoPOW(a);
        pow_consumed.push_back(poww);
    }

    cout<<"\n ***************** power consumed *************************"<<endl;
    for(auto &a : pow_consumed){
        cout<<a<<" ";
    }
    cout<<"\n ********** power consumed display end **************** "<<endl;

    double sum = 0.0;
    for (auto &a : pow_consumed)
    {
        sum += a;
    }
    avg = (double)((1.0 * sum) / Time);
    cout << " \n Avg Power consumed is " << avg << endl;
    return avg;
}
int pow_above_avg(int timeslot, double avg)
{
    double curr_pow = UtoPOW(consumed_util[timeslot]);
    if (curr_pow < avg)
        return 0;
    return (int)(curr_pow - avg);
}
void heapops(priority_queue<pair<double, int>> &maxheap,int timeslot,double avg_poww){
    while(!maxheap.empty())maxheap.pop();
    for (int i = 0; i < Time; i++)
    {
        maxheap.push({pow_above_avg(i, avg_poww), i}); // pushing the difference from the average
    }
}
double findavailablebatterypower(int &oldtime,int &newtime){
    int consumedmax = -1;
    for(int i = oldtime + 1 ;  i <= newtime ; i++ ){
        consumedmax = max(consumedmax,(int)batteryconsumed[i]);
    }
    return battery_capacity-consumedmax;
}

void updateconsumedbattery(int oldtime,int newtime,double amount){

    for(int i = oldtime +1 ; i <= newtime ; i++){
        batteryconsumed[i] += amount; //change here = to +=
    }
}

void phase2()
{
    // populate the difference of the average with consumption
    double avg_poww = avg_pow_consumption();
    cout<<" average poww "<<avg_poww<<endl;
    priority_queue<pair<double, int>> maxheap;
    for (int i = 0; i < Time; i++)
    {
        maxheap.push({pow_above_avg(i, avg_poww), i}); // pushing the difference from the average
    }

    double oldprofit = totalProfit;
    double profitphase1 = totalProfit;
    int itr = 0;
    int timeslot = -1;
    cout<<" ********** pHASE 2 STARTS *********** \n";
    int tt=0;
    while (itr < 5000)
    {
        timeslot = maxheap.top().second;
       // if(tt==Time)break;
        // timeslot = tt;
        // tt++;
        maxheap.pop();
        if (timeslot == (Time - 1))
            continue;
        int newtimestamp = nextMin_consumed_timeslot(timeslot, avg_poww);
        if (newtimestamp == -1)
            break;   
                                           // no consumption is below average
       // cout<<"\n time stamp is "    <<timeslot<<" "<<UtoPOW(consumed_util[timeslot])<<endl;                               
        double newtime_pow_cons = UtoPOW(consumed_util[newtimestamp]); // new timestamp power consumption
        double diff_with_avg_newtime = avg_poww - newtime_pow_cons;
        double diff_with_avg_oldtime = UtoPOW(consumed_util[timeslot]) - avg_poww;
      //  cout<<"avg_newtime "<<diff_with_avg_newtime<<" avg_oldtime "<<diff_with_avg_oldtime<<endl;
        double availablebattery = findavailablebatterypower(timeslot,newtimestamp);
        double min_transferrable_pow = min({availablebattery, diff_with_avg_newtime, diff_with_avg_oldtime});
       // cout<<"\n transferrable unit is "<<min_transferrable_pow<<endl;
       cout<<" old time "<<timeslot<<" power at old "<<UtoPOW(consumed_util[timeslot])<<" new timeslot "<<newtimestamp<<" pow new "<<newtime_pow_cons<<" transfer "<<min_transferrable_pow<<endl;
        if (min_transferrable_pow > 1)
        {
            double pow_add = removejobs(timeslot, min_transferrable_pow);
            addjobs(newtimestamp,pow_add);
            updateconsumedbattery(timeslot,newtimestamp,pow_add);
            //maxheap.push({pow_above_avg(timeslot, avg_poww), timeslot});
            heapops(maxheap,timeslot,avg_poww);
            cout<<"Profit after itr "<<itr<<" : "<<totalProfit<<" pow added " <<pow_add<<endl;

        }
        // think again whether to push back the current timeslot or not
        
        itr++;
    }

    cout << "\n  profit phas 1 " << phase1profit << endl;
    cout << "\n max profit is " << totalProfit << endl;
}

void show_init_util()
{
    cout << "\n ******************* Initial Util ****************************\n";
    for (auto &a : initial_util)
    {
        cout << a << " ";
    }
    cout << "\n ***************** End of Initial Util ***************************** \n";
}

void helper()
{
    // lets sort the task based on deadline : based on last time stamp first
    sort(Task.begin(), Task.end(), comp);
    // printTask();
    phase1();
     avg_pow_consumption();
     phase2();

//     show_consumed_util();
//     showutil();
//     show_init_util();
}
void schedulemaker(){
    taskscheuletime.assign(Task.size(),-1);
    for(int i=0;i<allocationmatrix.size();i++){
        for(int j=0;j<allocationmatrix[i].size();j++){
            int val = allocationmatrix[i][j];
            taskscheuletime[val] = i+1;
        }
    }
    long double profft = 0;
    int count = 0;
    for(int i=0;i<Task.size();i++){
        if(taskscheuletime[i]!=-1){profft+=profitbyid(i);count++;}
    }
    cout<<" verfied profit is "<<profft<<endl;
    cout<<"total task scheduled is "<<count<<endl;

}
void writeVectorToFile(const vector<int>& data, const string& filename) {
    // Open the file for writing
    ofstream outputFile(filename);
    
    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return; // Exit the function
    }
    
    // Write each element of the vector to the file
    for (const int& item : data) {
        outputFile << item << endl;
    }
    
    // Close the file
    outputFile.close();
    
    cout << "Data has been written to " << filename << "." << endl;
}
pair<double,pair<int,int>> findutilbyid(int idd){
    for(int i=0;i<Task.size();i++){
        if(Task[i].id ==idd)return {Task[i].util,{Task[i].arrival,Task[i].deadline}};
    }
    return {0,{0,0}};
}
bool isvalidschedule(){
    bool ans =true;
    vector<double>temp_init_util = initial_util;
    vector<double>temp_consumed_util(Time,0);
    for(int i=0;i<Task.size();i++){
        int tasktime = taskscheuletime[i];
        if(tasktime==-1)continue;
        pair<double,pair<int,int>>p = findutilbyid(i);
        int taskutil = p.first;
        int arv = p.second.first;
        int ded = p.second.second;
        if(!(tasktime>=arv && tasktime<=ded) && tasktime!=-1){
            ans = false;
            cout<<" task id "<<i<<" actual arrival "<<arv<<" actual deadline "<<ded<<" scheduled on "<<tasktime<<"\n";
            break;
        }
        tasktime--;
        if((taskutil+temp_consumed_util[tasktime])<=temp_init_util[tasktime])temp_consumed_util[tasktime]+=taskutil;
        else{
            ans = false;
            cout<<" task id "<<i<<" overconsumed "<<taskutil<<" consumed "<<temp_consumed_util[tasktime]<<" initial "<<temp_init_util[tasktime]<<"\n";
            break;
        }
       

    }



    size_t maxSize = std::max(temp_init_util.size(), temp_consumed_util.size());

    std::cout << "temp_init_util\ttemp_consumed_util" << std::endl;
    for (size_t i = 0; i < maxSize; ++i) {
        if (i < temp_init_util.size()) {
            std::cout << temp_init_util[i] << "\t\t\t";
        } else {
            std::cout << " \t\t\t";
        }
        if (i < temp_consumed_util.size()) {
            std::cout << temp_consumed_util[i];
        }
        std::cout << std::endl;
    }
    return ans;


}
int main()
{
    totalProfit = 0.0;
    filereader();
    helper();
    schedulemaker();
     writeVectorToFile(energyTimestamp, "energy_timestamps.txt");
     writeVectorToFile(taskscheuletime, "taskschedule.txt");
     cout<<"schedule validity "<<isvalidschedule();
    cout<<"\nRevenue after Algo 2: Schedule Tasks on ES without battery->"<<phase1profit<<endl;
    cout<<"\nRevenue after Algo 6: Offline Schedule for Finite Battery: TS-GES-SFB->"<<totalProfit<<endl;

   

    return 0;
}
