# Employee Payroll Management System

Console-based C++ mini project for **CIC-211: Object-Oriented Programming using C++** (Mini Project 4).

## Features
- Add Permanent and Contract employees (ID, name, designation, basic salary)
- Display all employees in a table (basic, gross, net)
- Search by ID and print a detailed pay slip
- Update basic salary and delete an employee
- Input validation (duplicate IDs, empty names, non-numeric or non-positive values)

## Salary Rules
| Category  | Allowances                  | Deductions        |
|-----------|-----------------------------|-------------------|
| Permanent | 20% HRA + 10% DA of basic   | 12% PF of basic   |
| Contract  | Fixed 3000                  | 10% TDS of basic  |

`Gross = Basic + Allowances`  |  `Net = Gross - Deductions`

## OOP Concepts Used
| Concept | Where |
|---------|-------|
| Classes and objects | `Employee`, `PermanentEmployee`, `ContractEmployee`, `PayrollSystem` |
| Encapsulation | Private data in `Employee`, accessed via getters/setter |
| Constructors | Initializer lists in every class |
| Inheritance | Both employee types derive from `Employee` |
| Virtual functions / overriding | `calcAllowances()`, `calcDeductions()`, `category()`, `formulaText()` |
| Runtime polymorphism | `vector<Employee*>` calls the derived versions |

## Build and Run
```bash
g++ -std=c++11 -o payroll payroll.cpp
./payroll        # on Windows: payroll.exe
```

## Sample Run
```
ID    Name            Designation   Category         Basic      Gross        Net
--------------------------------------------------------------------------------
101   Aman Sharma     Manager       Permanent     50000.00   65000.00   59000.00
102   Riya Verma      Developer     Contract      30000.00   33000.00   30000.00
```
