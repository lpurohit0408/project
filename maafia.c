#include<stdio.h>           //input output
#include<stdlib.h>          //standard library
#include<string.h>          //string library
#include<pthread.h>         //create POSIX threads
#include<semaphore.h>       //creation of semaphores

#define wait_chairs 3             //no. of waiting chairs
#define default_students 5        //default no. of students

void *student(void *stud_id);     //Student's activity
void *assistant();                //teaching assistant's activity

//semaphores
sem_t s_stud;
sem_t s_Tass;

//mutex
pthread_mutex_t mutex;

int wait_room[3],stud_wait=0,next_seat_pos=0,next_teach_pos=0,ta_flag=0;

int isnum(char num[])
{
    int i;
    for(i=0;num[i]!=0;i++)
    {
        if(!isdigit(num[i]))
        {
       	    return 0;
        }
    }
    return 1;
}
/*int iswait(int stud_id)
{
    int i;
    for(i=0;i<3;i++)
    {
        if(wait_room[i]==stud_id)
       	{
            return 1;
        }
    }
    return 0;
}*/

void *student(void *stud_id)
{
	int id=*(int*)stud_id;
	printf("\nStudent %d is going to TA office",id);
	while(1)
	{
	/*	if(iswait(id)==1)
		{
			//printf("\nStudent has to wait....");
			continue;
		}*/
		int p=rand()%5;
	//	printf("\n\n\tStudent %d is programming for %d seconds...",id,p);
		sleep(5);
		pthread_mutex_lock(&mutex);
		if(stud_wait<wait_chairs)
		{
			wait_room[next_seat_pos]=id;
			stud_wait++;
			printf("\n\t\tStudent %d has taken a seat in the waiting room....Number of students waiting : %d",id,stud_wait);
			printf("\nWaiting students : seat-1 <%d>  ||  seat-2 <%d>  ||  seat-3 <%d>",wait_room[0],wait_room[1],wait_room[2]);
			next_seat_pos=(next_seat_pos+1)%wait_chairs;
			pthread_mutex_unlock(&mutex);
			sem_post(&s_stud);
			sem_wait(&s_Tass);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			printf("\nStudent %d will try later...",id);
			//printf("\n\n\n\t\t-+-+-+-+-+-+-+-+-NO SEATS AVAILABLE.....BETTER LUCK NEXT TIME-+-+-+-+-+-+-+-+-");
		}
	}
}
void *assistant()
{
    printf("Checking for students.\n");
    while(1)
    {
	 	if(stud_wait>0)
    	{
       		ta_flag=0;
       		sem_wait(&s_stud);
       		pthread_mutex_lock(&mutex);
       		int ht=rand()%5;
			printf("\nTeacher is teaching student %d ",wait_room[next_teach_pos]);
			wait_room[next_teach_pos]=0;
			stud_wait--;
			printf("\nWaiting students : seat-1 <%d>  ||  seat-2 <%d>  ||  seat-3 <%d>",wait_room[0],wait_room[1],wait_room[2]);
			next_teach_pos=(next_teach_pos+1)%wait_chairs;
			sleep(ht);
			printf("\n\t*!*!*!*!*!*!*!*!*!TEACHING FINISH!*!*!*!*!*!*!*!*!*");
			//printf("\nNumber of students waiting : %d",stud_wait);
			pthread_mutex_unlock(&mutex);
			sem_post(&s_Tass);
		}
		else
		{
			if(ta_flag==0)
			{
				printf("\n\n\t#-#-#-#-#-#-#-#-#-NO STUDENTS WAITING....TEACHER IS SLEEPING-#-#-#-#-#-#-#-#-#");
				ta_flag=1;
			}
		}
	}
}
int main(int argc,char *argv[])
{
    int i,snum;
    if(argc>1)
    {
        if(isnum(argv[1]==1))
        {
           	snum=atoi(argv[1]);
        }
        else
        {
            printf("\n*!*!*!*!*!*!*!*INVALID INPUT.....BYE BYE*!*!*!*!*!*!*!*\n");
            return 0;
        }
    }
    else
    {
        snum=default_students;
    }
    int s_ids[snum];
    pthread_t st[snum],t;
    sem_init(&s_stud,0,0);
    sem_init(&s_Tass,0,1);
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&t,NULL,assistant,NULL);
    for(i=0;i<snum;i++)
	{
		s_ids[i]=i+1;
		pthread_create(&st[i],NULL,student,(void*)&s_ids[i] );
	}
	//Join threads
	pthread_join(t,NULL);
	for(i=0;i<snum;i++)
	{
		pthread_join(st[i],NULL);
	}
	return 0;
}
