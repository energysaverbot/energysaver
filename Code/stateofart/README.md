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
   ```cpp
   // Open the CSV files for reading
   FILE *f_pow = fopen("../../../Dataset/Multiple Day/Power/power_predicted.csv", "r");
    FILE *f_task = fopen("../../../Dataset/Multiple Day/Task/rho 0.1/task_predicted.csv", "r");
   
   Ensure that both dataset files ("power_predicted.csv" and "task_predicted_U5_D5.csv") are in the same directory as the code.

Finite Battery Case

In case of the finite battery code, the variable `batterycap` represents the battery capacity. Update this variable as needed for your specific scenario.

Hyperparameters

All the hyperparameter variables are declared as macros. Modify these macros according to your tuning requirements.

Additional Notes

- Review the code comments for more detailed explanations of functions, variables, and hyperparameters.
- Customize the data processing logic inside the `filereader` function based on your specific dataset and analysis needs.
- Ensure proper file permissions and directory access when compiling and running the code.



