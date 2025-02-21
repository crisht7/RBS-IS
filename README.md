# RBS-IS
This program implements a **backward chaining inference engine** for a **rule-based expert system**. It processes a **Knowledge Base (BC)** and a **Fact Base (BH)** to determine the certainty factor (FC) of a given objective. The system applies logical inference using predefined rules, certainty factors, and logical operators (`and`, `or`). 

# Rule-Based System - User Manual

## Input Files
The program requires two main input files: the **Knowledge Base (BC-i)** and the **Fact Base (BH-i)**, where `i` represents the section number. These files must follow the specified format:

### Knowledge Base:
n R1: …, FC=x … Rn: …, FC=y
- `n`: Total number of rules in the knowledge base.
- `R`: Rules describing the system.
- `FC`: Certainty factor associated with each rule.

### Fact Base:
n …, FC=y Objective x
- `n`: Number of facts in the system.
- `FC`: Certainty factor associated with each fact.
- `Objective x`: The fact for which we want to calculate the certainty factor.

## Output Files
If the program executes successfully, it will generate an output file named `fichero-i.txt`.  
This file will contain the computed certainty factor (`FC`) for each fact using **backward chaining**, ultimately obtaining the **certainty factor of the objective**.

## Execution Instructions
To run the program, ensure that `BC-i.txt` and `BH-i.txt` are located in the same directory as the executable.  
The program can be executed in different ways:

- **Using a terminal** (recommended):
  ```sh
  .\RBS.exe .\BC-i.txt .\BH-i.txt
