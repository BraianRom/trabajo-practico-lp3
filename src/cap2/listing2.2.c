#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* The name of this program. */
const char* program_name;

/* Prints usage information for this program. */
void print_usage(FILE* stream, int exit_code) {
    fprintf(stream, "Usage: %s options [ inputfile .... ]\n", program_name);
    fprintf(stream,
            "  -h  --help            Display this usage information.\n"
            "  -o  --output filename Write output to file.\n"
            "  -v  --verbose         Print verbose messages.\n");
    exit(exit_code);
}

int main(int argc, char* argv[]) {
    int next_option;

    /* Short options. */
    const char* const short_options = "ho:v";

    /* Long options. */
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "output",  1, NULL, 'o' },
        { "verbose", 0, NULL, 'v' },
        { NULL,      0, NULL,  0  }   /* End marker */
    };

    /* Option values. */
    const char* output_filename = NULL;
    int verbose = 0;

    /* Program name. */
    program_name = argv[0];

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch (next_option) {
            case 'h':   /* -h or --help */
                print_usage(stdout, 0);

            case 'o':   /* -o or --output */
                output_filename = optarg;
                break;

            case 'v':   /* -v or --verbose */
                verbose = 1;
                break;

            case '?':   /* Invalid option */
                print_usage(stderr, 1);

            case -1:    /* No more options */
                break;

            default:
                abort();
        }
    } while (next_option != -1);

    /* Print non-option arguments if verbose mode. */
    if (verbose) {
        int i;
        for (i = optind; i < argc; ++i)
            printf("Argument: %s\n", argv[i]);
    }

    /* Here would go the main program logic. */
    if (output_filename)
        printf("Output will be written to: %s\n", output_filename);

    return 0;
}
