#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LEVELS 12
#define NUM_CLASSES 10
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 11

typedef struct student {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    int grades[10];
    struct student* next;
} Student;

Student* create_student(char* first_name, char* last_name, char* phone, int* grades) {
    Student* new_student = (Student*)malloc(sizeof(Student));
    if (new_student != NULL) {
        strncpy(new_student->first_name, first_name, MAX_NAME_LENGTH - 1);
        new_student->first_name[MAX_NAME_LENGTH-1]='\0';
        strncpy(new_student->last_name, last_name, MAX_NAME_LENGTH - 1);
        new_student->last_name[MAX_NAME_LENGTH-1]='\0';
        strncpy(new_student->phone, phone, MAX_PHONE_LENGTH - 1);
        new_student->phone[MAX_PHONE_LENGTH-1]='\0';
        for (int i = 0; i < 10; i++) {
            new_student->grades[i] = grades[i];
        }
        new_student->next = NULL;
    }
    return new_student;
}

void add_student_to_class(Student** class, Student* new_student) {
    if (*class == NULL) {
        *class = new_student;
    } else {
        Student* current = *class;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_student;
    }
}

void print_student(Student* student) {
    printf("Name: %s %s, Phone: %s, Grades: ", student->first_name, student->last_name, student->phone);
    for (int i = 0; i < 10; i++) {
        printf("%d ", student->grades[i]);
    }
    printf("\n");
}
void insertNewStudent(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    int level, class_num;
    int grades[10];

    printf("\n| Insert New Student |\n");
    printf("---------------------------------------------------\n");

    // Get the student's information from the user
    printf("Enter First Name: ");
    fgets(first_name, MAX_NAME_LENGTH, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Last Name: ");
    fgets(last_name, MAX_NAME_LENGTH, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Phone Number (max %d characters): ", MAX_PHONE_LENGTH - 1);
    fgets(phone, MAX_PHONE_LENGTH, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Level (1-12): ");
    scanf("%d", &level);
    fflush(stdin);

    printf("Enter Class Number (1-10): ");
    scanf("%d", &class_num);
    fflush(stdin);

    printf("Enter Grades for 10 Courses:\n");
    for (int i = 0; i < 10; i++) {
        printf("Course %d: ", i + 1);
        scanf("%d", &grades[i]);
        fflush(stdin);
    }

    // Create a new student object
    Student* new_student = create_student(first_name, last_name, phone, grades);

    if (new_student != NULL && level > 0 && level <= NUM_LEVELS && class_num > 0 && class_num <= NUM_CLASSES) {
        // Add the new student to the appropriate class in the school data structure
        add_student_to_class(&school[level - 1][class_num - 1], new_student);
        printf("\nStudent %s %s added successfully to Level %d, Class %d.\n", first_name, last_name, level, class_num);
    } else {
        printf("\nError: Failed to add the student. Please check the input.\n");
    }
}
void readStudentsFromFile(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    FILE* fp;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    int level, class_num;
    int grades[10];

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s %d %d", first_name, last_name, phone, &level, &class_num) == 5) {
        for (int i = 0; i < 10; i++) {
            fscanf(fp, "%d", &grades[i]);
        }

        if (level > 0 && level <= NUM_LEVELS && class_num > 0 && class_num <= NUM_CLASSES) {
            Student* new_student = create_student(first_name, last_name, phone, grades);
            if (new_student != NULL) {
                add_student_to_class(&school[level - 1][class_num - 1], new_student);
            }
        }
    }

    fclose(fp);
}

void searchStudent(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];

    printf("\n| Search Student |\n");
    printf("----------------------------------------\n");

    // Get the student's first name from the user
    printf("Enter First Name: ");
    fgets(first_name, MAX_NAME_LENGTH, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove trailing newline

    // Get the student's last name from the user
    printf("Enter Last Name: ");
    fgets(last_name, MAX_NAME_LENGTH, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove trailing newline

    // Search for the student in the 2D school array
    int found = 0;
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            Student* current = school[level][class_num];
            while (current != NULL) {
                if (strcmp(current->first_name, first_name) == 0 && strcmp(current->last_name, last_name) == 0) {
                    // Found the student, display all information about them
                    printf("\nStudent Information:\n");
                    print_student(current); // Utilize the existing print_student function
                    found = 1;
                    break; // Exit the loop after displaying the student's information
                }
                current = current->next;
            }
            if (found) {
                break; // Exit the outer loop as well, if the student is found
            }
        }
        if (found) {
            break; // Exit the search entirely if the student is found
        }
    }

    if (!found) {
        printf("\nStudent %s %s not found in the school.\n", first_name, last_name);
    }
}

void printTopTenStudentsPerCourse(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    int course_number;
    printf("\n| Top 10 Students for Course |\n");
    printf("----------------------------------------\n");

    // Get the course number from the user
    printf("Enter the Course Number (1-10): ");
    scanf("%d", &course_number);
    fflush(stdin);

    if (course_number < 1 || course_number > 10) {
        printf("\nError: Invalid Course Number. Please enter a number between 1 and 10.\n");
        return;
    }

    // Initialize an array to store the top 10 students for each level
    Student* top_grades_per_level[NUM_LEVELS][10] = { NULL };

    // Iterate through every level and class
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            Student* current = school[level][class_num];
            // Go through all students in the current class
            while (current != NULL) {
                // Update the top_grades_per_level array with the top 10 students
                for (int i = 0; i < 10; i++) {
                    if (top_grades_per_level[level][i] == NULL || 
                        current->grades[course_number - 1] > top_grades_per_level[level][i]->grades[course_number - 1]) {
                        // Shift the students to make room for the new top student
                        for (int j = 9; j > i; j--) {
                            top_grades_per_level[level][j] = top_grades_per_level[level][j - 1];
                        }
                        top_grades_per_level[level][i] = current;
                        break;
                    }
                }
                current = current->next;
            }
        }
    }

    // Print the top 10 students for each level and course
    for (int level = 0; level < NUM_LEVELS; level++) {
        printf("\nTop 10 Students in Level %d for Course %d:\n", level + 1, course_number);
        for (int i = 0; i < 10; i++) {
            if (top_grades_per_level[level][i] != NULL) {
                printf("Student: %s %s (Grade: %d)\n", top_grades_per_level[level][i]->first_name,
                    top_grades_per_level[level][i]->last_name, top_grades_per_level[level][i]->grades[course_number - 1]);
            }
        }
    }
}

void deleteStudent(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    int level, class_num;

    printf("\n| Delete Student |\n");
    printf("----------------------------------------\n");

    // Get the student's information from the user
    printf("Enter First Name: ");
    fgets(first_name, MAX_NAME_LENGTH, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Last Name: ");
    fgets(last_name, MAX_NAME_LENGTH, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Level (1-12): ");
    scanf("%d", &level);
    fflush(stdin);

    printf("Enter Class Number (1-10): ");
    scanf("%d", &class_num);
    fflush(stdin);

    // Search for the student in the specified class
    if (level > 0 && level <= NUM_LEVELS && class_num > 0 && class_num <= NUM_CLASSES) {
        Student* current = school[level - 1][class_num - 1];
        Student* prev = NULL;
        while (current != NULL) {
            if (strcmp(current->first_name, first_name) == 0 && strcmp(current->last_name, last_name) == 0) {
                // Found the student, remove them from the class
                if (prev == NULL) {
                    // If the student to be deleted is the first node in the class
                    school[level - 1][class_num - 1] = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current); // Free the memory for the deleted student
                printf("\nStudent %s %s has been deleted from Level %d, Class %d.\n", first_name, last_name, level, class_num);
                return; // Exit the function after deleting the student
            }
            prev = current;
            current = current->next;
        }
        printf("\nStudent %s %s not found in Level %d, Class %d.\n", first_name, last_name, level, class_num);
    } 
    else {
        printf("\nError: Invalid Level or Class Number. Please enter valid values.\n");
    }
}

void editStudentGrade(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    int level, class_num;

    printf("\n| Edit Student Grade |\n");
    printf("----------------------------------------\n");

    // Get the student's information from the user
    printf("Enter First Name: ");
    fgets(first_name, MAX_NAME_LENGTH, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Last Name: ");
    fgets(last_name, MAX_NAME_LENGTH, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Level (1-12): ");
    scanf("%d", &level);
    fflush(stdin);

    printf("Enter Class Number (1-10): ");
    scanf("%d", &class_num);
    fflush(stdin);

    // Search for the student in the specified class
    if (level > 0 && level <= NUM_LEVELS && class_num > 0 && class_num <= NUM_CLASSES) {
        Student* current = school[level - 1][class_num - 1];
        while (current != NULL) {
            if (strcmp(current->first_name, first_name) == 0 && strcmp(current->last_name, last_name) == 0) {
                // Found the student, edit their grades
                printf("Enter New Grades for 10 Courses:\n");
                for (int i = 0; i < 10; i++) {
                    printf("New Grade for Course %d: ", i + 1);
                    scanf("%d", &current->grades[i]);
                    fflush(stdin);
                }
                printf("\nStudent %s %s grades have been updated.\n", first_name, last_name);
                return; // Exit the function after editing the student's grades
            }
            current = current->next;
        }
        printf("\nStudent %s %s not found in Level %d, Class %d.\n", first_name, last_name, level, class_num);
    } else {
        printf("\nError: Invalid Level or Class Number. Please enter valid values.\n");
    }
}


void print_class(Student* class) {
    Student* current = class;
    while (current != NULL) {
        print_student(current);
        current = current->next;
    }
}


void printAllStudents(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    printf("\n| All Students |\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < NUM_LEVELS; i++) {
        for (int j = 0; j < NUM_CLASSES; j++) {
            printf("Level %d, Class %d:\n", i + 1, j + 1);
            print_class(school[i][j]);
            printf("\n");
        }
    }
}

enum menu_inputs {
    Insert = '0',
    Delete = '1',
    Edit = '2',
    Search = '3',
    Showall = '4',
    Top10 = '5',
    UnderperformedStudents = '6',
    Average = '7',
    Export = '8',
    Exit = '9'
};

void menu(Student* school[NUM_LEVELS][NUM_CLASSES]) {
    char input;
    // school.name = "schoolName";
    do {
        printf("\n|School Manager<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        //printf("Welcome to ( %s ) School!\nYou have inserted ( %zu ) students.\n\n", school.name, school.numOfStudents);
        printf("\t[0] |--> Insert\n");
        printf("\t[1] |--> Delete\n");
        printf("\t[2] |--> Edit\n");
        printf("\t[3] |--> Search\n");
        printf("\t[4] |--> Show All\n");
        printf("\t[5] |--> Top 10 students per course\n");
        printf("\t[6] |--> Underperformed students\n");
        printf("\t[7] |--> Average per course\n");
        printf("\t[8] |--> Export\n");
        printf("\t[9] |--> Exit\n");
        printf("\tPlease Enter Your Choice (0-9):");
        input = getc(stdin);
        fflush(stdin);
        //getc(stdin);
        getchar(); // Consume the newline character
        switch (input) {
            case Insert:
                insertNewStudent(school);
                break;
            case Delete:
                deleteStudent(school);
                break;
            case Edit:
                editStudentGrade(school);
                break;
            case Search:
                searchStudent(school);
                break;
            case Showall:
                printAllStudents(school);
                break;
            case Top10:
                printTopTenStudentsPerCourse(school);
                break;
            case UnderperformedStudents:
                //printUnderperformedStudents();
                break;
            case Average:
                //printAverage();
                break;
            case Export:
                //exportDatabase();
                break;
            case Exit:
                //handleClosing();
                break;
            default:
                printf("\nThere is no item with symbol\"%c\".Please enter a number between 1-10!\n",input);
                getc(stdin);
                fflush(stdin);
                //getc(stdin);
                break;
        }
    } while (input != Exit);
}


int main() {
    FILE* fp;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    int level, class_num;
    int grades[10];

    Student* school[NUM_LEVELS][NUM_CLASSES] = { NULL };

    readStudentsFromFile(school);
   
    menu(school);

    // Printing the data structure for each level and class
    //printAllStudents(school); 

    // Freeing the memory allocated for students
    for (int i = 0; i < NUM_LEVELS; i++) {
        for (int j = 0; j < NUM_CLASSES; j++) {
            Student* current = school[i][j];
            while (current != NULL) {
                Student* temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    printf("Memory for all students has been freed.\n");


    return 0;
}
