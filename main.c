#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct slowa {
  char *slowoPrzed;
  char *slowoPo;
  struct slowa *nast;
};

void listaDodajNaKoniec(struct slowa **glowa, struct slowa *wezel) {
  if (wezel) {
    if (!(*glowa)) {
      (*glowa) = wezel;
    } else {
      (wezel->nast) = (*glowa);
      (*glowa) = wezel;
    }
  }
}

struct slowa *utworzWezel() {
  struct slowa *nowy = (struct slowa*) malloc (sizeof(struct slowa));
  nowy->slowoPrzed = NULL;
  nowy->slowoPo = NULL;
  nowy->nast = NULL;
  return nowy;
}

int wczytajSlowaDoPodmiany(char *sciezka, struct slowa **listaSlow) {
  FILE *plik = fopen(sciezka, "r");
  if (!plik)
    return 1;

  char bufor[512];
  while (feof(plik) == 0) {
    struct slowa *noweSlowo = utworzWezel();
    if (fgets(bufor, 512, plik) != NULL) {
      noweSlowo->slowoPrzed = strtok(bufor, " ");
      noweSlowo->slowoPo = strtok(NULL, " ");
      listaDodajNaKoniec(listaSlow, noweSlowo);
    }
  }
  fclose(plik);
  return 0;
}

int main (int argc, char *argv[]) {
  struct slowa *listaSlow = NULL;
  wczytajSlowaDoPodmiany("lista.txt", &listaSlow);
  printf("%s, %s", (listaSlow->slowoPrzed), (listaSlow->slowoPo));
  return 0;
}
