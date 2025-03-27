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

Updating File Names and Variables in `filereader` Function in Infinite Battery case[infinitebattery_offline.cpp]:

line 189: FILE *f_pow = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r");
line 190: FILE *f_task = fopen("../../../Dataset/Single Day/Task/rho 0.1/task_predicted.csv", "r");

Ensure that both dataset files ("power_predicted.csv" and "task_predicted.csv") are in the same directory as the code.

Updating File Names and Variables in `filereader` Function in Finite Battery case[finitebattery_offline.cpp]:

line 198: FILE *f_pow = fopen("../../../Dataset/Single Day/Power/power_predicted.csv", "r");
line 199: FILE *f_task = fopen("../../../Dataset/Single Day/Task/rho 0.1/task_predicted.csv", "r");

In case of the finite battery code, the variable `batterycap` represents the battery capacity. Update this variable as needed for your specific scenario.

Hyperparameters
All the hyperparameter variables are declared as macros. Modify these macros according to your tuning requirements.

The file first prints some temporary data for scheduling and prints the final profit at last

Additional Notes

- Review the code comments for more detailed explanations of functions, variables, and hyperparameters.
- Customize the data processing logic inside the `filereader` function based on your specific dataset and analysis needs.
- Ensure proper file permissions and directory access when compiling and running the code.
