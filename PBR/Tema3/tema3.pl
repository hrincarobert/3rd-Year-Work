%Topic Implement a program in PROLOG that solves the following logic problem:
%Four university professors were having a discussion using ancient oriental languages they knew and taught at their universities. From time to time one of %%the teachers acted as the translator for two other teachers who were talking. The four spoke the following foreign languages: Armenian, Persian, Greek and %%Aramaic. No language was common to all four interlocutors, but each knew two of the four languages, the others being foreign to them. Salal didn't know %%%Persian, but he translated when old Professor Atar wanted to talk to Eber. Eber spoke Aramaic and conversed freely with Zaman even though he did not know %%%Aramaic. Neither Salal nor Atar nor Zaman knew a single language in which they could understand each other. None of them spoke both Aramaic and Armenian. 
%What foreign languages did each of the four university professors know?

profesor(salal, L1, L2).
profesor(atar, L1, L2).
profesor(eber, L1, L2).
profesor(zaman, L1, L2).

stiu_2_limbi(L1, L2) :-
    member(L1, [armeana, persana, greaca, aramaica]),
    member(L2, [armeana, persana, greaca, aramaica]),
    L1 \= L2.


salal_nu_stie_persana(L1, L2) :-
    profesor(salal, L1, L2),
    \+ (L1 = persana ; L2 = persana).



eber_stie_aramaica(L1, L2) :-
    profesor(eber, L1, L2),
    (L1 = aramaica ; L2 = aramaica).

limbi_comune(L1, L2, L3, L4) :-
    (L1 = L3 ; L1 = L4 ; L2 = L3 ; L2 = L4).

zaman_nu_are_aramaica(L1, L2) :-
    profesor(zaman, L1, L2),
    \+ (L1 = aramaica ; L2 = aramaica).


no_common_language(L1, L2) :-
    \+ (L1 = L2).



not_both_aramaica_and_armeana(L1, L2) :-
    \+ (L1 = aramaica, L2 = armeana ; L1 = armeana, L2 = aramaica).


solution(Languages) :-
 
    profesor(salal, L1_salal, L2_salal),
    profesor(atar, L1_atar, L2_atar),
    profesor(eber, L1_eber, L2_eber),
    profesor(zaman, L1_zaman, L2_zaman),

    stiu_2_limbi(L1_salal, L2_salal),
    stiu_2_limbi(L1_atar, L2_atar),
    stiu_2_limbi(L1_eber, L2_eber),
    stiu_2_limbi(L1_zaman, L2_zaman),

    salal_nu_stie_persana(L1_salal, L2_salal),

   
    eber_stie_aramaica(L1_eber, L2_eber),
    limbi_comune(L1_eber, L2_eber, L1_zaman, L2_zaman),
    zaman_nu_are_aramaica(L1_zaman, L2_zaman),

    limbi_comune(L1_atar, L2_atar, L1_salal, L2_salal),

    

    no_common_language(L1_atar, L1_eber),
    no_common_language(L1_atar, L2_eber),
    no_common_language(L2_atar, L1_eber),
    no_common_language(L2_atar, L2_eber),


    not_both_aramaica_and_armeana(L1_salal, L2_salal),
    not_both_aramaica_and_armeana(L1_atar, L2_atar),
    not_both_aramaica_and_armeana(L1_zaman, L2_zaman),
    not_both_aramaica_and_armeana(L1_eber, L2_eber),


    Languages = [
        [salal, L1_salal, L2_salal],
        [atar, L1_atar, L2_atar],
        [eber, L1_eber, L2_eber],
        [zaman, L1_zaman, L2_zaman]
    ].

