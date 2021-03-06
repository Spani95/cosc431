#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4048

void removeEndTag(char *token, char *new_token);
void toLower(char* str);
void clean_token(char *str, char *new_token);
void parse(char *token, FILE *output);

int main(int argc, char *argv []) {

    // characters to that split 
    const char *delimiter_characters = " $.,\"\'\n();:{}+^#@*";

    FILE *file_handle;
    FILE *output_file_handle;
    char buffer[BUFFER_SIZE];
    file_handle = fopen(argv[1], "r");
    if (!file_handle) {
        fprintf(stderr, "Unable to open file!\n");
        return 1;
    }
    output_file_handle = fopen("./files/parser_output.txt", "w");
    if (!output_file_handle) {
        fprintf(stderr, "Unable to open file!\n");
        return 1;
    }

    if (!file_handle) {
        fprintf(stderr, "Unable to open file!\n");
        return 1;
    }
    int keep_next_token = 0;
    // Read each line into the buffer
    while(fgets(buffer,BUFFER_SIZE,file_handle) != NULL){
        // Gets each token as a string
        char *token = strtok(buffer, delimiter_characters);
        while(token != NULL) {
            if (keep_next_token == 1) {
                fprintf(output_file_handle, "%s\n", token);
                keep_next_token = 0;
                token = strtok(NULL, delimiter_characters);
                continue;
            }
            if(token[0] == '<'){
                // Check if the token represents the start of a new document.
                if(strcmp(token, "<DOC>") == 0) {
                    fprintf(output_file_handle, "\n");
                    token = strtok(NULL, delimiter_characters);
                    continue;
                } else if(strcmp(token, "<DOCNO>") == 0) {
                    keep_next_token = 1;
                    token = strtok(NULL, delimiter_characters);
                    continue;
                }
            }
            toLower(token);
            char *temp_token = strdup(token);
            if(token[strlen(token)-1] == '>'){
                removeEndTag(token, temp_token);
                clean_token(temp_token, token);
                parse(token, output_file_handle);
            } else {
                clean_token(token, temp_token);
                parse(temp_token, output_file_handle);
            }
            token = strtok(NULL, delimiter_characters);
            free(temp_token);
        }
        free(token);
    }

    if (ferror(file_handle)){
        perror("The following error occured");
    }

    // closing files.
    fclose(file_handle);  
    fclose(output_file_handle);
    return 0;
}

// Removes the xml tag at the end of a token.
void removeEndTag(char *token, char *new_token) {
    int i;
    for (i = 0; token[i] != '<'; i++){
        new_token[i] = token[i];
    }
    new_token[i] = '\0';
}

// Converts the token into lowercase.
void toLower(char* str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

// Removes any characters in the token that are not in the alphabet.
void clean_token(char *str, char *new_token) {
    int i = 0;
    int count = 0;
    while (i <= strlen(str)) {
        if (isalpha(str[i]) || str[i] == '&') {
            new_token[count] = str[i];
            count++;
        }
        i++;
    }
    new_token[count] = '\0';
}

// Checks that the token is valid then outputs it to the 
// parser_output.txt file.
void parse(char *token, FILE *output) {
    if (strlen(token) > 0 && strcmp(token, "&amp") != 0){
        fprintf(output, "%s\n", token);
    }
}
