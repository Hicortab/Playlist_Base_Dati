#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RECORDS 100
#define FILE_NAME "libreria.txt"

typedef struct {
    char nome[50];
    char cognome[50];
    char data_prestito[20];
} Prestito;

typedef struct {
    char nome[50];
    char cognome[50];
    char data_restituzione[20];
} Restituzione;

Prestito prestiti[MAX_RECORDS];
int num_prestiti = 0;

Restituzione restituzioni[MAX_RECORDS];
int num_restituzioni = 0;

// Funzione per caricare i dati dal file
void carica_dati() {
    FILE* file = fopen(FILE_NAME, "r");
    if (!file) {
        // File non esiste ancora, nessun errore
        return;
    }
    char linea[200];
    int sezione = 0; // 1 = prestiti, 2 = restituzioni
    while (fgets(linea, sizeof(linea), file)) {
        if (strncmp(linea, "PRESTITI", 8) == 0) {
            sezione = 1;
        }
        else if (strncmp(linea, "RESTITUZIONI", 12) == 0) {
            sezione = 2;
        }
        else if (strlen(linea) > 1) { // Evita righe vuote
            char nome[50], cognome[50], data[20];
            sscanf(linea, "%49[^;];%49[^;];%19s", nome, cognome, data);
            if (sezione == 1 && num_prestiti < MAX_RECORDS) {
                Prestito p;
                strcpy(p.nome, nome);
                strcpy(p.cognome, cognome);
                strcpy(p.data_prestito, data);
                prestiti[num_prestiti++] = p;
            }
            else if (sezione == 2 && num_restituzioni < MAX_RECORDS) {
                Restituzione r;
                strcpy(r.nome, nome);
                strcpy(r.cognome, cognome);
                strcpy(r.data_restituzione, data);
                restituzioni[num_restituzioni++] = r;
            }
        }
    }
    fclose(file);
}

// Funzione per salvare i dati nel file
void salva_dati() {
    FILE* file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Errore nell'aprire il file in scrittura.\n");
        return;
    }
    fprintf(file, "PRESTITI\n");
    for (int i = 0; i < num_prestiti; i++) {
        fprintf(file, "%s;%s;%s\n", prestiti[i].nome, prestiti[i].cognome, prestiti[i].data_prestito);
    }
    fprintf(file, "RESTITUZIONI\n");
    for (int i = 0; i < num_restituzioni; i++) {
        fprintf(file, "%s;%s;%s\n", restituzioni[i].nome, restituzioni[i].cognome, restituzioni[i].data_restituzione);
    }
    fclose(file);
}

// Funzione per aggiungere un prestito
void aggiungi_prestito() {
    if (num_prestiti >= MAX_RECORDS) {
        printf("Impossibile aggiungere altri prestiti.\n");
        return;
    }
    Prestito p;
    printf("Inserisci nome: ");
    scanf("%49s", p.nome);
    printf("Inserisci cognome: ");
    scanf("%49s", p.cognome);
    printf("Inserisci data del prestito (YYYY-MM-DD): ");
    scanf("%19s", p.data_prestito);

    prestiti[num_prestiti++] = p;
    printf("Prestito aggiunto con successo!\n");
    salva_dati();
}

// Funzione per restituire un libro
void restituisci_libro() {
    if (num_prestiti == 0) {
        printf("Nessun prestito da restituire.\n");
        return;
    }
    char nome[50], cognome[50], data[20];
    printf("Inserisci nome della persona che restituisce il libro: ");
    scanf("%49s", nome);
    printf("Inserisci cognome: ");
    scanf("%49s", cognome);
    printf("Inserisci data di restituzione (YYYY-MM-DD): ");
    scanf("%19s", data);

    int trovato = 0;
    for (int i = 0; i < num_prestiti; i++) {
        if (strcmp(prestiti[i].nome, nome) == 0 && strcmp(prestiti[i].cognome, cognome) == 0) {
            Restituzione r;
            strcpy(r.nome, nome);
            strcpy(r.cognome, cognome);
            strcpy(r.data_restituzione, data);
            restituzioni[num_restituzioni++] = r;

            for (int j = i; j < num_prestiti - 1; j++) {
                prestiti[j] = prestiti[j + 1];
            }
            num_prestiti--;
            trovato = 1;
            printf("Libro restituito con successo!\n");
            salva_dati();
            break;
        }
    }
    if (!trovato) {
        printf("Prestito non trovato.\n");
    }
}

// Funzione per mostrare la situazione attuale
void mostra_stato() {
    printf("\n----- Prestiti attuali (%d) -----\n", num_prestiti);
    for (int i = 0; i < num_prestiti; i++) {
        printf("%d) %s %s, preso il %s\n", i + 1,
            prestiti[i].nome, prestiti[i].cognome, prestiti[i].data_prestito);
    }

    printf("\n----- Restituzioni effettuate (%d) -----\n", num_restituzioni);
    for (int i = 0; i < num_restituzioni; i++) {
        printf("%d) %s %s, restituito il %s\n", i + 1,
            restituzioni[i].nome, restituzioni[i].cognome, restituzioni[i].data_restituzione);
    }
    printf("\n");
}

int main() {
    carica_dati();

    int scelta;
    do {
        printf("\nGestione Libreria:\n");
        printf("1) Aggiungi prestito\n");
        printf("2) Restituisci libro\n");
        printf("3) Mostra stato\n");
        printf("0) Esci\n");
        printf("Seleziona un'opzione: ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 1:
            aggiungi_prestito();
            break;
        case 2:
            restituisci_libro();
            break;
        case 3:
            mostra_stato();
            break;
        case 0:
            printf("Uscita dal programma.\n");
            break;
        default:
            printf("Opzione non valida. Riprova.\n");
        }
    } while (scelta != 0);

    return 0;
}
