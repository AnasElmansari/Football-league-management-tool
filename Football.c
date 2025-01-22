// Anas Elmansari Aalborg universitet software 1 semester
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_KAMPE 90
#define MAX_HOLD 12
#define NAVN_LEN 20

// Struct to store match data
typedef struct {
    char ugedag[10];
    char dato[10];
    char tid[10];
    char hjemmehold[NAVN_LEN];
    char udehold[NAVN_LEN];
    int hjemmeMål;
    int udeMål;
    int tilskuertal;
} Kamp;

// Struct to store team data
typedef struct {
    char navn[NAVN_LEN];
    int point;
    int målAfHold;
    int målModHold;
} Hold;

// Function prototypes
void indlaesKampe(Kamp kampe[], int *antalKampe);
void opdaterHold(Kamp kampe[], int antalKampe, Hold hold[], int *antalHold);
int findEllerTilføjHold(Hold hold[], int *antalHold, const char *navn);
void sorterHold(Hold hold[], int antalHold);
void udskrivStillingen(Hold hold[], int antalHold);

int main() {
    Kamp kampe[MAX_KAMPE];
    Hold hold[MAX_HOLD];
    int antalKampe = 0, antalHold = 0;

    // Read match data
    indlaesKampe(kampe, &antalKampe);

    // Process match data and update teams
    opdaterHold(kampe, antalKampe, hold, &antalHold);

    // Sort teams by points and goal difference
    sorterHold(hold, antalHold);

    // Print the standings
    udskrivStillingen(hold, antalHold);

    return 0;
}
//made by Anas
// Function to read match data from the file
void indlaesKampe(Kamp kampe[], int *antalKampe) {
    FILE *fil = fopen("kampe-2024-2025.txt", "r");
    if (!fil) {
        perror("Fejl ved åbning af fil");
        exit(1);
    }

    while (fscanf(fil, "%s %s %s %s - %s %d - %d %d",
                  kampe[*antalKampe].ugedag,
                  kampe[*antalKampe].dato,
                  kampe[*antalKampe].tid,
                  kampe[*antalKampe].hjemmehold,
                  kampe[*antalKampe].udehold,
                  &kampe[*antalKampe].hjemmeMål,
                  &kampe[*antalKampe].udeMål,
                  &kampe[*antalKampe].tilskuertal) == 8) {
        (*antalKampe)++;
    }

    fclose(fil);
}

// Function to process matches and update teams
void opdaterHold(Kamp kampe[], int antalKampe, Hold hold[], int *antalHold) {
    for (int i = 0; i < antalKampe; i++) {
        int indexHjemme = findEllerTilføjHold(hold, antalHold, kampe[i].hjemmehold);
        int indexUde = findEllerTilføjHold(hold, antalHold, kampe[i].udehold);

        hold[indexHjemme].målAfHold += kampe[i].hjemmeMål;
        hold[indexHjemme].målModHold += kampe[i].udeMål;
        hold[indexUde].målAfHold += kampe[i].udeMål;
        hold[indexUde].målModHold += kampe[i].hjemmeMål;

        if (kampe[i].hjemmeMål > kampe[i].udeMål) {
            hold[indexHjemme].point += 3;
        } else if (kampe[i].hjemmeMål < kampe[i].udeMål) {
            hold[indexUde].point += 3;
        } else {
            hold[indexHjemme].point += 1;
            hold[indexUde].point += 1;
        }
    }
}

// Function to find or add a team in the array
int findEllerTilføjHold(Hold hold[], int *antalHold, const char *navn) {
    for (int i = 0; i < *antalHold; i++) {
        if (strcmp(hold[i].navn, navn) == 0) {
            return i;
        }
    }

    strcpy(hold[*antalHold].navn, navn);
    hold[*antalHold].point = 0;
    hold[*antalHold].målAfHold = 0;
    hold[*antalHold].målModHold = 0;
    return (*antalHold)++;
}

// Comparison function for sorting qsort
int sammenlignHold(const void *a, const void *b) {
    Hold *holdA = (Hold *)a;
    Hold *holdB = (Hold *)b;

    if (holdB->point != holdA->point) {
        return holdB->point - holdA->point;
    }
    return (holdB->målAfHold - holdB->målModHold) - (holdA->målAfHold - holdA->målModHold);
}

// Function to sort the teams
void sorterHold(Hold hold[], int antalHold) {
    qsort(hold, antalHold, sizeof(Hold), sammenlignHold);
}

// Function to print the standings
void udskrivStillingen(Hold hold[], int antalHold) {
    printf("%-20s %-5s %-10s %-10s\n", "Holdnavn", "Point", "Mål-af-hold", "Mål-mod-hold");
    for (int i = 0; i < antalHold; i++) {
        printf("%-20s %-5d %-10d %-10d\n", hold[i].navn, hold[i].point, hold[i].målAfHold, hold[i].målModHold);
    }
}
