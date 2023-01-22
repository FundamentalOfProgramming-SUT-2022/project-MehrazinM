//
//  main.c
//  vim0.0.1
//
//  Created by Mehrazin on 10/29/1401 AP.
//



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>


//symbols and etc
#define MAX_COMMAND_SIZE 10000
#define MAX_FILE_NAME 10000
#define MAX_PATH_SIZE 100000
//declaring global variables

//prototyping all the required functions
int WhatIsTheCommand(char* command);
// this is for getting the inputs which are required to call the main createfile function,therefore I probably switch the func type to non-void
void createfile(char* command);

void CreateDirectories(char* path);

void MakeTheFile(char* path);

void ReadFromFile(char* path);

void Cat(char* command);
int IsValidPath(char* path,char* NewPathForFile);
int CheckValidFile(char* path);
int CheckValidDir(char* path);

//void InputThePath(char* path);
int main(int argc, const char * argv[]) {
    char command[MAX_COMMAND_SIZE];
    int CommandAsANumber;
    while(strcmp(command,"exit")){
        
        scanf("%s",command);
        CommandAsANumber=WhatIsTheCommand(command);
        switch(CommandAsANumber){
            case -1:
                //In order to handle printing "invalid input" only once I should do a lot more ...
                printf("Invalid command\n");
                break;
            case 0:
                return 0;
            case 1:
                createfile(command);
                break;
            case 2:
                Cat(command);
                break;
        }
        
    }
    return 0;
}
//0:exit 1:createfile 2:...
int WhatIsTheCommand(char* command){
    if(strcmp(command, "exit")==0){
        return 0;
    }
   else  if(strcmp(command,"createfile")==0){
        return 1;
    
        
    }
   else if(strcmp(command, "cat")==0){
       return 2;
   }
    
    
    
    
    
    
   else{
       return -1;
   }
}

void createfile(char* command){
    char TagOfCommand[MAX_COMMAND_SIZE];char spc;
    scanf("%s%c",TagOfCommand,&spc);
    if(strcmp(TagOfCommand,"--file")){
        printf("Invalid Input");
        return;
    }
    else{
       
        
        char path[MAX_PATH_SIZE];
        gets(path);
//        printf("%s",path);
       CreateDirectories(path);
    }
}



void CreateDirectories(char* path){
    //I will handle "" later,this block is without considering whitespaces
    unsigned long LenghtOfPath=strlen(path);
    int NumberOfSlashes=0;char* Address=malloc(MAX_PATH_SIZE*sizeof(char));
    char* CopyOfAddress=malloc(MAX_PATH_SIZE*sizeof(char));
    
    
    for(int i=0;i<LenghtOfPath;i++){
        if(path[i]=='/'&&path[i-1]!='/')
            NumberOfSlashes++;
        
        
    }
    
    //   this block dosen't consider white spaces...
    int index=0;int j=0;
    if(path[0]!='"')
    {
    for(int i=0;i<NumberOfSlashes-1;i++){
        while(j<LenghtOfPath){
            
            if(path[j]=='/'){
                if(j!=0){
                    Address[index]='/';
                    index++;
                    j++;
                    break;
                    
                }
                else{
                    j++;
                }
                
            }
            
            else{
                if(path[j]!=' '){
                    Address[index]=path[j];
                    index++;
                    j++;
                }
                else{
                    j++;
                }
                
            }
        }
        
        for(int t=0;t<strlen(Address)-1;t++){
            CopyOfAddress[t]=Address[t];
        }
        //     printf("%s\n",CopyOfAddress);
        
        
        mkdir(CopyOfAddress, 0755);
    }
    index=0;
    //    for(int i=j-1;i<LenghtOfPath;i++){
    //        filename[index]=path[i+1];
    //        index++;
    //    }
    char* NewPathForFile=malloc(MAX_PATH_SIZE*sizeof(char));
    for(int x=0;x<LenghtOfPath-1;x++){
        if(path[x+1]!=' '){
            NewPathForFile[index]=path[x+1];
            index++;
        }
    }
    //    printf("%s",NewPathForFile);
    
    MakeTheFile(NewPathForFile);
    //    for(int i=0;i<index;i++){
    //        printf("%c",filename[i]);
    //    }
}
    else{
        int Count=0;
    //    printf("%d",LenghtOfPath);
        for(int i=0;i<NumberOfSlashes-1;i++){
            while(j<LenghtOfPath){
                
                if(path[j]=='/'){
                    if(j>1){
                        Address[index]='/';
                        index++;
                        j++;
                        Count++;
                        break;

                    }
                    else{
                        j++;
                        Count++;
                    }

                }

                else{
                    if(path[j]!='"'){
                        Address[index]=path[j];
                        index++;
                        j++;
                    }
                    else{
                        j++;
                    }

                }
            }
      //      printf("%s\n",Address);

            for(int t=0;t<strlen(Address)-1;t++){
                CopyOfAddress[t]=Address[t];
            }
        //       printf("%s\n",CopyOfAddress);
            
            
            mkdir(CopyOfAddress, 0755);
        }
        index=0;
        //    for(int i=j-1;i<LenghtOfPath;i++){
        //        filename[index]=path[i+1];
        //        index++;
        //    }
        char* NewPathForFile=malloc(MAX_PATH_SIZE*sizeof(char));
        for(int x=2;x<LenghtOfPath-1;x++){
            if(path[x]!='"'){
                NewPathForFile[index]=path[x];
                index++;
            }
        }
    //    printf("%s",NewPathForFile);
        
        MakeTheFile(NewPathForFile);
        //    for(int i=0;i<index;i++){
        //        printf("%c",filename[i]);
        //    }
    }
    }

void MakeTheFile(char* path){
    FILE *file;
    file = fopen(path, "r");
    if(file!=0) {
        puts("This file alredy exists\n");
    }
    else {
        FILE *myfile = fopen(path, "w");
        fclose(myfile);
    }
       
}
void ReadFromFile(char path[]) {
    FILE *myfile = fopen(path, "r+");
    if(myfile == NULL) {
        puts("There's no such file!");
        return;
    }
    char c;
    while((c = fgetc(myfile)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
    fclose(myfile);
}
void Cat(char* command){
    char tag[MAX_COMMAND_SIZE];char sp;
    scanf("%s",tag);
    if(strcmp(tag, "--file")!=0){
        printf("Invalid syntax");
        return;
    }
    scanf("%c",&sp);
  
    char path[MAX_PATH_SIZE];char Newpath[MAX_PATH_SIZE];
    gets(path);
    int val=IsValidPath(path, Newpath);
    if(val==1){
        ReadFromFile(Newpath);
    }
    else{
        return;
    }
}
int IsValidPath(char* path,char* NewPathForFile){
    //I will handle "" later,this block is without considering whitespaces
    unsigned long LenghtOfPath=strlen(path);
    int NumberOfSlashes=0;char* Address=malloc(MAX_PATH_SIZE*sizeof(char));
    char* CopyOfAddress=malloc(MAX_PATH_SIZE*sizeof(char));
    
    
    for(int i=0;i<LenghtOfPath;i++){
        if(path[i]=='/'&&path[i-1]!='/')
            NumberOfSlashes++;
        
        
    }
    
    //   this block dosen't consider white spaces...
    int index=0;int j=0;
    if(path[0]!='"')
    {
    for(int i=0;i<NumberOfSlashes-1;i++){
        while(j<LenghtOfPath){
            
            if(path[j]=='/'){
                if(j!=0){
                    Address[index]='/';
                    index++;
                    j++;
                    break;
                    
                }
                else{
                    j++;
                }
                
            }
            
            else{
                if(path[j]!=' '){
                    Address[index]=path[j];
                    index++;
                    j++;
                }
                else{
                    j++;
                }
                
            }
        }
        
        for(int t=0;t<strlen(Address)-1;t++){
            CopyOfAddress[t]=Address[t];
        }
        //     printf("%s\n",CopyOfAddress);
        
        
      int b=  CheckValidDir(CopyOfAddress);
        if(b!=1){
            
            return -1;
        }
    }
    index=0;
    //    for(int i=j-1;i<LenghtOfPath;i++){
    //        filename[index]=path[i+1];
    //        index++;
    //    }
   
    for(int x=0;x<LenghtOfPath-1;x++){
        if(path[x+1]!=' '){
            NewPathForFile[index]=path[x+1];
            index++;
        }
    }
    //    printf("%s",NewPathForFile);
    
        int a=  CheckValidFile(NewPathForFile);
        return a;
    //    for(int i=0;i<index;i++){
    //        printf("%c",filename[i]);
    //    }
}
    else{
        int Count=0;
    //    printf("%d",LenghtOfPath);
        for(int i=0;i<NumberOfSlashes-1;i++){
            while(j<LenghtOfPath){
                
                if(path[j]=='/'){
                    if(j>1){
                        Address[index]='/';
                        index++;
                        j++;
                        Count++;
                        break;

                    }
                    else{
                        j++;
                        Count++;
                    }

                }

                else{
                    if(path[j]!='"'){
                        Address[index]=path[j];
                        index++;
                        j++;
                    }
                    else{
                        j++;
                    }

                }
            }
      //      printf("%s\n",Address);

            for(int t=0;t<strlen(Address)-1;t++){
                CopyOfAddress[t]=Address[t];
            }
        //       printf("%s\n",CopyOfAddress);
            
            
            int b=  CheckValidDir(CopyOfAddress);
              if(b!=1){
                  return -1;
              }
            
        }
        index=0;
    
        for(int x=2;x<LenghtOfPath-1;x++){
            if(path[x]!='"'){
                NewPathForFile[index]=path[x];
                index++;
            }
        }
    //    printf("%s",NewPathForFile);
        
      int a=  CheckValidFile(NewPathForFile);
        return a;
       
    }
    }



int CheckValidFile(char* path){
    FILE *file;
       file = fopen(path, "r");
       if(file!=0) {
           return 1;
       }
       else {
           printf("There's no such file!\n");
           return 0;
       }
    
}
int CheckValidDir(char* path){
    DIR* dir=opendir(path);
    if(dir){
        closedir(dir);
        return 1;
    }
    else if(ENOENT==errno){
        printf("There's no such Directory!\n");
        return 0;
    }
    else{
        printf("There was an issue during opening the directory!\n");
        return -1;
    }
}
