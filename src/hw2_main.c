#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

// uses flag, checks duplicate arg (prints error before returning)
bool check_duplicate(bool flag, char arg) {
    if (flag) {
        fprintf(stderr, "Duplicate argument for -%c.\n", arg);
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    // ask ta about declare extern like the article
    // ask if i can declare expected parameters as null and check them later on so i don't have to do string stuff
    char *input_path = NULL;
    char *output_path = NULL;
    char *c_parameters = NULL;
    char *p_parameters = NULL;
    char *r_parameters = NULL;
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    int opt;
    // ask ta if different way to implement this or if i should just follow the article
    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1) {
        switch (opt) {
            case 'i':
                if (check_duplicate(i_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                if (*optarg == '-') {
                    fprintf(stderr, "Missing parameter for -i.\n");
                    return MISSING_ARGUMENT;
                }
                i_flag = true;
                input_path = optarg;
                // fprintf(stderr, "INPUT PATH: %s\n", input_path);
                break;
            case 'o':
                if (check_duplicate(o_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                if (*optarg == '-') {
                    fprintf(stderr, "Missing parameter for -o.\n");
                    return MISSING_ARGUMENT;
                }
                o_flag = true;
                output_path = optarg;
                break;
            case 'c':
                if (check_duplicate(c_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                c_flag = true;
                // fprintf(stderr, "C OPTARG: %s.\n", optarg);
                c_parameters = optarg;
                break;
            case 'p':
                if (check_duplicate(p_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                p_flag = true;
                p_parameters = optarg;
                // fprintf(stderr, "P PARAMETERS: %s\n", p_parameters);
                break;
            case 'r':
                if (check_duplicate(r_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                r_flag = true;
                r_parameters = optarg;
                break;
            case ':':
                return MISSING_ARGUMENT;
            case '?':
                return UNRECOGNIZED_ARGUMENT;
        }
    }
    // this visibly checkds for missing parameters for -i and -o but also checks for missing arguments indirectly, the paths wont be assigned anything if there was no arg in the first place (don't have to check the token after arg)
    if ((input_path == NULL) || (output_path == NULL)) {
        fprintf(stderr, "At least one of the -i and -o arguments is missing; or, a command-line argument is given, but no parameter\n");
        return MISSING_ARGUMENT;
    }

    // check if missing input file CLOSE AFTER
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        fprintf(stderr, "The input file '%s' is missing or cannot be opened for reading.\n", input_path);
        return INPUT_FILE_MISSING;
    }
    fclose(input_file);

    // check if output file is writable CLOSE AFTER
    FILE *output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot create output file '%s' for writing.\n", output_path);
        return OUTPUT_FILE_UNWRITABLE;
    }
    fclose(output_file);

    // check if -c was provided given that -p was)
    if (p_flag && !c_flag) {
        fprintf(stderr, "The -p argument has been provided at least once, but the -c argument was not provided.\n");
        return C_ARGUMENT_MISSING;
    }

    // check if c parameters are valid, make a tokenized array
    if (c_flag) {
        char* token;
        int count = 0;
        int split_c_parameters[4];
        // fprintf(stderr, "C PARAM BEFORE: %s.\n", c_parameters);
        token = strtok(c_parameters, ",");
        // fprintf(stderr, "C PARAM AFTER: %s.\n", c_parameters);
        // fprintf(stderr, "LENGTH: %zu\n", strlen(token));
        while (token != NULL) {
            if (count > 4) {
                fprintf(stderr, "-c argument has too many parameters.\n");
                return C_ARGUMENT_INVALID;
            }
            split_c_parameters[count] = strtol(token, NULL, 10);
            token = strtok(NULL, ",");
            count++;
        }
        if (count != 4) {
            fprintf(stderr, "-c argument has incorrect number of parameters.\n");
            return C_ARGUMENT_INVALID;
        }

        int c_row = split_c_parameters[0];
        int c_col = split_c_parameters[1];
        int c_width = split_c_parameters[2];
        int c_height = split_c_parameters[3];

        // Comment out, print statement to remove unused error
        printf("I DID USE IT %d%d%d%d\n", c_col, c_row, c_width, c_height);
    }

    // check if p parameters are valid
    if (p_flag) {
        char *token;
        int count = 0;
        int split_p_parameters[2];
        token = strtok(p_parameters, ",");
        while (token != NULL && count < 2) {
            split_p_parameters[count] = strtol(token, NULL, 10);
            token = strtok(NULL, ",");
            count++;
        }
        if (count != 2) {
            fprintf(stderr, "-p argument has incorrect number of parameters.\n");
            return P_ARGUMENT_INVALID;
        }

        int p_row = split_p_parameters[0];
        int p_col = split_p_parameters[1];

        // Comment out, print statement to remove unused error
        printf("I DID USE IT %d%d\n", p_row, p_col);
    }

    // cant save actual parameter assignment for later, have to split and tokenize to be able to check the file
    if (r_flag) {
        char *message, *path_to_font;
        int font_size, row, col;
        char *token = strtok(r_parameters, ",");
        message = token;
        path_to_font = strtok(NULL, ",");
        if (!path_to_font) return R_ARGUMENT_INVALID; 
        token = strtok(NULL, ",");
        if (token) {
            font_size = strtol(token, NULL, 10);
        } else return R_ARGUMENT_INVALID; // if null
        token = strtok(NULL, ",");
        if (token) {
            row = strtol(token, NULL, 10);
        } else return R_ARGUMENT_INVALID;
        token = strtok(NULL, ",");
        if (token) {
            col = strtol(token, NULL, 10);
        } else return R_ARGUMENT_INVALID; // All check if not enough args


        if (strtok(NULL, ",")) return R_ARGUMENT_INVALID; // checks for if there are too many args

        // Comment out, print statement to remove unused error
        printf("I DID USE IT %s%s%d%d%d\n", message, path_to_font, font_size, row, col);

        // Check if font file can be opened
        FILE *fontFile = fopen(path_to_font, "r");
        if (!fontFile) {
            fprintf(stderr, "Cannot open font file '%s'.\n", path_to_font);
            return R_ARGUMENT_INVALID;
        }
        fclose(fontFile);
    }

    // typedef struct {
    //     unsigned char r, g, b;
    // } Color;
    
    // typedef struct {
    //     int width, height;
    //     Color* pixels; 
    // } Image;





}
