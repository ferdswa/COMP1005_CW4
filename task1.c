#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

void countLines(FILE*,int*,int*);
int main(int argc, char **argv)
{
    int preint1,preint2,stCount,acCount,i,absStu,tStu;
    int commas1, commas2,ensure1,ensure2,currentGrade;
    double meanAvg,sdv, runningTot=0;
    double res,vari=0;
    int *contentsArr;
    char cstring[64],*cv=",",*split,*ptr;
    FILE *students;
    FILE *activity;
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
        contentsArr=calloc(acCount,sizeof(int));
        /*calcs*/
        fgets(cstring,64,activity);/*This should skip head line*/
        absStu=stCount-acCount;
        tStu=stCount;
        for(i=0;i<acCount;i++)/*read file and enter to array*/
        {
            fgets(cstring,64,activity);
            cstring[strlen(cstring)-1]='\0';
            split = strtok(cstring,cv);
            split = strtok(NULL,cv);/*grades are here*/
            currentGrade = strtol(split,&ptr,10);
            contentsArr[i]=currentGrade;
        }
        /*Calculate mean*/
        for(i=0;i<acCount;i++)
        {
            runningTot+=contentsArr[i];
        }
        meanAvg=(runningTot/stCount);
        /*Calculate standard deviation*/
        for(i=0;i<acCount;i++)
        {
            res=contentsArr[i];
            res=res-meanAvg;
            vari += res*res;
        }
        for(i=acCount;i<stCount;i++)
        {
            res = meanAvg;
            vari+=res*res;
        }
        vari/=stCount;
        sdv=sqrt(vari);
        printf("total students = %d\n",tStu);
        printf("absent students = %d\n",absStu);
        printf("grade mean = %.2f\n",meanAvg);
        printf("grade sd = %.2f\n",sdv);
        free(contentsArr);
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