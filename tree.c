#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max_auto 512

typedef struct veicolo {
    int autonomia, conta;

    //Caratteristiche albero:
    char colore;
    struct veicolo * padre, * left, * right;

} parcheggio;

typedef struct stazione {
    int distanza;
    parcheggio * automobili;
    int max, num_auto;

    //Caratteristiche albero:
    char colore;
    struct stazione * padre, * left, * right;

    //Caratteristiche lista dinamica:
    struct stazione * prev, * next;

} autostrada;

void pianifica_percorso(autostrada * iniz, autostrada * fin) {
    if(iniz->distanza == fin->distanza) {
        printf("%d\n", iniz->distanza);
        return;
    }

    autostrada * punt = NULL, ** array = malloc(0);
    int conta = 0;

    if(iniz->distanza < fin->distanza) {
        //Creazione array di puntatori a stazioni:
        for(punt = iniz; punt != NULL && punt->distanza <= fin->distanza; punt = punt->next) {
            conta++;
            array = realloc(array, conta * sizeof(autostrada *));       //serve per usare max
            array[conta-1] = punt;
        }

        int prec[conta], start = 0, end = 0;
        for(int x = 0; x < conta; x++) prec[x] = -1;

        int controllo = end;                        //Parto da 0 (stazione iniziale)
        while (controllo < conta - 1) {
            while(start <= end) {
                while(controllo < conta - 1 && (array[start])->distanza + (array[start])->max >= (array[controllo+1])->distanza) {
                    controllo++;
                    prec[controllo] = start;
                }
                start++;
            }
            start = end+1;
            if(controllo == end) break;             //Se non sono state aggiunte altre stazioni da controllare non continuare
            else end = controllo;
        }

        int from, tappe = 0, * percorso = malloc(0);
        //Costruzione array percorso (reversed):
        for(from = conta-1; prec[from] != -1; from = prec[from]) {
            tappe++;
            percorso = realloc(percorso, tappe * sizeof(int));
            percorso[tappe-1] = from;
        }
        //Stampa solo se l'ultima stazione e' quella iniziale
        if(from == 0) {
            printf("%d", iniz->distanza);
            for(int x = tappe; x > 0; x--) printf(" %d", (array[percorso[x-1]])->distanza);
            printf("\n");
            free(array);
            array = NULL;
            free(percorso);
            percorso = NULL;
            return;
        }
        printf("nessun percorso\n");
        free(array);
        array = NULL;
        free(percorso);
        percorso = NULL;
        return;
    }

    if(iniz->distanza > fin->distanza) {
        //Creazione array di puntatori a stazioni:
        for(punt = fin; punt != NULL && punt->distanza <= iniz->distanza; punt = punt->next) {
            conta++;
            array = realloc(array, conta * sizeof(autostrada *));
            array[conta-1] = punt;
        }
        int prec[conta], start = conta-1, end = conta-1;
        for(int x = 0; x < conta; x++) prec[x] = -1;

        int controllo = start;              //Parto dalla stazione iniziale
        while (controllo > 0) {             //Controllo fino ad arrivare alla stazione finale
            int x = start;
            while(x <= end) {
                while(controllo > 0 && (array[x])->distanza - (array[x])->max <= (array[controllo-1])->distanza) {
                    controllo--;
                    prec[controllo] = x;
                }
                x++;
            }
            end = start-1;
            if(controllo == start) break;
            else start = controllo;
        }

        int from, tappe = 0, * percorso = malloc(0);
        //Creazione array di percorso (reversed):
        for(from = 0; prec[from] != -1; from = prec[from]) {
            tappe++;
            percorso = realloc(percorso, tappe * sizeof(int));
            percorso[tappe-1] = from;
        }
        //Stampa solo all'arrivo della stazione iniziale
        if(from == conta-1) {
            printf("%d", iniz->distanza);
            for(int x = tappe; x > 0; x--) printf(" %d", (array[percorso[x-1]])->distanza);
            printf("\n");
            free(array);
            array = NULL;
            free(percorso);
            percorso = NULL;
            return;
        }
        printf("nessun percorso\n");
        free(array);
        array = NULL;
        free(percorso);
        percorso = NULL;
        return;
    }
    //Teoricamente mai usato
    printf("nessun percorso\n");
    return;
}

parcheggio * ricerca_auto (parcheggio * Albero, int autonomia) {
    while (Albero != NULL && Albero->autonomia != autonomia) {
        if (autonomia < Albero->autonomia) return ricerca_auto (Albero->left, autonomia);
        if (autonomia > Albero->autonomia) return ricerca_auto (Albero->right, autonomia);
    }
    return Albero;
}

void left_rotate_auto (parcheggio ** Albero, parcheggio * perno) {
    parcheggio * scambio = perno->right;
    perno->right = scambio->left;

    if (scambio->left != NULL) scambio->left->padre = perno;
    scambio->padre = perno->padre;

    if(perno->padre == NULL) *Albero = scambio;
    else if(perno->padre->left == perno) perno->padre->left = scambio;
    else perno->padre->right = scambio;

    scambio->left = perno;
    perno->padre = scambio;
    return;
}

void right_rotate_auto (parcheggio ** Albero, parcheggio * perno) {
    parcheggio * scambio = perno->left;
    perno->left = scambio->right;

    if (scambio->right != NULL) scambio->right->padre = perno;
    scambio->padre = perno->padre;

    if(perno->padre == NULL) *Albero = scambio;
    else if(perno->padre->left == perno) perno->padre->left = scambio;
    else perno->padre->right = scambio;

    scambio->right = perno;
    perno->padre = scambio;
    return;
}

parcheggio * predecessore_auto (parcheggio * stazione) {
    if (stazione->left != NULL) {
        stazione = stazione->left;
        while (stazione->right != NULL) stazione = stazione->right;
        return stazione;
    }

    parcheggio * punt = stazione->padre;
    while (punt != NULL && stazione == punt->left) {
        stazione = punt;
        punt = punt->padre;
    }
    return punt;
}

void fixup_auto_aggiunta (parcheggio ** Albero, parcheggio * macchina) {
    if (macchina == *Albero) macchina->colore = 'B';
    else {
        parcheggio * punt = macchina->padre, * fratello = NULL;
        if (punt->colore == 'R') {
            if (punt == punt->padre->left) {
                fratello = punt->padre->right;

                if (fratello != NULL && fratello->colore == 'R') {          //Caso 1:
                    fratello->colore = 'B';
                    punt->colore = 'B';
                    punt->padre->colore = 'R';
                    fixup_auto_aggiunta (Albero, punt->padre);
                }
                else {
                    if (macchina == punt->right) {                          //Caso 2:
                        macchina = punt;                    
                        left_rotate_auto (Albero, macchina);
                        punt = macchina->padre;
                    }
                    punt->colore = 'B';                                     //Caso 3:
                    punt->padre->colore = 'R';
                    right_rotate_auto (Albero, punt->padre);
                }
                return;
            }
            else {
                fratello = punt->padre->left;

                if (fratello != NULL && fratello->colore == 'R') {          //Caso 1:
                    fratello->colore = 'B';
                    punt->colore = 'B';
                    punt->padre->colore = 'R';
                    fixup_auto_aggiunta (Albero, punt->padre);
                }
                else {
                    if (macchina == punt->left) {                           //Caso 2:
                        macchina = punt;                    
                        right_rotate_auto (Albero, macchina);
                        punt = macchina->padre;
                    }
                    punt->colore = 'B';                                     //Caso 3:
                    punt->padre->colore = 'R';
                    left_rotate_auto (Albero, punt->padre);
                }
                return;
            }
        }
    }
}

bool aggiungi_auto (autostrada * stazione, int autonomia) {
    if(autonomia < 0 || stazione->num_auto == max_auto) return false;

    parcheggio * padre = NULL, * punt = stazione->automobili;
    while (punt != NULL) {
        padre = punt;
        if (autonomia == punt->autonomia) {
            (punt->conta)++;
            (stazione->num_auto)++;
            return true;
        }
        else if (autonomia < punt->autonomia) punt = punt->left;
        else punt = punt->right;
    }

    parcheggio * nuovo = malloc(sizeof(parcheggio));

    nuovo->autonomia = autonomia;
    nuovo->conta = 1;
    if (stazione->max < autonomia) stazione->max = autonomia;
    (stazione->num_auto)++;

    nuovo->padre = padre;
    if (padre == NULL) stazione->automobili = nuovo;
    else if (nuovo->autonomia < padre->autonomia) padre->left = nuovo;
    else padre->right = nuovo;
    nuovo->left = NULL;
    nuovo->right = NULL;
    nuovo->colore = 'R';
    fixup_auto_aggiunta (&(stazione->automobili), nuovo);
    return true;
}

void fixup_auto_rottamata (parcheggio ** Albero, parcheggio * macchina) {
    if (macchina->colore == 'R' || macchina->padre == NULL) macchina->colore = 'B';

    else if (macchina->padre != NULL && macchina == macchina->padre->left) {
        parcheggio * fratello = macchina->padre->right;
        if(fratello->colore == 'R') {
            fratello->colore = 'B';
            macchina->padre->colore = 'R';
            left_rotate_auto (Albero, macchina->padre);
            fratello = macchina->padre->right;
        }
        if((fratello->left == NULL || fratello->left->colore == 'B') && (fratello->right == NULL || fratello->right->colore == 'B')) {
            fratello->colore = 'R';
            fixup_auto_rottamata (Albero, macchina->padre);
        }
        else {
            if (fratello->right == NULL || fratello->right->colore == 'B') {
                fratello->left->colore = 'B';
                fratello->colore = 'R';
                right_rotate_auto (Albero, fratello);
                fratello = macchina->padre->right;
            }
            fratello->colore = macchina->padre->colore;
            macchina->padre->colore = 'B';
            fratello->right->colore = 'B';
            left_rotate_auto (Albero, macchina->padre);
        }
    }
    else {
        parcheggio * fratello = macchina->padre->left;
        if(fratello->colore == 'R') {
            fratello->colore = 'B';
            macchina->padre->colore = 'R';
            right_rotate_auto (Albero, macchina->padre);
            fratello = macchina->padre->left;
        }
        if((fratello->left == NULL || fratello->left->colore == 'B') && (fratello->right == NULL || fratello->right->colore == 'B')) {
            fratello->colore = 'R';
            fixup_auto_rottamata (Albero, macchina->padre);
        }
        else {
            if (fratello->left == NULL || fratello->left->colore == 'B') {
                fratello->right->colore = 'B';
                fratello->colore = 'R';
                left_rotate_auto (Albero, fratello);
                fratello = macchina->padre->left;
            }
            fratello->colore = macchina->padre->colore;
            macchina->padre->colore = 'B';
            fratello->left->colore = 'B';
            right_rotate_auto (Albero, macchina->padre);
        }
    }
    return;
}

void rottama_auto (autostrada ** Albero, parcheggio * macchina) {
    if (macchina->left == NULL && macchina->right == NULL && macchina->padre == NULL) {
        free (macchina);
        (*Albero)->max = 0;
        (*Albero)->num_auto = 0;
        (*Albero)->automobili = NULL;
        return;
    }
    int autonomia = macchina->autonomia;
    
    parcheggio * eliminato = NULL, * sostituto = NULL;

    if (macchina->left == NULL || macchina->right == NULL) eliminato = macchina;
    else eliminato = predecessore_auto (macchina);

    if (eliminato->left != NULL) sostituto = eliminato->left;
    else sostituto = eliminato->right;

    if (sostituto == NULL) {
        sostituto = malloc(sizeof(parcheggio));     //Creazione foglia temporanea
        sostituto->autonomia = -1;
        sostituto->conta = 0;
        sostituto->colore = 'B';
        sostituto->left = NULL;
        sostituto->right = NULL;
        eliminato->right = sostituto;
    }
    sostituto->padre = eliminato->padre;

    if (eliminato->padre == NULL) (*Albero)->automobili = sostituto;
    else if (eliminato == eliminato->padre->left) eliminato->padre->left = sostituto;
    else eliminato->padre->right = sostituto;

    if(eliminato != macchina) {
        macchina->autonomia = eliminato->autonomia;
        macchina->conta = eliminato->conta;
    }
    if(eliminato->colore == 'B') fixup_auto_rottamata (&((*Albero)->automobili), sostituto);
    free (eliminato);
    if (sostituto->autonomia == -1) {
        if (sostituto == sostituto->padre->left) sostituto->padre->left = NULL;
        if (sostituto == sostituto->padre->right) sostituto->padre->right = NULL;
        free(sostituto);
    }

    if ((*Albero)->max == autonomia) {
        parcheggio * punt = NULL;
        for(punt = (*Albero)->automobili; punt->right != NULL; punt = punt->right);
        (*Albero)->max = punt->autonomia;
    }
    ((*Albero)->num_auto)--;
    return;
}

autostrada * ricerca_stazione (autostrada * Albero, int distanza) {
    while (Albero != NULL && Albero->distanza != distanza) {
        if (distanza < Albero->distanza) return ricerca_stazione (Albero->left, distanza);
        if (distanza > Albero->distanza) return ricerca_stazione (Albero->right, distanza);
    }
    return Albero;
}

void left_rotate_stazione (autostrada ** Albero, autostrada * perno) {
    autostrada * scambio = perno->right;
    perno->right = scambio->left;

    if (scambio->left != NULL) scambio->left->padre = perno;
    scambio->padre = perno->padre;

    if(perno->padre == NULL) *Albero = scambio;
    else if(perno->padre->left == perno) perno->padre->left = scambio;
    else perno->padre->right = scambio;

    scambio->left = perno;
    perno->padre = scambio;
    return;
}

void right_rotate_stazione (autostrada ** Albero, autostrada * perno) {
    autostrada * scambio = perno->left;
    perno->left = scambio->right;

    if (scambio->right != NULL) scambio->right->padre = perno;
    scambio->padre = perno->padre;

    if(perno->padre == NULL) *Albero = scambio;
    else if(perno->padre->left == perno) perno->padre->left = scambio;
    else perno->padre->right = scambio;

    scambio->right = perno;
    perno->padre = scambio;
    return;
}

autostrada * predecessore (autostrada * stazione) {
    if (stazione->left != NULL) {
        stazione = stazione->left;
        while (stazione->right != NULL) stazione = stazione->right;
        return stazione;
    }

    autostrada * punt = stazione->padre;
    while (punt != NULL && stazione == punt->left) {
        stazione = punt;
        punt = punt->padre;
    }
    return punt;
}

autostrada * successore (autostrada * stazione) {
    if (stazione->right != NULL) {
        stazione = stazione->right;
        while (stazione->left != NULL) stazione = stazione->left;
        return stazione;
    }
    autostrada * punt = stazione->padre;
    while (punt != NULL && stazione == punt->right) {
        stazione = punt;
        punt = punt->padre;
    }
    return punt;
}

void fixup_stazione_aggiunta (autostrada ** Albero, autostrada * stazione) {
    if (stazione == *Albero) stazione->colore = 'B';
    else {
        autostrada * punt = stazione->padre, * fratello = NULL;
        if (punt->colore == 'R') {
            if (punt == punt->padre->left) {
                fratello = punt->padre->right;

                if (fratello != NULL && fratello->colore == 'R') {          //Caso 1:
                    fratello->colore = 'B';
                    punt->colore = 'B';
                    punt->padre->colore = 'R';
                    fixup_stazione_aggiunta (Albero, punt->padre);
                }
                else {
                    if (stazione == punt->right) {                          //Caso 2:
                        stazione = punt;                    
                        left_rotate_stazione (Albero, stazione);
                        punt = stazione->padre;
                    }
                    punt->colore = 'B';                                     //Caso 3:
                    punt->padre->colore = 'R';
                    right_rotate_stazione (Albero, punt->padre);
                }
                return;
            }
            else {
                fratello = punt->padre->left;

                if (fratello != NULL && fratello->colore == 'R') {          //Caso 1:
                    fratello->colore = 'B';
                    punt->colore = 'B';
                    punt->padre->colore = 'R';
                    fixup_stazione_aggiunta (Albero, punt->padre);
                }
                else {
                    if (stazione == punt->left) {                           //Caso 2:
                        stazione = punt;                    
                        right_rotate_stazione (Albero, stazione);
                        punt = stazione->padre;
                    }
                    punt->colore = 'B';                                     //Caso 3:
                    punt->padre->colore = 'R';
                    left_rotate_stazione (Albero, punt->padre);
                }
                return;
            }
        }
    }
}

bool aggiungi_stazione (autostrada ** Albero, int distanza, int numero) {
    autostrada * padre = NULL, * punt = * Albero;
    
    while (punt != NULL) {
        padre = punt;
        if (distanza == punt->distanza) return false;
        if (distanza < punt->distanza) punt = punt->left;
        else punt = punt->right;
    }

    autostrada * nuovo = malloc(sizeof(autostrada));
    int autonomia;

    nuovo->distanza = distanza;
    nuovo->automobili = NULL;
    nuovo->max = 0;
    nuovo->num_auto = 0;
    for (int i = 0; i < numero; i++) {
        if(scanf("%d", &autonomia) != 1) exit(2);
        aggiungi_auto (nuovo, autonomia);
    }

    nuovo->padre = padre;
    if (padre == NULL) *Albero = nuovo;
    else if (nuovo->distanza < padre->distanza) padre->left = nuovo;
    else padre->right = nuovo;
    nuovo->left = NULL;
    nuovo->right = NULL;
    nuovo->colore = 'R';
    fixup_stazione_aggiunta (Albero, nuovo);

    punt = successore (nuovo);
    nuovo->next = punt;
    if (punt != NULL) punt->prev = nuovo;

    punt = predecessore (nuovo);
    nuovo->prev = punt;
    if (punt != NULL) punt->next = nuovo;

    return true;
}

void fixup_stazione_demolita (autostrada ** Albero, autostrada * stazione) {
    if (stazione->colore == 'R' || stazione->padre == NULL) stazione->colore = 'B';

    else if (stazione->padre != NULL && stazione == stazione->padre->left) {
        autostrada * fratello = stazione->padre->right;
        if(fratello->colore == 'R') {
            fratello->colore = 'B';
            stazione->padre->colore = 'R';
            left_rotate_stazione (Albero, stazione->padre);
            fratello = stazione->padre->right;
        }
        if((fratello->left == NULL || fratello->left->colore == 'B') && (fratello->right == NULL || fratello->right->colore == 'B')) {
            fratello->colore = 'R';
            fixup_stazione_demolita (Albero, stazione->padre);
        }
        else {
            if (fratello->right == NULL || fratello->right->colore == 'B') {
                fratello->left->colore = 'B';
                fratello->colore = 'R';
                right_rotate_stazione (Albero, fratello);
                fratello = stazione->padre->right;
            }
            fratello->colore = stazione->padre->colore;
            stazione->padre->colore = 'B';
            fratello->right->colore = 'B';
            left_rotate_stazione (Albero, stazione->padre);
        }
    }
    else if (stazione->padre != NULL) {
        autostrada * fratello = stazione->padre->left;
        if(fratello->colore == 'R') {
            fratello->colore = 'B';
            stazione->padre->colore = 'R';
            right_rotate_stazione (Albero, stazione->padre);
            fratello = stazione->padre->left;
        }
        if((fratello->left == NULL || fratello->left->colore == 'B') && (fratello->right == NULL || fratello->right->colore == 'B')) {
            fratello->colore = 'R';
            fixup_stazione_demolita (Albero, stazione->padre);
        }
        else {
            if (fratello->left == NULL || fratello->left->colore == 'B') {
                fratello->right->colore = 'B';
                fratello->colore = 'R';
                left_rotate_stazione (Albero, fratello);
                fratello = stazione->padre->left;
            }
            fratello->colore = stazione->padre->colore;
            stazione->padre->colore = 'B';
            fratello->left->colore = 'B';
            right_rotate_stazione (Albero, stazione->padre);
        }
    }
    return;
}

void demolisci_stazione (autostrada ** Albero, autostrada * stazione) {
    while (stazione->automobili != NULL) rottama_auto (&stazione, stazione->automobili);
    if (stazione->left == NULL && stazione->right == NULL && stazione->padre == NULL) {
        free (stazione);
        (*Albero) = NULL;
        return;
    }    
    autostrada * eliminato = NULL, * sostituto = NULL;

    if (stazione->left == NULL || stazione->right == NULL) eliminato = stazione;
    else eliminato = predecessore (stazione);

    if (eliminato->left != NULL) sostituto = eliminato->left;
    else sostituto = eliminato->right;

    if (sostituto == NULL) {
        sostituto = malloc(sizeof(autostrada));     //Creazione foglia temporanea
        sostituto->distanza = -1;
        sostituto->colore = 'B';
        sostituto->left = NULL;
        sostituto->right = NULL;
        eliminato->right = sostituto;
    }
    sostituto->padre = eliminato->padre;

    if (eliminato->padre == NULL) *Albero = sostituto;
    else if (eliminato == eliminato->padre->left) eliminato->padre->left = sostituto;
    else eliminato->padre->right = sostituto;

    if (stazione->prev != NULL) stazione->prev->next = stazione->next;
    if (stazione->next != NULL) stazione->next->prev = stazione->prev;

    if(eliminato != stazione) {
        stazione->distanza = eliminato->distanza;
        stazione->automobili = eliminato->automobili;
        stazione->max = eliminato->max;
        stazione->num_auto = eliminato->num_auto;

        stazione->prev = eliminato->prev;
        stazione->next = eliminato->next;
        if (stazione->prev != NULL) stazione->prev->next = stazione;
        if (stazione->next != NULL) stazione->next->prev = stazione;
    }
    if(eliminato->colore == 'B') fixup_stazione_demolita (Albero, sostituto);
    free (eliminato);
    if (sostituto->distanza == -1) {
        if (sostituto == sostituto->padre->left) sostituto->padre->left = NULL;
        if (sostituto == sostituto->padre->right) sostituto->padre->right = NULL;
        free(sostituto);
    }
    return;
}

void stampa_auto (parcheggio * Albero) {
    if(Albero == NULL) return;
    if(Albero->left != NULL) stampa_auto (Albero->left);
    if(Albero->right != NULL) stampa_auto (Albero->right);
    printf(" %d x%d |", Albero->autonomia, Albero->conta);
    return;
}

void stampa_stazioni(autostrada * Lista) {
    while(Lista->left != NULL) Lista = Lista->left;
    while(Lista != NULL) {
        printf("-> %d [%d](%d: |", Lista->distanza, Lista->max, Lista->num_auto);
        stampa_auto (Lista->automobili);
        printf(")\n");
        Lista = Lista->next;
    }
    printf("\n");
    return;
}

int main () {
    int dist1, dist2, numero, autonomia;
    autostrada * Lista = NULL, * punt = NULL, * punt2 = NULL;
    parcheggio * macchina;
    char stringa[20], foo;

    while(scanf("%s", stringa) != EOF) {
        foo = '\0';

        if(strcmp(stringa, "aggiungi-stazione") == 0) {
            if(scanf("%d %d", &dist1, &numero) != 2) exit(2);

            if(aggiungi_stazione (&Lista, dist1, numero)) printf("aggiunta\n");
            else printf("non aggiunta\n");
        }

        else if(strcmp(stringa, "demolisci-stazione") == 0) {
            if(scanf("%d", &dist1) != 1) exit(2);
            punt = ricerca_stazione (Lista, dist1);

            if(punt != NULL) {
                demolisci_stazione (&Lista, punt);
                printf("demolita\n");
            } else printf("non demolita\n");
        }

        else if(strcmp(stringa, "aggiungi-auto") == 0) {
            if(scanf("%d %d", &dist1, &autonomia) != 2) exit(2);
            punt = ricerca_stazione (Lista, dist1);

            if(punt != NULL && aggiungi_auto (punt, autonomia)) printf("aggiunta\n");
            else printf("non aggiunta\n");
        }

        else if(strcmp(stringa, "rottama-auto") == 0) {
            if(scanf("%d %d", &dist1, &autonomia) != 2) exit(2);
            punt = ricerca_stazione (Lista, dist1);
            macchina = NULL;

            if(punt != NULL) macchina = ricerca_auto (punt->automobili, autonomia);
            if(macchina != NULL && punt->automobili != NULL) {
                if(macchina->conta > 1) {
                    (macchina->conta)--;
                    (punt->num_auto)--;
                }
                else rottama_auto (&punt, macchina);
                printf("rottamata\n");
            }
            else printf("non rottamata\n");
        }

        else if(strcmp(stringa, "pianifica-percorso") == 0) {
            //stampa_stazioni (Lista);

            if(scanf("%d %d", &dist1, &dist2) != 2) exit(2);
            punt = ricerca_stazione (Lista, dist1);
            punt2 = ricerca_stazione (Lista, dist2);
            if(punt != NULL && punt != NULL) pianifica_percorso (punt, punt2);
            else printf("nessun percorso\n");
        }

        while(foo != '\n') if(scanf("%c", &foo) != 1) exit(2);
    }
    //stampa_stazioni (Lista);
    while(Lista != NULL) demolisci_stazione (&Lista, Lista);
    return 0;
}