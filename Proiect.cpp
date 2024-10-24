#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define M 10
int n = 9;
int mat[9][9], matin[9][9];
int verbose = 0; // flag for verbose mode
int show_time = 0; // flag for showing solving time

// Function to get current time in seconds
double get_time_in_seconds() {
    SYSTEMTIME st;
    FILETIME ft;
    ULARGE_INTEGER uli;

    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);

    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    return (double)(uli.QuadPart / 10000000ULL - 11644473600ULL); // Convert to seconds since UNIX epoch
}

void print_help(const char* prog_name) {
    printf("Usage: %s [options] [input file] [output file]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help      Display this help message\n");
    printf("  -v, --verbose   Toggle verbose mode\n");
    printf("  -t              Show solving time\n");
    printf("If no input and output files are specified, the program reads from stdin and writes to stdout.\n");
}

int verifica(int x, int y, int val) {
    int i, j;
    // pentru coloanele matricii
    for (j = 0; j < n; j++) {
        if (mat[x][j] == val) return 0;
        if (mat[j][y] == val) return 0;
    }
    // cauta patratul din care face parte, si verifica pentru fiecare valoare
    if (x < 3 && y < 3) {
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++) if (mat[i][j] == val) return 0;
    }
    if (x < 3 && (y >= 3) && (y < 6)) {
        for (i = 0; i < 3; i++)
            for (j = 3; j < 6; j++) if (mat[i][j] == val) return 0;
    }
    if (x < 3 && y >= 6) {
        for (i = 0; i < 3; i++)
            for (j = 6; j < 9; j++) if (mat[i][j] == val) return 0;
    }
    if ((x >= 3 && x < 6) && y < 3) {
        for (i = 3; i < 6; i++)
            for (j = 0; j < 3; j++) if (mat[i][j] == val) return 0;
    }
    if ((x >= 3 && x < 6) && (y >= 3) && (y < 6)) {
        for (i = 3; i < 6; i++)
            for (j = 3; j < 6; j++) if (mat[i][j] == val) return 0;
    }
    if (x < 3 && y >= 6) {
        for (i = 0; i < 3; i++)
            for (j = 6; j < 9; j++) if (mat[i][j] == val) return 0;
    }
    if (x >= 6 && y < 3) {
        for (i = 6; i < 9; i++)
            for (j = 0; j < 3; j++) if (mat[i][j] == val) return 0;
    }
    if (x >= 6 && (y >= 3) && (y < 6)) {
        for (i = 6; i < 9; i++)
            for (j = 3; j < 6; j++) if (mat[i][j] == val) return 0;
    }
    if (x >= 6 && y >= 6) {
        for (i = 6; i < 9; i++)
            for (j = 6; j < 9; j++) if (mat[i][j] == val) return 0;
    }
    return val;
}

int back(int x, int y) {
    int aux;
    int i, j; // variabila auxiliara
    if (mat[x][y] == 0) { // daca elementul 
        for (i = 1; i < 10; i++) {
            aux = verifica(x, y, i); // se retine variabila in aux
            if (aux) { // daca aux e diferit de 0
                mat[x][y] = aux;
                if (x == 8 && y == 8) return 1; // daca am terminat de parcurs
                else
                    if (x == 8) // daca se trece pe linia urmatoare
                    {
                        if (back(0, y + 1)) return 1;
                    }
                    else
                        if (back(x + 1, y)) return 1;
            }
        }

        mat[x][y] = 0;
        return 0;
    }
    else { // daca e diferit de 0, se trece mai departe
        if (x == 8 && y == 8) return 1;
        else
            if (x == 8) { if (back(0, y + 1)) return 1; }
            else
                if (back(x + 1, y)) return 1;
    }
    return 0;
}

int citire(FILE* f) {
    int nr, i, j, x, y, val;
    if (fscanf(f, "%d", &nr) != 1) {
        fprintf(stderr, "Error reading number of initial values\n");
        return -1;
    }
    for (i = 0; i < nr; i++) {
        if (fscanf(f, "%d%d%d", &x, &y, &val) != 3) {
            fprintf(stderr, "Error reading initial values\n");
            return -1;
        }
        matin[x - 1][y - 1] = val;
        mat[x - 1][y - 1] = val;
    }
    return 0;
}

void afisare(FILE* f) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fprintf(f, "%d ", mat[i][j]);
        }
        fprintf(f, "\n");
    }
}

int main(int argc, char* argv[]) {
    FILE* input_file = stdin;
    FILE* output_file = stdout;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-t") == 0) {
            show_time = 1;
        }
        else if (argv[i][0] != '-' && input_file == stdin) {
            input_file = fopen(argv[i], "r");
            if (!input_file) {
                fprintf(stderr, "Error opening input file: %s\n", argv[i]);
                return 1;
            }
        }
        else if (argv[i][0] != '-' && output_file == stdout) {
            output_file = fopen(argv[i], "w");
            if (!output_file) {
                fprintf(stderr, "Error opening output file: %s\n", argv[i]);
                if (input_file != stdin) fclose(input_file);
                return 1;
            }
        }
        else {
            fprintf(stderr, "Unknown option or too many files: %s\n", argv[i]);
            print_help(argv[0]);
            if (input_file != stdin) fclose(input_file);
            if (output_file != stdout) fclose(output_file);
            return 1;
        }
    }

    double tp1, tp2;
    tp1 = get_time_in_seconds();

    int i, j;
    int sem = 0; // sem indica daca programul s-a terminat cu succes
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++) {
            matin[i][j] = 0;
            mat[i][j] = 0;
        }

    if (citire(input_file) != 0) { // Ensure input reading is successful
        if (input_file != stdin) fclose(input_file);
        if (output_file != stdout) fclose(output_file);
        return -1;
    }

    if (back(0, 0)) {
        afisare(output_file);
        sem = 1;
    }
    else {
        fprintf(output_file, "No solution found!\n");
    }

    tp2 = get_time_in_seconds();

    if (show_time) {
        fprintf(output_file, "Time: %e seconds\n", tp2 - tp1);
    }

    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);

    return 0;
}
