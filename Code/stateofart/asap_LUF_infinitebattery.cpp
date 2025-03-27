

#include<bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 200

vector<int> energyTimestamp;

int Time;
double totalProfit = 0.0; // returns the totalprofit
double phase1profit = 0.0;
double oldprofit = 0.0;
int taskphase1,taskphase2;
int Totaltask;
vector<vector<int>> allocationmatrix;
vector<int> taskscheduled; // flag whethere task is scheduled or not
vector<double> remain_util;
vector<double> consumed_util;
vector<double>initial_util;
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
    Totaltask = row-1;
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
    double num = cuberoot((((P*1.0) / (P_max*1.0)) - 0.000001) / (0.7*1.0)) * (U_max*1.0);
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
    while (fscanf(f_task, "%1023[^\n]%*c", line) == 1) {
        // Call the function to parse each line and store it in the 2D vector
        std::vector<std::string> row = parseCSVLine(line);
        task_dump.push_back(row);
    }

   
}

void populateUtilatTime()
{

    cout<<"\n time is "<<Time<<endl;
    for (int i = 0; i < Time; i++)
    {
        double p = energyTimestamp[i];
        //cout<<p<<" ";
        double getu = calculateU(p);
        initial_util.push_back(getu);
    }
    remain_util  = initial_util;
    consumed_util.assign(Time,0.0);//nothing is consumed intially
}



void show_consumed_util(){

    cout<<"\n &&&&&&&&&&&&&&&&&&&&&& Displaying consumed status at every time stamp &&&&&&&&&&&&&&&&&&&&&&&\n";
    for(auto &a : consumed_util){
        cout<<a<<" ";
    }
    cout<<"\n &&&&&&&&&&&&&&&&&&&&& End of consumed util &&&&&&&&&&&&&&&&&&&\n";

}

//this function will update the consumed util at every time stamp
void update_consumed_util(){

    for(int i=0;i<Time;i++){
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
    populateUtilatTime();
    showutil();
}

void filereader()
{
    FILE *f_pow = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r");
    FILE *f_task = fopen("../../../Dataset/Single Day/Task/rho 0.1/task_predicted.csv", "r");
    // Check if the file was successfully opened
    if (f_task == nullptr)
    {
        cerr << "Error opening file: "
             << "task.csv" << std::endl;
        //  return 1;
    }
    if (f_pow == nullptr)
    {
        cerr << "Error opening file: "
             << "pow.csv" << std::endl;
        // return 1;
    }

    vector<vector<string>> task_dump;
    vector<vector<string>> energy_dump;
    
    // fseek(f_task, 0, SEEK_SET);
    csv2vector(f_task, task_dump);
    csv2vector(f_pow, energy_dump);
    cout<<"\n energy dump size is "<<energy_dump.size()<<endl;
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
    return (int)util + consumed_util[timestamp-1] <= (int)initial_util[timestamp - 1];
}

void printAllocation(){
    cout<<"\n &&&&&&&&&&&&&&&& Printing allocation matrix &&&&&&&&&&&&&&&&&\n";
    for(int i = 0;i<Time ; i++){
        
        for(int j = 0;j< allocationmatrix[i].size();j++){
           cout<<allocationmatrix[i][j]<<" ";

        }
        cout<<endl;
       // cout<<allocationmatrix[i].size()<<" ";
    }
    cout<<" &&&&&&&&&&&&&&& allocation matrix ends &&&&&&&&&&&&&&&&&&\n";
}
//function to check whether the task is available at current timestamp or not
bool istaskavailable(int arrival,int deadline,int curr_time){
    int cur_time = curr_time+1;
    return ((arrival<=cur_time) && (cur_time<=deadline));
}
void phase1()
{   int count = 0;

    for (int i = 1; i <= Time ; i++) //ASAP
    {

        int lookup = i - 1;
        priority_queue < pair<double, pair<int, int>>> maxheap;

        for (int j = 0; j < Task.size(); j++)
        {
            if (Task[j].arrival == i)
            {
                maxheap.push({Task[j].util*(-1), {Task[j].id, Task[j].util}});
            }
        }
        
        while (!maxheap.empty())
        {
            int id = maxheap.top().second.first;
            int util = maxheap.top().second.second;
            double profit = profitbyid(id);
            maxheap.pop();
            if(!taskscheduled[id]){
                if(ismoreprocessallowed(util,i) && (taskscheduled[id]==0)){
                    taskscheduled[id]=1;
                    //cout<<" ** Task "<<id<<" scheduled\n";
                    allocationmatrix[lookup].push_back(id);
                    remain_util[lookup]-=util;
                    totalProfit+=profit;
                    count++;
                }
            }
        }
        //now scheduled all the task which are available in this time slot
        for(int k=0;k<Task.size();k++){
            int arvl = Task[k].arrival;
            int deadlin = Task[k].deadline;
            double prft  = Task[k].profit;
            int tid = Task[k].id;
            double utill = Task[k].util;
            if(istaskavailable(arvl,deadlin,i-1)){
                maxheap.push({utill*(-1),{tid,utill}});

            }
        }
        while(!maxheap.empty()){
            
            int tid = maxheap.top().second.first;
            double prft  = profitbyid(tid);
            double utill = maxheap.top().second.second;
            maxheap.pop();
            if(ismoreprocessallowed(utill,i) && (taskscheduled[tid]==0)){
                    taskscheduled[tid]=1;
                    //cout<<" ** Task "<<id<<" scheduled\n";
                    allocationmatrix[lookup].push_back(tid);
                    remain_util[lookup]-=utill;
                    totalProfit+=prft;
                    count++;

            }

        }
    }
    cout<<"\n Total task scheduled are "<<count<<endl;
    taskphase1 = count;
    //printAllocation();
    cout<<"\n $$$$$$$$$$$ Total Profit after Phase 1 "<<totalProfit<<endl;
    phase1profit = totalProfit;
    update_consumed_util();
    show_consumed_util();
}




/*

Input : U
Output : P

*/
double UtoPOW(int U){

    double Pi = P_max * (0.000001 + 1.0* 0.7 * pow((U*1.0)/U_max,3));
    return Pi;

}

/*
from current time slot find the next minimum consumed time slot
note : it returns the index of time stamp
when actually using the time stamp increment the index by 1
*/
int nextMin_consumed_timeslot(int t){
    if(t == Time-1)return -1;//no next time slot as it is the last slot
    int mn = t;
    int val = consumed_util[t];
    for(int i=t+1;i<Time;i++){
        if(consumed_util[i]<val){
            val = consumed_util[i];
            mn = i;
            // break;
           
        }
    }
    return mn;//return index of next minimum consumed time slot
}

double removejobs(int timeslot){
    int col = allocationmatrix[timeslot].size();
    int jobid = allocationmatrix[timeslot][col-1];
    allocationmatrix[timeslot].pop_back();
    taskscheduled[jobid] = 0;
   // cout<<" job removed is*  "<<jobid<<endl;
    //find utilsation
    int ui=0;
    double lastprofit = 0;
    for(int i=0;i<Task.size();i++){
        if(Task[i].id == jobid){
            ui = Task[i].util;
            lastprofit = Task[i].profit;
        }
    }
  //  cout<<"remove called"<<endl;
    cout<<" job removed is*  "<<jobid<<" util is  "<<ui<<endl;
    double newutil = initial_util[timeslot] - ui;
    double oldutil = initial_util[timeslot];
    double p_onnewutil = UtoPOW(newutil);
    double p_onoldutil = UtoPOW(oldutil);
    double power_reduced = p_onoldutil - p_onnewutil;
    initial_util[timeslot]-=ui;
    energyTimestamp[timeslot] = p_onnewutil;
    consumed_util[timeslot]-=ui;
    remain_util[timeslot] = initial_util[timeslot] - consumed_util[timeslot];
    totalProfit-=lastprofit;
    return power_reduced;

}


pair<int,pair<int,double>> findmostProfitable_unfinishedjob(int timeslot){
    int jobid  = -1;
    double utl = 100000;
    double proft =0;
    double available_util = remain_util[timeslot];
    int required_util = 0;
    for(int i = 0;i<Task.size();i++){
        //check if that task is available at that time slot or not
        if(istaskavailable(Task[i].arrival,Task[i].deadline,timeslot)){
            //check if the task is not yet scheduled and required util for that task is available
            if((taskscheduled[Task[i].id]==0) && (Task[i].util <=available_util)){
                //means task is not scheduled
                if(Task[i].util < utl){

                    utl =Task[i].util;
                    jobid = Task[i].id;
                    required_util = Task[i].util;
                    proft = Task[i].profit;
                }

            }
        }
    }
    
    return {jobid,{required_util,proft}};

}
void addjobs(int timeslot,double power_added){
    //update the power at that time stamp
    double current_pow = UtoPOW(initial_util[timeslot]);
    double newpow = current_pow + power_added ;
    double new_total_util = calculateU(newpow);
    double diff_util = new_total_util - initial_util[timeslot];
    energyTimestamp[timeslot] =  newpow;
    initial_util[timeslot] = new_total_util;
    remain_util[timeslot] = initial_util[timeslot]-consumed_util[timeslot];
    double tempui =0.0;

    int itr = 0;
    while(itr<900){
    pair<int,pair<int,double>>data = findmostProfitable_unfinishedjob(timeslot);
    int newjobid = data.first;
    int required_util = data.second.first;
    double newprofit = data.second.second;

    if(newjobid ==-1)return;//no job satisfy our condition
    //we have a job
    cout<<"job added is "<<newjobid<<endl;
    tempui+=required_util;
    taskscheduled[newjobid]=1;
    allocationmatrix[timeslot].push_back(newjobid);
    consumed_util[timeslot] += required_util;
    remain_util[timeslot] = initial_util[timeslot] - consumed_util[timeslot];
    totalProfit+=newprofit;
    itr++;
    }
    cout<<"\n total util required was "<<tempui<<endl;

}






void heapops(priority_queue<pair<double, int>> &maxheap){
    while(!maxheap.empty())maxheap.pop();
    for(int i=0;i<Time;i++){
        maxheap.push({consumed_util[i],i});
    }


}
void phase2(){
    //get index of two time stamp
    //one having higher utilisation
    //other having lower utilisation
    //try to move the last task from higher utilisation to lower utilisation
    priority_queue<pair<double,int>>maxheap;
    for(int i=0;i<Time;i++){
        maxheap.push({consumed_util[i],i});
    }
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> minheap;
    for(int i=0;i<Time;i++){
        minheap.push({consumed_util[i],i});
    }

    
     oldprofit = totalProfit;
    double profitphase1 = totalProfit;
    int itr = 0;
    int timeslot  = -1;
    while(itr <5000){
        timeslot = maxheap.top().second;
        //double con_util = maxheap.top().first;
        maxheap.pop();
        if(timeslot ==(Time-1))continue;
        double pw = removejobs(timeslot);
        int newtimestamp = nextMin_consumed_timeslot(timeslot);
        //cout<< "time slot "<<newtimestamp<<endl;
        if(newtimestamp==-1)continue;
        addjobs(newtimestamp,pw);
        // maxheap.push({consumed_util[timeslot],timeslot});
        heapops(maxheap);
        cout<<"Profit after itr "<<itr<<" : "<<totalProfit<<endl;
        if(totalProfit > oldprofit)oldprofit = totalProfit;
        int count = 0;
        for(auto &a : taskscheduled){
           if(a==1)count++;
        }
        cout<<"total task scheduled after phase 2 are "<<count<<endl;
        taskphase2 = count;

        itr++;
    }

    cout<<"\n  profit phas 1 "<<phase1profit<<endl;
    cout<<"\n max profit is "<< oldprofit<<endl;


}

void helper()
{
    // lets sort the task based on deadline : based on last time stamp first
     // sort(Task.begin(), Task.end(), comp);
   // printTask();
      phase1();
      phase2();

   
}



void printHorizontalLine(int width) {
    cout << setfill('-') << setw(width) << "-" << endl;
    cout << setfill(' ');
}

void printTableRow(const string& left, const string& right) {
    const int totalWidth = 40;
    const int leftWidth = 25;
    const int rightWidth = totalWidth - leftWidth;

    cout << "| " << setw(leftWidth) << left << " | ";
    cout << setw(rightWidth) << right << " |" << endl;
}






void displaytable(){
    double profit_phase1 = phase1profit;
    double profit_phase2 = totalProfit;
    int total_tasks = Totaltask;
    int tasks_scheduled = taskphase2;

    // Calculate percentage improvement
    double percentage_improvement = ((profit_phase2 - profit_phase1) / profit_phase1) * 100;

    // Print table
    printHorizontalLine(40);
    //printTableRow("Profit after phase 1", to_string(profit_phase1));
    printTableRow("\nFinal Revenue->", to_string(profit_phase2));
    //printTableRow("Total number of tasks", to_string(total_tasks));
    //printTableRow("Number of tasks scheduled", to_string(tasks_scheduled));
    //printTableRow("Percentage improvement", to_string(percentage_improvement) + "%");
    printHorizontalLine(40);

}

void displayUtils() {
    
    // Display data for all time steps
    for (int i = 0; i < remain_util.size(); ++i) {
        cout << "Time Step " << i+1 << ":" << endl;
        cout << "Remain Util: " << remain_util[i] << endl;
        cout << "Consumed Util: " << consumed_util[i] << endl;
        cout << "Initial Util: " << initial_util[i] << endl << endl;
    }
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
// Function to display contents of global vectors separately
void displayVectors() {
    std::cout << "Contents of remain_util vector:" << std::endl;
    for (const auto& value : remain_util) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "Contents of consumed_util vector:" << std::endl;
    for (const auto& value : consumed_util) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "Contents of initial_util vector:" << std::endl;
    for (const auto& value : initial_util) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int main()
{
    totalProfit = 0.0;
    filereader();
    helper();
    // displayVectors();
     displaytable();
   
    //  schedulemaker();
    //  vector<int>tempenergy;
    //  for(auto &a:initial_util){
    //     long double ee = UtoPOW(a);
    //     tempenergy.push_back(ceil(ee));
    //  }
    //  writeVectorToFile(tempenergy, "energy_timestamps.txt");
    //  writeVectorToFile(taskscheuletime, "taskschedule.txt");
    //  cout<<"schedule validity "<<isvalidschedule();



    return 0;
}
