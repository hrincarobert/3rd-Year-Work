%Implement in CLIPS a program that displays a menu with options:
%Add a student.
%Add lab scores for a student.
%Add exam score for a student.
%Add project score for a student.
%Show student status.
%Shows passed and failed students.
%Exit.
%
%After solving the selected option, the program returns to the main menu.

(deftemplate Student
    (slot nume)
    (multislot laborator)
    (slot examen)
    (slot proiect)  
)

(deffacts fapte_initiale
    (meniu)
)

(defrule afiseaza_meniu
    ?a <- (meniu)
    =>
    (retract ?a)
    (printout t "1. Adauga un student" crlf)
    (printout t "2. Adauga punctaje la laborator pentru un student" crlf)
    (printout t "3. Adauga punctaj la examen pentru un student" crlf)
    (printout t "4. Adauga punctaj la proiect pentru un student" crlf)
    (printout t "5. Afiseaza situatie student" crlf)
    (printout t "6. Afiseaza studentii promovati si cei nepromovati" crlf)
    (printout t "7. Iesire" crlf)
    (printout t "Dati optiunea: ")
    (assert (optiune (read)))
)
(defrule meniu-initial
    (not (meniu))
    =>
    (assert (meniu))
)

(defglobal ?*studenti* = (create$))

(defrule adauga-student
    ?a <- (optiune 1)
    
    =>
    (retract ?a)
    (printout t "Introduceti numele student: ")
    (bind ?nume (read))
    (assert (Student (nume ?nume)))
    (printout t "Studentul adaugat cu succes." crlf)
    (assert (afiseaza_meniu))
)

(defrule adauga-punctaje-laborator
    ?a <- (optiune 2)
    ?student <- (Student (nume ?name)
                        (laborator $?laborator)
                        (examen ?examen)
                        (proiect ?proiect))
    =>
    (retract ?a)
    (printout t "Introduceti numele student: ")
    (bind ?name (read))
    (bind ?student1 (find-fact ((?f Student)) (eq ?f:nume ?name)))
    (printout t "Nume gasit: " ?name crlf)
    (if ?student1
        then
            (printout t "Dati note " ?name ": ")
            (bind ?laborator (explode$ (readline)))
            (modify ?student (laborator ?laborator))
            (printout t "Note laborator pentru " ?name " sunt " ?laborator ".")
        else
            (printout t "Studentul " ?name " nu a fost gasit.")
    )
    (assert (afiseaza_meniu))
)


(defrule adauga-punctaje-examen

    ?a <- (optiune 3)
    ?student <- (Student (nume ?name)
                        (laborator $?laborator)
                        (examen ?examen)
                        (proiect ?proiect))
    =>
    (retract ?a)
    (printout t "Introduceti numele student: ")
    (bind ?name (read))
    (bind ?student1 (find-fact ((?f Student)) (eq ?f:nume ?name)))
    (printout t "Nume gasit: " ?name crlf)
    (if ?student1
        then
            (printout t "Nota examen " ?name ": ")
            (bind ?examen (read))
            (modify ?student (examen ?examen))
            (printout t "Nota examen pt. " ?name " este " ?examen ".")
        else
            (printout t "Studentul " ?name " nu a fost gasit.")
    )
    (assert (afiseaza_meniu))

)


(defrule adauga-punctaje-proiect

    ?a <- (optiune 4)
    ?student <- (Student (nume ?name)
                        (laborator $?laborator)
                        (examen ?examen)
                        (proiect ?proiect))
    =>
    (retract ?a)
    (printout t "Introduceti numele student: ")
    (bind ?name (read))
    (bind ?student1 (find-fact ((?f Student)) (eq ?f:nume ?name)))
    (printout t "Nume gasit: " ?name crlf)
    (if ?student1
        then
            (printout t "Nota proiect pt. " ?name ": ")
            (bind ?proiect (read))
            (modify ?student (proiect ?proiect))
            (printout t "Nota proiect pt. " ?name " este " ?proiect ".")
        else
            (printout t "Studentul " ?name " nu a fost gasit.")
    )
    (assert (afiseaza_meniu))


)

(defrule afiseaza-situatie-student
    ?a <- (optiune 5)
    =>
    (retract ?a)
    (printout t "Introduceti numele studentului: ")
    (bind ?nume (read))
    (bind ?student (find-all-facts ((?s Student)) (eq ?s:nume ?nume)))
    (if (eq (length$ ?student) 0)
        then
            (printout t "Studentul nu exista." crlf)
        else
            (bind ?student (nth$ 1 ?student))
            (printout t "Situatia studentului " ?nume ":" crlf)
            (printout t "Punctaj laborator: " (fact-slot-value ?student laborator) crlf)
            (printout t "Punctaj examen: " (fact-slot-value ?student examen) crlf)
            (printout t "Punctaj proiect: " (fact-slot-value ?student proiect) crlf)
    )
    (assert (afiseaza_meniu))
)


(defrule afiseaza-promovati
   ?a <- (optiune 6)
   =>
   (retract ?a)
   (printout t "Studenti promovati:" crlf)
   (foreach ?student ?*studenti*
      (bind ?total 0)
      (bind ?num-lab (length (fact-slot-value ?student laborator))) 
      (foreach ?val (fact-slot-value ?student laborator)
         (bind ?total (+ ?total ?val)))
      (bind ?average (/ ?total ?num-lab)) ; Calculate the average
      (if (and (>= ?average 5)
               (>= (fact-slot-value ?student examen) 5)
               (>= (fact-slot-value ?student proiect) 5))
         then
         (printout t "Student promovat:" (fact-slot-value ?student nume) crlf)
         else
         (printout t "Student nepromovat:" (fact-slot-value ?student nume) crlf)))
   (assert (afiseaza_meniu)))




(defrule initializare-global-studenti
    ?a <- (optiune 8)
    =>
    (retract ?a)
    (do-for-all-facts ((?s Student)) TRUE
        =>
        (bind ?*studenti* (create$ ?s ?*studenti*))
    )
)

(defrule iesire
    ?a <- (optiune 7)
    =>
    (retract ?a)
    (printout t "Programul se inchide." crlf)
    (halt)
)