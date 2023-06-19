# COMP1005 2022 Coursework 04

## Introduction

This coursework focuses on writing programs involving file input/output and string processing. There are **three tasks** to complete, details of which are given below. Links to external pages are provided in the details below which you will need to follow and read to complete each task. Information which relates to all task is given in the [Background](#background) section below and should be read before attempting the tasks.

## Assessment

This coursework is worth **10%** of your final course grade. The points awarded for each task are as follows:

|            | Compilation | Implementation | Execution | **Total** |
| ---------- | :---------: | :------------: | :-------: | :-------: |
| **Task 1** | 2           | 3              | 20        | **25**    |
| **Task 2** | 2           | 3              | 20        | **25**    |
| **Task 3** | 2           | 3              | 20        | **25**    |

For each task, you are awarded *two* points if your program compiles correctly: *one* point if it compiles without errors, and a further *one* point if it compiles without errors *and* warnings. If your program follows the implementation instructions given in the task details below, you are awarded *three* points. If your program produces the correct output as described in the task details below, you are awarded *ten* points. If your program handles invalid input correctly as described in task details below, you are awarded *ten* points. Only programs which compile without errors or warnings and pass all the implementation checks will be awarded the points for execution.

Your score for each task can be viewed on GitLab after every push to `projects.cs.nott.ac.uk`. You can make as many revisions and pushes as you like, hopefully increasing your score as you do so. Your final score for the coursework will the score of your last push.

The contribution towards your final course grade is calculated as follows:

```maths
	floor(10 * (task1_points_awarded + task2_points_awarded + task3_points_awarded) / 75)
```

Your provisional score for each task can be viewed on GitLab after every push to `projects.cs.nott.ac.uk`. Your final score will be based on the code in the last commit pushed to `projects.cs.nott.ac.uk` *before* the coursework deadline. Commits pushed after the coursework deadline will be disregarded. After the coursework deadline, your code will undergo further review and, based on this review, your provisional score may go up or down. This further review will include checks for code plagiarism and for trivial implementations e.g. implementations just containing an empty main function or clearly not written following the task guidelines. Final scores will be published on the COMP1005 moodle pages around a week after the coursework deadline.

Your repository contains a file called `.gitlab-ci.yml`. This file is used during the assessment process and *must not* be removed or edited in any way. Any tampering with this file will result in a score of zero for this coursework.

This coursework is individual work i.e. *must be your own work* and follow the University [Academic integrity and misconduct](https://www.nottingham.ac.uk/studyingeffectively/studying/integrity/index.aspx) guidance.

## Background

Tabular data is often stored in a **comma-separated value** (**CSV**) file. A CSV file is a text file which is formatted in a specific way (more details [here](https://en.wikipedia.org/wiki/Comma-separated_values)). The first line of a CSV file is often used as a header to give a label to the values stored in following lines. For all the tasks in this coursework, you can assume that the first line of all CSV files is a header line, and that all subsequent lines in the CSV file contain the same number of fields as given in the header line.

For the tasks in this coursework, you will be writing programs that process the grades of (fictitious) students in a class. For each class, there is a *student CSV file* which lists the ID number, last name and first name of all the students in the class. Example student CSV files for three classes are provided in this repository: `class01_students.csv`, `class02_students.csv`, `class03_students.csv`. Class 01 is a small class, which will be useful for initial testing of your programs. Classes 02 and 03 are larger classes of a similar size to the classes your programs will be tested on in the GitLab assessment pipeline and will be useful for further testing of your programs.

Each class has taken a number of activities which they have been graded on. The grades for each activity are stored in *activity CSV files*, which list the student ID number and grade for a particular activity. For example, the grades for class 01 for two activities are given in the `class01_activity01.csv` and `class01_activity02.csv` files. Grades in the activity CSV file are integer percentages. Some students in a class were absent for a particular activity and have no grade recorded in the activity CSV file. The grade for these students is 0.

## Task 1

### Overview

Your task is to write a program to read in student grades from a file and calculate statistics for these grades.

Your C source file for this task *must* be named `task1.c` and placed in the top-level directory of your git project for this coursework.

### Details

Your program should read in a student CSV file and an activity CSV file and output the following statistics in the format detailed below:

* total number of students
* number of students absent from the activity
* [arithmetic mean](https://en.wikipedia.org/wiki/Mean) grade for activity
* [population standard deviation](https://en.wikipedia.org/wiki/Standard_deviation) of grades for activity

You are free to choose whichever method you want to implement your program with the following restrictions:

* your implementation must contain a correctly declared `main` function
* you are not allowed to use global variables
* all dynamically allocated memory should be freed
* all user file handles should be closed

You can use any standard library functions (including those in the C maths library). If using the maths library you should remember to link to the maths library when compiling your program:

```bash
$ gcc -Wall -ansi -pedantic-errors -o task1 task1.c -lm
```

Your program should take two command line arguments: the filename of the student CSV file and filename of the activity CSV file respectively. It should output the statistics detailed above in the format shown in the example below. All real numbers should be output to two decimal places. For example:

```bash
$ ./task1 class01_students.csv class01_activity01.csv
total students = 10
absent students = 2
grade mean = 44.00
grade sd = 31.77
```

Further examples of correct output to help you test your program are given in the `task1.example.log` file. Your program will be tested against a number of randomly generated classes in the GitLab assessment pipeline. Your program will also be tested against a number of invalid cases e.g. an incorrect number of command line arguments, or incorrect numbers of columns in the student or activity CSV files. In these cases, an error message should be printed and your program should immediately exit with a non-zero exit code. All error messages in your implementation should be printed to `stderr`.

There are many ways to implement your program. I chose to use the following approach:

* parse and sanity check the command line arguments:
	* sanity check the user has provided two command line arguments.
	* sanity check the CSV files passed as command line arguments are readable files (using the `access` standard library function).
* count the number of lines in the student CSV file. I wrote a `count_lines` function to do this. The total number of students is one less than this number.
* count the number of lines in the activity CSV file (using the `count_lines` function again).
* dynamically allocate an integer array to store the grades in the activity CSV file (of size one less than the number of lines in the activity CSV file).
* loop through and read in each line (ignoring the header line) in the activity CSV file (using `fgets`):
	* for each line remove the line feed and carriage return (using `strcspn`).
	* find the grade in the second field of each line (using `strtok`).
	* convert the grade to an integer (I used the `convert_integer` function I wrote in previous courseworks) and store it in my dynamically allocated grade array.
* close the activity CSV file
* sum all the elements in the grade array and divide by the total number of students to calculate the mean grade.
* loop through the grade array again to calculate the standard deviation (using the `sqrt` function in the calculation).
* print out the statistics.
* free the grade array.

Your program should handle dynamically allocated memory correctly i.e. free all dynamically allocated memory. It should also make sure all user file handles are closed before it exits. As discussed in lectures, a good tool for assessing if a program has handled dynamic memory allocation correctly is `valgrind`. Remember to compile your code with the `-g` flag to show line numbers in the `valgrind` output. To check your program using `valgrind` you can type:

```bash
$ valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./task1 class01_students.csv class01_activity01.csv
```

If your program has correctly handled dynamic memory allocation, the last line of output should read:

```
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

If you have closed all user file handles, you should see a line reading:

```
FILE DESCRIPTORS: 3 open at exit.
```

i.e. only the 3 standard streams are open at exit.

## Task 2

### Overview

Your task is to write a program to read in student names and grades from separate files, and print out the students names and grades.

Your C source file for this task *must* be named `task2.c` and placed in the top-level directory of your git project for this coursework.

### Details

Your program should read in a student CSV file and an activity CSV file and output the student first name, last name and grade in the format specified below.

You are free to choose whichever method you want to implement your program with the following restrictions:

* your implementation must contain a correctly declared `main` function
* you are not allowed to use global variables
* all dynamically allocated memory should be freed
* all user file handles should be closed

You can use any standard library functions (including those in the C maths library).

Your program should take two command line arguments: the filename of the student CSV file and filename of the activity CSV file respectively. It should output the student first name, last name and grade. For example:

```bash
$ ./task2 class01_students.csv class01_activity01.csv
Sherri Hurrington 30
William Rich 0
Erin Simmons 16
Antonio Levi 75
Mabel Henderson 41
Paula Coffey 62
Tom Green 56
Hazel Willis 70
Marty Osterberg 0
Nancy Winders 90
```

```bash
$ ./task2 class01_students.csv class01_activity02.csv
Sherri Hurrington 44
William Rich 0
Erin Simmons 75
Antonio Levi 97
Mabel Henderson 29
Paula Coffey 89
Tom Green 62
Hazel Willis 90
Marty Osterberg 13
Nancy Winders 34

```

All students should be output in the same order as they appear in the student CSV file. If a student was absent for an activity, they should be awarded a grade of 0.

Your program will be tested against a number of randomly generated classes in the GitLab assessment pipeline. Your program will also be tested against a number of invalid cases e.g. an incorrect number of command line arguments, or incorrect numbers of columns in the student or activity CSV files. In these cases, an error message should be printed and your program should immediately exit with a non-zero exit code. All error messages in your implementation should be printed to `stderr`.

There are many ways to implement your program. I chose to use the following approach:

* parse and sanity check the command line arguments:
	* sanity check the user has provided two command line arguments.
	* sanity check the CSV files passed as command line arguments are readable files (using `access` standard library function).
* read in the student CSV file in to a dynamically allocated two dimensional array of strings. Each row in this array corresponds to a single student, and each column to the fields in the student CSV file. I wrote a `read_csv` function to do this, which returns a pointer to the dynamically allocated array, and the number of rows and columns in the array. I called the `count_lines` function I wrote in Task 1 from my `read_csv` function.
* sanity check that the student array contains the correct number of columns.
* read in the activity CSV file in to a dynamically allocated two dimensional array of string (using the `read_csv` function again).
* sanity check that the activity array contains the correct number of columns.
* loop though the student array:
	* for each student in the student array get their ID number from the student array.
	* loop though the grade array and find the grade associated with the ID number.
	* if no grade is found set the grade to 0.
	* print out the student first name, last name and grade.
* free the memory for the activity and student arrays.

Your program should handle dynamically allocated memory correctly i.e. free all dynamically allocated memory. It should also make sure all user file handles are closed before it exits. You should use `valgrind` to help you debug your program for memory leaks.

## Task 3

### Overview

Your task is to write a program to read in student names and grades from separate files, and write a third file containing a summary of student grades.

Your C source file for this task *must* be named `task3.c` and placed in the top-level directory of your git project for this coursework.

### Details

Your program should read in a student or results CSV file, an activity CSV file, and output a third results CSV file containing a summary of the grades for each student. Your program should take three command line arguments: the filename of the student CSV file or a previously generated results CSV file, the filename of the activity CSV file, and the filename of the results CSV to write to respectively.

The first line of results CSV file should contain a header with the following format:

```
id,last_name,first_name,average,grade01,grade02,...
```

And then a line for each student (in the same order as they appear in the student CSV file) giving their ID, last name, first name, arithmetic mean of activity grades (to two decimal places), and the grades for each activity.

If your program is run with a student CSV file as its first argument, the results CSV file will just contain the following columns:

```
id,last_name,first_name,average,grade01
```

If your program is run with a results CSV file as its first argument, an extra column should be created listing the grades in the activity CSV file, and the average column updated to reflect the average grade across all previously recorded activities.

For example, if the following command is run:

```bash
$ ./task3 class01_students.csv class01_activity01.csv class01_results01.csv
```

The contents of the `class01_results01.csv` file will be:

```
id,last_name,first_name,average,grade01
3444670,Hurrington,Sherri,30.00,30
3611390,Rich,William,0.00,0
2939734,Simmons,Erin,16.00,16
288301,Levi,Antonio,75.00,75
1295620,Henderson,Mabel,41.00,41
757790,Coffey,Paula,62.00,62
3149175,Green,Tom,56.00,56
3173019,Willis,Hazel,70.00,70
2890792,Osterberg,Marty,0.00,0
1274266,Winders,Nancy,90.00,90
```

Then, if the following command is run:

```bash
$ ./task3 class01_results01.csv class01_activity02.csv class01_results02.csv
```

The contents of the `class01_results02.csv` file will be:

```
id,last_name,first_name,average,grade01,grade02
3444670,Hurrington,Sherri,37.00,30,44
3611390,Rich,William,0.00,0,0
2939734,Simmons,Erin,45.50,16,75
288301,Levi,Antonio,86.00,75,97
1295620,Henderson,Mabel,35.00,41,29
757790,Coffey,Paula,75.50,62,89
3149175,Green,Tom,59.00,56,62
3173019,Willis,Hazel,80.00,70,90
2890792,Osterberg,Marty,6.50,0,13
1274266,Winders,Nancy,62.00,90,34
```

You are free to choose whichever method you want to implement your program with the following restrictions:

* your implementation must contain a correctly declared `main` function
* you are not allowed to use global variables
* all dynamically allocated memory should be freed
* all user file handles should be closed

You can use any standard library functions (including those in the C maths library). You will probably want to reuse functions you have written (such as `read_csv`) in the previous two tasks.

Your program will be tested against a number of randomly generated classes in the GitLab assessment pipeline. Your program will also be tested against a number of invalid cases e.g. an incorrect number of command line arguments, or incorrect numbers of columns in the student or activity CSV files. In these cases, an error message should be printed and your program should immediately exit with a non-zero exit code. All error messages in your implementation should be printed to `stderr`.

Your program should handle dynamically allocated memory correctly i.e. free all dynamically allocated memory. It should also make sure all user file handles are closed before it exits. You should use `valgrind` to help you debug your program for memory leaks.