// Employee Payroll Management System
// CIC-211 : Object-Oriented Programming using C++ (Mini Project 4)
//
// OOP concepts used:
//   1. Classes & objects     - Employee, PermanentEmployee, ContractEmployee, PayrollSystem
//   2. Encapsulation         - private data members + public getters/setters
//   3. Constructors          - every class initialises its objects through constructors
//   4. Inheritance           - PermanentEmployee and ContractEmployee derive from Employee
//   5. Virtual functions     - calcAllowances(), calcDeductions(), category() are overridden
//   6. Runtime polymorphism  - base-class pointers (Employee*) call derived-class versions

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// ---------------------------------------------------------------
// Base class (abstract): common data and common salary formulas
// ---------------------------------------------------------------
class Employee {
private:
    int id;
    string name;
    string designation;
    double basicSalary;

public:
    Employee(int id, const string& name, const string& designation, double basicSalary)
        : id(id), name(name), designation(designation), basicSalary(basicSalary) {}

    virtual ~Employee() {}

    // Accessors
    int getId() const { return id; }
    string getName() const { return name; }
    string getDesignation() const { return designation; }
    double getBasicSalary() const { return basicSalary; }
    void setBasicSalary(double b) { basicSalary = b; }

    // Category-specific rules (each derived class supplies its own version)
    virtual double calcAllowances() const = 0;
    virtual double calcDeductions() const = 0;
    virtual string category() const = 0;
    virtual string formulaText() const = 0;

    // Common formulas
    double grossSalary() const { return basicSalary + calcAllowances(); }
    double netSalary() const { return grossSalary() - calcDeductions(); }

    // One row of the table
    void displayRow() const {
        cout << left << setw(6) << id
             << setw(16) << name
             << setw(14) << designation
             << setw(11) << category()
             << right << fixed << setprecision(2)
             << setw(11) << basicSalary
             << setw(11) << grossSalary()
             << setw(11) << netSalary() << "\n";
    }

    // Detailed pay slip
    void displaySlip() const {
        cout << fixed << setprecision(2);
        cout << "\n------------- PAY SLIP -------------\n";
        cout << "ID          : " << id << "\n";
        cout << "Name        : " << name << "\n";
        cout << "Designation : " << designation << "\n";
        cout << "Category    : " << category() << "\n";
        cout << "Basic       : " << basicSalary << "\n";
        cout << "Allowances  : " << calcAllowances() << "\n";
        cout << "Gross       : " << grossSalary() << "\n";
        cout << "Deductions  : " << calcDeductions() << "\n";
        cout << "Net Salary  : " << netSalary() << "\n";
        cout << "Rule        : " << formulaText() << "\n";
        cout << "------------------------------------\n";
    }
};

// ---------------------------------------------------------------
// Derived class 1: Permanent employee
//   Allowances = 20% HRA + 10% DA of basic
//   Deductions = 12% PF of basic
// ---------------------------------------------------------------
class PermanentEmployee : public Employee {
private:
    static constexpr double HRA_RATE = 0.20;
    static constexpr double DA_RATE = 0.10;
    static constexpr double PF_RATE = 0.12;

public:
    PermanentEmployee(int id, const string& name, const string& designation, double basic)
        : Employee(id, name, designation, basic) {}

    double calcAllowances() const override { return getBasicSalary() * (HRA_RATE + DA_RATE); }
    double calcDeductions() const override { return getBasicSalary() * PF_RATE; }
    string category() const override { return "Permanent"; }
    string formulaText() const override {
        return "Gross = Basic + 20% HRA + 10% DA; Net = Gross - 12% PF";
    }
};

// ---------------------------------------------------------------
// Derived class 2: Contract employee
//   Allowances = fixed 3000 (travel allowance)
//   Deductions = 10% TDS of basic
// ---------------------------------------------------------------
class ContractEmployee : public Employee {
private:
    static constexpr double FIXED_ALLOWANCE = 3000.0;
    static constexpr double TDS_RATE = 0.10;

public:
    ContractEmployee(int id, const string& name, const string& designation, double basic)
        : Employee(id, name, designation, basic) {}

    double calcAllowances() const override { return FIXED_ALLOWANCE; }
    double calcDeductions() const override { return getBasicSalary() * TDS_RATE; }
    string category() const override { return "Contract"; }
    string formulaText() const override {
        return "Gross = Basic + 3000 fixed; Net = Gross - 10% TDS";
    }
};

// ---------------------------------------------------------------
// Input helpers (validation)
// ---------------------------------------------------------------
int readInt(const string& prompt, int minVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a whole number >= " << minVal << ".\n";
    }
}

double readPositiveDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number greater than 0.\n";
    }
}

string readText(const string& prompt) {
    string text;
    while (true) {
        cout << prompt;
        getline(cin, text);
        if (!text.empty()) return text;
        cout << "Input cannot be empty.\n";
    }
}

// ---------------------------------------------------------------
// Manager class: holds all employees and the menu operations
// ---------------------------------------------------------------
class PayrollSystem {
private:
    vector<Employee*> employees;

    int findIndex(int id) const {
        for (size_t i = 0; i < employees.size(); i++)
            if (employees[i]->getId() == id) return (int)i;
        return -1;
    }

public:
    ~PayrollSystem() {
        for (Employee* e : employees) delete e;
    }

    void addEmployee(bool permanent) {
        int id = readInt("Employee ID: ", 1);
        if (findIndex(id) != -1) {
            cout << "Employee ID already exists. Record not added.\n";
            return;
        }
        string name = readText("Name: ");
        string designation = readText("Designation: ");
        double basic = readPositiveDouble("Basic salary: ");

        if (permanent)
            employees.push_back(new PermanentEmployee(id, name, designation, basic));
        else
            employees.push_back(new ContractEmployee(id, name, designation, basic));
        cout << "Employee added successfully.\n";
    }

    void displayAll() const {
        if (employees.empty()) {
            cout << "No employee records available.\n";
            return;
        }
        cout << "\n" << left << setw(6) << "ID" << setw(16) << "Name" << setw(14) << "Designation"
             << setw(11) << "Category" << right << setw(11) << "Basic" << setw(11) << "Gross"
             << setw(11) << "Net" << "\n";
        cout << string(80, '-') << "\n";
        for (const Employee* e : employees) e->displayRow();   // runtime polymorphism
    }

    void searchEmployee() const {
        int id = readInt("Enter employee ID to search: ", 1);
        int idx = findIndex(id);
        if (idx == -1)
            cout << "Employee not found.\n";
        else
            employees[idx]->displaySlip();
    }

    void updateSalary() {
        int id = readInt("Enter employee ID to update: ", 1);
        int idx = findIndex(id);
        if (idx == -1) {
            cout << "Employee not found.\n";
            return;
        }
        double basic = readPositiveDouble("New basic salary: ");
        employees[idx]->setBasicSalary(basic);
        cout << "Basic salary updated.\n";
    }

    void deleteEmployee() {
        int id = readInt("Enter employee ID to delete: ", 1);
        int idx = findIndex(id);
        if (idx == -1) {
            cout << "Employee not found.\n";
            return;
        }
        delete employees[idx];
        employees.erase(employees.begin() + idx);
        cout << "Employee deleted.\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n===== EMPLOYEE PAYROLL SYSTEM =====\n"
                 << "1. Add Permanent Employee\n"
                 << "2. Add Contract Employee\n"
                 << "3. Display All Employees\n"
                 << "4. Search Employee (Pay Slip)\n"
                 << "5. Update Basic Salary\n"
                 << "6. Delete Employee\n"
                 << "7. Exit\n";
            choice = readInt("Enter choice: ", 0);
            switch (choice) {
                case 1: addEmployee(true); break;
                case 2: addEmployee(false); break;
                case 3: displayAll(); break;
                case 4: searchEmployee(); break;
                case 5: updateSalary(); break;
                case 6: deleteEmployee(); break;
                case 7: cout << "Goodbye!\n"; break;
                default: cout << "Invalid choice. Enter 1-7.\n";
            }
        } while (choice != 7);
    }
};

int main() {
    PayrollSystem system;
    system.run();
    return 0;
}
