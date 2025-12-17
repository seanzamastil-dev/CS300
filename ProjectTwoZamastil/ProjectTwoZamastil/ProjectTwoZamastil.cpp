// ProjectTwoZamastil.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Developer: Sean Zamastil
// SNHU CS300
// 12/14/25
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// Course structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course(string cn = "", string t = "", vector<string> prereqs = {})
        : courseNumber(cn), title(t), prerequisites(prereqs) {
    }
};

// Convert string to uppercase
string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Load courses from file
vector<Course> loadCoursesFromFile(const string& fileName) {
    vector<Course> courses;
    ifstream file(fileName);
    string line;

    cout << "Starting to load courses from file." << endl;

    if (!file.is_open()) {
        cout << "Failed to open file: " << fileName << endl;
        return courses;
    }

    cout << "File opened successfully." << endl;

    int courseCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string courseNumber, title, prereq;

        getline(ss, courseNumber, ',');
        getline(ss, title, ',');

        vector<string> prerequisites;
        while (getline(ss, prereq, ',')) {
            prerequisites.push_back(toUpper(prereq));
        }

        courses.push_back(Course(toUpper(courseNumber), title, prerequisites));
        cout << "Loaded course: " << courseNumber << " - " << title << endl;
        courseCount++;
    }

    cout << "Total courses loaded: " << courseCount << endl;
    return courses;
}

// Build fast lookup map for course titles
map<string, string> buildCourseMap(const vector<Course>& courses) {
    map<string, string> courseMap;
    for (const auto& course : courses) {
        courseMap[toUpper(course.courseNumber)] = course.title;
    }
    return courseMap;
}

// Print all courses ALPHABETICALLY
void printAllCourses(vector<Course> courses) {
    sort(courses.begin(), courses.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    cout << "\nHere is a sample schedule:\n";
    for (const auto& course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }
}

// Print a single course with VALIDATED prerequisites
void printCourse(const vector<Course>& courses,
    const map<string, string>& courseMap,
    const string& courseNumber) {
    string target = toUpper(courseNumber);

    for (const auto& course : courses) {
        if (toUpper(course.courseNumber) == target) {
            cout << course.courseNumber << ", " << course.title << endl;

            if (!course.prerequisites.empty()) {
                bool first = true;
                for (const auto& prereq : course.prerequisites) {
                    if (courseMap.count(prereq)) { // VALIDATE prerequisite
                        if (first) {
                            cout << "Prerequisites: ";
                            first = false;
                        }
                        else {
                            cout << ", ";
                        }
                        cout << prereq << " (" << courseMap.at(prereq) << ")";
                    }
                }
                if (!first) cout << endl;
            }
            return;
        }
    }

    cout << "Course not found." << endl;
}

// Main menu
int main() {
    vector<Course> courses;
    map<string, string> courseMap;
    string filename, courseNumber;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            cout << "Enter file name: ";
            getline(cin, filename);
            courses = loadCoursesFromFile(filename);
            courseMap = buildCourseMap(courses);
            break;

        case 2:
            if (courses.empty())
                cout << "Please load data first." << endl;
            else
                printAllCourses(courses);
            break;

        case 3:
            if (courses.empty())
                cout << "Please load data first." << endl;
            else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);
                printCourse(courses, courseMap, courseNumber);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
