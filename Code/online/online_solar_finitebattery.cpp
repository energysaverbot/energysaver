#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 200
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
double battery_capacity = 9000;
double remain_battery_cap = battery_capacity;
vector<double> batteryconsumed;


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
    batteryconsumed.assign(Time,0);
    allocationmatrix.assign(Time, vector<int>());
    
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
    
    cout<<" here phase profit "<<totalProfit<<endl;
    phase1profit = totalProfit;

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
        taskscheduled_pred[newjobid] = 1;
        predictedtask_scheduletime[newjobid]=timeslot;
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


double avg_pow_consumption()
{
    double avg = 0.0;
    vector<double> pow_consumed;
   
    for(int i=0;i<Time;i++){
        double poww = UtoPOW(consumed_util[0][i]);
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
    double curr_pow = UtoPOW(consumed_util[0][timeslot]);
    if (curr_pow < avg)
        return 0;
    return (int)(curr_pow - avg);
}

int nextMin_consumed_timeslot(int t, double avg_pow)
{
    if (t == Time - 1)
        return -1; // no next time slot as it is the last slot
    int mn = t;
    int val = consumed_util[0][t];
    double diff = 0;
    for (int i = t + 1; i < Time; i++)
    {
        double diff_temp = avg_pow - UtoPOW(consumed_util[0][i]);
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






double findavailablebatterypower(int &oldtime,int &newtime){
    int consumedmax = -1;
    for(int i = oldtime + 1 ;  i <= newtime ; i++ ){
        consumedmax = max(consumedmax,(int)batteryconsumed[i]);
    }
    return battery_capacity-consumedmax;
}


pair<double,double> findutilityProfit_byid(int Tid)
{
    double u = 0.0;
    double proft = 0.0;
    for (auto &a : Task_pred)
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


bool isremove_equals_to_powReduce(int &timeslot, double &pow_to_reduce, int no_task_Toremove)
{

    int no_task_allocated = allocationmatrix[timeslot].size();
    int task_no = no_task_allocated - 1; // to iterate on array and 0 based indexing
    double total_utility_toRemove = 0.0;
    int tid;
    double oldutil, diffutil;
    double pow_oldutil, pow_newutil;
    double diffpow;

    oldutil = consumed_util[0][timeslot];
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
        taskscheduled_pred[tid]=0;
        predictedtask_scheduletime[tid]=-1;
        allocationmatrix[timeslot].pop_back();
        pair<double,double> p = findutilityProfit_byid(tid);
        totalutil +=p.first;
        profit_remove+=p.second;
        toremove--;
    }
    double oldutil, newutil;
    double pow_oldutil, pow_newutil;
    double diffpow;
    oldutil = consumed_util[0][timeslot];
    newutil = oldutil - totalutil;
    pow_oldutil = UtoPOW(oldutil);
    pow_newutil = UtoPOW(newutil);
    diffpow = pow_oldutil - pow_newutil;
    energyTimestamp[0][timeslot] = pow_newutil;
    consumed_util[0][timeslot] = newutil;
    initial_util[0][timeslot]-=totalutil;
    remain_util[0][timeslot] = initial_util[0][timeslot] - consumed_util[0][timeslot];
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

void heapops(priority_queue<pair<double, int>> &maxheap,int timeslot,double avg_poww){
    while(!maxheap.empty())maxheap.pop();
    for (int i = 0; i < Time; i++)
    {
        maxheap.push({pow_above_avg(i, avg_poww), i}); // pushing the difference from the average
    }
}



void updateconsumedbattery(int oldtime,int newtime,double amount){

    for(int i = oldtime +1 ; i <= newtime ; i++){
        batteryconsumed[i] += amount;
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

   // double oldprofit = totalProfit;
    double profitphase1 = totalProfit;
    int itr = 0;
    int timeslot = -1;
    cout<<" ********** pHASE 2 STARTS *********** \n";
    while (itr < 5000)
    {
        timeslot = maxheap.top().second;
        maxheap.pop();
        if (timeslot == (Time - 1))
            continue;
        int newtimestamp = nextMin_consumed_timeslot(timeslot, avg_poww);
        if (newtimestamp == -1)
            break;   
                                           // no consumption is below average
       // cout<<"\n time stamp is "    <<timeslot<<" "<<UtoPOW(consumed_util[timeslot])<<endl;                               
        double newtime_pow_cons = UtoPOW(consumed_util[0][newtimestamp]); // new timestamp power consumption
        double diff_with_avg_newtime = avg_poww - newtime_pow_cons;
        double diff_with_avg_oldtime = UtoPOW(consumed_util[0][timeslot]) - avg_poww;
      //  cout<<"avg_newtime "<<diff_with_avg_newtime<<" avg_oldtime "<<diff_with_avg_oldtime<<endl;
        double availablebattery = findavailablebatterypower(timeslot,newtimestamp);
        double min_transferrable_pow = min({availablebattery, diff_with_avg_newtime, diff_with_avg_oldtime});
       // cout<<"\n transferrable unit is "<<min_transferrable_pow<<endl;
       cout<<" old time "<<timeslot<<" power at old "<<UtoPOW(consumed_util[0][timeslot])<<" new timeslot "<<newtimestamp<<" pow new "<<newtime_pow_cons<<" transfer "<<min_transferrable_pow<<endl;
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
    oldprofit = totalProfit;
    //cout << "\n  profit phas 1 " << phase1profit << endl;
    //cout << "\n max profit is " << totalProfit << endl;
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
            maxheap.push({prft, {tid,{prft,utill}}});
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
            maxheap.push({prft, {tid,{prft,utill}}});
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
    cout << "\nRevenue after Algo 6: Offline Schedule for Finite Battery: TS-GES-SFB->" << oldprofit << endl;

    cout<<" \nRevenue after Algo 5: Online Scheduling Approach->"<<onlineprofit<<"\n";
}



int main()
{
    
   
    filereader();
    
    phase1();
    phase2();
   
     onlinephase();
 
     taskateachtimeslot();

    return 0;
}
