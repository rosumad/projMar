#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sciezki {
  char *sciezkaWejscie;
  char *sciezkaLista;
  char *sciezkaWyjscie;
} Sciezki;

typedef struct slowa {
  char *slowoPrzed;
  char *slowoPo;
  struct slowa *nast;
} Wezel;

Wezel *utworzWezel() {
  Wezel *nowy = (Wezel*)malloc(sizeof(Wezel));
  nowy->slowoPrzed = NULL;
  nowy->slowoPo = NULL;
  nowy->nast = NULL;
  return nowy;
}

Wezel *listaPush(Wezel **glowa, Wezel *wezel) {
  if (!(*glowa)) {
    glowa = &wezel;
    return (*glowa);
  } else {
    wezel->nast = (*glowa);
    glowa = &wezel;
    return (*glowa);
  }
}

Wezel *wczytajListeSlowZPliku(char *sciezka) {
  FILE *plik = fopen(sciezka, "r");
  if (!plik) {
    printf("Blad otwarcia pliku %s\n", sciezka);
    return NULL;
  }
  Wezel *listaSlow = NULL;
  Wezel *temp = NULL;
  int rozmiarBufora = 512;
  char *bufor = (char*)malloc(sizeof(char)*rozmiarBufora);
  while (feof(plik) == 0) {
    if (fgets(bufor, rozmiarBufora, plik) == NULL) {
      printf("Blad wczytania linii z pliku %s\n", sciezka);
      return NULL;
    }
    temp = utworzWezel();
    temp->slowoPrzed = (char*)malloc(sizeof(char)*512);
    temp->slowoPo = (char*)malloc(sizeof(char)*512);
    if (sscanf(bufor, "%s %s", temp->slowoPrzed, temp->slowoPo) != 2) {
      printf("Bledna struktura linii w pliku %s\n", sciezka);
      return NULL;
    }
    listaSlow = listaPush(&listaSlow, temp);
  }
  free(bufor);
  fclose(plik);
  return listaSlow;
}

void okreslSlowoDoWpisania(Wezel *listaSlow, char **slowo) {
  Wezel *temp = listaSlow;
  while (temp) {
    if (strcmp(temp->slowoPrzed, (*slowo)) == 0)
      strcpy((*slowo), listaSlow->slowoPo);

    temp = temp->nast;
  }
}

int przetworzPlik(char *sciezkaWejscie, char* sciezkaWyjscie, Wezel *listaSlow) {
  FILE *plikWejscie = fopen(sciezkaWejscie, "r");
  if (!plikWejscie) {
    printf("Blad otwarcia pliku %s\n", sciezkaWejscie);
    return 1;
  }
  FILE *plikWyjscie = fopen(sciezkaWyjscie, "w");
  if (!plikWyjscie) {
    printf("Blad otwarcia pliku %s\n", sciezkaWyjscie);
    return 1;
  }
  int rozmiarSlowa = 512;
  char *slowo = (char*)malloc(sizeof(char)*rozmiarSlowa);
  while (feof(plikWejscie) == 0) {
    if (fscanf(plikWejscie, "%s", slowo) != 1) {
      printf("Blad wczytywania tekstu z pliku %s\n", sciezkaWejscie);
      return 2;
    }
    okreslSlowoDoWpisania(listaSlow, &slowo);
    fputs(slowo, plikWyjscie);
    fputc(fgetc(plikWejscie), plikWyjscie); //przepisywanie spacji i koncow linii
  }
  fclose(plikWejscie);
  fclose(plikWyjscie);
  free(slowo);
  return 0;
}

void zwolnijListe(Wezel *lista) {
  Wezel *nastepny, *usuwany = lista;

  while (usuwany) {
    nastepny = usuwany->nast;
    free(usuwany->slowoPo);
    free(usuwany->slowoPrzed);
    free(usuwany);
    usuwany = nastepny;
  }
}

void zwolnijSciezki(Sciezki *sciezki) {
  free(sciezki->sciezkaLista);
  free(sciezki->sciezkaWejscie);
  free(sciezki->sciezkaWyjscie);
}

void wyswietlPomoc() {
  printf("\n");
  printf("Tutaj powinna byc pomoc...");
  //TUTAJ DAJ KOD POMOCY
}

char *wezWejscieZParam(int argc, char *argv[]) {
  int i;
  for(i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-i") == 0) && (argc > i) ) {
      char *sciezkaWejscie = (char*)malloc(sizeof(char) * strlen(argv[i+1]));
      strcpy(sciezkaWejscie, argv[i+1]);
      return sciezkaWejscie;
    }
  }
  printf("Blad wczytania parametru -i.\n");
  return NULL;
}

char *wezListeZParam(int argc, char *argv[]) {
  int i;
  for(i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-l") == 0) && (argc > i) ) {
      char *sciezkaLista = (char*)malloc(sizeof(char) * strlen(argv[i+1]));
      strcpy(sciezkaLista, argv[i+1]);
      return sciezkaLista;
    }
  }
  printf("Blad wczytania parametru -l.\n");
  return NULL;
}

char *wezWyjscieZParam(int argc, char *argv[]) {
  int i;
  for(i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-o") == 0) && (argc > i) ) {
      char *sciezkaWyjscie = (char*)malloc(sizeof(char) * strlen(argv[i+1]));
      strcpy(sciezkaWyjscie, argv[i+1]);
      return sciezkaWyjscie;
    }
  }
  printf("Blad wczytania parametru -o.\n");
  return NULL;
}

int znajdzPomocWParam(int argc, char *argv[]) {
  int i;
  for(i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-h") == 0)) {
      return 1;
    }
  }
  return 0;
}

Sciezki *analizujParametry(int argc, char *argv[]) {
  if (argc > 1) {
    Sciezki *sciezki = (Sciezki*)malloc(sizeof(Sciezki));

    if (znajdzPomocWParam(argc, argv) == 1)
      return NULL;

    sciezki->sciezkaLista = wezListeZParam(argc, argv);
    if (!(sciezki->sciezkaLista))
      return NULL;

    sciezki->sciezkaWyjscie = wezWyjscieZParam(argc, argv);
    if (!(sciezki->sciezkaWyjscie))
      return NULL;

    sciezki->sciezkaWejscie = wezWejscieZParam(argc, argv);
    if (!(sciezki->sciezkaWejscie))
      return NULL;

    return sciezki;
  } else {
    return NULL;
  }
  return NULL;
}

int main (int argc, char *argv[]) {
  Sciezki *sciezki = analizujParametry(argc, argv);
  if (!sciezki) {
    wyswietlPomoc();
    return 1;
  }
  Wezel *listaSlow = wczytajListeSlowZPliku(sciezki->sciezkaLista);
  if(!listaSlow) {
    wyswietlPomoc();
    return 1;
  }
  if (przetworzPlik(sciezki->sciezkaWejscie, sciezki->sciezkaWyjscie, listaSlow) != 0) {
    wyswietlPomoc();
    return 1;
  }
  zwolnijListe(listaSlow);
  zwolnijSciezki(sciezki);
  free(sciezki);
  return 0;
}
