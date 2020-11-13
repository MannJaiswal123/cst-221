#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h> 


sem_t* uniqueBook;
pid_t otherPid;
sigset_t sigSet;


void signalHandler1(int signum)
{
    printf("Caught Signal: %d\n", signum);
    printf("    Exit Student Process.\n");
    sem_post(uniqueBook);
    _exit(0);
}

void signalHandler2(int signum)
{
    printf("Its me!\n");
}

void studentProcess()
{
    
    signal(SIGUSR1, signalHandler1);
    signal(SIGUSR2, signalHandler2);

   
    int value; 
    sem_getvalue(uniqueBook, &value);
    printf("    student Process uniqueBook count is %d.\n", value);
    printf("    student Process is using the unique book.\n");
    sem_wait(uniqueBook);
    sem_getvalue(uniqueBook, &value);
    printf("    student Process uniqueBook count is %d.\n", value);

   
    printf("   lets start a long student process ..... its time for student to learn some long stuff......\n");
    for(int x = 0; x < 60; ++x)
    {
        printf(".\n");
        sleep(1);
    }
    
    printf("    Exit student Process.\n");
    _exit(0);
}

void* checkStuckStudent(void *a)
{
  
    int* status = a;
    printf("Here we gonna check that student process stucked or no.....\n");
    sleep(10);
    if(sem_trywait(uniqueBook) != 0)
    {
        printf("ohh look it is hung.....\n");
        *status = 1;
    }
    else
    {
        printf("No its working fine.....\n");
        *status = 0;
    }
    return NULL;    
}

void teacherProcess()
{
   
    sleep(2);
    if(getpgid(otherPid) >= 0)
    {
        printf("Student Process is running.....\n");
    }
    int value; 
    sem_getvalue(uniqueBook, &value);
    printf("teacher Process uniqueBook count is %d.\n", value);

  
    if(sem_trywait(uniqueBook) != 0)
    {
        
        pthread_t tid1;
        int status = 0;
        printf("trying to check status of student process.....\n");
        if(pthread_create(&tid1, NULL, checkStuckStudent, &status))
        {
            printf("ERROR creating timer thread.");
            _exit(1);
        }
        if(pthread_join(tid1, NULL))
        {
            printf("\nERROR joining timer thread.\n");
            exit(1);
        }
        if(status == 1)
        {
            
            printf(" killing student process  with ID of %d\n", otherPid);
            
           
            printf("Press 1 to continue to kill student  Process");
            scanf("%d", &status); 
            while (status != 1)
            {
                printf("if you want to continue you have to kill the student process so for that please enter 1");
                scanf("%d", &status); 
            }            

          
            kill(otherPid, SIGTERM); 
            printf("student Process is killed\n");
        }

        
        printf("its time to proove that student process is killed\n");
        sleep(5);
        kill(otherPid, SIGUSR2);
        sleep(1);
        printf("Proved\n");

       
        sem_getvalue(uniqueBook, &value);
        printf("In the student Process with the unique book count of %d.\n", value);
        if(sem_trywait(uniqueBook)!= 0)
        {
            if(value = 0)
            {
                sem_post(uniqueBook);
                printf("Cleaned up and finally using the unique book.\n");
                sem_getvalue(uniqueBook, &value);
                printf("In  teacher Process with the uniqueBook count of %d.\n", value);
            }
            else
            {
                printf("Finally using the unique book.\n");
            }            
        }
    }

    
    printf("Exit teacher Process.\n");
    _exit(0);
}

int main(int argc, char* argv[])
{
    pid_t pid;

  
   uniqueBook = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

   
    int usrInput;
    printf("Press Enter to continue. 1 = Yes | 2 = No");
    scanf("%d", &usrInput);
    printf("Entered Value was %d\n", usrInput);
    if (usrInput == 2)
    {
        
        return 0;
    }

    
    if(sem_init(uniqueBook, 1, 1) != 0)
    {
        printf("Failed to create uniqueBook.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("\n Successfully built unique book.\n");
    }

    
    pid = fork();
    if (pid == -1)
    {
      
        printf("Can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
   
    if (pid == 0)
    {
        printf("Started student Process with Process ID of %d......\n", getpid());
        otherPid = getppid();
        studentProcess();
    }
    else
    {
        printf("Started teacher Process with Process ID of %d......\n", getpid());
        otherPid = pid;
        teacherProcess();
    }

 
    sem_destroy(uniqueBook);

    
    return 0;
}
