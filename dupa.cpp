#include <cstdio>
#include <cstdlib>
#include <ctime>

#define LICZBA_DANYCH 1000
#define MIN_DEGREE 0
#define MAX_DEGREE 3
#define MIN_COEFFICIENT -10
#define MAX_COEFFICIENT 10

#define RAND(min, max) ((min) + (rand() % ((max) - (min) + 1)))

char* createPolynomial() {
    char* result = new char[1000];
    int degree = RAND(MIN_DEGREE, MAX_DEGREE);
    
    sprintf(result, "POLYNOMIAL(%i", degree);
    while (degree >= 0) {
        sprintf(result, "%s, %i", result, RAND(MIN_COEFFICIENT, MAX_COEFFICIENT));
        degree -= 1;
    }
    sprintf(result, "%s)", result);

    return result;
}

int main() {
    FILE* Randomiser = fopen("RandomTest.cpp", "w");
    int i = 0;
    char* arg1;
    char* arg2;

    srand((unsigned)time(NULL));

    while (i < LICZBA_DANYCH) {
        arg1 = createPolynomial();
        arg2 = createPolynomial();
        fprintf(Randomiser, "cout << %s / %s << endl;\n", arg1, arg2);
        fprintf(Randomiser, "cout << %s %% %s << endl;\n", arg1, arg2);
        delete[] arg1;
        delete[] arg2;
        i += 1;
    }

    fclose(Randomiser);

    return 0;
}