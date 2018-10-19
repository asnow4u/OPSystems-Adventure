

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


void writeConnections(char*, char*);
int testFileConnections(char[7][50]);
int getConnectionNum(char*);
void createRooms(int, char*);

int main(){

    int count;
    int dirExist;
    int randomRoom1;
    int randomRoom2;
    int connectionNum;
    
    char *connectionArray[10];
    char buffer[50];
    char roomName[50];
    
    FILE* file;
    DIR *dir;
    struct stat sb;
    struct dirent *d;
    
    char fileArray[7][50];
    int i = 0;
    int pid;
    char pidBuffer[10];
    char dirPath[50] = "./snowan.rooms.";
    
    srand(time(NULL));

    //Create PID
    pid = getpid();
    sprintf(pidBuffer, "%d", pid); 
    strcat(dirPath, pidBuffer);
    strcat(dirPath, "/");
 
    //Attept to open directory and store result in dir
    dir = opendir(dirPath);

    //Check if directory exists, and create it if not
    if (dir == NULL) {
        
        dirExist = mkdir(dirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
        //Check directory was made otherwise through error
        if (dirExist == 0) {
            dir = opendir(dirPath);
        } else {
            printf("Could not make directory");
            return 1; //Exit(1)
        }
    }

    //Check for existing files in directory including '.' and '..' 
    //Source: http://stackoverflow.com/questions/6383584/check-if-a-directory-is-empty-using-linux-c-on-linux
    while ((d = readdir(dir)) != NULL) {
        if (++count > 2){
            break;
        } 
    } 


    //Loop through and randomly pick 7 rooms
    count = 0;

    while (count < 7){
       
        createRooms(count, dirPath);
        count++;
    }

    //Reset Directory for proper reading
    rewinddir(dir);
    count = 0; 
   
    //Check for which files are in directory and add them to a file Array
    while ((d = readdir (dir)) != NULL){
         
        if ((!strcmp (d->d_name, ".")) || (!strcmp (d->d_name, ".."))){
        } else {
       
            strcpy (fileArray[count], dirPath);
            strcat (fileArray[count], d->d_name);
            
            //Add NULL terminator
            fileArray[count][strlen(dirPath) + strlen(d->d_name) + 1] = '\0';             
            count++; 
        }    
    }

    //As long as files in the array dont have at least 3 connections run loop
    while (testFileConnections(fileArray) == 0){
         
        //pick random room from array
        randomRoom1 = rand() %7;
        connectionNum = getConnectionNum(fileArray[randomRoom1]);

        //Call Random again until connections of the file is not 6
        while (connectionNum > 5){ 
            randomRoom1 = rand() %7;
            connectionNum = getConnectionNum(fileArray[randomRoom1]);
        }
       
        //Check randomRoom for connections aready pressent 
        if (connectionNum > 0){

            i = 0;

            //Reset array
            while (i < 10){
                connectionArray[i] = NULL;
                i++;
            }

            i = 0;
            
            file = fopen(fileArray[randomRoom1], "r");
            
            //Allocate space for array for each connection and the name of the room(+1)
            while (i < connectionNum + 1){
                
                connectionArray[i] = (char*) malloc(50);
                i++;
            }
         
            i = 0;

            //Loop through each line in the file
            while (i < connectionNum + 1){
                fscanf(file, "%s %s %s", buffer, buffer, buffer); 

                //Create Array of aready connected rooms
                strcpy(roomName, dirPath);
                strcat(roomName, buffer);
                connectionArray[i] =  roomName;
                        
                i++;
            } 


            fclose(file);

            i = 0;

            randomRoom2 = rand() %7; 
            
            while (i < connectionNum + 1){
                
                //Compare random room ro connecting rooms and check number of connections for potential reroll
                if (strcmp(connectionArray[i], fileArray[randomRoom2]) == 0 || getConnectionNum(fileArray[randomRoom2]) > 5 || randomRoom1 == randomRoom2){
                                 
                   i = 0;
                   randomRoom2 = rand() %7;
                } 
                
                i++;
            } 

        } else {
           
                        
            randomRoom2 = rand() %7;
            connectionNum = getConnectionNum(fileArray[randomRoom2]);

            //Check number of connections for potential reroll
            while (randomRoom1 == randomRoom2 || connectionNum > 5){
                randomRoom2 = rand() %7;
                connectionNum = getConnectionNum(fileArray[randomRoom2]);
            } 
        }
        
        //Connect the two rooms
        writeConnections(fileArray[randomRoom1], fileArray[randomRoom2]);
        writeConnections(fileArray[randomRoom2], fileArray[randomRoom1]);

    }

    closedir (dir);

    return 0;
}

//Takes room name from file2 and writes it as a connection in file1
void writeConnections(char* file1, char* file2){
    
    char notNeeded[50];
    char room[50];
    char buffer[100];
    int num = 0;
    int i=0;
    FILE* file;
    
    file = fopen(file2, "r");
    
    fscanf(file,"%s %s %s", room, room, room);

    fclose(file);
    file = fopen(file1, "r+");
   
    //Grab the ending part of file and throw into var
    //Var will later be rewriten into file
    while((fgets(buffer, 100, file)) != NULL){
        num++;
    }
    
    rewind(file);
    
    while (i < num-1){ 
        fscanf(file,"%s %s %s", notNeeded, notNeeded, notNeeded);
        i++;
    }

    fprintf(file, "\nCONNECTION %d: %s\n%s", num-1, room, buffer);
        
    fclose(file);

}


int testFileConnections(char fileArray[7][50]){
    
    int count = 0;
    int connectionNum = 0;
    char* fileName;

    //Loop through array
    while (count < 7){

        connectionNum = getConnectionNum(fileArray[count]);
         
        //See if 3 connections exist in file, if not return as failed
        if (connectionNum < 3){
            return 0;
        } 

        count++;
    }
    
    return 1;
}


int getConnectionNum(char* fileName){
     
    char c;
    int num = 0;
    FILE* file; 

    file = fopen(fileName, "r");
    
    //Loop through each line in the file
    c = getc(file);

    while(c != EOF){ 
               
        if (c == '\n'){
            num++;
        }        
        c = getc(file);
    }
    
    //Connections not including the first line
    num--; 

    fclose(file);
    return num;
}


void createRooms(int count, char* dirPath){
    
    int randomNum = rand() %10 +1;
    char filePath[50];
    
    strcpy(filePath, dirPath);

    //Use switch statement to randomly select rooms.
    //Each case will check if file already exists and will run the functuion again if that is the case (1 = true, 0 = false)
    //Uses count to determine which file type 

    switch(randomNum){
        
        case 1: strcat(filePath, "Attic");
                    
                if (fileCheck(filePath) == 0){

                    //Create file
                    FILE* file = fopen(filePath, "w"); 
                                        
                    //Write to file
                    fprintf(file, "ROOM NAME: Attic\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    }

                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break;


        case 2: strcat(filePath, "Basement");

                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Basement\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                                        
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break;

        case 3: strcat(filePath, "Armory"); 
        
            if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Armory\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break;  
 
        case 4: strcat(filePath, "Lab"); 
        
                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Lab\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break;   

        case 5: strcat(filePath, "Kitchen");
        
                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Kitchen\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break; 

        case 6: strcat(filePath, "Bedroom");
        
                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Bedroom\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break; 

        case 7: strcat(filePath, "Garden");
                
                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Garden\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break; 


        case 8: strcat(filePath, "Hallway");
        
                if (fileCheck(filePath) == 0){
                   
                 //Create file
                 FILE* file = fopen(filePath, "w");
                    
                 //Write to file
                 fprintf(file, "ROOM NAME: Hallway\n");

                 if (count == 0){
                     fprintf(file, "ROOM TYPE: START_ROOM");
                 } else if (count == 6){
                     fprintf(file, "ROOM TYPE: END_ROOM");
                 } else {
                     fprintf(file, "ROOM TYPE: MID_ROOM");
                 } 
                 
                 fclose(file);

              } else {  
                 createRooms(count, dirPath);
              }
              break; 


        case 9: strcat(filePath, "Yard");
        
                if (fileCheck(filePath) == 0){
                    
                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                   //Write to file
                    fprintf(file, "ROOM NAME: Yard\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break; 


        case 10:strcat(filePath, "Pantry");
            
                if (fileCheck(filePath) == 0){

                    //Create file
                    FILE* file = fopen(filePath, "w");
                    
                   //Write to file
                    fprintf(file, "ROOM NAME: Pantry\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count, dirPath);
                }
                break; 
    }
}

//Given file path, check if file exists (1 file exists, 0 file doesnt)
int fileCheck(char* filePath){
    
    FILE *file;
    if (file = fopen(filePath, "r")){
        fclose(file);
        return 1;

    } else {
        return 0;
    }
}
