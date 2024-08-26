#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
using namespace std;

struct Course {
    string courseNumber;
    string name;
    vector<string> prerequisites;
};

vector<string> tokenize(string s, string del = ",") {
    vector<string> stringArray;
    size_t start = 0;
    size_t end = s.find(del);

    while (end != string::npos) {
        stringArray.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }

    stringArray.push_back(s.substr(start, end - start));
    return stringArray;
}

vector<Course> loadDataStructure(const string& fileName) {
    ifstream fin(fileName);
    if (!fin.is_open()) {
        cerr << "Error while attempting to open: " << fileName << endl;
        exit(EXIT_FAILURE);
    }

    vector<Course> courses;
    unordered_map<string, Course> courseMap;

    string line;

    while (getline(fin, line)) {
        if (line == "-1") {
            break;
        }

        vector<string> tokenizedInformation = tokenize(line);

        Course course;
        course.courseNumber = tokenizedInformation[0];
        course.name = tokenizedInformation[1];

        for (size_t i = 2; i < tokenizedInformation.size(); ++i) {
            course.prerequisites.push_back(tokenizedInformation[i]);
        }

        courses.push_back(course);
        courseMap[course.courseNumber] = course;
    }

    for (auto& course : courses) {
        for (auto& prereq : course.prerequisites) {
            if (courseMap.find(prereq) != courseMap.end()) {
                courseMap[course.courseNumber].prerequisites.push_back(courseMap[prereq].name);
            }
        }
    }

    fin.close();
    return courses;
}

void printCourseList(const vector<Course>& courses) {
    vector<Course> sortedCourses = courses;
    sort(sortedCourses.begin(), sortedCourses.end(),
        [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    for (const auto& course : sortedCourses) {
        cout << course.courseNumber << ", " << course.name << endl;
    }
}

void searchCourse(const vector<Course>& courses) {
    string courseNumber;
    cout << "Enter the course number: ";
    cin >> courseNumber;

    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    bool found = false;
    for (const auto& course : courses) {
        string upperCaseCourseNumber = course.courseNumber;
        transform(upperCaseCourseNumber.begin(), upperCaseCourseNumber.end(), upperCaseCourseNumber.begin(), ::toupper);

        if (upperCaseCourseNumber == courseNumber) {
            found = true;
            cout << course.courseNumber << ", " << course.name << endl;
            cout << "Prerequisites: ";
            for (const auto& prereq : course.prerequisites) {
                cout << prereq << " ";
            }
            cout << "\n";
            break;
        }
    }

    if (!found) {
        cout << "Course with the given course number not found.\n";
    }
}

int main() {
    cout << "Project Two CS-300 Course Planner." << endl;

    vector<Course> courses;

    int choice;
    do {
        cout << "1. Load Data Structure.\n"
            << "2. Print Course List.\n"
            << "3. Print Course.\n"
            << "9. Exit\n"
            << "What would you like to do? ";

        cin >> choice;

        switch (choice) {
        case 1: {
            string fileName;
            cout << "Enter the file name: ";
            cin >> fileName;
            courses = loadDataStructure(fileName);
            break;
        }
        case 2:
            cout << "All Courses Sorted: " << endl;
            printCourseList(courses);
            break;
        case 3:
            searchCourse(courses);
            break;
        case 9:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << choice << " is not a valid input, please enter a number from 1 - 3, or 9 to exit the program." << endl;
        }
    } while (choice != 9);

    return 0;
}