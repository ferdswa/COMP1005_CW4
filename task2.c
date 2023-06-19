#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

void countLines(FILE*,int*,int*);
char* readCSV(char *);
struct studentInfo{
    char name[64];char surName[64];char id[64];/*put these the wrong way round by accident too late*/
};
struct gradeInfo{
    char id[64];int score;
};
/*I am reusing quite a bit of my task 1 code for this as the sanity is basically the same*/
int main(int argc, char **argv)
{
    int preint1,preint2,stCount,acCount,i,j;
    int commas1, commas2,ensure1,ensure2,currentGrade;
    int tempGrade;
    struct studentInfo *studentData;
    struct gradeInfo *gradeData;
    FILE *students;
    FILE *activity;
    char str[64],*cv=",",*split,*ptr;
    char* currentID,*cmpID;
    /*Sanity*/
    if(argc!=3)
    {
        fprintf(stderr,"You have entered an invalid number of arguments. There should be 3.\n");
        return -1;
    }
    preint1=access(argv[1],F_OK);
    preint2=access(argv[2],F_OK);
    if((preint1==0)&&(preint2==0))
    {
        /*opening the files, counting lines and counting commas to make sure corrent column numbers*/
        students = fopen(argv[1],"r");
        activity = fopen(argv[2],"r");
        countLines(students,&stCount,&commas1);/* # of student commas == 2* # ofstudents */
        countLines(activity,&acCount,&commas2);/* # of activity commas == # of activities */
        fclose(students);
        fclose(activity);
        ensure1=commas1-stCount;
        ensure2=commas2-acCount;
        if((ensure1!=stCount)||(ensure2!=0))
        {
            fprintf(stderr,"There is something wrong with the CSV files you tried\n");
            return -1;
        }
        /*reopen files to reset pointers*/
        students = fopen(argv[1],"r");
        activity = fopen(argv[2],"r");
        acCount-=1;
        stCount-=1;
        studentData=calloc(stCount*128,_SC_CHAR_MAX);
        gradeData=calloc(stCount*128,_SC_CHAR_MAX);
        fgets(str,64,students);
        fgets(str,64,activity);
        for(i=0;i<stCount;i++)
        {
            fgets(str,64,students);
            str[strlen(str)-1]='\0';
            split = strtok(str,cv);
            for(j=0;j<strlen(split);j++)
            {
                studentData[i].id[j]=split[j];
            }
            split = strtok(NULL,cv);
            for(j=0;j<strlen(split);j++)
            {
                studentData[i].name[j]=split[j];
            }
            split = strtok(NULL,cv);
            for(j=0;j<strlen(split);j++)
            {
                studentData[i].surName[j]=split[j];
            }
        }
        for(i=0;i<acCount;i++)
        {
            fgets(str,64,activity);
            str[strlen(str)-1]='\0';
            split = strtok(str,cv);
            for(j=0;j<strlen(split);j++)
            {
                gradeData[i].id[j]=split[j];
            }
            split = strtok(NULL,cv);
            currentGrade = strtol(split,&ptr,10);
            for(j=0;j<strlen(split);j++)
            {
                gradeData[i].score=currentGrade;
            }
        }
        for(i=0;i<stCount;i++)
        {
            /*Surnames are stored in .name and names are stored in .surName*/
            /*Calculating where to put zeroes*/
            printf("%s %s ",studentData[i].surName,studentData[i].name);
            tempGrade=0;
            currentID=studentData[i].id;
            for(j=0;j<acCount;j++)
            {
                cmpID=gradeData[j].id;
                if(strcmp(currentID,cmpID)==0)
                {
                    tempGrade=gradeData[j].score;
                }
            }
            printf("%d\n",tempGrade);
        }
        free(studentData);
        free(gradeData);
        fclose(students);
        fclose(activity);
        return 0;
    }
    else{
        fprintf(stderr,"You have entered at least one file name that doesnt exist! Please check and try again\n");
        return -1;
    }
 
}
void countLines(FILE *currentFile,int *lines,int *commas)
{
    int l=0;
    int co=0;
    char c;
    do
    {
        c=fgetc(currentFile);
        if(c=='\n')
            ++l;
        else if(c==',')
            co++;
    } while (!feof(currentFile));
    *lines=l;
    *commas=co;
}