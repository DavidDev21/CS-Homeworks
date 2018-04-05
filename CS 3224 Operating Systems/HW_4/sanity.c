#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[])
{
    int pid, pidArray[20];
    int wtime[20],rutime[20],iotime[20],status[20];
    int waitTotal = 0,runTotal = 0, ioTotal,turnAroundTotal = 0;

    // Note: turnAround = time of completion - time of submitions

    uint i,numChildren;
    numChildren = 20;
    for(i = 0; i< numChildren; ++i)
    {
        // fork between parent (original) and child (new)
        if((pid =fork()) == 0) break;
        if(pid < 0)
        { 
            printf(1,"Error\n"); break;
        }
        pidArray[i] = pid; // store pids of children
    }

    // at this point we have 20 children
    // if we the parent
    if(pid > 0)
    {
        // parent have to wait for its children to exit
        // we must added the stats of the children when they come back
        // parent must validate a child's exit status with its pid (store in pidArry)
        uint i;
        for(i = 0; i < numChildren; ++i)
        {

            //////printf(1,"%d -> pid: %d\n", i, pidArray[i]);
            //printf(1,"address of wtime: %d\n",&wtime[i]);
            wait_stat(&wtime[i],&rutime[i],&iotime[i],&status[i]);
            ///printf(1,"w:%d r:%d io:%d status:%d\n",wtime[i],rutime[i],iotime[i],status[i]);

            // validate the exit status
            if(pidArray[i] == status[i])
            {
                // if valid, then we shall perform stat calculations
                // and print out stats for child
                waitTotal += wtime[i];
                runTotal += rutime[i];
                ioTotal += iotime[i];
                turnAroundTotal +=  (wtime[i]+rutime[i]+iotime[i]); // total time to complete for the child
                printf(1,"  Child %d  \nWait Time: %d\nRun Time: %d\nIO Time: %d\nTurnaround Time: %d\n\n",
                        pidArray[i],wtime[i],rutime[i],iotime[i],(wtime[i]+rutime[i]+iotime[i]));

            }

            printf(1,"\n\n");
        }
        
        // print out average stats across all children
        printf(1,"  Overall Stats  \nWait Time Avg: %d\nRun Time Avg: %d\nIO Time Avg: %d\nTurnaround Time Avg: %d\n\n"
                ,waitTotal/numChildren,runTotal/numChildren,ioTotal/numChildren,turnAroundTotal/numChildren);
     
        
        exit();

    }
    // child process
    else if(pid == 0)
    {
        /*
        if(i > 1 || i < 63)
        {
            sleep(20);
        }
        */
       
        // Do some useless calcualtions (more than 30 clock ticks)
        uint i, sum;
        for(i = 0; i< 5000000;++i)
        {
            sum+=i;
            sum*=(i*i);
            sum/=3;
        }
        //printf(1,"I am child: %d\n",getpid());
        exit();
    }
    // error
    else
    {
        printf(1,"Failed To Fork()\n");
    }

    exit();
}