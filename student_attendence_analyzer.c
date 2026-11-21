#include <stdio.h>
#include <ctype.h>

#define N 10

struct Student
{
    int roll;
    char name[30];
    int present;
    char attendance[50];      // 'P' or 'A' for each class
};
struct Student s[N];
int total = 0;                // classes held so far

float percent(int i)
{
    return s[i].present * 100.0 / total;
}
int main()
{
    int choice, i, r;
    char def, other;
    float threshold = 75;
    FILE *fp;
    // Open the file containing student details
    fp = fopen("students.txt", "r");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return 0;
    }
    // Read student details from the file
    for (i = 0; i < N; i++)
    {
        fscanf(fp, "%d %29[^\n]", &s[i].roll, s[i].name);
        s[i].present = 0;
    }
    fclose(fp);
    printf("Student data loaded successfully!\n");

    do
    {
        printf("\n1. Mark Attendance");
        printf("\n2. View Report");
        printf("\n3. Find Defaulters");
        printf("\n4. Change Threshold");
        printf("\n5. Exit");

        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            break;
        }

        if (choice == 1)
        {
            if (total >= 50)
            {
                printf("Maximum classes reached!\n");
                continue;
            }

            // Display students
            for (i = 0; i < N; i++)
            {
                printf("%d %s\n", s[i].roll, s[i].name);
            }

            // Choose default attendance
            do
            {
                printf("Mark everyone as (P)resent or (A)bsent by default? ");

                if (scanf(" %c", &def) != 1)
                {
                    return 0;
                }

                def = toupper(def);

            } while (def != 'P' && def != 'A');

            // Find the opposite attendance
            other = (def == 'P') ? 'A' : 'P';
            // Give everyone default attendance
            for (i = 0; i < N; i++)
            {
                s[i].attendance[total] = def;
            }
            printf("Enter roll numbers of students who were %s (0 when done): ",
                   other == 'A' ? "ABSENT" : "PRESENT");

            while (scanf("%d", &r) == 1 && r != 0)
            {
                int found = 0;

                for (i = 0; i < N; i++)
                {
                    if (s[i].roll == r)
                    {
                        s[i].attendance[total] = other;
                        found = 1;
                        break;
                    }
                }
                if (found == 0)
                {
                    printf("Invalid roll number!\n");
                }
            }
            // Count present students
            for (i = 0; i < N; i++)
            {
                if (s[i].attendance[total] == 'P')
                {
                    s[i].present++;
                }
            }
            total++;
        }
        else if (choice == 2 || choice == 3)
        {
            if (total == 0)
            {
                printf("No attendance recorded!\n");
                continue;
            }
            if (choice == 3)
            {
                printf("\nStudents below %.0f%%:", threshold);
            }
            printf("\nRoll  Name        Present  Percentage\n");
			
            for (i = 0; i < N; i++)
            {
                if (choice == 2 || percent(i) < threshold)
                {
                    printf("%-5d %-11s %-8d %.2f%%\n",
                           s[i].roll,
                           s[i].name,
                           s[i].present,
                           percent(i));
                }
            }
        }
        else if (choice == 4)
        {
            printf("Enter new threshold (current %.0f%%): ", threshold);
            scanf("%f", &threshold);
        }
    } while (choice != 5);
    return 0;
}