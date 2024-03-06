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
    // char *c_parameters = NULL;
    char *p_parameters = NULL;
    // char *r_parameters = NULL;
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    int opt;
    // ask ta if different way to implement this or if i should just follow the article
    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1) {
        switch (opt) {
            case 'i':
                if (check_duplicate(i_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                i_flag = true;
                input_path = optarg;
                fprintf(stderr, "INPUT PATH: %s\n", input_path);
                break;
            case 'o':
                if (check_duplicate(o_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                o_flag = true;
                output_path = optarg;
                break;
            case 'c':
                if (check_duplicate(c_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                c_flag = true;
                // c_parameters = optarg;
                break;
            case 'p':
                if (check_duplicate(p_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                p_flag = true;
                p_parameters = optarg;
                fprintf(stderr, "P PARAMETERS: %s\n", p_parameters);
                break;
            case 'r':
                if (check_duplicate(r_flag, (char)opt)) {
                    return DUPLICATE_ARGUMENT;
                }
                r_flag = true;
                // r_parameters = optarg;
                break;
            case ':':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return MISSING_ARGUMENT;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
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


}
