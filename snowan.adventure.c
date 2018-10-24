
///////////////////////////////////////////////
//Name: Andrew Snow
//Program: Adventure
///////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

void writeFile(char*);
int checkConnection(char*, char*);
void* getTime(void*);

pthread_t tim;
pthread_mutex_t mutex;


int main(){

    
    char dirPath[100]; 
    char fileArray[7][100];
    char compareFile[7][100];
    char completionArray[10][100];
    char room[50];
    char timeStr[50];
    int dirTime = -1;
    int startRoom;
    int endRoom;
    int roomNum;
    int completionNum = 0;
    int err;
    int count = 0;
    FILE* file;
    FILE* fp;
    DIR* d;
    time_t t1;
    time_t t2;
    struct dirent *dir;
    struct stat dirInfo;
    

    
    //read directorys in file
    //Refferenced from block 2 notes
    d = opendir(".");

    if (d == NULL){
        printf("Could not open current directory");
        return 1;
    }

    while ((dir = readdir(d)) != NULL){
        
        if (strstr(dir->d_name, "snowan.rooms") != NULL){
            stat(dir->d_name, &dirInfo);

            if ((int)dirInfo.st_mtime > dirTime){
                dirTime = (int)dirInfo.st_mtime;
                memset(dirPath, '\0', sizeof(dirPath));

                strcat(dirPath, dir->d_name);

            }
        }
    }

    closedir(d);
    d = opendir(dirPath);

    if (d == NULL){
        printf("Directory cant be opened");
        return 1;
    }
    
    while ((dir = readdir(d)) != NULL){
        if ((!strcmp(dir->d_name, ".")) || (!strcmp(dir->d_name, ".."))){
        } else {
            
            strcpy (fileArray[count], dirPath);
            strcat (fileArray[count], "/");
            strcat (fileArray[count], dir->d_name);

            strcpy (compareFile[count], dir->d_name);

            count++;

        }
    }
    
    count = 0;

    //Loop through to find the start room and end room
    //char* room is used to find the type of room
    while (count < 7){
   
        file = fopen(fileArray[count], "r");

        if (file == NULL){
            printf("file cant be opened");
        }
        
        while (fscanf(file, "%s", room) != EOF){
           
            if (!strcmp(room, "START_ROOM")){
                startRoom = count;
            }
            if (!strcmp(room, "END_ROOM")){
                endRoom = count;
            }
        }
        
        fclose(file);
        count++;
    }


    writeFile(fileArray[startRoom]);

    count = 0;
    roomNum = startRoom;
 
    //As long as the entered room is not equal to the end room, loop
    while (strcmp(room, compareFile[endRoom])){
 
        //char* room is used to read input from user
        scanf("%s", room);


        //Compare the files for valid input, also checking for the connection 
        if (checkConnection(room, fileArray[roomNum]) == 1){
            
            printf("\n");
            
            //find roomNum of room
            while (count < 7){
                
                if (!strcmp(room, compareFile[count])){
                    roomNum = count;
                }

                count++;
            }

            //write selected file
            writeFile(fileArray[roomNum]);
            
            strcpy(completionArray[completionNum], room);
            completionNum++;

            count = 0;


        } else if (strcmp(room, "time") == 0) {
           
            //Call thread
            err = pthread_create(&(tim), NULL, &getTime, NULL);
            if (err != 0){
                printf("\ncant create thread :[%s]", strerror(err));
            }

            pthread_join(tim, NULL);
            

            //Read from file
            fp = fopen("currentTime.txt", "r");
            printf("\n");

            while (fscanf(fp, "%s", timeStr) == 1){ 
            
                printf("%s", timeStr); 
            }

            if (feof(fp)){
                printf("\n");
            }

            writeFile(fileArray[roomNum]);
        
        
        } else {
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");

            writeFile(fileArray[roomNum]);

        }
    }

    //Congrads Statement
    printf("\n\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");

    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", completionNum);

    count = 0;

    while (count < completionNum){
        printf("%s\n", completionArray[count]);
        count++;
    }

    return 0;
}


void* getTime(void *arg){
    
    pthread_mutex_lock(&mutex);

    char timeStr[50];
    time_t rawTime;
    struct tm *timeInfo;
    FILE* fp;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(timeStr, sizeof(timeStr), "%1:%M%P, %A, %B, %d, %Y", timeInfo);
   
    fp = fopen("currentTime.txt", "w+");

    if (fp == NULL){
        printf("Failed to open file");
    }

    fprintf(fp, "%s", timeStr);
    fclose(fp);


    pthread_mutex_unlock(&mutex);

    return NULL;

}


//Write everything in the file to the screen
void writeFile(char* fileName){
    
    FILE* file;
    char current[50];
    char connection[7][50];
    int connectionNum = 0;
    int count = 0;

    file = fopen(fileName, "r");
    
    //Read from file to get the current room and the connections for that room
    fscanf(file, "%s %s %s", current, current, current);

    while (fscanf(file, "%s", connection[count]) != EOF){
        if (!strcmp(connection[count], "CONNECTION")){
            
            fscanf(file, "%s %s", connection[count], connection[count]);

            connectionNum++;
            count++;
        }

    }
    
    count = 0;

    //Print Text
    printf("\nCURRENT LOCATION: %s\n", current);
    
    printf("POSSIBLE CONNECTIONS:");
    while (count < connectionNum){
     
        printf(" %s", connection[count]); 
        
        if (count == connectionNum - 1){
            printf(".");
        } else {
            printf(",");
        }
        
        count++;
    }

    printf("\nWHERE TO? >");

    fclose(file);
}


int checkConnection(char* room, char* fileArray){

    FILE* file;
    char connection[50];
    
    file = fopen(fileArray, "r");

    fscanf(file, "%s %s %s", connection, connection, connection);

    while (fscanf(file, "%s", connection) != EOF){
       
        if (!strcmp(connection, "CONNECTION")){
            
            fscanf(file, "%s %s", connection, connection);

            //if a connection is the same as room, return true
            if (!strcmp(connection, room)){  

                return 1;
            }
        }
    }

    fclose(file);

    return 0;
}
