#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 100
// vector<int> energyTimestamp_pred;
vector<vector<int>> energyTimestamp;

vector<vector<int>> energy; // we will not manipulate this vector -: first row predicted energy,second row:- actual energy
vector<vector<double>> storeutils;
int Time;
vector<vector<int>> allocationmatrix; // used only on prediction
long double totalProfit = 0.0;        // returns the totalprofit
long double phase1profit = 0.0;
long double oldprofit = 0.0;
int taskphase1, taskphase2;
int Totaltask;

vector<int> taskscheduled_pred; // flag whethere task is scheduled or not

vector<vector<double>> remain_util; // first index for predicted second for actual
vector<vector<double>> consumed_util;
vector<vector<double>> initial_util;

vector<int> predictedtask_scheduletime;
unordered_map<int, int> newtask_scheduletime;



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

vector<job> Task_pred;
vector<job> Task_actual;
vector<job> Task_new;
unordered_set<int> findactualtask;

void taskPopulate(vector<vector<string>> &task_dump, vector<vector<string>> &task_dump1, vector<vector<string>> &task_dump2)

{
    int id;
    int arrival;
    int deadline;
    double util;
    double profit;
    int row = task_dump.size();
    Totaltask = row - 1;
    for (int i = 1; i < row; i++)
    {

        id = stoi(task_dump[i][0]);
        arrival = stoi(task_dump[i][1]);
        deadline = stoi(task_dump[i][2]);
        util = stod(task_dump[i][3]);
        profit = stod(task_dump[i][4]);
        // profit = (double)(util*1.0f/deadline)*(util*(1.000f)/deadline);
        job t(id, arrival, deadline, util, profit);
        Task_pred.push_back(t);
    }
    taskscheduled_pred.assign(Task_pred.size(), 0);
    predictedtask_scheduletime.assign(Task_pred.size(), -1);

    row = task_dump1.size();
    for (int i = 1; i < row; i++)
    {
        id = stoi(task_dump1[i][0]);
        arrival = stoi(task_dump1[i][1]);
        deadline = stoi(task_dump1[i][2]);
        util = stod(task_dump1[i][3]);
        profit = stod(task_dump1[i][4]);
        job t(id, arrival, deadline, util, profit);
        Task_actual.push_back(t);
        findactualtask.insert(id);
    }
    row = task_dump2.size();
    for (int i = 1; i < row; i++)
    {
        id = stoi(task_dump2[i][0]);
        arrival = stoi(task_dump2[i][1]);
        deadline = stoi(task_dump2[i][2]);
        util = stod(task_dump2[i][3]);
        profit = stod(task_dump2[i][4]);
        job t(id, arrival, deadline, util, profit);
        Task_new.push_back(t);
        newtask_scheduletime[id] = -1; // the task which are new
    }
}

double calculateU(double P)
{

    if (P == 0.0)
        return 0;
    double num = cbrt((((P * 1.0) / (P_max * 1.0)) - 0.000001) / (0.7 * 1.0)) * (U_max * 1.0);
    return num;
}

double UtoPOW(int U)
{

    double Pi = P_max * (0.000001 + 1.0 * 0.7 * pow((U * 1.0) / U_max, 3));
    return Pi;
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

void populateUtilatTime()
{

    initial_util.assign(2, vector<double>(Time, 0));
    consumed_util.assign(2, vector<double>(Time, 0));
    remain_util.assign(2, vector<double>(Time, 0));
    storeutils.assign(2, vector<double>(Time, 0));

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < Time; j++)
        {
            double p = energyTimestamp[i][j];
            double getu = calculateU(p);
            initial_util[i][j] = getu;
            remain_util[i][j] = initial_util[i][j];
        }
    }
    cout<<"up\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < energy[i].size(); j++)
        {
            storeutils[i][j] = calculateU(energy[i][j]);
        }
    }
    cout<<"below\n";
}

void storeenergytimestamp(vector<vector<string>> &energy_dump, vector<vector<string>> &energy_dump2)
{

    int count = 0;
    vector<int> pred, actual;
    for (int i = 1; i < energy_dump.size(); i++)
    {

        pred.push_back(stoi(energy_dump[i][1]));
        // cout<<stoi(energy_dump[i][1])<<endl;
    }
    
    for (int i = 1; i < energy_dump2.size(); i++)
    {

        actual.push_back(stoi(energy_dump2[i][1]));
        // cout<<stoi(energy_dump[i][1])<<endl;
    }
    energyTimestamp.push_back(pred);
    energyTimestamp.push_back(actual);
    energy = energyTimestamp;

    Time = energyTimestamp[0].size();
    allocationmatrix.assign(Time, vector<int>());
    cout<<" check ";
    populateUtilatTime();
    
}

void filereader()
{
    FILE *f_pow_actual = fopen("../../../Dataset/Single Day/Power/power_actual_5_percent_deviation.csv", "r"); 
    FILE *f_task_actual = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_actual_5_percent_deviation.csv", "r");//task data from predicted data which actually comes in online scenario
    // Check if the file was successfully opened

    FILE *f_pow_pred = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r"); //predicted solar power
    FILE *f_task_pred = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_predicted.csv", "r");//predicted task data

    FILE *f_task_new = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_new_5_percent_deviation.csv", "r");//task data which are new in online case
    if (f_task_actual == nullptr)
    {
        cerr << "Error opening file: "
             << "task.csv" << endl;
        //  return 1;
    }
    if (f_pow_actual == nullptr)
    {
        cerr << "Error opening file: "
             << "pow.csv" << endl;
        // return 1;
    }
    
    vector<vector<string>> task_dump_actual;
    vector<vector<string>> energy_dump_actual;
    vector<vector<string>> task_dump_predicted;
    vector<vector<string>> energy_dump_predicted;
    vector<vector<string>> task_dump_new;

    // fseek(f_task, 0, SEEK_SET);
    csv2vector(f_task_actual, task_dump_actual);
    csv2vector(f_pow_actual, energy_dump_actual);
    csv2vector(f_task_pred, task_dump_predicted);
    csv2vector(f_pow_pred, energy_dump_predicted);
    csv2vector(f_task_new, task_dump_new);
    

    // cout << "\n energy dump size is " << energy_dump.size() << endl;
    storeenergytimestamp(energy_dump_predicted, energy_dump_actual);
    cout<<" reached csv\n";
    // read from vector to struct task
    taskPopulate(task_dump_predicted, task_dump_actual, task_dump_new);
    // printTask();
}

// function to check whether the task is available at current timestamp or not
bool istaskavailable(int arrival, int deadline, int curr_time)
{
    int cur_time = curr_time + 1;
    return ((arrival <= cur_time) && (cur_time <= deadline));
}

bool ismoreprocessallowed(int util, int timestamp)
{
    int r_util = (int)remain_util[0][timestamp - 1];
    return (int)util <= r_util;
    // return (int)util + consumed_util[0][timestamp-1] <= (int)initial_util[0][timestamp - 1];
}

void print_vector_content(vector<vector<double>> &data, int row_num)
{
    if (row_num >= 0 && row_num < data.size())
    {
        int col = data[row_num].size();
        for (int i = 0; i < col; i++)
        {
            cout << data[row_num][i] << " ";
        }
    }
    else
    {
        cout << "Invalid row number." << endl;
    }
}

void phase1()
{
    int count = 0;
    cout<<" called 2\n";

    for (int i = Time; i > 0; i--)
    {

        int lookup = i - 1;
        priority_queue<pair<double, pair<int, pair<double, double>>>> maxheap;

        for (int j = 0; j < Task_pred.size(); j++)
        {
            if (Task_pred[j].deadline == i)
            {
                maxheap.push({Task_pred[j].profit, {Task_pred[j].id, {Task_pred[j].util, Task_pred[j].profit}}});
            }
        }

        while (!maxheap.empty())
        {
            int id = maxheap.top().second.first;
            int util = maxheap.top().second.second.first;
            double profit = maxheap.top().second.second.second;
            maxheap.pop();
            if (!taskscheduled_pred[id])
            {
                if (ismoreprocessallowed(util, i) && (taskscheduled_pred[id] == 0))
                {
                    taskscheduled_pred[id] = 1;
                    predictedtask_scheduletime[id] = i;

                    // cout<<" ** Task "<<id<<" scheduled\n";
                    allocationmatrix[lookup].push_back(id);
                    remain_util[0][lookup] -= util;
                    totalProfit += profit;
                    count++;
                }
            }
        }
        // now scheduled all the task which are available in this time slot
        for (int k = 0; k < Task_pred.size(); k++)
        {
            int arvl = Task_pred[k].arrival;
            int deadlin = Task_pred[k].deadline;
            double prft = Task_pred[k].profit;
            int tid = Task_pred[k].id;
            double utill = Task_pred[k].util;
            if (istaskavailable(arvl, deadlin, i - 1))
            {
                maxheap.push({prft, {tid, {utill, prft}}});
            }
        }
        while (!maxheap.empty())
        {
            int tid = maxheap.top().second.first;
            double prft = maxheap.top().second.second.second;

            double utill = maxheap.top().second.second.first;
            maxheap.pop();
            if (ismoreprocessallowed(utill, i) && (taskscheduled_pred[tid] == 0))
            {
                taskscheduled_pred[tid] = 1;
                predictedtask_scheduletime[tid] = i;
                // cout<<" ** Task "<<id<<" scheduled\n";
                allocationmatrix[lookup].push_back(tid);
                remain_util[0][lookup] -= utill;
                totalProfit += prft;
                count++;
            }
        }
    }
    cout << "\n Total task scheduled are " << count << endl;
    taskphase1 = count;
    // printAllocation();
    
    phase1profit = totalProfit;
    cout<<" phase1 profit is "<<phase1profit<<endl;

    for (int i = 0; i < Time; i++)
    {
        consumed_util[0][i] = initial_util[0][i] - remain_util[0][i];
    }
   // cout << "\n $$$$$$$$$$$ Total Profit after Phase 1 " << totalProfit << endl;
}

double removejobs(int timeslot)
{
    int col = allocationmatrix[timeslot].size();
    int jobid = allocationmatrix[timeslot][col - 1];
    allocationmatrix[timeslot].pop_back();
    taskscheduled_pred[jobid] = 0;
    predictedtask_scheduletime[jobid] = -1;
    // cout<<" job removed is*  "<<jobid<<endl;
    // find utilsation
    int ui = 0;
    double lastprofit = 0;
    for (int i = 0; i < Task_pred.size(); i++)
    {
        if (Task_pred[i].id == jobid)
        {
            ui = Task_pred[i].util;
            lastprofit = Task_pred[i].profit;
        }
    }
    //  cout<<"remove called"<<endl;
    cout << " job removed is*  " << jobid << " util is  " << ui << endl;
    double newutil = initial_util[0][timeslot] - ui;
    double oldutil = initial_util[0][timeslot];
    double p_onnewutil = UtoPOW(newutil);
    double p_onoldutil = UtoPOW(oldutil);
    double power_reduced = p_onoldutil - p_onnewutil;
    initial_util[0][timeslot] -= ui;
    energyTimestamp[0][timeslot] = p_onnewutil;
    consumed_util[0][timeslot] -= ui;
    remain_util[0][timeslot] = initial_util[0][timeslot] - consumed_util[0][timeslot];
    totalProfit -= lastprofit;
    return power_reduced;
}

int nextMin_consumed_timeslot(int t)
{
    if (t == Time - 1)
        return -1; // no next time slot as it is the last slot
    int mn = t;
    int val = consumed_util[0][t];
    for (int i = t + 1; i < Time; i++)
    {
        if (consumed_util[0][i] < val)
        {
            val = consumed_util[0][i];
            mn = i;
            // break;
        }
    }
    return mn; // return index of next minimum consumed time slot
}

pair<int, pair<int, double>> findmostProfitable_unfinishedjob(int timeslot)
{
    int jobid = -1;
    double proft = 0;
    double available_util = remain_util[0][timeslot];
    int required_util = 0;
    for (int i = 0; i < Task_pred.size(); i++)
    {
        // check if that task is available at that time slot or not
        if (istaskavailable(Task_pred[i].arrival, Task_pred[i].deadline, timeslot))
        {
            // check if the task is not yet scheduled and required util for that task is available
            if ((taskscheduled_pred[Task_pred[i].id] == 0) && (Task_pred[i].util <= available_util))
            {
                // means task is not scheduled
                if (Task_pred[i].profit > proft)
                {

                    proft = Task_pred[i].profit;
                    jobid = Task_pred[i].id;
                    required_util = Task_pred[i].util;
                }
            }
        }
    }

    return {jobid, {required_util, proft}};
}

void addjobs(int timeslot, double power_added)
{
    // update the power at that time stamp
    double current_pow = UtoPOW(initial_util[0][timeslot]);
    double newpow = current_pow + power_added;
    double new_total_util = calculateU(newpow);
    double diff_util = new_total_util - initial_util[0][timeslot];
    energyTimestamp[0][timeslot] = newpow;
    initial_util[0][timeslot] = new_total_util;
    remain_util[0][timeslot] = initial_util[0][timeslot] - consumed_util[0][timeslot];
    double tempui = 0.0;

    int itr = 0;
    while (itr < 900)
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
        taskscheduled_pred[newjobid] = 1;
        predictedtask_scheduletime[newjobid] = timeslot+1;

        allocationmatrix[timeslot].push_back(newjobid);
        consumed_util[0][timeslot] += required_util;
        remain_util[0][timeslot] = initial_util[0][timeslot] - consumed_util[0][timeslot];
        totalProfit += newprofit;
        itr++;
    }
    cout << "\n total util required was " << tempui << endl;
}

void heapops(priority_queue<pair<double, int>> &maxheap)
{
    while (!maxheap.empty())
        maxheap.pop();
    for (int i = 0; i < Time; i++)
    {
        maxheap.push({consumed_util[0][i], i});
    }
}




double profitfromnewbytask(int idd){
    for(int i=0;i< Task_new.size();i++){
        if(Task_new[i].id==idd)return Task_new[i].profit;
    }
    return 0;
}
double profitfromactualbytask(int idd){
    for(int i=0;i< Task_pred.size();i++){
        if(Task_pred[i].id==idd)return Task_pred[i].profit;
    }
    return 0;
}







void phase2()
{
    // get index of two time stamp
    // one having higher utilisation
    // other having lower utilisation
    // try to move the last task from higher utilisation to lower utilisation
    
    priority_queue<pair<double, int>> maxheap;
    for (int i = 0; i < Time; i++)
    {
        maxheap.push({consumed_util[0][i], i});
    }

    oldprofit = totalProfit;
    double profitphase1 = totalProfit;
    int itr = 0;
    int timeslot = -1;
    while (itr < 5000)
    {
        timeslot = maxheap.top().second;
        // double con_util = maxheap.top().first;
        maxheap.pop();
        if (timeslot == (Time - 1))
            continue;
        double pw = removejobs(timeslot);
        int newtimestamp = nextMin_consumed_timeslot(timeslot);
        // cout<< "time slot "<<newtimestamp<<endl;
        if (newtimestamp == -1)
            continue;
        addjobs(newtimestamp, pw);
        // maxheap.push({consumed_util[timeslot],timeslot});
        heapops(maxheap);
        cout << "Profit after itr " << itr << " : " << totalProfit << endl;
        if (totalProfit > oldprofit)
            oldprofit = totalProfit;
        int count = 0;
        for (auto &a : taskscheduled_pred)
        {
            if (a == 1)
                count++;
        }
        cout << "total task scheduled after phase 2 are " << count << endl;
        taskphase2 = count;

        itr++;
    }

    // cout << "remain_util:" << endl;
    // print_vector_content(remain_util, 0);
    // cout << endl;
    // cout << "consumed_util:" << endl;
    // print_vector_content(consumed_util, 0);
    // cout << endl;
    // cout << "initial_util:" << endl;
    // print_vector_content(initial_util, 0);

   
}

vector<int> checkabsenttask(int timeslot)
{
    int timestamp = timeslot + 1;
    vector<int> ans;
    for (int i = 0; i < allocationmatrix[timeslot].size(); i++)
    {
        int tid = allocationmatrix[timeslot][i];
        bool flag;
        if (findactualtask.count(tid))
            flag = true;
        else
            flag = false;
        if (!flag)
        {
            ans.push_back(tid);
            // remove entry from allocation matrix
            auto &firstRow = allocationmatrix[timeslot];
            auto it = find(firstRow.begin(), firstRow.end(), tid);
            // Check if value was found
            if (it != firstRow.end())
            {
                // Erase the value if found
                firstRow.erase(it);
               // cout << "Value " << tid << " deleted " << endl;
            }
            else
            {
               // cout << "Value " << tid << " not found " << endl;
            }
        }
    }
    return ans;
}

pair<double, double> findprofitutilbyid(int tid)
{
    for (int i = 0; i < Task_pred.size(); i++)
    {
        if (Task_pred[i].id == tid)
        {
            return {Task_pred[i].profit, Task_pred[i].util};
        }
    }
    return {0, 0};
}

void droptaskonline(int timeslot,double util,long double &onlineprofit){
    double accumulatedutil=0;

    for(int i=allocationmatrix[timeslot].size()-1;i>=0;i--){
        

        pair<double,double> p = findprofitutilbyid(allocationmatrix[timeslot][i]);
        onlineprofit-=p.first;
        int tid = allocationmatrix[timeslot][i];
        accumulatedutil+=p.second;
        allocationmatrix[timeslot].pop_back();
        taskscheduled_pred[tid] = 0;
        predictedtask_scheduletime[tid] = -1;
        if(accumulatedutil>=util){
            break;

        }
        

    }
    accumulatedutil=0;
    for(int i=0;i< allocationmatrix[timeslot].size();i++){
        pair<double,double> p = findprofitutilbyid(allocationmatrix[timeslot][i]);
        accumulatedutil+=p.second;
    }
    consumed_util[1][timeslot]=accumulatedutil;

}




void taskateachtimeslot(){

    vector<int>freq(Time+2,0);
    long double profittest = 0;
    // for(int i=0;i< Time;i++){
    //     int lookup = i+1;
    //     int col = allocationmatrix[i].size();
    //     freq[lookup]+=col;
    //     for(int j=0;j<col;j++){
    //         profittest+=profitfromactualbytask(allocationmatrix[i][j]);

    //     }
    // }
    for(int i=0;i< Task_pred.size();i++){
        if(taskscheduled_pred[i]!=0){
            freq[predictedtask_scheduletime[i]]++;
            profittest+=profitfromactualbytask(i);
        }

    }
    for(auto &a:newtask_scheduletime){
        int time = a.second;
        if(time!=-1){
            freq[time]++;
           // if(a.first==4089)cout<<" collect "<<profittest<<endl;
            profittest+=profitfromnewbytask(a.first);
           // if(a.first==4089)cout<<" now collect "<<profitfromnewbytask(a.first)<<" *** "<<profittest<<endl;

        }else{
          //  cout<<" **************************************************** FFFFFFFFFFFFFFFFAILLLLLLLLLLED\n";
        }
    }
   // printTable(freq);
   // cout<<" ****** test profit "<<profittest<<endl;

}












void addtaskonline(int timeslot,double utiladded,long double &onlineprofit){


    priority_queue<pair<double,pair<int,pair<double,double>>>>maxheap;
    for(int k=0;k< Task_actual.size();k++){
        int arvl = Task_actual[k].arrival;
        int deadlin = Task_actual[k].deadline;
        double prft = Task_actual[k].profit;
        int tid = Task_actual[k].id;
        double utill = Task_actual[k].util;
        if (istaskavailable(arvl, deadlin, timeslot))
        {
            maxheap.push({prft / utill, {tid,{prft,utill}}});
        }
    }

    for(int k=0;k< Task_new.size();k++){
        int arvl = Task_new[k].arrival;
        int deadlin = Task_new[k].deadline;
        double prft = Task_new[k].profit;
        int tid = Task_new[k].id;
        double utill = Task_new[k].util;
        if (istaskavailable(arvl, deadlin, timeslot))
        {
            maxheap.push({prft / utill, {tid,{prft,utill}}});
        }
    }

    double consumedutill = 0;
    double addedu =0;
    while(!maxheap.empty()){
        int tid = maxheap.top().second.first;
        double prft = maxheap.top().second.second.first;
        double utill = maxheap.top().second.second.second;
        maxheap.pop();
        consumedutill+=utill;
        if(consumedutill<=utiladded){
            addedu = consumedutill;
           
            if(newtask_scheduletime.count(tid) && newtask_scheduletime[tid]==-1){//it is new task
              onlineprofit+=prft;
              newtask_scheduletime[tid] = timeslot+1;
              consumed_util[1][timeslot]+=utill;
             

            }
            else{ //old actual task
            if(!newtask_scheduletime.count(tid) && taskscheduled_pred[tid]==0){
               
                onlineprofit+=prft;
                taskscheduled_pred[tid] = 1;
                predictedtask_scheduletime[tid] = timeslot+1;
                allocationmatrix[timeslot].push_back(tid);
               

            }
            }

        }else break;
    }
    consumed_util[1][timeslot]=addedu;
}








void printTable(const std::vector<int>& data) {
    // Print table header
    std::cout << "-----------------" << std::endl;
    std::cout << "  Time  |  No of Task" << std::endl;
    std::cout << "-----------------" << std::endl;

    // Print table content
    for (size_t i = 1; i <= Time; ++i) {
        std::cout << "    " << i << "    |    " << data[i] << std::endl;
    }

    // Print table footer
    std::cout << "-----------------" << std::endl;
}





void phase2profitverif(){

   cout<<"here\n";


    long double profittest=0;
    for(int i=0;i< Task_pred.size();i++){
        if(predictedtask_scheduletime[i]!=-1){
            
            profittest+=profitfromactualbytask(i);
        }

    }
    cout<<" phase 2 profit verify "<<profittest<<"\n";
}



void onlinephase()
{
    long double onlineprofit = oldprofit;

    for (int i = 0; i < Time; i++)
    {
        double utiladded = 0;
        vector<int> t = checkabsenttask(i);
        for (auto &a : t)
        {
            pair<double, double> p = findprofitutilbyid(a);
            onlineprofit -= p.first;
            utiladded += p.second;
            taskscheduled_pred[a] = 0;
            predictedtask_scheduletime[a] = -1;
        }
        double util_pred = storeutils[0][i];
        double utils_actual = storeutils[1][i];
        double newactual = utils_actual + utiladded;
        if (newactual < util_pred)
        { // we need to drop the task
            double diff =  util_pred - newactual;
            droptaskonline(i,diff,onlineprofit);
          //  if(i==77)cout<<"we dropped\n";
        }else if(newactual > util_pred){
            //add new task
            double diff = newactual - util_pred;
            addtaskonline(i,diff,onlineprofit);
           // if(i==77)cout<<"we added\n";

        }
      //  cout<<" time "<<i<<endl;
       // taskateachtimeslot();
       // cout<<" online "<<onlineprofit<<endl;

    }
    for(int i=0;i<Time;i++){
        remain_util[1][i] = initial_util[1][i]-consumed_util[1][i];
    }
    

    cout << "\nRevenue after Algo 2: Schedule Tasks on ES without battery->" << phase1profit << endl;
    cout << "\nRevenue after Algo 4: Offline Schedule for Infinite Battery: TS-GES-SIB->" << oldprofit << endl;
    cout<<" \nRevenue after Algo 5: Online Scheduling Approach->"<<onlineprofit<<"\n";
}



int main()
{
    
   
    filereader();
    //cout<<issame();
    phase1();
     phase2();
//    //  phase2profitverif();
    onlinephase();
//   //  phase2profitverif();
   taskateachtimeslot();

    return 0;
}
