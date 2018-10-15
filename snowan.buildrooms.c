

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


void writeConnections(char*, char*);
int testFileConnections(char**);
int getConnectionNum(char*);
void createRooms(int);

int main(){

    int count;
    int dirExist;
    int randomRoom1;
    int randomRoom2;
    int connectionNum;
    
    DIR *dir;
    struct stat sb;
    struct dirent *d;
    char* fileArray[7];
    int i = 0;

    const char dirPath[20] = "./snowan.rooms/";
    srand(time(NULL));

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
    
    //Directory is empty if count is less then or equal to 2, otherwise directory has files that need removing
    if (count > 2){

        //Remove room files
        remove("./snowan.rooms/Thrown_Room");
        remove("./snowan.rooms/Dinning_Hall");
        remove("./snowan.rooms/Armory");
        remove("./snowan.rooms/Laboratory");
        remove("./snowan.rooms/Kitchen");
        remove("./snowan.rooms/Sleeping_Quarters");
        remove("./snowan.rooms/Gate_House");
        remove("./snowan.rooms/Great_Hall");
        remove("./snowan.rooms/Servants_Quarters");
        remove("./snowan.rooms/Storeroom");
        
    }



    //Loop through and randomly pick 7 rooms
    count = 0;

    while (count < 7){

        createRooms(count);
        count++;
    }

    //Rest Directory for proper reading
    rewinddir(dir);
    count = 0;
   

    //Check for which files are in directory and add them to a file Array
    while ((d = readdir (dir)) != NULL){
        
        if ((!strcmp (d->d_name, ".")) || (!strcmp (d->d_name, ".."))){
        } else {
        
            fileArray[count] = malloc (strlen(d->d_name) + strlen(dirPath) +1 );
            strncpy (fileArray[count], dirPath, strlen(dirPath));
            strncat (fileArray[count], d->d_name, strlen(d->d_name));
            count++; 
        }    
                        
    }
    
    //As long as files in the array dont have at least 3 connections run the loop
    //while (testFileConnections(fileArray) == 0){
    if (testFileConnections(fileArray) == 0){

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
            //create new array without the connections aready pressent and not including self
           //char filePath[50];
           //FILE* file;
           //file = fopen(fileArray[randomRoom1], "r");

           //Loop through each line in the file
           //while ((fgets(filePath, 50, file)) != NULL){
                
            //    sscanf(filePath, "%s %s %s", filePath, filePath, filePath);
            //    printf("%s\n", filePath); //test
            //}
        
            //fclose(file);

            //check for over 6 connections

        } else {
           
            randomRoom2 = rand() %7;
            connectionNum = getConnectionNum(fileArray[randomRoom2]);

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
    
    printf("writeConnections()\n");

    char notNeeded[50];
    char room[50];
    char buffer[100];
    int num = 0;
    int i=0;
    FILE* file;
    
    file = fopen(file2, "r");
    
    fscanf(file,"%s %s %s", room, room, room);
    printf("ROOMS: %s\n", room); //test


    fclose(file);
    file = fopen(file1, "r+");
   
    //Grab the ending part of file and throw into var
    //Var will later be rewriten into file
    while((fgets(buffer, 100, file)) != NULL){
        num++;
    }
    
    rewind(file);
    
    while (i < num-1){ //mpt sure if right!!!!!!!!!!!!
        fscanf(file,"%s %s %s", notNeeded, notNeeded, notNeeded);
        i++;
    }

    fprintf(file, "\nCONNECTION %d: %s\n%s", num-1, room, buffer);
        
    fclose(file);

}


int testFileConnections(char** fileArray){
    
    printf("testFileConnections()\n");

    int count = 0;
    int connectionNum;

    //Loop through array
    while (count < 7){

        connectionNum = getConnectionNum(fileArray[count]);
        
        //See if 3 connections exist in file, if not return as failed
        if (connectionNum < 3){
            printf("connectionNum is less then 3\n"); //test
            return 0;
        }
 
        printf("\n\n"); //test

        count++;
    }
    
    return 1;
}


int getConnectionNum(char* fileName){
     
    printf("getConnectionNum()\n");

    char string[50]; 
    int num = 0;
    FILE* file;

    file = fopen(fileName, "r");
        
        //Loop through each line in the file
        while((fgets(string, 50, file)) != NULL){
            
            sscanf(string, "%s", string);
            printf("%s\n", string); //test

            //Compare string to see if its a connection
            if (strcmp(string, "CONNECTION") == 0){
                num++;
            }
        }

        printf("Connections: %d\n", num);

        fclose(file);
        return num;
}


void createRooms(int count){
    
    int randomNum = rand() %10 +1;

    //Use switch statement to randomly select rooms.
    //Each case will check if file already exists and will run the functuion again if that is the case (1 = true, 0 = false)
    //Uses count to determine which file type 

    switch(randomNum){
        
        case 1: if (fileCheck("./snowan.rooms/Thrown_Room") == 0){

                    //Create file
                    FILE* file = fopen("./snowan.rooms/Thrown_Room", "w"); 
                                        
                    //Write to file
                    fprintf(file, "ROOM NAME: Thrown_Room\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    }

                    fclose(file);

                } else {
                    createRooms(count);
                }
                break;

        case 2: if (fileCheck("./snowan.rooms/Dinning_Hall") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Dinning_Hall", "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Dinning_Hall\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                                        
                    fclose(file);

                } else {
                    createRooms(count);
                }
                break;

        case 3: if (fileCheck("./snowan.rooms/Armory") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Armory", "w");
                    
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
                    createRooms(count);
                }
                break;  
 
        case 4: if (fileCheck("./snowan.rooms/Laboratory") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Laboratory", "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Laboratory\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count);
                }
                break;   

        case 5:if (fileCheck("./snowan.rooms/Kitchen") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Kitchen", "w");
                    
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
                    createRooms(count);
                }
                break; 

        case 6: if (fileCheck("./snowan.rooms/Sleeping_Quarters") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Sleeping_Quarters", "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Sleeping_Quarters\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count);
                }
                break; 

        case 7:if (fileCheck("./snowan.rooms/Gate_House") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Gate_House", "w");
                    
                    //Write to file
                    fprintf(file, "ROOM NAME: Gate_House\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count);
                }
                break; 


        case 8:if (fileCheck("./snowan.rooms/Great_Hall") == 0){
                   
                 //Create file
                 FILE* file = fopen("./snowan.rooms/Gate_House", "w");
                    
                 //Write to file
                 fprintf(file, "ROOM NAME: Gate_House\n");

                 if (count == 0){
                     fprintf(file, "ROOM TYPE: START_ROOM");
                 } else if (count == 6){
                     fprintf(file, "ROOM TYPE: END_ROOM");
                 } else {
                     fprintf(file, "ROOM TYPE: MID_ROOM");
                 } 
                 
                 fclose(file);

              } else {  
                 createRooms(count);
              }
              break; 


        case 9:if (fileCheck("./snowan.rooms/Servants_Quarters") == 0){
                    
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Servants_Quarters", "w");
                    
                   //Write to file
                    fprintf(file, "ROOM NAME: Servants_Quarters\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count);
                }
                break; 


        case 10:if (fileCheck("./snowan.rooms/Storeroom") == 0){
                    //Create file
                    FILE* file = fopen("./snowan.rooms/Storeroom", "w");
                    
                   //Write to file
                    fprintf(file, "ROOM NAME: Storeroom\n");

                    if (count == 0){
                        fprintf(file, "ROOM TYPE: START_ROOM");
                    } else if (count == 6){
                        fprintf(file, "ROOM TYPE: END_ROOM");
                    } else {
                        fprintf(file, "ROOM TYPE: MID_ROOM");
                    } 
                    
                    fclose(file);

                } else {
                    createRooms(count);
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
