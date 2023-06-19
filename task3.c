#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

void countLines(FILE*,int*,int*);
int countColumns(FILE*);
struct studentInfo{
    char name[64];char surName[64];char id[64];/*put these the wrong way round by accident too late*/
};
struct gradeInfo{
    char id[64];int score[8];
};
struct newFile{
    char name[64];char surName[64];char id[64];int score[8];double avg;
};
/*Again sanity is similar*/
int main(int argc, char **argv)
{
    int preint1,preint2,stCount,acCount,i,j,k,sum,count;
    int commas1, commas2,ensure1,ensure2,currentGrade;
    int tempGrade, columnCount,difference;
    double avg,alpha,beta;
    struct studentInfo *studentData;
    struct gradeInfo *gradeData;
    struct newFile *newData;
    FILE *students;
    FILE *activity;
    FILE *target;/*results csv*/
    char str[128],*cv=",",*split,*ptr;
    char* currentID,*cmpID,*targetFN,*convertedS,*strToWrite,*tmpstring;
    /*Sanity*/
    if(argc!=4)
    {
        fprintf(stderr,"You have entered an invalid number of arguments. There should be 3.\n");
        return -1;
    }
    preint1=access(argv[1],F_OK);
    preint2=access(argv[2],F_OK);
    targetFN=argv[3];
    if((preint1==0)&&(preint2==0))
    {
        /*opening the files, counting lines and counting commas to make sure corrent column numbers*/
        students = fopen(argv[1],"r");
        activity = fopen(argv[2],"r");
        countLines(students,&stCount,&commas1);/* # of student commas == 2* # ofstudents */
        countLines(activity,&acCount,&commas2);/* # of activity commas == # of activities */
        fclose(students);
        fclose(activity);
        students = fopen(argv[1],"r");
        columnCount=countColumns(students);
        fclose(students);
        ensure1=commas1-stCount;
        ensure2=commas2-acCount;
        if(columnCount<4){/*check corrent student file if not results file*/
            if((ensure1!=stCount)||(ensure2!=0))
            {
                fprintf(stderr,"There is something wrong with the CSV files you tried\n");
                return -1;
            }
            difference=0;
        }
        else{
            difference=columnCount-3-1;
        }
        /*reopen files to reset pointers*/
        students = fopen(argv[1],"r");
        activity = fopen(argv[2],"r");
        target=fopen(targetFN,"w"); 
        acCount-=1;
        stCount-=1;
        studentData=calloc(stCount*128,_SC_CHAR_MAX);
        gradeData=calloc(stCount*128,_SC_CHAR_MAX);
        newData=calloc(stCount*128,_SC_CHAR_MAX);
        convertedS=calloc(64,sizeof(char));
        tmpstring=calloc(4,sizeof(char));
        strToWrite=calloc(256,sizeof(char));
        fgets(str,128,students);
        fgets(str,128,activity);
        for(i=0;i<stCount;i++)/*Read in file 1*/
        {
            fgets(str,128,students);
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
            if(columnCount>3)/*This loop assigns existing scores to the other positions in the second structure's int array*/
            {
                split = strtok(NULL,cv);/*Skipping average...*/
                for(k=1;k<=difference;k++)
                {
                    split = strtok(NULL,cv);
                    currentGrade = strtol(split,&ptr,10);
                    gradeData[i].score[k]=currentGrade;
                    newData[i].score[k-1]=currentGrade;
                }
            }
        }
        for(i=0;i<acCount;i++)/*read in next assignment file*/
        {
            fgets(str,128,activity);
            str[strlen(str)-1]='\0';
            split = strtok(str,cv);
            for(j=0;j<strlen(split);j++)
            {
                gradeData[i].id[j]=split[j];
            }
            split = strtok(NULL,cv);
            currentGrade = strtol(split,&ptr,10);
            gradeData[i].score[0]=currentGrade;
        }
        for(i=0;i<stCount;i++)
        {
            /*Surnames are stored in .name and names are stored in .surName*/
            /*Calculating new zeros*/
            tempGrade=0;
            currentID=studentData[i].id;
            for(j=0;j<acCount;j++)
            {
                cmpID=gradeData[j].id;
                if(strcmp(currentID,cmpID)==0)
                {
                    tempGrade=gradeData[j].score[0];
                }
            }
            strcpy(newData[i].id,currentID);
            strcpy(newData[i].surName,studentData[i].surName);
            strcpy(newData[i].name,studentData[i].name);
            if(columnCount>3)
            {
                newData[i].score[difference]=tempGrade;
            }
            else
            {
                newData[i].score[0]=tempGrade;
            }
        }
        strcpy(strToWrite,"id,last_name,first_name,average,grade01");
        for(k=0;k<difference;k++)
        {
            sprintf(tmpstring,"0%d",k+2);
            strcat(strToWrite,",grade");
            strcat(strToWrite,tmpstring);
        }
        strcat(strToWrite,"\n");
        count=difference+1;
        fwrite(strToWrite,1,strlen(strToWrite),target);/*write header*/
        for(i=0;i<stCount;i++)
        {
            sum=0;
            for(k=0;k<=difference;k++)
            {
                sum+=newData[i].score[k];
            }
            alpha=sum;
            beta=count;
            avg=(alpha/beta);
            strcpy(strToWrite,newData[i].id);
            strcat(strToWrite,",");
            strcat(strToWrite,newData[i].name);
            strcat(strToWrite,",");
            strcat(strToWrite,newData[i].surName);
            strcat(strToWrite,",");
            sprintf(convertedS,"%.2f",avg);
            strcat(strToWrite,convertedS);
            for(k=0;k<=difference;k++){
                strcat(strToWrite,",");
                sprintf(convertedS,"%d",newData[i].score[k]);
                strcat(strToWrite,convertedS);
            }
            strcat(strToWrite,"\n");
            fwrite(strToWrite,1,strlen(strToWrite),target);
        }
        free(studentData);
        free(tmpstring);
        free(gradeData);
        free(newData);
        free(convertedS);
        free(strToWrite);
        fclose(target);
        fclose(students);
        fclose(activity);
        return 0;
    }
    else{
        fprintf(stderr,"You have entered at least one file name that doesnt exist! Please check and try again\n");
        return -1;
    }
}
int countColumns(FILE *currentfile)
{
    char firststring[128],*split,*cv=",";
    int i=0;
    fgets(firststring,128,currentfile);
    split = strtok(firststring,cv);
    while(split!=NULL)
    {
        split=strtok(NULL,cv);
        i++;
    }
    return i;
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