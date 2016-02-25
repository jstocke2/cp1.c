//Modified by Jake Stocker
//Systems programming
//hw 1
// Modified cp1.c and ls1.c from Understanding Unix Linux programming

// Modified:  To do a string check on identical CLI arguments
// Modified:  To work for two directories given at CLI creating the second if neccessary

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main (int argc,char *av[]){
  int outFile, inFile, len, nChars;  // full path name input file, full path name output file, length of chars for directory name, length of chars for file  
  char buf[BUFFERSIZE];  // length for file
  struct dirent *inputDirent; // read all files in directory
  DIR *inputDir; //input file stream
  DIR *outputDir; //output file stream
  int PathLength = 100;  // maximum file pathlength
  
  //check for correct number of arguments
  if (argc < 3){
    printf("not enough arguments <dirname>\n");
    return 1;
  }

  // string comparison for duplicate arguments at CLI
  if (strcmp(av[1],av[2]) == 0){
	  printf("The argument '%s is used twice cannot create directory",av[1]);
	  return 1;
  }
  //open directory 
  //if argument is not directory do not open
  inputDir = opendir (av[1]);
  if(inputDir == NULL){
    printf ("Cannot open directory '%s'\n", av[1]);
    return 1;
  }
  //opens directory
  //if target is not directory do not open
  outputDir = opendir (av[2]);
  if (outputDir == NULL){
    printf("Creating new directory '%s'\n");
    mkdir(av[2], 0700);
	outputDir = opendir(av[2]);
  }
  
  printf("Copying files from %s to %s \n", av[1], av[2]);
  // grab input files contents while files are still in directory
  while ((inputDirent = readdir(inputDir)) != NULL) {
    
    char *fileName = (char *) malloc(PathLength);
	// create file path
    strcpy (fileName, av[1]);
	strcat(fileName,"/");
    strcat (fileName, inputDirent->d_name);
	// open file by pathname
    inFile = open(fileName, O_RDONLY);

    //take the name and path from the first directory
    //and make it available to the second directory
    //so that a new file is created with the same name
	
    char * newFile = (char *) malloc(PathLength);
	// create file path
    strcpy (newFile, av[2]);
	strcat(newFile, "/");
    strcat (newFile, inputDirent->d_name);
	
	// create new file in new directory path 
    outFile = creat(newFile, COPYMODE);
    
    //copy the file contents from the first directory to the second
    while((nChars = read(inFile, buf, BUFFERSIZE)) > 0)
      if( write(outFile, buf, nChars) != nChars)
	oops("Write error to ", newFile);
    
  }
  // close files 
  printf("Copy successful!\n");
  close(inFile);
  close(outFile);
  closedir(inputDir);
  closedir(outputDir);
  return 0;
} 

void oops(char *s1, char *s2){
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}