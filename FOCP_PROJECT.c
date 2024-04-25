#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
FILE *fp;                     //Global file pointer
                        // ALL FUNCTIONS USED
void addnote();                               //function for adding a new note
void viewnote();                              //function for viewing the notes
void editnote();                              //function for editing the notes
void deletenote();                            //function for deleting the notes
void memoryAdjust(char**, int*, int);         //function using malloc to adjust input string size
void STF(const char *, FILE *);               //function for copying string into file
void saveNoteNametoFile(char[]);              //function for saving the Note Names to File
void printNoteNames();                        //function for printing all note names
void ErrorOpeningNote(FILE*);                 //function for exiting program if file opening fails
void ErrorDuringMemoryAlloc(char*);           //function for exiting program if error during memory allocation
void options();                               //function to return to main menu
void removeLastNewLine(char *str);            //function to remove last newline character automatically input with Enter
void removeTrailingNewLine(const char *);     //function to remove last newline character after editig note
void checkSuccessfulDelete(const char *);     //function to remove & check if note has successfully been deleted
void removeFromNoteName(char[]);              //function to remove the deleted note name from file
void TruncateLastNewLine();                   //function to truncate last new line left after deletion of note  
void checkIfNoteExists(char[]);               //function to check if note exists while opening it to edit
void SelectOption(char[]);                    //function to Select Option duing program
char *checkForInputInt(char[]);               //function to check if user entered number instead of string

                                    // MAIN FUNCTION
int main()
{

    int input;
    char ch[50];
    printf("\033[1;37;44mWelcome to the Terminal-based Notes App Project created by Ali.\033[0m\n");
    printf("\n\033[1;31;45mChoose your desired function(-1 to Exit):\033[0m\n\n\033[1m***1.ADD***\n***2.VIEW***\n***3.EDIT***\n***4.DELETE***\n\033[0m");
    printf("\nEnter Option: ");
    if (scanf("%d", &input) == 1){
    switch (input){
    case (1):{
        addnote();
        break;
    }
    case (2):{
        viewnote();
        break;
    }
    case (3):{
        editnote();
        break;
    }
    case (4):{
        deletenote();
        break;
    }
    case (-1):{
        exit(1);
    }
    default:{
        printf("\033[31;41mInvalid input. Try again.\033[0m\n\n");
        main();
        break;
        }
    }
}
    else if(scanf(" %[^\n]", ch) == 1)
        SelectOption(ch);
}

void addnote(){

    char filename[50], *inputString;
    
    // Getting name of the new note 
    printf("\nEnter name of note: ");
    scanf(" %[^\n]", filename);
    
    // putting filename into NoteNames
    saveNoteNametoFile(filename);

    // adding .txt extension to the filename
    strcat(filename, ".txt");

    getchar(); //consume the newline characte left in buffer

    // Opening the new content
    fp = fopen(filename, "w+");
    
    ErrorOpeningNote(fp);                   
    
    int bufferSize = 100;                   //using malloc() for dynamic memory allocation
    inputString = (char *)malloc(bufferSize * sizeof(char));

    ErrorDuringMemoryAlloc(inputString);    //printing error and exiting program if memory alloc fails
    
    printf("\n\033[1;43mEnter content (-1 to save and exit):\033[0m\n\n");

    // Adjusting memory stuff
    int c, index = 0;
    while (1) {
        if (fgets(inputString + index, bufferSize - index, stdin) == NULL) {
        // Check for end-of-file or error
            if (feof(stdin)) {
            // End of file reached, break out of the loop
            break;}
            
            else {
            // Handle error, print a message, and exit or take appropriate action
            fprintf(stderr, "\033[1;41mError reading input.\033[0m\n");
            exit(1);
        }
    }
        // Check for EOF
        if (strchr(inputString + index, '\n') != NULL) {

            // Newline character encountered
            if(strcmp(inputString + index, "-1\n") == 0){
        
                // User entered -1, remove -1 and break out of the loop
                char *substringPos = strstr(inputString + index, "-1\n");
                if(substringPos!= NULL){
                    *substringPos = '\0';
            }
            removeLastNewLine(inputString);
            break;}
    }

    // No newline character found, adjust memory
    memoryAdjust(&inputString, &bufferSize, index);
    index += strlen(inputString + index);
}
    // writing the string into the file
    STF(inputString, fp);

    fclose(fp);
    
    free(inputString);
    
    options();
}

void viewnote(){
    char lastChar;
    char str[100], NN[100];
    int input[50];
    printf("\n\033[1mWhich note do you want to view?\033[0m\n");
    printf("\n\033[1;46mAvailable Notes:\033[0m\n\n");

    TruncateLastNewLine();                  //function to truncate last newline character in NoteNames

    printNoteNames();                       //print the Names of Notes on screen
    
    printf("\n\033[1;34mEnter name of note that you want to view:\033[0m ");

    scanf(" %[^\n]", NN);

    char *result = checkForInputInt(NN);

    strcpy(NN, result); 

    checkIfNoteExists(NN);

    puts("\n**********");
    strcat(NN, ".txt");

    fp = fopen(NN, "r");

    ErrorOpeningNote(fp);                   //if file opening failed


    while (fgets(str, 100, fp) !=NULL){     //reading content from file
        printf("%s", str);
    }
    fclose(fp);

    puts("\n**********");

    options();
}

void editnote(){

    char str[100], NN[50], command[100];
    printf("\n\033[1;35mWhich note do you want to edit?\033[0m\n\n");

    printNoteNames();                      //print the Names of Notes on screen
    
    printf("\n\033[1;34mEnter name of note that you want to edit:\033[0m ");

    scanf(" %[^\n]", NN);

    char *result = checkForInputInt(NN);

    strcpy(NN, result); 

    checkIfNoteExists(NN);
    
    strcat(NN, ".txt");

    fp = fopen(NN, "a+");
    fseek(fp, SEEK_END, 0);
    fclose(fp);

    snprintf(command, sizeof(command), "nano \"%s\"", NN);
    system(command);

    removeTrailingNewLine(NN);              //remove the automatic last new line after editing

    printf("\n\033[1;42mSuccessfully Edited Note.\033[0m\n");
    
    options();
}

void deletenote(){
    char NoteName[100];
    
    printf("\033[1;46mCurrent Notes:\033[0m\n\n");
    
    printNoteNames();
    
    printf("\n\033[1;35mEnter Name of Note you want to delete:\033[0m ");

    scanf(" %[^\n]", NoteName);

    char *result = checkForInputInt(NoteName);

    strcpy(NoteName, result);

    checkIfNoteExists(NoteName);

    removeFromNoteName(NoteName);           //function to remove note from NoteNames

    strcat(NoteName, ".txt");

    checkSuccessfulDelete(NoteName);        //function to remove note & check if note deletion is successful or not
    
    TruncateLastNewLine();                  //function to truncate last newline character in NoteNames

    options();

}

// Function to dynamically adjust the size of buffer using malloc
void memoryAdjust(char **inputString, int *bufferSize, int index){
    if (index >= *bufferSize - 1)
        *bufferSize *= 2;
    *inputString = realloc(*inputString, *bufferSize * sizeof(char));

    if (*inputString==NULL){
        printf("\033[1;41mError adjusting size of string.\033[0m");
        exit(1);
    }
}

// Function to write string into file/note
void STF(const char *inputString, FILE *fp){
    fputs(inputString, fp);
    printf("\n\033[1;42mContent successfully written into note.\033[0m\n");
}

// function to save NoteNames to file
void saveNoteNametoFile(char filename[])
{
    fp=fopen("NoteNames.txt", "a+");
    fputs("\n", fp);
    fputs(filename, fp);
    fclose(fp);
}

// function to display notes
void printNoteNames(){
    int count = 1;
    char ch;
    fp=fopen("NoteNames.txt", "r");

    ErrorOpeningNote(fp);

    rewind(fp);

    while (!feof(fp)){
        printf("%d. ", count);
        while ((ch=fgetc(fp)) != '\n' && ch != EOF){
            printf("%c", ch);
        }
        printf("\n");
        count += 1;
    }
    fclose(fp);
}

// function to print error and exit program if there is error in opening note
void ErrorOpeningNote(FILE *fp){
    if (fp == NULL){                        //exit program condition if file opening fails
        printf("\033[1;31mError opening note.\033[0m\n");
        exit(1);
        
    }
}

// function to print error if memory allocation fails
void ErrorDuringMemoryAlloc(char *inputString){
        if (inputString == NULL){
        printf("\033[1;31mError during memory allocation.\033[0m");
        fclose(fp);
        exit(1);
    }
}

// function to display options
void options(){
    int option;
    printf("\n\033[1;47mEnter 00 for Main Menu, -1 for Exit.\033[0m\n");
    while(1){
        scanf("%d", &option);
    if (option == 00){
        break;
    }
    else if (option == -1){
        exit(1);
    }
    else 
    printf("\033[1;41mInvalid input. Try again.\033[0m\n");
    }

    if (option == 00){
        main();
    }
    else
        exit;
}

// function to remove the last newline character while taking input
void removeLastNewLine(char *str){
    char *lastNewLinePos = NULL;

    // Find all newline characters and update the lastNewLinePosition
    char *newlinePos = strchr(str, '\n');
    while (newlinePos != NULL){
        lastNewLinePos = newlinePos;
        newlinePos = strchr(newlinePos+1, '\n');
    }

    // Remove the last newline character
    if (lastNewLinePos != NULL){
        *lastNewLinePos = '\0';
    }
}

// function to remove the last automatic newline after editing
void removeTrailingNewLine(const char *filename){
    fp = fopen(filename, "r+");
    ErrorOpeningNote(fp);

    fseek(fp, -1, SEEK_END);
    char lastChar = fgetc(fp);

    if (lastChar == '\n'){
        fseek(fp, -1, SEEK_END);
        ftruncate(fileno(fp), ftell(fp));   //Remove the last character
    }
    fclose(fp);

}

void checkSuccessfulDelete(const char *filename){
    if (remove(filename) == 0){
        printf("\n\033[1;42mNOTE SUCCESSFULLY DELETED.\033[0m\n");
    }
    else {
        printf("\033[1;41mError deleting Note.\033[0m\n");
    }
}

void removeFromNoteName(char filename[]) {
    fp = fopen("NoteNames.txt", "r");
    ErrorOpeningNote(fp);

    FILE *tempFile = fopen("temp.txt", "w");
    ErrorOpeningNote(tempFile);
    
    char line[100];  // Adjust the size based on your needs

    // Read lines from the original file and write back only those that don't match the specified name
    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t lineLength = strlen(line);

        // Check if the line ends with a newline character and exclude it
        if (lineLength > 0 && line[lineLength - 1] == '\n') {
            line[lineLength - 1] = '\0';
            lineLength--;  // Reduce the length to exclude the newline character
        }

        // Check if the line is not empty after removing the newline character
        if (lineLength > 0 && strstr(line, filename) == NULL) {
            fputs(line, tempFile);

            // Add a newline character after each line (except the last one)
            if (lineLength > 0) {
                fputc('\n', tempFile);
            }
        }
    }

    fclose(fp);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("NoteNames.txt");
    rename("temp.txt", "NoteNames.txt");
}

void TruncateLastNewLine(){
    char lastChar;
        fp = fopen("NoteNames.txt", "a+");
    fseek(fp, -1, SEEK_END);
    // read the last character
    lastChar = fgetc(fp);
    if (lastChar == '\n'){
        // Move back one position and remove the newline character
        fseek(fp, -1, SEEK_CUR);
        // Truncate the file to remove the last character
        ftruncate(fileno(fp), ftell(fp));
    }
    fclose(fp);

}

void checkIfNoteExists(char NN[]){
    int input;
    char str[1000];
    fp = fopen("NoteNames.txt", "r");
    while (fgets(str, sizeof(str), fp) != NULL){
        if (strstr(str, NN) !=NULL){
            fclose(fp);
            return;
        }
    }
        fclose(fp);

        printf("\n\n\033[1;41mNote doesn't exist, Try again.\033[0m\n");
        printf("Please enter name of note(-1 to exit, 00 for main menu): ");

        int result;

        if((result = scanf("%d", &input)) == 1){
        switch(input){
            case(00):{
                main();
                break;
            }
            case(-1):{
                exit(1);
            }
            default:{
                printf("\nInvalid input, try again.\n");
                printf("Enter an option(00 for main menu and-1 for exit): ");
                while(scanf("%d", &input) !=0){
                    switch(input){
                        case(00):{
                            main();
                            break;
                        }
                        case(-1):{
                            exit(1);
                            break;
                    }
                }
                break;
            }
        }
    }
}
        else if ((result = scanf(" %[^\n]", NN)) == 1){
        checkIfNoteExists(NN);}
}

void SelectOption(char SelectOption[]){
    if (strcmp(SelectOption, "ADD") == 0 || strcmp(SelectOption, "add") == 0)
    addnote();
    else if (strcmp(SelectOption, "VIEW") == 0 || strcmp(SelectOption, "view") == 0)
    viewnote();
    else if (strcmp(SelectOption, "EDIT") == 0 || strcmp(SelectOption, "edit") == 0)
    editnote();
    else if (strcmp(SelectOption, "DELETE") == 0 || strcmp(SelectOption, "delete") == 0)
    deletenote();
    else
    printf("\033[31;41mInvalid input. Try again.\033[0m\n\n");
    main();
}

char *checkForInputInt(char NN[]){
    double x;
    char temp[100];

    // check if input string contains a number
    while(1){
        if(sscanf(NN, "%lf", &x) == 1){
            // means input is a number
            printf("\n\033[1;41mError: You entered a number. Enter the name of note instead.\033[0m\n");
            printf("\033[1mEnter name of note:\033[0m ");
            scanf(" %[^\n]", temp);
            return checkForInputInt(temp);
            break;
        }
        else {
            return NN;              //return original array if array doesn't contain number
        }
    }
}