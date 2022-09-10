Prunescu Bogdan-Andrei 311CA

Pentru a rezolva aceasta tema am folosit 3 fisiere: "my_octave.c"
care citeste datele de intrare si executa comenzile, "my_functions.c", unde
am scris functiile necesare rularii programului, si "my_commands.h", un
header in care am declarat functiile din "my_functions.c".
	Se citeste comanda cat timp aceasta este diferita de Q si, folosind
un switch am tratat toate cazurile posibile. Am creat un vector de tipul
struct, numit "data" in care memorez pentru fiecare element o matrice alocata
dinamic, dimensiunile matricei si suma elementelor din matrice(pentru functia
de sortare).

Cazul 'L' aloca dinamic o matrice care citeste elementele din input
si o insereaza in vectorul "data". In plus, am verificat daca acest vector
este plin, caz in care i-am dublat memoria. Functia insert_matrix() insereaza
un element prin metoda deep_copy(pentru ca lucram cu pointeri in interiorul
structului). Complexitate comanda: O(n^2).

Cazul 'D' printeaza in O(1) dimensiunile matricei cerute.

Cazul 'P' are complexitatea O(n^2), deoarece afisam o matrice.

Cazul 'C' aloca dinamic o matrice in care inserez linile si  
coloanele specificate la intrare, dupa care este copiata in locul matricei 
vechi. Complexitate comanda: O(n^2).

Cazul 'M' inmulteste doua matrici in complexitate O(n^3).

Cazul 'O' sorteaza folosind sortarea prin selectie, in O(n^2).

Cazul 'T' copiaza transpusa unei matrici intr-o matrice noua, care
este copiata in locul celei vechi. Complexitate timp: O(n^2).

Cazul 'F' sterge matricea cu indicele citit prin permutarea la stanga
cu o pozitie a elementelor de la index pana la size - 1. Daca, dupa prea
multe stergeri, capacitatea este mai mare decat dublul dimensiunii folosite
de catre vectorul de matrici, atunci trebuie sa injumatatim memoria folosita
de catre vector. Complexitate comanda: O(n).

Cazul 'S' se foloseste de trei vectori de tip octave: 2 vectori care
retin cate 4 matrici(cele 4 submatrici ale lui a si 4 ale lui b) si un
vector care retine 8 matrici intermediare necesare calcularii produsului
celor doua matrici. Functia de apeleaza recursiv de fiecare data cand avem
nevoie de inmultirea a doua matrici (pana cand matricile au 1 elemente).
Complexitate algoritm Strassen: O(n^2.81).
	In plus, cazul 'M' si 'S' folosesc aceasi functie pentru ca amandoua
creeaza o matrice care este inserata la coada vectorului de matrici.

Complexitate timp program: O(n^3);
Complexitate memorie program: O(n * sizeof(octave)), unde n este numarul de
elemente alocate in vectorul "data", iar la final memoria este eliberata.
De asemenea, verificam de fiecare data cand alocam memorie daca s-a alocat
cu succes. Daca nu, eliberam toate variabilele cu care lucram si terminam
executia programului.
