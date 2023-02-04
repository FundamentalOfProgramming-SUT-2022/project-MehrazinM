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
#include <stdbool.h>


//symbols and etc
#define MAX_COMMAND_SIZE 10000
#define MAX_FILE_NAME 10000
#define MAX_PATH_SIZE 10000
#define MAX_STR_SIZE 10000
#define CLIPBOARD "clipboard.txt"
#define GrepFileName ".grepNames.txt"
#define GrepContents ".grepText.txt"
//declaring global variables
char PrevChangedName[MAX_FILE_NAME];
int IndexFor_FoundOut;
//prototyping all the required functions
int WhatIsTheCommand(char* command);

void createfileWpath(char* path);

void createfile(char* command,int ShowMode);
void CreateDirectories(char* path,int ShowMode);
void MakeTheFile(char* path,int ShowMode);
void ReadFromFile(char* path);

void WriteToFile(char* pathSrc,char* pathDst);

void cat(char* command);
int IsValidPath(char* path,char* NewPathForFile,int ShowMode);
int CheckValidFile(char* path,int ShowMode);
int CheckValidDir(char* path,int ShowMode);

int Insertstr(char* command);
void InputWordByWord(char* Path);
void InputStr(char* str);
void PrepareStr(char* str,char* NewStr);
void WriteWPos(int line,int place,char* path,char* str);

int InputForRmWOq(char* path,char* Copyfilepath);
int InputRmWq(char* path,char* Copyfilepath);

void RemoveStrCommands(char* command);
void RemovestrF(int Size,char* path,int Line,int Place);
void RemovestrB(int Size,char* path,int Line,int Place);

void CopyStrInp(char* command);
void CopystrF(int size,char* path,int Line,int Place);
void CopystrB(int Size,char* path,int Line,int Place);

void CutStrInp(char* command);
void CutstrF(int size,char* path,int Line,int Place);
void CutstrB(int Size,char* path,int Line,int Place);

void PasteStr(char* command);

//void treeInp(char* command);
//void listFilesRecursively(char *basePath,int depth);

void AutoIndentInp(char* command);
void AutoIndent(char* path);

void InpTree(char* command);
void DirTree(char *basePath, const int root,int depth);

void CompareInp(char* command);
void CompareFiles(char* path1,char* path2);

void UndoInp(char* command);
void MakeTheFile4Undo(char* path);
void Undo(char* path,char* UndofilePath);
void pathOfXfile(char* path,char* Xfilepath);

void InputFind(char* command);
int IndexOfWords(char* path,int* indii);
int FindandAddPlaces(char* str,char* pattern,int* found_at,int HowManyPrevs);

void Grep(char* path,char* str);
void grepInp(char* command);



//void InputThePath(char* path);
int main(int argc, const char * argv[]) {
    char c;
    char command[MAX_COMMAND_SIZE];
    int CommandAsANumber;int a;
    while(strcmp(command,"exit")){
        
        scanf("%s",command);
        CommandAsANumber=WhatIsTheCommand(command);
        switch(CommandAsANumber){
            case -1:
                //In order to handle printing "invalid input" only once I should do a lot more ...
                while (1) {
                    c=getchar();
                    if(c=='\n'){
                        break;
                    }
                }
                printf("~%s~ is not a valid command!\n",command);
                break;
            case 0:
                return 0;
            case 1:
                createfile(command,1);
                break;
            case 2:
                cat(command);
                break;
            case 3:
              a= Insertstr(command);
                if(a==0){
                    break;
                }
                break;
            case 4:
                RemoveStrCommands(command);
                break;
            case 5:
                CopyStrInp(command);
                break;
            case 6:
                CutStrInp(command);
                break;
            case 7:
                PasteStr(command);
                break;
            case 8:
                AutoIndentInp(command);
                break;
            case 9:
                InpTree(command);
                break;
            case 10:
                CompareInp(command);
                break;
            case 11:
                UndoInp(command);
                break;
            case 12:
                InputFind(command);
                break;
            case 13:
                grepInp(command);
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
    else if(strcmp(command, "insertstr")==0){
        return 3;
    }
    else if(strcmp(command, "remove")==0){
        return 4;
    }
    else if(strcmp(command,"copystr")==0){
        return 5;
    }
    else if(strcmp(command,"cutstr")==0){
        return 6;
    }
    else if(strcmp(command,"pastestr")==0){
        return 7;
    }
    else if(strcmp(command,"auto-indent")==0){
        return 8;
    }
    else if(strcmp(command,"tree")==0){
        return 9;
    }
    else if(strcmp(command,"compare")==0){
        return 10;
    }
    else if(strcmp(command,"undo")==0){
        return 11;
    }
    else if(strcmp(command,"find")==0){
        return 12;
    }
    else if(strcmp(command,"grep")==0){
        return 13;
    }
   else{
       return -1;
   }
}

void createfile(char* command,int ShowMode){
    char TagOfCommand[MAX_COMMAND_SIZE];char spc;
    scanf("%s%c",TagOfCommand,&spc);
    if(strcmp(TagOfCommand,"--file")){
        printf("Invalid Input\n");
        return;
    }
    else{
       
        
        char path[MAX_PATH_SIZE];
        gets(path);
//        printf("%s",path);
       CreateDirectories(path, ShowMode);
    }
}
void createfileWpath(char* path){
   
       
//        printf("%s",path);
       CreateDirectories(path, 0);
    
}
void CreateDirectories(char* path,int ShowMode){
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
    
    MakeTheFile(NewPathForFile,ShowMode);
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
              printf("%s\n",CopyOfAddress);
            
            
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
        
        MakeTheFile(NewPathForFile,ShowMode);
        //    for(int i=0;i<index;i++){
        //        printf("%c",filename[i]);
        //    }
    }
    }

void MakeTheFile(char* path,int ShowMode){
    FILE *file;
    file = fopen(path, "r");
    if(file!=0) {
        if(ShowMode){
            puts("This file alredy exists\n");
        }
    }
    else {
        FILE *myfile = fopen(path, "w");
        fclose(myfile);
    }
       
}
void ReadFromFile(char path[]) {
    FILE *myfile = fopen(path, "r+");
    if(myfile == NULL) {
        puts("There's no such file!\n");
        return;
    }
    char c;
    while((c = fgetc(myfile)) != EOF) {
       printf("%c", c);
    
    }
    printf("\n");
    fclose(myfile);
}
void cat(char* command){
    char tag[MAX_COMMAND_SIZE];char sp;
    scanf("%s",tag);
    if(strcmp(tag, "--file")!=0){
        printf("Invalid syntax\n");
        return;
    }
    scanf("%c",&sp);
  
    char path[MAX_PATH_SIZE];char Newpath[MAX_PATH_SIZE];
    gets(path);
    int val=IsValidPath(path, Newpath, 1);
    if(val==1){
        ReadFromFile(Newpath);
    }
    else{
        return;
    }
}
int IsValidPath(char* path,char* NewPathForFile,int ShowMode){
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
        
        
      int b=  CheckValidDir(CopyOfAddress, ShowMode);
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
    
        int a=  CheckValidFile(NewPathForFile, ShowMode);
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
            
            
            int b=  CheckValidDir(CopyOfAddress, ShowMode);
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
        
      int a=  CheckValidFile(NewPathForFile, ShowMode);
        return a;
       
    }
    }



int CheckValidFile(char* path,int ShowMode){
    FILE *file;
       file = fopen(path, "r");
       if(file!=0) {
           return 1;
       }
       else {
           if(ShowMode){
               printf("There's no such file!\n");
           }
           return 0;
       }
    
}
int CheckValidDir(char* path,int ShowMode){
    DIR* dir=opendir(path);
    if(dir){
        closedir(dir);
        return 1;
    }
    else if(ENOENT==errno){
        if(ShowMode){
            printf("There's no such Directory!\n");
           
        }
        return 0;
    }
    else{
        if(ShowMode){
            printf("There was an issue during opening the directory!\n");
        }
        return -1;
    }
}
//void RemoveStr(char* command,int val){
//    char TagForCommand[1000];
//
//
//    scanf("%s",TagForCommand);
//    if(strcmp(TagForCommand,"--file")){
//        printf("Undefined keyword!\n");
//        val=0;
//        return;
//    }
//
//}
int Insertstr(char* command){

    char TagForCommand[MAX_COMMAND_SIZE];char TagForCommand1[MAX_COMMAND_SIZE];char TagForCommand2[MAX_COMMAND_SIZE];
    char sp;int val1,val2=1;
    scanf("%s%c",TagForCommand,&sp);
    if(strcmp(TagForCommand, "--file"))
        return 0;
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    //int i=0;
    char str[MAX_STR_SIZE];
    //char lineofinputs[MAX_STR_SIZE];char Word[MAX_STR_SIZE];
    int PlaceOfchar;int line;
   // InputWordByWord(filepath);
  //  scanf("%s%c", word,&sp);
    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = c;
    }
 //   printf("%s",filepath);


    val1=IsValidPath(filepath, Copyfilepath,1);
   
   //--str
    scanf("%s%c",TagForCommand1,&sp);

    InputStr(str);

    scanf("%s%c",TagForCommand2,&sp);
  //  printf("%s",TagForCommand);

    scanf("%d:%d",&line,&PlaceOfchar);
  //  printf("%d  %d",line,PlaceOfchar);
    //I'll check the validty later
    char NewStr[MAX_STR_SIZE];
   
//    if(val1==0){
//      //  printf("There must be an error in the address!\n");
//        return 0;
//    }
    if(strcmp(TagForCommand1,"--str")||strcmp(TagForCommand2, "--pos")){
        printf("Undefined keyword\n");
        val2=0;
    }
    if(val1==0||val2==0){
        return 0;
    }

    
    char undAdd[MAX_FILE_NAME];
    pathOfXfile(Copyfilepath, undAdd);

    MakeTheFile4Undo(Copyfilepath);
  //  printf("%s",Copyfilepath);
    WriteToFile(Copyfilepath, undAdd+1);

    PrepareStr(str,NewStr);
    WriteWPos(line, PlaceOfchar, Copyfilepath, NewStr);





    return 1;
}
void InputWordByWord(char* Path){
    char word[100000];char sp;
    scanf("%s%c", word,&sp);
    char c;
    c = getchar();
    if (c != '\"')
    {
        Path[0] = c;
        scanf("%s", Path + 1);
    }
    else
    {
        int i = 1;
        Path[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && Path[i - 1] != '\\'){
                 Path[i]=c;
                  break;

             }
            Path[i] = c;
            i++;
        }
        Path[i] = 0;
    }

}

void InputStr(char* str){
    char c;
      c = getchar();

      if (c != '\"')
      {
          str[0] = c;
          scanf("%s", str + 1);
      }
      else{
          int i = 0;
          while (1)
            {
                c = getchar();
                if (c == '\"' && str[i - 1] != '\\')
                    break;
                str[i] = c;
                i++;
                }
                str[i] = 0;

      }
}
void PrepareStr(char* str,char* NewStr){

    int i = 0;
    int j = 0;
    while (i < strlen(str))
    {
        if (str[i] == '\\')
        {
            i++;
            if (str[i] == 'n')
            {
                NewStr[j] = '\n';
            }
            else if (str[i] == '\"'|| str[i] == '\\')
            {
                NewStr[j] = str[i];
            }
            else
            {
                return;               }
           }
           else
               NewStr[j] = str[i];

           i++;
           j++;
       }
}
void WriteWPos(int line,int place,char* path,char* str){
 //   printf("%s",path);
    
    FILE* OrgFile=fopen(path,"r");
    
    
    
    
    FILE* TempFile=fopen("root/temp.txt", "w");int i=0;
    
   
    char ContentOfLine[MAX_STR_SIZE];char CharContent = '\0';

        while((fgets(ContentOfLine, 2000, OrgFile))!=NULL&&i<line-1){
            fprintf( TempFile,"%s", ContentOfLine);
            i++;
        }
   
    for(int i=0;i<place;i++){
        CharContent=fgetc(OrgFile);
       // ContentOfLine[i]=CharContent;
        fprintf(TempFile, "%c",CharContent);
    }
 //   printf("%s",ContentOfLine);
    strcat(str,ContentOfLine);
    fprintf(TempFile, "%s",str);
//    while (1)
//     {
//         CharContent=fgetc(OrgFile);
//         if(CharContent==EOF||CharContent=='\n')
//             break;
//         fprintf(TempFile, "%c",CharContent);
//     }
    while (fgets(ContentOfLine, 2000, OrgFile) != NULL)
     {
         fprintf( TempFile,"%s",ContentOfLine);
     }
     fclose(OrgFile);
     fclose(TempFile);


    OrgFile=fopen(path, "w");
    TempFile=fopen("root/temp.txt", "r");
    while((fgets(ContentOfLine, 2000, TempFile))!=NULL){
        fprintf( OrgFile,"%s",ContentOfLine);
    }
    fclose(OrgFile);
    fclose(TempFile);
    
    remove("root/temp.txt");
}

void RemoveStrCommands(char* command){
    char FileTag[MAX_COMMAND_SIZE];
    char PosTag[MAX_COMMAND_SIZE];
    char Mode[100];
    char SizeTag[MAX_COMMAND_SIZE];
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    int Place,Line,size;char sp;
    int IsQuot=0;

    //It must scan
    scanf("%s%c",FileTag,&sp);

    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {
        IsQuot=1;
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
     //   filepath[i] = 0;
    }
  //  printf("%s",filepath);
    scanf("%s%c",PosTag,&sp);
  //  printf("%s",PosTag);
    scanf("%d:%d ",&Line,&Place);
    scanf("%s%c",SizeTag,&sp);
    scanf("%d%c",&size,&sp);
    scanf("%s",Mode);
    if(strcmp("--file", FileTag)||strcmp("--pos", PosTag)||strcmp("-size", SizeTag)){
        printf("There must be an error in the inputs\n");
        return;
    }
    if(strcmp("-f",Mode)&&strcmp("-b", Mode)){
        printf("There must be an error in the inputs\n");
        return;
    }
//    int val=IsValidPath(filepath, Copyfilepath);
    int val;
    if(IsQuot){
        val=InputRmWq(filepath,Copyfilepath);
    }
    else{
        val=InputForRmWOq(filepath,Copyfilepath);
    }
    
    if(val==0){
        return;
    }
    
  //  printf("%s",Copyfilepath);
    strcpy(Copyfilepath, filepath+1);
    
    printf("%s",Copyfilepath);
    char undAdd[MAX_FILE_NAME];
    pathOfXfile(Copyfilepath, undAdd);

    MakeTheFile4Undo(Copyfilepath);
  //  printf("%s",Copyfilepath);
    WriteToFile(Copyfilepath, undAdd+1);
    if(!strcmp("-f", Mode)){
        RemovestrF(size,Copyfilepath ,Line, Place);
        
    }
    else{
        RemovestrB(size, Copyfilepath, Line, Place);


    }

    return;
}
void RemovestrF(int Size,char* path,int Line,int Place){
    //we assume the position is valid
    char ContentOfLine[MAX_STR_SIZE];char CharContent = '\0';
    int i=0;
    FILE* TempFile=fopen("root/temp.txt", "w");
    FILE* OrgFile=fopen(path,"r");
    
    while((fgets(ContentOfLine, 2000, OrgFile))!=NULL&&i<Line-1){
        fprintf( TempFile,"%s", ContentOfLine);
        i++;
    }
    for(int i=0;i<Place;i++){
        CharContent=fgetc(OrgFile);
        fprintf(TempFile, "%c",CharContent);

    }
    for(int i=0;i<Size;i++){
        CharContent=fgetc(OrgFile);
       
    }
  //  fprintf(TempFile,"%s", ContentOfLine);
    while((fgets(ContentOfLine, 2000, OrgFile))!=NULL){
        fprintf( TempFile,"%s", ContentOfLine);
        
    }
    fclose(OrgFile);
    fclose(TempFile);


   OrgFile=fopen(path, "w");
   TempFile=fopen("root/temp.txt", "r");
   while((fgets(ContentOfLine, 2000, TempFile))!=NULL){
       fprintf( OrgFile,"%s",ContentOfLine);
   }
   fclose(OrgFile);
   fclose(TempFile);
   
   remove("root/temp.txt");
    return;
    
}
void RemovestrB(int Size,char* path,int Line,int Place){
   ;int i=0;
    int lineLen[MAX_STR_SIZE];char content[MAX_STR_SIZE];

    if (Size<=Place+1){
        Place-=Size-1;
        RemovestrF(Size,path,Line,Place);
        return;
    }
    if (Line == 0){
        return RemovestrF(Size, path,Line,  Place - Size);
    }
    FILE* OrgFile=fopen(path,"r");
    
    while (i < Line)
        {
            fgets(content, 2000, OrgFile);
            lineLen[i] += strlen(content);
            if (content[strlen(content) - 1] == '\n')
                i++;
        }
    fclose(OrgFile);

  
    int t,s1=Size;
    int s2=Size-(Place+1);
    for( t=Line-1;t>0;t--){
        s1=s2;
        s2-=lineLen[t-1];
        if(s2<0){
            break;
        }
    }
    Line=t;
    Place=lineLen[t]-s1;
    return  RemovestrF(Size, path, Line, Place);
    
}

void CopyStrInp(char* command){
    char FileTag[MAX_COMMAND_SIZE];
    char PosTag[MAX_COMMAND_SIZE];
    char Mode[100];
    char SizeTag[MAX_COMMAND_SIZE];
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    int Place,Line,size;char sp;int IsQuot=0;

    //It must scan
    scanf("%s%c",FileTag,&sp);

    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {   IsQuot=1;
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = 0;
    }
    scanf("%s%c",PosTag,&sp);
  //  printf("%s",PosTag);
    scanf("%d:%d ",&Line,&Place);
    scanf("%s%c",SizeTag,&sp);
    scanf("%d%c",&size,&sp);
    scanf("%s",Mode);
    if(strcmp("--file", FileTag)||strcmp("--pos", PosTag)||strcmp("-size", SizeTag)){
        printf("There must be an error in the inputs\n");
        return;
    }
    if(strcmp("-f",Mode)&&strcmp("-b", Mode)){
        printf("There must be an error in the inputs\n");
        return;
    }
    
    int val;
    if(IsQuot){
        val=InputRmWq(filepath,Copyfilepath);
    }
    else{
        val=InputForRmWOq(filepath,Copyfilepath);
    }
    
    if(val==0){
        return;
    }
    if(!strcmp("-f", Mode)){
        CopystrF(size,Copyfilepath ,Line, Place);
        
    }
    else{
        CopystrB(size, Copyfilepath, Line, Place);


    }
    
    
}
void CopystrF(int size,char* path,int Line,int Place){
    FILE* TempFile=fopen(CLIPBOARD, "w");
    FILE* OrgFile=fopen(path,"r");
    char ContentOfLine[MAX_STR_SIZE];char CharContent = '\0';
    int i=0;
    while((fgets(ContentOfLine, 2000, OrgFile))!=NULL&&i<Line-1){
      //  fprintf( TempFile,"%s", ContentOfLine);
        i++;
    }
    for(int i=0;i<Place;i++){
        CharContent=fgetc(OrgFile);
        
      //  fprintf(TempFile, "%c",CharContent);

    }
    for(int i=0;i<size;i++){
        CharContent=fgetc(OrgFile);
      //  printf("%c",CharContent);
        fprintf(TempFile, "%c",CharContent);

       
    }
  //  fprintf(TempFile,"%s", ContentOfLine);
   
    fclose(OrgFile);
    fclose(TempFile);


  
   
   
    return;
}
void CopystrB(int Size,char* path,int Line,int Place){
    int i=0;
    int lineLen[MAX_STR_SIZE];char content[MAX_STR_SIZE];

    if (Size<=Place+1){
        Place-=Size-1;
        CopystrF(Size,path,Line,Place);
        return;
    }
    if (Line == 0){
        return CopystrF(Size, path,Line,  Place - Size);
    }
    FILE* OrgFile=fopen(path,"r");
    
    while (i < Line)
        {
            fgets(content, 2000, OrgFile);
            lineLen[i] += strlen(content);
            if (content[strlen(content) - 1] == '\n')
                i++;
        }
    fclose(OrgFile);

    
    int t,s1=Size;
    int s2=Size-(Place+1);
    for( t=Line-1;t>0;t--){
        s1=s2;
        s2-=lineLen[t-1];
        if(s2<0){
            break;
        }
    }
    Line=t;
    Place=lineLen[t]-s1;
    return  CopystrF(Size, path, Line, Place);
    
   


  
   
   
    return;
}
void CutStrInp(char* command){
    char FileTag[MAX_COMMAND_SIZE];
    char PosTag[MAX_COMMAND_SIZE];
    char Mode[100];
    char SizeTag[MAX_COMMAND_SIZE];
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    int Place,Line,size;char sp;int IsQuot=0;

    //It must scan
    scanf("%s%c",FileTag,&sp);

    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {   IsQuot=1;
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = 0;
    }
    scanf("%s%c",PosTag,&sp);
  //  printf("%s",PosTag);
    scanf("%d:%d ",&Line,&Place);
    scanf("%s%c",SizeTag,&sp);
    scanf("%d%c",&size,&sp);
    scanf("%s",Mode);
    if(strcmp("--file", FileTag)||strcmp("--pos", PosTag)||strcmp("-size", SizeTag)){
        printf("There must be an error in the inputs\n");
        return;
    }
    if(strcmp("-f",Mode)&&strcmp("-b", Mode)){
        printf("There must be an error in the inputs\n");
        return;
    }
    int val;
    
    if(IsQuot){
        val=InputRmWq(filepath,Copyfilepath);
    }
    else{
        val=InputForRmWOq(filepath,Copyfilepath);
    }
    
    
    if(val==0){
        return;
    }
    strcpy(Copyfilepath, filepath+1);
    char undAdd[MAX_FILE_NAME];
    pathOfXfile(Copyfilepath, undAdd);

    MakeTheFile4Undo(Copyfilepath);
  //  printf("%s",Copyfilepath);
    WriteToFile(Copyfilepath, undAdd+1);
    if(!strcmp("-f", Mode)){
        CutstrF(size,Copyfilepath ,Line, Place);
        
    }
    else{
        CutstrB(size, Copyfilepath, Line, Place);


    }
    
    
    
    
}
void CutstrF(int size,char* path,int Line,int Place){
    FILE* TempFile=fopen(CLIPBOARD, "w");
    FILE* OrgFile=fopen(path,"r");
    char ContentOfLine[MAX_STR_SIZE];char CharContent;
    int i=0;
    while((fgets(ContentOfLine, 2000, OrgFile))!=NULL&&i<Line-1){
      //  fprintf( TempFile,"%s", ContentOfLine);
        i++;
    }
    for(int i=0;i<Place;i++){
        CharContent=fgetc(OrgFile);
      //  fprintf(TempFile, "%c",CharContent);

    }
    for(int i=0;i<size;i++){
        CharContent=fgetc(OrgFile);
       // printf("%c",CharContent);
        
        fprintf(TempFile, "%c",CharContent);

       
    }
  //  fprintf(TempFile,"%s", ContentOfLine);
   
    fclose(OrgFile);
    fclose(TempFile);
    RemovestrF(size, path, Line, Place);


  
   
   
    return;
}
void CutstrB(int Size,char* path,int Line,int Place){
    int i=0;
    int lineLen[MAX_STR_SIZE];char content[MAX_STR_SIZE];

    if (Size<=Place+1){
        Place-=Size-1;
        CutstrF(Size,path,Line,Place);
        return;
    }
    if (Line == 0){
        return CopystrF(Size, path,Line,  Place - Size);
    }
    FILE* OrgFile=fopen(path,"r");
    
    while (i < Line)
        {
            fgets(content, 2000, OrgFile);
            lineLen[i] += strlen(content);
            if (content[strlen(content) - 1] == '\n')
                i++;
        }
    fclose(OrgFile);
  //  int sum = 0;
    int t,s1=Size;
    int s2=Size-(Place+1);
    for( t=Line-1;t>0;t--){
        s1=s2;
        s2-=lineLen[t-1];
        if(s2<0){
            break;
        }
    }
    Line=t;
    Place=lineLen[t]-s1;
    
    CopystrF(Size, path, Line, Place);
    
    return;
}
void PasteStr(char* command){
    char FileTag[MAX_COMMAND_SIZE];
    char PosTag[MAX_COMMAND_SIZE];
   
   
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    int Place,Line;char sp;

    //It must scan
    scanf("%s%c",FileTag,&sp);

    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = 0;
    }
    scanf("%s%c",PosTag,&sp);
  //  printf("%s",PosTag);
    scanf("%d:%d ",&Line,&Place);
 
    if(strcmp("--file", FileTag)||strcmp("--pos", PosTag)){
        printf("There must be an error in the inputs\n");
        return;
    }
   
    int val=IsValidPath(filepath, Copyfilepath,1);
    if(val==0){
        return;
    }
    char undAdd[MAX_FILE_NAME];
    pathOfXfile(Copyfilepath, undAdd);

    MakeTheFile4Undo(Copyfilepath);
  //  printf("%s",Copyfilepath);
    WriteToFile(Copyfilepath, undAdd+1);
    
    

    FILE *clipboard = fopen(CLIPBOARD, "r");
       if (clipboard == NULL)
       {
          printf("Nothing to Paste\n");
           return ;
       }
    char content[2000];
    while (1)
       {
           if (fgets(content, 2000, clipboard) == NULL)
               break;
//           if (WriteWPos(Line, Place, Copyfilepath, content) == -1)
//           {
//               printf("error\n");
//               fclose(clipboard);
//               return ;
//           }
           WriteWPos(Line, Place, Copyfilepath, content);
           if (command[strlen(content) - 1] == '\n')
           {
               Line++;
               Place = 0;
           }
           else
           {
               Place += strlen(content);
           }
       }

       fclose(clipboard);

}
void InpTree(char* command){
    int depth;
    scanf("%d",&depth);
    if(depth<-1){
        printf("Invalid\n");
        return;
    }
    char path[]="root/";
    DirTree(path, 0,depth);
    printf("----------------------\n");

    
}
void DirTree(char *basePath, const int root,int depth)
{
    
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;
    if(depth==0)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++)
            {
                if (i%2 == 0 || i == 0){
                    
                    printf("%s", "│");
                }
                else
                    printf(" ");

            }
            

            printf("%s%s\n","├─", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            DirTree(path, root + 2,depth-1);
        }
    }
    
    
  //  printf("----------------------");
   

    closedir(dir);
}








void AutoIndentInp(char* command){
    
  
    char sp;int val1,val2=1;
   
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
 
    
    char c;
    c = getchar();

    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = 0;
    }

    val1=IsValidPath(filepath, Copyfilepath,1);
   // printf("%s Copy:%s",filepath,Copyfilepath);
    if (val1!=1)
        return;
    //undo
    char undAdd[MAX_FILE_NAME];
    pathOfXfile(Copyfilepath, undAdd);

    MakeTheFile4Undo(Copyfilepath);
   // printf("%s",Copyfilepath);
    WriteToFile(Copyfilepath, undAdd+1);

    AutoIndent(Copyfilepath);
    return;
}
//void PrepFileForIndent(char* path){
//
//}
void AutoIndent(char* path){
    //char ContentOfLine[MAX_STR_SIZE];
    FILE* TempFile=fopen("temp2.txt", "w");
    FILE* OrgFile=fopen(path, "r");
    int op=0;
    char c,prev_c,last_non_wspace;
    while (1){
        prev_c=c;
        c=fgetc(OrgFile);
      //  printf("%c",c);
        if(c=='{'){
            last_non_wspace=c;
//            for(int i=0;i<op;i++){
//                fprintf(TempFile, "%c",' ');
//            }
//            op++;
//            fprintf(TempFile, "%c",c);
//            fprintf(TempFile, "%c",'\n');
//            c='\n';
            if(prev_c==' '){
                fprintf(TempFile, "%c",c);
                fprintf(TempFile, "%c",'\n');
                for(int i=0;i<op;i++){
                  fprintf(TempFile, "%s","    ");
                }
                 
                 
                 
                 
                // c='\n';
            }
            else{
                
                fprintf(TempFile, "%c",'\n');
                for(int i=0;i<op;i++){
                  fprintf(TempFile, "%s","    ");
                }
                fprintf(TempFile, "%c",c);
                
                prev_c=' ';
//                for(int i=0;i<op;i++){
//                  fprintf(TempFile, "%s","    ");
//                }
            }
            op++;


        }
        else if(c=='}'){
            last_non_wspace=c;
           // fprintf(TempFile, "%c",'\n');
            fprintf(TempFile, "%c",'\n');
            for(int i=op-1;i>0;i--){
                fprintf(TempFile,"%s","    ");
            }
            
            fprintf(TempFile, "%c",c);
            
            op--;


        }
        else if(c==EOF){
            break;
        }
        else{
            if(c==' '){
//                for(int i=0;i<op;i++){
//                  fprintf(TempFile, "%s","    ");
//                }
//                fprintf(TempFile, "%c",c);
                if((last_non_wspace=='{')||(last_non_wspace=='}')){
                    continue;
                }
                else{
                    fprintf(TempFile, "%c",c);
                }

            }
           
            else{
                if((last_non_wspace=='{')||(last_non_wspace=='}')){
                    for(int i=op-1;i>0;i--){
                        fprintf(TempFile,"%s","    ");
                    }
                }
                last_non_wspace=c;
                fprintf(TempFile, "%c",c);

            }
        }
        
        
    }
    fclose(TempFile);
    fclose(OrgFile);
    
  
    OrgFile=fopen(path, "w");
    TempFile=fopen("temp2.txt", "r");
    while(1){
        c=fgetc(TempFile);
        if(c==EOF){
            break;
        }

        fprintf( OrgFile,"%c",c);

    }
//    while((fgets(ContentOfLine, 2000, TempFile))!=NULL){
//        fprintf( OrgFile,"%s",ContentOfLine);
//    }

    fclose(OrgFile);
    fclose(TempFile);
    remove("temp2.txt");
}

void CompareInp(char* command){
    char sp;int val1,val2;
   
    char filepath1[MAX_PATH_SIZE];char Copyfilepath1[MAX_PATH_SIZE];
    char filepath2[MAX_PATH_SIZE];char Copyfilepath2[MAX_PATH_SIZE];

 
    
    char c;
    c = getchar();

    c = getchar();
    if (c != '\"')
    {
        filepath1[0] = c;
        scanf("%s", filepath1 + 1);
    }
    else
    {
        int i = 1;
        filepath1[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath1[i - 1] != '\\'){
                 filepath1[i]=c;
                  break;

             }
             printf("%c",c);
            filepath1[i] = c;
            i++;
        }
        filepath1[i] = '\0';
    }

    val1=IsValidPath(filepath1, Copyfilepath1,1);
    getchar();
    gets(filepath2);
    val2=IsValidPath(filepath2, Copyfilepath2,1);
    printf("%s %s\n",Copyfilepath1,Copyfilepath2);
    if (val1!=1||val2!=1){
        printf("%d %d\n",val1,val2);

        return;
    }
    
    CompareFiles(Copyfilepath1, Copyfilepath2);
    
}
void CompareFiles(char* path1,char* path2){
    FILE* file1=fopen(path1, "r");
    FILE* file2=fopen(path2, "r");
    int endf1=0,endf2=0;int line=1;
    char Cont1[MAX_STR_SIZE],Cont2[MAX_STR_SIZE];

    while(1){
        if (fgets(Cont1, 2000, file1) == NULL){
            endf1 = 1;
            
        }
        if (fgets(Cont2, 2000, file2) == NULL){
            endf2 = 1;
        }
        if (endf1 || endf2){
            break;
        }
        if (Cont1[strlen(Cont1) - 1] == '\n'){
            Cont1[strlen(Cont1) - 1] = 0;
        }
        if (Cont2[strlen(Cont2) - 1] == '\n'){
            Cont2[strlen(Cont2) - 1] = 0;
            
        }
        if(strcmp(Cont1,Cont2)){
            printf("============ #%d ============\n", line);
            printf("%s\n",Cont1);
            printf("%s\n",Cont2);
        }
        line++;
    }
    if(endf1&&endf2)
        return;
    int start=line;int end=line;
  
    if(!endf1&&endf2){
        
        while(fgets(Cont1, 2000, file1) != NULL){
            end++;
           
        }
     
        printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", start, end);
        fclose(file1);
        file1=fopen(path1, "r");
        int i=1;

        while(fgets(Cont1, 2000, file1)!= NULL){
           // printf("%d",i);
            if(i>=start){
                printf("%s\n",Cont1);
            }
            i++;
        }
        fclose(file1);
        return;
    }
    if(endf1&&!endf2){
        

        while(fgets(Cont2, 2000, file2) != NULL){
            end++;
            
        }
        fclose(file2);
        file2=fopen(path2, "r");
        int i=1;

        while(fgets(Cont2, 2000, file2)!= NULL){
          //  printf("%d",i);

            if(i>=start){
                printf("%s\n",Cont2);
            }
            i++;
        }
        fclose(file2);
        return;
    }
       

    
    
    return;
}

int InputForRm(char* path){
    unsigned long LenghtOfPath=strlen(path);
    int NumberOfSlashes=0;char* Address=malloc(MAX_PATH_SIZE*sizeof(char));
    char* CopyOfAddress=malloc(MAX_PATH_SIZE*sizeof(char));
    
    
    for(int i=0;i<LenghtOfPath;i++){
        if(path[i]=='/'&&path[i-1]!='/')
            NumberOfSlashes++;
        
        
    }
    
    //   this block dosen't consider white spaces...
    int index=0;int j=0;
 
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
        
        
      int b=  CheckValidDir(CopyOfAddress,1);
        if(b!=1){
            
            return -1;
        }
    }
    index=0;
    //    for(int i=j-1;i<LenghtOfPath;i++){
    //        filename[index]=path[i+1];
    //        index++;
    //    }
//
//    for(int x=0;x<LenghtOfPath-1;x++){
//        if(path[x+1]!=' '){
//            NewPathForFile[index]=path[x+1];
//            index++;
//        }
//    }
    //    printf("%s",NewPathForFile);
   
        int a=  CheckValidFile(path+1,1);
        return a;
    //    for(int i=0;i<index;i++){
    //        printf("%c",filename[i]);
    //    }

    
    
}
int InputRmWq(char* path,char* Copyfilepath){
    unsigned long LenghtOfPath=strlen(path);
    int NumberOfSlashes=0;char* Address=malloc(MAX_PATH_SIZE*sizeof(char));
    char* CopyOfAddress=malloc(MAX_PATH_SIZE*sizeof(char));
    
    
    for(int i=0;i<LenghtOfPath;i++){
        if(path[i]=='/'&&path[i-1]!='/')
            NumberOfSlashes++;
        
        
    }
    int Count=0;int index=0;int j=0;
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
        
        
        int b=  CheckValidDir(CopyOfAddress,1);
          if(b!=1){
              return -1;
          }
        
    }
    index=0;

//    for(int x=2;x<LenghtOfPath-1;x++){
//        if(path[x]!='"'){
//            NewPathForFile[index]=path[x];
//            index++;
//        }
//    }
//    printf("%s",NewPathForFile);
    path=path+1;
    path=path+1;
    path[strlen(path)-1]='\0';
    
   
  int a=  CheckValidFile(path,1);
    strcpy(Copyfilepath, path);
    return a;
   
   
}
int InputForRmWOq(char* path,char* Copyfilepath){
    unsigned long LenghtOfPath=strlen(path);
    int NumberOfSlashes=0;char* Address=malloc(MAX_PATH_SIZE*sizeof(char));
    char* CopyOfAddress=malloc(MAX_PATH_SIZE*sizeof(char));
    
    
    for(int i=0;i<LenghtOfPath;i++){
        if(path[i]=='/'&&path[i-1]!='/')
            NumberOfSlashes++;
        
        
    }
    
    //   this block dosen't consider white spaces...
    int index=0;int j=0;
 
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
     
        
        //    printf("%s\n",CopyOfAddress);
        
        
      int b=  CheckValidDir(CopyOfAddress,1);
        if(b!=1){
            
            return -1;
        }
    }
    index=0;

   
    int a=  CheckValidFile(path+1,1);
   
    
        return a;
   
    
}
void MakeTheFile4Undo(char* path){
   char arr[1000];
//    strcat(arr,path);
    strcpy(arr, path);
    arr[0]='/';arr[1]='u';arr[2]='n';arr[3]='d';
  //  printf("%s",arr);
    createfileWpath(arr);
  //  printf("%s","success");
}
void pathOfXfile(char* path,char* Xfilepath){
        char arr[1000]="undo/";
    //    strcat(arr,path);
        strcpy(arr, path);
        arr[0]='/';arr[1]='u';arr[2]='n';arr[3]='d';
   //     printf("%s",arr);
  // printf("path:%s arr:%s\n",path,arr);
    strcpy(Xfilepath, arr);
}
void UndoInp(char* command){
    //action same as cat and etc
    char tag[MAX_COMMAND_SIZE];char sp;
    scanf("%s",tag);int v=1;
    if(strcmp(tag, "--file")!=0){
       v=0;
        
    }
    scanf("%c",&sp);
  
    char path[MAX_PATH_SIZE];char Newpath[MAX_PATH_SIZE];char Expath[MAX_PATH_SIZE];char Ex_path[MAX_PATH_SIZE];
    gets(path);
    if(v!=1){
        printf("Undefined keyword!\n");
        return;
    }
    int val=IsValidPath(path, Newpath,1);
    if (val!=1){
        return;
    }
    pathOfXfile(Newpath, Expath);
     val=IsValidPath(Expath, Ex_path,0);
    if (val!=1){
        printf("It seems there has been no changes made into this file!\n");
        return;
    }
    Undo(Newpath, Ex_path);
    
}
void Undo(char* path,char* UndofilePath){
    char tempfile[]="TempUndo.txt";
    WriteToFile(path, tempfile);
    WriteToFile(UndofilePath, path);
    WriteToFile(tempfile, UndofilePath);
    remove("TempUndo.txt");
}
void WriteToFile(char* pathSrc,char* pathDst){
    char line[MAX_STR_SIZE];
    FILE* FileW=fopen(pathDst, "w");
    FILE* FileR=fopen(pathSrc, "r");
    while((fgets(line, 2000, FileR))!=NULL){
        fprintf(FileW, "%s",line);
    }
    fclose(FileW);
    fclose(FileR);
    //or we could use temp? which is not cool:(
}



void InputFind(char* command){
    IndexFor_FoundOut=0;
    char StrTag[MAX_COMMAND_SIZE];
    char FileTag[MAX_COMMAND_SIZE];
    char opt[MAX_COMMAND_SIZE];
    char filepath[MAX_PATH_SIZE];char Copyfilepath[MAX_PATH_SIZE];
    char str1[MAX_STR_SIZE]; char str[MAX_STR_SIZE];
    int Place,Line;char sp;
    int all,byword,at,count;
    int filecmd,strcmd;int posi,SizeFound;
    scanf("%s%c",FileTag,&sp);
    filecmd=strcmp("--file", FileTag);
    char c;
    c = getchar();
    if (c != '\"')
    {
        filepath[0] = c;
        scanf("%s", filepath + 1);
    }
    else
    {
        int i = 1;
        filepath[0]='"';
         while (1)
         {
             c = getchar();
             if (c == '\"' && filepath[i - 1] != '\\'){
                 filepath[i]=c;
                  break;

             }
            filepath[i] = c;
            i++;
        }
        filepath[i] = c;
    }
   
   // printf("%s",filepath);
   
    scanf("%s%c",StrTag,&sp);
    //problems with \*  should make a new inputstr!
    InputStr(str);
   //  printf("%s",str);
   
   // printf("%s",str);
    count =0; all = 0;byword = 0;at = 0;
    if (getchar() != '\n')
    {
        char att[30];
      
        count =0; all = 0;byword = 0;at = 0;
        do
        {
            scanf("%s", att);
         //   printf("%s",att);
            if (!strcmp(att, "-at"))
            {
                at = 1;
                scanf("%d",&posi);
            }
            else if (!strcmp(att, "-count"))
            {
                count = 1;
            }
            else if (!strcmp(att, "-all"))
            {
                all = 1;
            }
            else if (!strcmp(att, "-byword"))
            {
                byword=1;
            }
        } while (getchar() != '\n');
    }
  //  printf("%d %d %d %d\n",count,at,all,byword);
    int val=IsValidPath(filepath, Copyfilepath,1);
    strcpy(filepath+1, Copyfilepath);
    if(val!=1){
        return;
    }
    int indii[100000];
    int size=0;
    size=IndexOfWords(Copyfilepath, indii);
  //  printf("%d\t",size);
    FILE* TheFile=fopen(Copyfilepath, "r");
    char content[MAX_STR_SIZE];int PrevCount=0;
    int FoundAt[10000] ;
//    for(int i=0;i<size;i++){
//        printf("%d\t",indii[i]);
//    }
    //this func is not working
    while((fgets(content, 2000, TheFile))!=NULL){
        SizeFound=FindandAddPlaces(content, str, FoundAt, PrevCount);
//        printf("%s\n",content);
//        printf("%d\t",SizeFound);
        PrevCount+=strlen(content);
//            for(int i=0;i<SizeFound;i++){
//                printf("%d found at: %d\n",i,FoundAt[i]);
//            }
        
    }
//    for(int i=0;i<SizeFound;i++){
//        printf("%d\t",indii[i]);
//    }
  //  printf("SizeFound:%d\n",SizeFound);
    if (!SizeFound){
        FoundAt[0]=-1;
    }
    if(count==0&&byword==0&&at==0&&all==0){
        printf("%d\n",FoundAt[0]);
        return;
    }
    else if(count==1&&at==0&&all==0){
        int c= SizeFound;
        printf("%d\n",c);
        return;
    }
    else if(all==1&&count==0&&byword==0&&at==0){
        unsigned long c=SizeFound;
        if(FoundAt[0]==-1){
            printf("Nothing is found!\n");
            return;
        }
        for(int i=0;i<c;i++){
            printf("%d | ",FoundAt[i]);
            
        }
        printf("\n");
        return;
    }
    else if(all==0&&count==0&&byword==0&&at==1){
        if(FoundAt[0]==-1){
            printf("Nothing is found!\n");
            return;
        }
        if(posi>SizeFound){
            printf("There isn't this many matches in the file!\n");
            return;

        }
        printf("%d\n",FoundAt[posi-1]);
        return;
    }
    else if(count==0&&byword==1&&at==0&&all==0){
        if(FoundAt[0]==-1){
            printf("Nothing is found!\n");
            return;
        }
        int s=size;
        for(int i=0;i<s;i++){
            if(indii[i]<=FoundAt[0]&&indii[i+1]>FoundAt[0]){
                printf("%d\n",i+1);
                return;
            }
        }
    }
    else if(count==0&&byword==1&&at==1&&all==0){
        if(FoundAt[0]==-1){
            printf("Nothing is found!\n");
            return;
        }
        if(posi>SizeFound){
            printf("There isn't this many matches in the file!\n");
            return;
        }
        int s=SizeFound;
        for(int i=0;i<s;i++){
            if(indii[i]<=FoundAt[posi-1]&&indii[i+1]>FoundAt[posi-1]){
                printf("%d\n",i+1);
                break;
            }
        }
        
        
        return;
    }
    else if(count==0&&byword==1&&at==0&&all==1){
        if(FoundAt[0]==-1){
            printf("Nothing is found!\n");
            return;
        }
        int s=size;
        int c=SizeFound;
        for(int i=0;i<c;i++){
            for(int j=0;j<s;j++){
                if(indii[j]==FoundAt[i]){
                    printf("<<%d>>\t",j+1);
                }
            }
        }
        printf("\n");
        return;
    }
    else{
        printf("sorry,this options can't be performed at a mean time!\n");
        return;
    }
        
        return;
    }




int IndexOfWords(char* path,int* indii){
    FILE* myfile=fopen(path, "r");int Size=0;
    char cont,prev='a';int i=0;int x=0;
    while((cont=fgetc(myfile))!=EOF){
        if(x==0){
            indii[0]=0;
            x++;
            i++;
            prev=cont;
            Size++;
        }
        else if((cont!=' '&&cont!='\n')&&(prev==' '||prev=='\n')){
            indii[x]=i;
            x++;
            i++;
            prev=cont;
            Size++;
        }
        else{
            i++;
            prev=cont;
        }
       
        
    }
    fclose(myfile);
    return Size;
}
int FindandAddPlaces(char* str,char* pattern,int* found_at,int HowManyPrevs){
    
    char* b;
    b=strstr(str, pattern);
    if(b==NULL){
       // found_at[0]=-1;
        return IndexFor_FoundOut ;
    }
    int c=str;
    char* a;int k;char *arr;
    while(true){
        a=strstr(str, pattern);
        if(a==NULL){
            
            break;
        }
        
        k=a;
        found_at[IndexFor_FoundOut]=(k-c)+HowManyPrevs;
        IndexFor_FoundOut++;
        arr=str+k/sizeof(char);
        str=a;
        str=str+strlen(pattern);
        //        printf("%d  \n",k-c);
        //        printf("%s is str\n",str);
    }
    return IndexFor_FoundOut;
}

void grepInp(char* command){
    char TagForCommand[1000];char sp;char fline[20000];int i=0;
    char str[MAX_STR_SIZE];
    scanf("%s%c",TagForCommand,&sp); char c;char filepath[10000];
    if(!strcmp("--str",TagForCommand)){
        
        InputStr(str);
        sp=getchar();
        scanf("%s%c",TagForCommand,&sp);
       // printf("sp is:%c",sp);
        if(strcmp(TagForCommand, "--files")){
            gets(fline);
            return;
        }
        FILE* names=fopen(GrepFileName, "w");
        fclose(names);
        FILE* contents=fopen(GrepContents, "w");
        fclose(contents);
        while(1){
        //filepath=frepath;
           
        c = getchar();
        if (c != '\"')
        {
            filepath[0] = c;
            scanf("%s", filepath + 1);
        }
        else
        {
            int i = 1;
            filepath[0]='"';
            while (1)
            {
                c = getchar();
                if (c == '\"' && filepath[i - 1] != '\\'){
                    filepath[i]=c;
                    break;
                    
                }
                filepath[i] = c;
                i++;
            }
            filepath[i] = c;
        }
          //  printf("%s\n",filepath);
            //grep action
            c=getchar();
            Grep(filepath,str);
            if(c=='\n')
                break;
    }
        FILE* grep1=fopen(GrepContents, "r");
        while ((fgets(fline, 4000, grep1))!=NULL) {
            printf("%s",fline);
        }
        fclose(grep1);
    }
    else if(!strcmp("-l", TagForCommand)){
        //option line /get --str and the string
        scanf("%s%c",TagForCommand,&sp);
        if(strcmp("--str",TagForCommand)){
            gets(fline);
            printf("%s is an invalid keyword\n",TagForCommand);
            return;
        }
        InputStr(str);
        sp=getchar();
        scanf("%s%c",TagForCommand,&sp);
      //  printf("sp is:%c",sp);
        if(strcmp(TagForCommand, "--files")){
            gets(fline);
            return;
        }
        FILE* names=fopen(GrepFileName, "w");
        fclose(names);
        FILE* contents=fopen(GrepContents, "w");
        fclose(contents);
        while(1){
        //filepath=frepath;
           
        c = getchar();
        if (c != '\"')
        {
            filepath[0] = c;
            scanf("%s", filepath + 1);
        }
        else
        {
            int i = 1;
            filepath[0]='"';
            while (1)
            {
                c = getchar();
                if (c == '\"' && filepath[i - 1] != '\\'){
                    filepath[i]=c;
                    break;
                    
                }
                filepath[i] = c;
                i++;
            }
            filepath[i] = c;
        }
          //  printf("%s\n",filepath);
            //grep action
            c=getchar();
            Grep(filepath,str);
            if(c=='\n')
                break;
    }
        FILE* grep1=fopen(GrepFileName, "r");
        while ((fgets(fline, 4000, grep1))!=NULL) {
            printf("%s",fline);
        }
        fclose(grep1);
       // printf("%d\n",i);
    }
    else if(!strcmp("-c", TagForCommand)){
        scanf("%s%c",TagForCommand,&sp);
        if(strcmp("--str",TagForCommand)){
            gets(fline);
            printf("%s is an invalid keyword\n",TagForCommand);
            return;
        }
        InputStr(str);
        sp=getchar();
        scanf("%s%c",TagForCommand,&sp);
//        printf("sp is:%c",sp);
        if(strcmp(TagForCommand, "--files")){
            gets(fline);
            return;
        }
        FILE* names=fopen(GrepFileName, "w");
        fclose(names);
        FILE* contents=fopen(GrepContents, "w");
        fclose(contents);
        while(1){
        //filepath=frepath;
           
        c = getchar();
        if (c != '\"')
        {
            filepath[0] = c;
            scanf("%s", filepath + 1);
        }
        else
        {
            int i = 1;
            filepath[0]='"';
            while (1)
            {
                c = getchar();
                if (c == '\"' && filepath[i - 1] != '\\'){
                    filepath[i]=c;
                    break;
                    
                }
                filepath[i] = c;
                i++;
            }
            filepath[i] = c;
        }
          //  printf("%s\n",filepath);
            //grep action
            c=getchar();
            Grep(filepath,str);
            if(c=='\n')
                break;
          }
        FILE* grep1=fopen(GrepContents, "r");
        while ((fgets(fline, 4000, grep1))!=NULL) {
            if(fline[0]!='\n'){
                i++;
            }
        }
        fclose(grep1);
        printf("%d\n",i);
    }
    

    else{
        gets(fline);
        printf("%s is an invalid keyword\n",TagForCommand);
        return;
        
    }
   
    
    
}
void Grep(char* path,char* str){
    char cpFilePath[10000];char* valid;int flag;
    int val=IsValidPath(path, cpFilePath, 0);
    if(val!=1){
        FILE* Res=fopen(GrepContents,"a");
        fprintf(Res, "%s",path);
        fprintf(Res, "%s"," : Is not a valid address!\n");
        fclose(Res);
        return;
    }
    //cpFilePath=(path+1);
    strcpy(cpFilePath, path+1);
    FILE* Res=fopen(GrepContents,"a");
    FILE* myfile=fopen(cpFilePath, "r");
    FILE* names=fopen(GrepFileName, "a");
    char content[MAX_STR_SIZE];
    while(fgets(content, 4000, myfile)!=NULL){
        valid=strstr(content, str);
        if(valid!=NULL){
            flag=1;
            fprintf(Res, "%s",path);
            fprintf(Res, "%s"," : ");
            fprintf(Res, "%s",content);
            fprintf(Res, "%s","\n");
//            fprintf(names, "%s",path);
//            fprintf(Res, "%s","\n");
        }
    }
    if(flag==1){
        fprintf(names, "%s",path);
        fprintf(names, "%s","\n");
    }
    fclose(myfile);
    fclose(Res);
    fclose(names);
    
    return;
    
}
