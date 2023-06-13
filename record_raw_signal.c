#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <lirc/lirc_client.h>

#define OUTPUT_FILE "raw_ir_signals.txt"

int main() {
    FILE *file;
    struct lirc_config *config;
    const char *code;

    // Initialize LIRC in raw mode
    if (lirc_init("appname", 1) == -1) {
        fprintf(stderr, "LIRC initialization failed.\n");
        return EXIT_FAILURE;
    }

    // Open output file
    file = fopen(OUTPUT_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open output file.\n");
        lirc_deinit();
        return EXIT_FAILURE;
    }

    printf("Recording raw IR signals. Press Ctrl+C to stop.\n");

    // Loop to continuously read IR signals
    while (lirc_nextcode(&code) == 0) {
        if (code != NULL) {
            // Write the raw IR signal to the output file
            fprintf(file, "%s\n", code);
            fflush(file);
            free(code);
        }
    }

    // Close output file
    fclose(file);

    // Clean up LIRC
    lirc_deinit();

    return EXIT_SUCCESS;
}

