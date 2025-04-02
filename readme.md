# Maximizing Revenue in Roadside Green Edge Server System

This repository contains the codebase and datasets for the proposed work on task scheduling in solar-powered Green Edge Server Systems (GESS) to support autonomous vehicles.

---

## 📁 Directory Structure

### 1. `Dataset/`
Contains all the datasets used in the experiments.

#### └─ `Single Day/`
- **Power/**
  - `power_predicted.csv`: Predicted solar power profile throughout the day.
  - `power_actual_5_percent_deviation.csv`: 5% deviation from predicted solar power.
  - `power_actual_10_percent_deviation.csv`: 10% deviation from predicted solar power.

- **Task/**
  - Each `rho` folder (e.g., `rho 0.03/`, `rho 0.05/`, ...) contains:
    - `task_predicted.csv`: Predicted task profile.
    - `task_actual_5_percent_deviation.csv`: 5% deviation for existing tasks.
    - `task_actual_10_percent_deviation.csv`: 10% deviation for existing tasks.
    - `task_new_5_percent_deviation.csv`: 5% deviation with new unpredicted tasks.
    - `task_new_10_percent_deviation.csv`: 10% deviation with new unpredicted tasks.

#### └─ `Multiple Day/`
- **Power/**
  - `power_predicted.csv`: Predicted solar power across 7 days.
- **Task/**
  - Folders for different rho values (e.g., `rho 0.03`, `rho 0.1`, `rho 0.18`) contain:
    - `task_predicted.csv`: Multi-day task profile.

---

### 2. `Code/`
Contains the implementation of all scheduling algorithms.

#### └─ `offline/`
- `infinitebattery_offline.cpp`: Implements Algo 2 + Algo 4.
- `finitebattery_offline.cpp`: Implements Algo 2 + Algo 6.

#### └─ `online/`
- `online_solar_infinite_battery.cpp`: Implements Algo 2 + Algo 4 + Algo 5.
- `online_solar_finitebattery.cpp`: Implements Algo 2 + Algo 6 + Algo 5.

#### └─ `stateofart/`
Baseline methods from the literature:
- `NPEDF_*`: Non-preemptive Earliest Deadline First (finite/infinite battery).
- `EA_*`: Execute-on-Arrival methods.
- `asap_HUF_*`: As Soon As Possible – Highest Utilization First.
- `asap_LUF_*`: As Soon As Possible – Lowest Utilization First.

---

## 🔧 Usage

- **Compile the C++ files** with `g++`:
  ```bash
  g++ infinitebattery_offline.cpp -o infinitebattery
  ./infinitebattery
