# laboratoire 4 - Perf & Valgrind
#### auteur : Alexandre Gabrielli
#### date : 

## matériel

j'effectue ce laboratoire sur un rasbery Pi 3 Model B Rev 1.2 donc voici les informations essentiels:
Hardware	: BCM2835
Revision	: a02082
Serial		: 0000000040be5365
le fabriquant est donc Sony Uk
et les informations sur le processeur: 
processor	: 1
model name	: ARMv7 Processor rev 4 (v7l)
BogoMIPS	: 38.40
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x0
CPU part	: 0xd03
CPU revision	: 4

## version de linux 
debian 10.3
## version du kernel
4.19.97-v7+

### installation 

perf sur debian pose quelque problème , après avoir installer linux tools j'ai due installer une autre version de perf car le package 4.19 n'as pas été écris sur debian et je n'ai pas envie de l'écrire moi même.
du coup j'ai installer linux-perf-4.9 et ensuite modifier le fichier /usr/bin/perf  et modifier la ligne exec "perf_4.$version" "$@" par exec "perf_4.9" "$@" cella implique que perf n'est pas a as 100% fonctionnel et peu nécessiter d'utilisé les paramètre  --no-demangle ou --call-graph=lbr works.


## perf

pour commencer a profiler nos deux fonctions nous allons commencer par utilisé perf en mesurant les events suivants:

- task-clock pour voir l'utilisation du cpu
- context-switches pour voir s'il y a beaucoup de changement de contexte
- page-faults afin de voir si on a bien géré nos data
- branches et branch-misses pour voir si on est limité par des branch mis-prediction

nous allons lancé perf sur ./sort array 100000 et ./sort list 100000

### array 

![array with perf first version](.\_array_perf_first_version.png)

### list

![list with perf first version](.\_list_perf_first_version.png)


### discutions array 

on voit que nous avons 0 % de cache miss mais notre programme prend beaucoup de temps, on utilise beaucoup de temps cpu et nous avons énormément de branches car nous utilisons un tri par insertion et nous devons donc effectuer beaucoup de comparaison.

en changeant la premières version de notre code (ci dessous): 

```c
/* Arrange a array in increasing order of value */
void array_sort(uint64_t *data, const size_t len) {
    int i = 0, j = 0, tmp;
    for (i = 0; i < len; i++) {   // loop n times - 1 per element
        for (j = 0; j < len - i - 1; j++) { // last i elements are sorted already
            if (data[j] > data[j + 1]) {  // swop if order is broken
                tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }
}
```
par un tri nécessitant moins de comparaison comme un tri par sélection (code ci-dessous): 
```c

/* Arrange a array in increasing order of value */
void array_sort(uint64_t *data, const size_t len) {
    /*tri par selection*/
       int passage = 0;
    bool permutation = true;
    int en_cours;
   
    while ( permutation) {
        permutation = false;
        passage ++;
        for (en_cours=0;en_cours<20-passage;en_cours++) {
            if (data[en_cours]>data[en_cours+1]){
                permutation = true;
                // on echange les deux elements
                int temp = data[en_cours];
                data[en_cours] = data[en_cours+1];
                data[en_cours+1] = temp;
            }
        }
    }

}
```
on obtiens de bien meilleur performance. 

![array with perf selection sort](.\_array_perf_selection.png)

on voit que nous avons plus de branch-misses (environ 1.4%) mais nous effectuons beaucoup moins de branches (environ 700'000 contre plus de 5'000'00'000) et nécessitons moins de temps cpu (12ms contre ~120'000 ms). 

le résultat est donc un programme beaucoup plus rapide malgrès le fait que nous avons beaucoup plus de branch-misses.

essayons maintenant d'amélioré encore cella avec le tri rapide:

```c
void array_sort(int *tableau, int taille) {
    int mur, courant, pivot, tmp;
    if (taille < 2) return;
    // On prend comme pivot l element le plus a droite
    pivot = tableau[taille - 1];
    mur  = courant = 0;
    while (courant<taille) {
        if (tableau[courant] <= pivot) {
            if (mur != courant) {
                tmp=tableau[courant];
                tableau[courant]=tableau[mur];
                tableau[mur]=tmp;              
            }
            mur ++;
        }
        courant ++;
    }
    tri_rapide(tableau, mur - 1);
    tri_rapide(tableau + mur - 1, taille - mur + 1);
}

```

on obtiens 

![array with perf quick sort](.\_array_perf_rapide.png)

on voit que les deux derniers tri sont assez similaire, pour les séparer augmentons de manière significatif
la taille de l'array en le mettant par exemple a 


on peu voir très clairement que le tri rapide est le plus optimal pour ici

### discution list

maitenant nous allons essayer d'utiliser aussi le 
on peu voir que lorssque nous utilisons list nous avons énormément de branch-misses alors que nous avons quasiment le même nombre de branch total ce qui ralenti énormément le programme lorsque nous utilisons list (par rapport a array)
nous allons donc investigué notre code avec perf record et nous voyons très bien que c'est bien la fonction de tri qui pose problème. 
pour que le cpu aient moins de branch-misses nous allons changer le code suivant : 
```c
/* Arrange a list in increasing order of value */
void list_sort(struct list_element *start) {
    int swapped, i;
    struct list_element *ptr1;
    struct list_element *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr) {
            if (ptr1->data > ptr1->next->data) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
```
/* Arrange a list in increasing order of value */
void list_sort(struct list_element *start) {
    /*tri par selection*/
    struct list_element * current = start;
    while(current){
        struct list_element* min = current;
        struct list_element* next = current-> next;
        while(next){
            if (min->data > next->data){
                min = next; 
                next = next->next;
            }
            uint64_t temp = current->data;
            current->data = min->data;
            min->data = temp;
            current = current->next;
         }
     }
}

```c 

### résultat de la modification

on peu voir que l'on est beaucoup plus rapide surement par ce que nous avons beaucoup moins de branches et malgrès les 6% de branches miss nous somme bien meilleur avec ce tri

nous allons donc changer notre tri d'array pour avoir moins de branches afin d'amélioré le temps cpu

```
/* Arrange a array in increasing order of value */
void array_sort(uint64_t *data, const size_t len) {
    int i = 0, j = 0, tmp;
    for (i = 0; i < len; i++) {   // loop n times - 1 per element
        for (j = 0; j < len - i - 1; j++) { // last i elements are sorted already
            if (data[j] > data[j + 1]) {   // swop if order is broken
                tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }

}
```c

## valgrind 




```