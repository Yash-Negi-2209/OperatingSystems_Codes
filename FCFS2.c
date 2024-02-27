#include<stdio.h>
struct process{
 int pid;
 int bt;
 int wt, tt;
}p[10];
int main(){
 int i, n, totwt, tottt, avg1, avg2;
 printf("Enter the no. of process:\n");
 scanf("%d",&n);
 for(i = 0; i < n; i++){
 p[i].pid = i + 1;
 printf("Enter the burst time %d: ",i + 1);
 scanf("%d",&p[i].bt);
 }
 p[0].wt = 0;
 p[0].tt = p[0].bt + p[0].wt;
 i = 1;
 while(i < n){
 p[i].wt = p[i - 1].bt + p[i - 1].wt;
 p[i].tt = p[i].bt + p[i].wt;
 i++;
 }
 i = 0;
 totwt = 0;
 tottt = 0;
 printf("\nProcessID\tburst.t\t wait.t\tturnaround.t\n");
 while(i < n){
 printf("\n\t%d\t%d\t%d\t%d",p[i].pid, p[i].bt, p[i].wt, p[i].tt);
 totwt += p[i].wt;
 tottt += p[i].tt;
 i++;
 }
 avg1 = totwt / n;
 avg2 = tottt / n;
 printf("\nAverage Waiting Time = %d\tAverage Turnaround Time = %d\t",avg1,avg2);
 return 0;
}