#include <string>
#include "codearray.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main()
{
	
	/* The child process id */
	pid_t childProcId = -1;
	
	char* fileName;
	FILE * pFile;
	/* Go through the binaries */
	for(int progCount = 0; 	progCount < NUM_BINARIES; ++progCount)
	{
			
		//Create a temporary file you can use the tmpnam() function for this.
		// E.g. fileName = tmpnam(NULL)
		fileName = tmpnam(NULL);
		
		//Open the file and write the bytes of the first program to the file.
		//These bytes are found in codeArray[progCount]
		 
		pFile = fopen(fileName, "wb");
	
		/* Make sure the file was opened */
		if(!pFile)
		{
			perror("fopen");
			exit(-1);
		}
		
		/* The arguments are as follows:
		 * @arg1 - the array containing the elements we would like to write to the file.
		 * @arg2 - the size of a single element.
		 * @arg3 - the number of elements to write to the file
		 * @arg4 - the file to which to write the bytes
		 * The function returns the number of bytes written to the file or -1 on error
		 */
		if(fwrite(codeArray[progCount], sizeof(char), programLengths[progCount], pFile) < 0)
		{
			perror("fwrite");
			exit(-1);
		}
		
		/* Close the file */
		fclose(pFile);

		//Make the file executable: this can be done using 
		chmod(fileName, 0777);
		
		
		//Create a child process using fork
		/* fork another process */
		childProcId = fork();
		if (childProcId < 0) { /* error occurred */
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
		else if (childProcId == 0) { /* child process */
			//use execlp() in order to turn the child process into the process
			//running the program in the above file.
			if(execlp(fileName, NULL) < 0)
			{
				perror("execlp");
				exit(-1);
			}
		}
	}
	
	/* Wait for all programs to finish */
	for(int progCount = 0; progCount < NUM_BINARIES; ++progCount)
	{
		/* Wait for one of the programs to finish */
		if(wait(NULL) < 0)
		{
			perror("wait");
			exit(-1);
		}	
	}
}
