#include <iostream>
#include <vector>

using namespace std;

// Function to display the contents of the vector
void displayGrades(const vector<int>& grades) {
    if (grades.empty()) {
        cout << "No grades available." << endl;
    } else {
        cout << "Student Grades: ";
        for (size_t i = 0; i < grades.size(); ++i) {
            cout << grades[i] << " ";
        }
        cout << endl;
    }
}

// Function to add a grade
void addGrade(vector<int>& grades) {
    int grade;
    cout << "Enter the grade to add: ";
    cin >> grade;
    grades.push_back(grade);
    cout << "Grade added successfully!" << endl;
}

// Function to update a grade
void updateGrade(vector<int>& grades) {
    int index, newGrade;
    cout << "Enter the index of the grade to update: ";
    cin >> index;
    if (index >= 0 && index < grades.size()) {
        cout << "Enter the new grade: ";
        cin >> newGrade;
        grades[index] = newGrade;
        cout << "Grade updated successfully!" << endl;
    } else {
        cout << "Invalid index!" << endl;
    }
}

// Function to delete a grade
void deleteGrade(vector<int>& grades) {
    int index;
    cout << "Enter the index of the grade to delete: ";
    cin >> index;
    if (index >= 0 && index < grades.size()) {
        grades.erase(grades.begin() + index);
        cout << "Grade deleted successfully!" << endl;
    } else {
        cout << "Invalid index!" << endl;
    }
}

// Function to display menu options
void displayMenu() {
    cout << "\nMenu:" << endl;
    cout << "1. Add Grade" << endl;
    cout << "2. Display Grades" << endl;
    cout << "3. Update Grade" << endl;
    cout << "4. Delete Grade" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    vector<int> grades; // Vector to store grades
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addGrade(grades);
                break;
            case 2:
                displayGrades(grades);
                break;
            case 3:
                updateGrade(grades);
                break;
            case 4:
                deleteGrade(grades);
                break;
            case 5:
                cout << "Exiting program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
