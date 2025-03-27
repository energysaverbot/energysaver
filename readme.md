# Readme

There are two folders:
Dataset- It contains all the datasets used in the proposed work.
Code- It contains all the codes for the proposed work.

TECT/
├── readme.txt
├── Dataset/
│   ├── readme.md
│   ├── Single Day/
│   │   ├── Power/
│   │   │   ├── power_predicted.csv [predicted solar power throughout the day]
│   │   │   ├── power_actual_5_percent_deviation.csv [deviation of 5% from the predicted solar power]
│   │   │   └── power_actual_10_percent_deviation.csv [deviation of 10% from the predicted solar power]
│   │   └── Task/
│   │       ├── rho 0.03/ 
│   │       │   ├── task_predicted.csv predicted task profile throughout the day]
│   │       │   ├── task_actual_5_percent_deviation.csv [deviation of 5% from the predicted task profile, this file contains tasks that are predicted and actually arrive]
│   │       │   ├── task_actual_10_percent_deviation.csv [deviation of 10% from the predicted task profile, this file contains tasks that are predicted and actually arrive]
│   │       │   ├── task_new_5_percent_deviation.csv [deviation of 5% from the predicted task profile, this file contains new unpredicted tasks that arrive]
│   │       │   └── task_new_10_percent_deviation.csv [deviation of 10% from the predicted task profile, this file contains new unpredicted tasks that arrive]
│   │       ├── rho 0.05/
│   │       │   ├── task_predicted.csv
│   │       │   ├── task_actual_5_percent_deviation.csv
│   │       │   ├── task_actual_10_percent_deviation.csv
│   │       │   ├── task_new_5_percent_deviation.csv
│   │       │   └── task_new_10_percent_deviation.csv
│   │       ├── rho 0.1/
│   │       │   ├── task_predicted.csv
│   │       │   ├── task_actual_5_percent_deviation.csv
│   │       │   ├── task_actual_10_percent_deviation.csv
│   │       │   ├── task_new_5_percent_deviation.csv
│   │       │   └── task_new_10_percent_deviation.csv
│   │       ├── rho 0.15/
│   │       │   ├── task_predicted.csv
│   │       │   ├── task_actual_5_percent_deviation.csv
│   │       │   ├── task_actual_10_percent_deviation.csv
│   │       │   ├── task_new_5_percent_deviation.csv
│   │       │   └── task_new_10_percent_deviation.csv
│   │       └── rho 0.18/
│   │           ├── task_predicted.csv
│   │           ├── task_actual_5_percent_deviation.csv
│   │           ├── task_actual_10_percent_deviation.csv
│   │           ├── task_new_5_percent_deviation.csv
│   │           └── task_new_10_percent_deviation.csv
│   └── Multiple Day/ [same as before, number of days is 7]
│       ├── Power/
│       │   └── power_predicted.csv
│       └── Task/
│           ├── rho 0.03/
│           │   └── task_predicted.csv
│           ├── rho 0.1/
│           │   └── task_predicted.csv
│           └── rho 0.18/
│               └── task_predicted.csv
│
└── Code/
    ├── readme.txt
    ├── offline/
    │   ├── README.txt
    │   ├── infinitebattery_offline.cpp [Algo 2 + Algo 4]
    │   ├── finitebattery_offline.cpp [Algo 2 + Algo 6]
    │   └── a.out
    ├── online/
    │   ├── README.txt
    │   ├── online_solar_finitebattery.cpp [Algo 2 + Algo 6 + Algo 5]
    │   ├── online_solar_infinite_battery.cpp [Algo 2 + Algo 4 + Algo 5]
    └── stateofart/ [the names are self explanatory]
        ├── README.txt
        ├── NPEDF_finitebattery.cpp
        ├── NPEDF_infinitebattery.cpp
        ├── EA_finitebattery.cpp
        ├── EA_infinitebattery.cpp
        ├── asap_HUF_finitebattery.cpp
        ├── asap_HUF_infinitebattery.cpp
        ├── asap_LUF_finitebattery.cpp
        └── asap_LUF_infinitebattery.cpp

