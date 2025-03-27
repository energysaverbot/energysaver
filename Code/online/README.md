# Readme

Instructions to Compile and Run

1. Ensure you have the GNU C++ compiler (g++) installed on your system.
2. Clone or download this repository to your local machine.
3. Open a terminal or command prompt and navigate to the directory containing the code files.
4. Compile the code using the following command:
   
   g++ -o a filename.cpp
   
   Replace `filename.cpp` with the actual name of your C++ source file.

5. Execute the compiled program using the following command:
   
   ./a.out

Updating File Names and Variables in `filereader` Function

1. Open the `filereader` function in the code.
2. Update the filenames for solar power data and task data as needed. Modify the following lines within the `filereader` function:
  
Infinite Battery Case[online_solar_infinite_battery.csv]
   // Open the CSV files for reading
line 211: FILE *f_pow_actual = fopen("../../../Dataset/Single Day/Power/power_actual_5_percent_deviation.csv", "r"); 
line 212: FILE *f_task_actual = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_actual_5_percent_deviation.csv", "r");//task data from predicted data which actually comes in online scenario
    // Check if the file was successfully opened

line 215: FILE *f_pow_pred = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r"); //predicted solar power
line 216: FILE *f_task_pred = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_predicted.csv", "r");//predicted task data

line 218: FILE *f_task_new = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_new_5_percent_deviation.csv", "r");//task data which are new in online case
   
   Ensure that all necessary dataset files ("power_actual_5_percent_deviation.csv", "task_actual_5_percent_deviation.csv", "power_predicted.csv", "task_predicted.csv", "task_new_5_percent_deviation.csv") are in the same directory as the code.

Finite Battery Case[online_solar_finitebattery.csv]

line 215: FILE *f_pow_actual = fopen("../../../Dataset/Single Day/Power/power_actual_5_percent_deviation.csv", "r"); 
line 216: FILE *f_task_actual = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_actual_5_percent_deviation.csv", "r");//task data from predicted data which actually comes in online scenario
    // Check if the file was successfully opened

line 219: FILE *f_pow_pred = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r"); //predicted solar power
line 220: FILE *f_task_pred = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_predicted.csv", "r");//predicted task data

line 222: FILE *f_task_new = fopen("../../../Dataset/Single Day/Task/rho 0.03/task_new_5_percent_deviation.csv", "r");//task data which are new in online case
In the case of the finite battery code, the variable `batterycap` represents the battery capacity. Update this variable as needed for your specific scenario.

Hyperparameters

All the hyperparameter variables are declared as macros. Modify these macros according to your tuning requirements.

Additional Notes

- Review the code comments for more detailed explanations of functions, variables, and hyperparameters.
- Customize the data processing logic inside the `filereader` function based on your specific dataset and analysis needs.
- Ensure proper file permissions and directory access when compiling and running the code.
