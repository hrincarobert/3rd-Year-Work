% CRYPTARITHMETICS
% Predicat pentru a inversa o lista


inversare_lista([], []).
inversare_lista([A|B], D) :-
    inversare_lista(B, C),
    append(C, [A], D).

% Predicat pentru a inversa o lista de liste
inversare_lista_de_liste([], []).
inversare_lista_de_liste([A|B], [C|D]) :-
    inversare_lista(A, C),
    inversare_lista_de_liste(B, D).

% Predicate pentru a genera o lista cu numere de la 0 la 9

generare_lista_cifre(Numere) :-
    generare_lista_cifre_(0, Numere).

generare_lista_cifre_(10, []).
generare_lista_cifre_(N, [N|Rest]) :-
    N < 10,
    N1 is N + 1,
    generare_lista_cifre_(N1, Rest).



% Predicate pentru a verifica daca 2 liste sunt identice, inafara de primul element

verificare_lista(A, [A|B], B).
verificare_lista(A, [F|D], [F|E]) :-
    verificare_lista(A, D, E).


% Predicat care selectează o cifra din lista și generează alta lista fara cifra aia. 
% Daca cifra este deja instantiat si este in Cifre, atunci nu va aparea in lista noua.
% Daca cifra nu este instantiat sau nu este in Cifre, noua listă = lista originală.

cifra_verificare(D, Cifre, Cifre0) :-
    (   var(D)
    ->  verificare_lista(D, Cifre, Cifre0)
    ;   verificare_lista(D, Cifre, Cifre0)
    ->  true
    ;   Cifre0 = Cifre
    ).


adunare(A, D, B) :-
    B is A + D.

suma_maiMic_10(ColSum, Sum, 0) :-
    ColSum < 10,
    Sum = ColSum.


suma_maiMare_10(ColSum, Sum, 1) :-
    ColSum >= 10,
    Sum is ColSum - 10.

% Predicate pentru a calcula suma unei coloane intr-o matrice in forma unei liste de liste.

suma_coloana_([], Cifre, Sum, Sum, Cifre, []).

suma_coloana(SumaRanduri, Cifre, Carry, ColSum, Cifre0, SumaRanduriNew) :-
    suma_coloana_(SumaRanduri, Cifre, 0, ColSum0, Cifre0, SumaRanduriNew),
    adunare(ColSum0, Carry, ColSum).
   
suma_coloana_([G|M], Cifre, A, Sum, Cifre01, [XY|SumaRanduriNew]) :-
    G = [D|XY],
    cifra_verificare(D, Cifre, Cifre0),
    adunare(A,D, B),
    suma_coloana_(M, Cifre0, B, Sum, Cifre01, SumaRanduriNew).


% Predicat folosit pentru a aduna coloanele si pentru a tine cont carry in timpul adunarii.

adaugare_cifra([], _, _, 0).
adaugare_cifra([Sum|SumaCifre], SumaRanduri, Cifre, Carry) :-
    suma_coloana(SumaRanduri, Cifre, Carry, ColSum, Cifre0, SumaRanduriNew),
    (
        suma_maiMic_10(ColSum, Sum, Carry1) ;
        suma_maiMare_10(ColSum, Sum, Carry1)
    ),
    cifra_verificare(Sum, Cifre0, Cifre00),
    adaugare_cifra(SumaCifre, SumaRanduriNew, Cifre00, Carry1).


% Predicat folosit pentru a intializa listele (se inverseaza pentru simplificarea adunarii), si pentru a ne asigura ca prima litera nu o sa fie asignata cu 0

solver(SumaRanduri, SumaCifre) :-

    inversare_lista_de_liste(SumaRanduri, SumaRanduri1),
    inversare_lista(SumaCifre, SumaCifre2),
    generare_lista_cifre(Cifre),
    SumaCifre = [Starter|_],
    dif(Starter,0),
    adaugare_cifra(SumaCifre2, SumaRanduri1, Cifre, 0).

% ?- solver([[0,S,E,N,D], [0,M,O,R,E]], [M,O,N,E,Y])
% ?- solver([[0,C,O,C,A], [0,C,O,L,A]], [O,A,S,I,S]).