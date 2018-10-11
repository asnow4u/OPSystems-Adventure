

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


int main(){

    int count;
    int dirExist;
    DIR *dir;
    struct stat sb;
    struct dirent *d;

    const char dirPath[20] = "./snowan.rooms";
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

        randomRoom(count);
        count++;
    }


    return 0;
}



void randomRoom(){
    
    int randomNum = rand() %10 +1;
    
    switch(randomNum){
        
        case 1: printf("Thrown_Room\n");
                break;

        case 2: printf("Dinning_Hall\n");
                break;

        case 3: printf("Armory\n");
                break;

        case 4: printf("Laboratory\n");
                break;

        case 5: printf("Kitchen\n");
                break;

        case 6: printf("Sleeping_Quarters\n");
                break;

        case 7: printf("Gate_House\n");
                break;

        case 8: printf("Great_Hall\n");
                break;

        case 9: printf("Servants_Quarters\n");
                break;

        case 10:printf("Storeroom\n");
                break;

    }
}

