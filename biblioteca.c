#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Am definit o constanta DIMAX cu valoarea 1000
#define DIMAX 1000

// definire structura
typedef struct {
  int ID;
  char titlu[100];
  char autor[100];
  int an;
  char *descriere;
} carte;

// implementare adaugare carte, citesc mai intai in sirul de caractere s cu
// fgets, apoi adaug NULL si copiez s in titlu, autor, etc. Getchar() goleste
// buffer-ul, apoi retunez cartea noua
carte ADD_BOOK(void) {
  carte carteNoua;
  // citesc temporar in s titlul, autorul, descrierea
  char s[DIMAX];

  scanf("%d", &carteNoua.ID);
  getchar();

  fgets(s, sizeof(s), stdin);
  s[strlen(s) - 1] = '\0';
  strcpy(carteNoua.titlu, s);

  fgets(s, sizeof(s), stdin);
  s[strlen(s) - 1] = '\0';
  strcpy(carteNoua.autor, s);

  scanf("%d", &carteNoua.an);
  getchar();

  fgets(s, sizeof(s), stdin);
  s[strlen(s) - 1] = '\0';
  carteNoua.descriere = (char *)malloc((strlen(s) + 1) * sizeof(char));
  strcpy(carteNoua.descriere, s);

  return carteNoua;
}

// folosesc aceasta functie pentru REPLACE, daca nu contine niciun cuvant, adaug
// word(echivalent cu string) altfel concatenez propozitia cu WORD2
void adaugaStringProp(char *prop, char *string, char *WORD2) {
  if (strlen(prop) == 0) {
    // descrierea nu contine nimic, adaug primul cuvant
    strcpy(prop, string);
  } else {
    strcat(prop, WORD2);
  }
}

// inlocuire WORD1 cu WORD2, am declarat o variabila pentru descriere noua, in
// word impart descrierea initiala in subsiruri, cat timp mai am cuvinte, daca
// in descriere_noua am adaugat un cuvant, adaug si spatiul, verific daca
// cuvantul retinut in word coincide cu WORD1(cuvantul pe care vreau sa il
// inlocuiesc), pentru conditia indeplinita apelez adaugaStringProp pentru a
// inlocui cu WORD2, altfel apelez aceeasi functie, dar adaug cuvantul vechi, nu
// efectuez nicio inlocuire
char *replaceDesc(char *descriere, char *WORD1, char *WORD2) {
  char *descriere_noua = (char *)malloc(10000 * sizeof(char));
  char *word = strtok(descriere, " ");  // impart in subsiruri

  while (word != NULL) {
    // adaug spatiu daca am adaugat cuvinte in descriere_noua

    if (strlen(descriere_noua) != 0) {
      strcat(descriere_noua, " ");
    }

    // verific daca word coincide cu ceea ce vreau sa inlocuiesc

    if (strcmp(word, WORD1) == 0) {
      // inlocuiesc WORD1 cu WORD2
      adaugaStringProp(descriere_noua, word, WORD2);
    } else {
      // nu inlocuiesc niciun cuvant
      adaugaStringProp(descriere_noua, word, word);
    }
    // se continua divizarea, obtinand noul cuvant
    word = strtok(NULL, " ");
  }

  descriere_noua[strlen(descriere_noua)] = '\0';
  // inlocuiesc descrierea veche cu cea noua
  strcpy(descriere, descriere_noua);

  return descriere;
}

// functia REPLACE, apelez replaceDesc si adaug noua descriere in structura
void REPLACE(int N, carte *carti, char WORD1[], char WORD2[]) {
  int i;
  for (i = 0; i < N; i++) {
    carti[i].descriere = replaceDesc(carti[i].descriere, WORD1, WORD2);
  }
}

// convertire in baza 16, salvez restul si testez in ce caz ma aflu(A, B, C, D,
// E, F), in cazul default convertesc retul la char, apoi returnez hexa
char *convertireLaHexa(int n) {
  char *hexa = (char *)malloc(150000 * sizeof(char));
  int nr = 0, r;

  while (n != 0) {
    r = n % 16;
    switch (r) {
      case 10:
        hexa[nr++] = 'A';
        break;
      case 11:
        hexa[nr++] = 'B';
        break;
      case 12:
        hexa[nr++] = 'C';
        break;
      case 13:
        hexa[nr++] = 'D';
        break;
      case 14:
        hexa[nr++] = 'E';
        break;
      case 15:
        hexa[nr++] = 'F';
        break;
      default:
        // r intreg, convertesc la char
        hexa[nr++] = r + '0';
    }
    n = n / 16;
  }
  hexa[nr] = '\0';

  // free(hexa);

  return hexa;

  
}

char *reverse(char *descriere);

// initializez litera cu 1(litera apare cel putin o data), testez daca este
// litera, compar literele, salvez numarul de aparitii in litera, cat timp nu
// depasesc lungimea cuvantului si literele sunt egale, apoi copiez in
// descriere_noua litera care se repeta,
// salvez in p conversia la baza 16, concatenez descriere_noua cu p(de ex a1),
// in nr salvez noua lungime a descrierii
// alfel daca nu este litera nu fac nicio schimbare si adaug in descriere_noua
char *encodeDesc(char *descriere) {
  int i, nr = 0, litera;
  char *p = (char *)malloc(150000 * sizeof(char));
  char *descriere_noua = (char *)malloc(150000 * sizeof(char));
  int lungime = strlen(descriere);

  i = 0;
  while (i < lungime) {
    litera = 1;
    if ((descriere[i] >= 'a' && descriere[i] <= 'z') ||
        (descriere[i] >= 'A' && descriere[i] <= 'Z')) {
      while ((i + litera) < lungime && descriere[i] == descriere[i + litera]) {
        litera++;
      }

      // adaug litera care se repeta
      descriere_noua[nr++] = descriere[i];
      // convertesc la baza 16
      p = convertireLaHexa(litera);
      reverse(p);
      // concatenez litera si nr de apariti acum in baza 16
      strcat(descriere_noua, p);
      // modific lungimea
      nr = strlen(descriere_noua);
    } else {
      // daca nu este litera, trec peste
      descriere_noua[nr++] = descriere[i];
    }

    // i + litera, de ex daca am aaaae, sar peste grupul repetat de a la e
    i = i + litera;
  }

  return descriere_noua;
}

// inversez descriere, parcurg inversez si readaug (a1 devine 1a)
char *reverse(char *descriere) {
  int i, nr = 0;
  char *descriere_noua = (char *)malloc(150000 * sizeof(char));

  for (i = strlen(descriere) - 1; i >= 0; i--) {
    descriere_noua[nr++] = descriere[i];
  }

  return descriere_noua;

  //free(descriere_noua);
}

// Functia ENCODE, folosesc un auxiliar(carte_aux) unde apelez encodeDesc,
// realochez descrierea pentru carti, apoi copiez in descriere noua descriere si
// apelez reverse, pentru a inversa
void ENCODE(int N, carte *carti, int ID) {
  int i;
  char carte_aux[100000];
  for (i = 0; i < N; i++) {
    if (carti[i].ID == ID) {
      strcpy(carte_aux, encodeDesc(carti[i].descriere));
      carti[i].descriere = (char *)realloc(
          carti[i].descriere, (strlen(carte_aux) + 1) * sizeof(char));
      strcpy(carti[i].descriere, carte_aux);
      carti[i].descriere = reverse(carti[i].descriere);
    }
  }
}

// Functia comparare pt qsort, retunrneaza la fel ca strcmp diferenta, pentru 1
// le ordoneaza crescator, pentru -1 descrescator, iar 0, nu se schimba nimic
int cmp(const void *a, const void *b) {
  carte *ca = (carte *)a;
  carte *cb = (carte *)b;

  if (ca->autor == cb->autor) {
    // strcmp -1, 0, 1
    return ca->ID - cb->ID;
  }

  return -1;
}

// transforma toate literele in litere mici
void caseinsensitive(char *cuvant) {
  int i;
  int a = strlen(cuvant);
  for (i = 0; i < a; i++) {
    if (cuvant[i] >= 'A' && cuvant[i] <= 'Z') {
      cuvant[i] = cuvant[i] + 32;
    }
  }
}

// implementare functie SEARCH pentru titlu, autor, verific daca ceea ce caut
// coincide cu ceva din structura, daca da gasit devine 1(sunt le fel), apoi in
// cartiGasite salvez cartea gasita, apelez functia qsort din stdlib pentru a le
// sorta crescator
void SEARCH1(int N, carte *carti, char *element, carte *cartiGasite, int *K,
             int *gasit) {
  int nr = 0, i;
  char carteautorcs[1000], cartetitlucs[1000];
  for (i = 0; i < N; i++) {
    strcpy(carteautorcs, carti[i].autor);
    strcpy(cartetitlucs, carti[i].titlu);
    caseinsensitive(carteautorcs);
    caseinsensitive(cartetitlucs);
    caseinsensitive(element);
    if ((strcmp(carteautorcs, element) == 0) ||
        (strcmp(cartetitlucs, element) == 0)) {
      *gasit = 1;
      cartiGasite[nr].ID = carti[i].ID;
      strcpy(cartiGasite[nr].titlu, carti[i].titlu);
      strcpy(cartiGasite[nr].autor, carti[i].autor);
      cartiGasite[nr].an = carti[i].an;
      cartiGasite[nr].descriere = (char *)malloc(DIMAX * sizeof(char));
      strcpy(cartiGasite[nr].descriere, carti[i].descriere);
      nr++;
    }
  }

  // le ordonez
  qsort(cartiGasite, nr, sizeof(carte), cmp);

  *K = nr;
}

// implementare functie SEARCH pentru an, la fel ca si la SEARCH1
void SEARCH2(int N, carte *carti, int an, carte *cartiGasite, int *K,
             int *gasit) {
  int nr = 0, i;
  for (i = 0; i < N; i++) {
    if (carti[i].an == an) {
      *gasit = 1;
      cartiGasite[nr].ID = carti[i].ID;
      strcpy(cartiGasite[nr].titlu, carti[i].titlu);
      strcpy(cartiGasite[nr].autor, carti[i].autor);
      cartiGasite[nr].an = carti[i].an;
      cartiGasite[nr].descriere = (char *)malloc(DIMAX * sizeof(char));
      strcpy(cartiGasite[nr].descriere, carti[i].descriere);
      nr++;
    }
  }

  *K = nr;
}

// afisez cartile si testez daca este necesar new line cu ajutorul variabilelor
// afisareFinala si afisareSearch
void afisare(int N, carte *carti, int afisareFinala, int afisareSearch) {
  int i;
  // printf("*\n");
  for (i = 0; i < N; i++) {
    printf("%d\n", carti[i].ID);
    printf("%s\n", carti[i].titlu);
    printf("%s\n", carti[i].autor);
    printf("%d\n", carti[i].an);
    printf("%s\n", carti[i].descriere);

    if (afisareSearch == 1 && i != N - 1) {
      printf("\n");
    }
  }

  if (afisareFinala == 0) {
    printf("\n");
  }
}

// salvez un numar convertit cu ajutorul lui atoi, daca nu este numar returnez
// -1
int intreg(char *a) {
  int i;
  int lungime = strlen(a);

  for (i = 0; i < lungime; i++) {
    // daca nu e cifra
    if (a[i] < '0' || a[i] > '9') {
      return -1;
    }
  }

  // convertesc de la string la intreg
  return atoi(a);
}

// citesc cartile cu ajutorul comenzii ADD_BOOK, apoi comenzile, am citit un
// string pe care il testez daca coincide cu vreo comanda, daca da se efctueaza
// apelurile
int main(void) {
  int N, M, i;
  int ID, index, K, gasit;
  char *WORD1 = (char *)malloc(DIMAX * sizeof(char));
  char *WORD2 = (char *)malloc(DIMAX * sizeof(char));
  char *comanda = (char *)malloc(DIMAX * sizeof(char));
  char a[DIMAX] = "";
  char aNou[DIMAX];
  scanf("%d", &N);

  carte *carti = (carte *)malloc(N * sizeof(carte));

  for (i = 0; i < N; i++) {
    carti[i] = ADD_BOOK();
  }

  scanf("%d", &M);
  getchar();

  for (i = 0; i < M; i++) {
    scanf("%s", comanda);

    if (strcmp(comanda, "SEARCH") == 0) {
      // dimensiunea
      K = 0;
      // variabila care verifica daca am gasit cuvantul
      gasit = 0;
      carte *cartiGasite = (carte *)malloc(DIMAX * sizeof(carte));
      fgets(a, sizeof(a), stdin);  // citesc ce vreau sa caut
      a[strlen(a) - 1] = '\0';
      // elimin spatiul
      strcpy(aNou, a + 1);
      strcpy(a, aNou);

      // salvez numarul in index (de ex 2002)
      index = intreg(a);

      // daca index este -1, atunci apelez SEARCH1(pt titlu, autor), daca este
      // numar apelez SEARCH2(pt an)
      if (index == -1) {
        SEARCH1(N, carti, a, cartiGasite, &K, &gasit);
      } else {
        SEARCH2(N, carti, index, cartiGasite, &K, &gasit);
      }

      if (gasit == 0) {
        printf("NOT FOUND\n\n");
      } else {
        // afisareSearch ia valoarea 1 pentru a afisa \n
        afisare(K, cartiGasite, 0, 1);
      }
    }

    if (strcmp(comanda, "REPLACE") == 0) {
      scanf("%s%s", WORD1, WORD2);
      REPLACE(N, carti, WORD1, WORD2);
    }

    if (strcmp(comanda, "ENCODE") == 0) {
      scanf("%d", &ID);
      ENCODE(N, carti, ID);
      getchar();
    }

    if (strcmp(comanda, "ADD_BOOK") == 0) {
      N = N + 1;
      carti = (carte *)realloc(carti, N * sizeof(carte));
      carti[N - 1] = ADD_BOOK();
    }
  }

  printf("%d\n", N);
  // afisareFinala = 1 si afisareSearch = 0 pentru a nu mai afisa \n
  afisare(N, carti, 1, 0);

  // for (i = 0; i < N; i++) {
  //   free(carti[i].descriere);
  // }

  free(carti);
  //free(WORD1);
  //free(WORD2);
  //free(comanda);

  return 0;
}
