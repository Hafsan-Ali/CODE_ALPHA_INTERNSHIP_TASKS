#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

// ─────────────────────────────────────────
//  Course Class
// ─────────────────────────────────────────
class Course {
public:
    string name;
    string grade;
    float  creditHours;
    float  gradePoints;   // grade letter → grade point (e.g. A → 4.0)

    Course(string n, string g, float ch, float gp)
        : name(n), grade(g), creditHours(ch), gradePoints(gp) {}
};

// ─────────────────────────────────────────
//  Grade Converter
// ─────────────────────────────────────────
float gradeToPoints(const string& grade) {
    if (grade == "A+" || grade == "a+") return 4.0f;
    if (grade == "A"  || grade == "a")  return 4.0f;
    if (grade == "A-" || grade == "a-") return 3.7f;
    if (grade == "B+" || grade == "b+") return 3.3f;
    if (grade == "B"  || grade == "b")  return 3.0f;
    if (grade == "B-" || grade == "b-") return 2.7f;
    if (grade == "C+" || grade == "c+") return 2.3f;
    if (grade == "C"  || grade == "c")  return 2.0f;
    if (grade == "C-" || grade == "c-") return 1.7f;
    if (grade == "D+" || grade == "d+") return 1.3f;
    if (grade == "D"  || grade == "d")  return 1.0f;
    if (grade == "F"  || grade == "f")  return 0.0f;
    return -1.0f;  // invalid
}

// ─────────────────────────────────────────
//  CGPACalculator Class
// ─────────────────────────────────────────
class CGPACalculator {
private:
    vector<vector<Course>> semesters;  // each semester has a list of courses

public:

    // ── Input one semester ────────────────
    void addSemester() {
        int semNum = semesters.size() + 1;
        cout << "\n  ╔══════════════════════════════════╗\n";
        cout << "  ║       SEMESTER " << semNum << " INPUT           ║\n";
        cout << "  ╚══════════════════════════════════╝\n";

        int numCourses;
        cout << "  Enter number of courses: ";
        cin >> numCourses;

        if (numCourses <= 0) {
            cout << "  Invalid number of courses.\n";
            return;
        }

        vector<Course> semCourses;
        cin.ignore();

        for (int i = 1; i <= numCourses; i++) {
            cout << "\n  --- Course " << i << " ---\n";

            string name, grade;
            float  credits;

            cout << "  Course Name  : ";
            getline(cin, name);

            cout << "  Credit Hours : ";
            cin >> credits;

            float gp = -1;
            while (gp < 0) {
                cout << "  Grade        : ";
                cin >> grade;
                gp = gradeToPoints(grade);
                if (gp < 0)
                    cout << "  [!] Invalid grade. Use A+/A/A-/B+/.../F\n";
            }
            cin.ignore();

            semCourses.emplace_back(name, grade, credits, gp);
        }

        semesters.push_back(semCourses);
        cout << "\n  Semester " << semNum << " added successfully!\n";
    }

    // ── GPA for one semester ──────────────
    float calcSemGPA(const vector<Course>& courses) const {
        float totalCredits    = 0;
        float totalGradePoints = 0;
        for (const auto& c : courses) {
            totalCredits     += c.creditHours;
            totalGradePoints += c.gradePoints * c.creditHours;
        }
        return (totalCredits == 0) ? 0 : totalGradePoints / totalCredits;
    }

    // ── Overall CGPA ──────────────────────
    float calcCGPA() const {
        float totalCredits     = 0;
        float totalGradePoints = 0;
        for (const auto& sem : semesters) {
            for (const auto& c : sem) {
                totalCredits     += c.creditHours;
                totalGradePoints += c.gradePoints * c.creditHours;
            }
        }
        return (totalCredits == 0) ? 0 : totalGradePoints / totalCredits;
    }

    // ── Display all results ───────────────
    void displayResults() const {
        if (semesters.empty()) {
            cout << "\n  No semesters recorded yet.\n";
            return;
        }

        cout << "\n  ╔══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                   ACADEMIC REPORT                       ║\n";
        cout << "  ╚══════════════════════════════════════════════════════════╝\n";

        for (int s = 0; s < (int)semesters.size(); s++) {
            float gpa = calcSemGPA(semesters[s]);

            cout << "\n  ┌─────────────────────────────────────────────────────┐\n";
            cout << "  │  SEMESTER " << (s + 1)
                 << "                              GPA: "
                 << fixed << setprecision(2) << gpa << "        │\n";
            cout << "  ├──────────────────────┬───────────┬────────┬─────────┤\n";
            cout << "  │ Course               │  Credits  │ Grade  │  Points │\n";
            cout << "  ├──────────────────────┼───────────┼────────┼─────────┤\n";

            float semCredits = 0, semGP = 0;
            for (const auto& c : semesters[s]) {
                cout << "  │ " << left  << setw(20) << c.name
                     << " │ " << right << setw(9)  << c.creditHours
                     << " │ " << left  << setw(6)  << c.grade
                     << " │ " << right << setw(7)  << fixed
                     << setprecision(1) << c.gradePoints << " │\n";
                semCredits += c.creditHours;
                semGP      += c.gradePoints * c.creditHours;
            }

            cout << "  ├──────────────────────┼───────────┼────────┼─────────┤\n";
            cout << "  │ " << left  << setw(20) << "TOTAL"
                 << " │ " << right << setw(9)  << semCredits
                 << " │        │ " << setw(7)
                 << setprecision(2) << semGP << " │\n";
            cout << "  └──────────────────────┴───────────┴────────┴─────────┘\n";
        }

        // ── Final CGPA ────────────────────
        float cgpa = calcCGPA();
        string remark;
        if      (cgpa >= 3.7) remark = "Excellent  🎓";
        else if (cgpa >= 3.3) remark = "Very Good";
        else if (cgpa >= 3.0) remark = "Good";
        else if (cgpa >= 2.5) remark = "Satisfactory";
        else if (cgpa >= 2.0) remark = "Pass";
        else                  remark = "Fail";

        cout << "\n  ╔══════════════════════════════════════╗\n";
        cout << "  ║   OVERALL CGPA : "
             << fixed << setprecision(2) << cgpa
             << "  /  4.00          ║\n";
        cout << "  ║   REMARK       : " << left << setw(20) << remark << "║\n";
        cout << "  ╚══════════════════════════════════════╝\n";
    }

    // ── Grade scale reference ─────────────
    void showGradeScale() const {
        cout << "\n  ┌─────────────────────────┐\n";
        cout << "  │     GRADE SCALE         │\n";
        cout << "  ├──────────┬──────────────┤\n";
        cout << "  │  Grade   │  Points      │\n";
        cout << "  ├──────────┼──────────────┤\n";
        cout << "  │  A+/A    │  4.0         │\n";
        cout << "  │  A-      │  3.7         │\n";
        cout << "  │  B+      │  3.3         │\n";
        cout << "  │  B       │  3.0         │\n";
        cout << "  │  B-      │  2.7         │\n";
        cout << "  │  C+      │  2.3         │\n";
        cout << "  │  C       │  2.0         │\n";
        cout << "  │  C-      │  1.7         │\n";
        cout << "  │  D+      │  1.3         │\n";
        cout << "  │  D       │  1.0         │\n";
        cout << "  │  F       │  0.0         │\n";
        cout << "  └──────────┴──────────────┘\n";
    }

    int semesterCount() const { return semesters.size(); }
};

// ─────────────────────────────────────────
//  Menu
// ─────────────────────────────────────────
void printMenu(int semCount) {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║       CGPA CALCULATOR            ║\n";
    cout << "  ╠══════════════════════════════════╣\n";
    cout << "  ║  1. Add Semester                 ║\n";
    cout << "  ║  2. View Results & CGPA          ║\n";
    cout << "  ║  3. Show Grade Scale             ║\n";
    cout << "  ║  0. Exit                         ║\n";
    cout << "  ╠══════════════════════════════════╣\n";
    cout << "  ║  Semesters recorded: " << semCount
         << "           ║\n";
    cout << "  ╚══════════════════════════════════╝\n";
    cout << "  Choice: ";
}

// ─────────────────────────────────────────
//  Main
// ─────────────────────────────────────────
int main() {
    CGPACalculator calc;
    int choice;

    do {
        printMenu(calc.semesterCount());
        cin >> choice;

        switch (choice) {
        case 1:
            calc.addSemester();
            break;
        case 2:
            calc.displayResults();
            break;
        case 3:
            calc.showGradeScale();
            break;
        case 0:
            cout << "\n  Goodbye!\n\n";
            break;
        default:
            cout << "  Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}