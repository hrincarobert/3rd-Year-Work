import sys
import xml.etree.cElementTree as ET
import argparse

import clips

def get_words():
    words = []
    tree = ET.parse('2024-05-18-lexems.xml')
    root = tree.getroot()
    for item in root.findall(f'.//Lexem'):
        form = item.find('InflectedForm').find('Form').text
        inflectionId = item.find('InflectedForm').find('InflectionId').text
        if int(inflectionId) <= 24:
            # split_form = form.split('\'')
            # base_form = split_form[0] if len(split_form) > 1 else form
            # ending = split_form[1] if len(split_form) > 1 else None
            #print(base_form)
            words.append({'form': form.replace('\'', ""),
                          'inflectionid': inflectionId,
                        #   'base': base_form,
                        #   'ending': ending
                          })
    #print(words)
    
    return words

def get_inflections():
    inflections = {}
    tree = ET.parse('2024-05-11-inflections.xml')
    root = tree.getroot()
    for item in root.findall('.//Inflection'):
        id = int(item.attrib['id'])
        if id <= 24:
            description_string = item.find('Description').text.replace(" ", "")
            description_list = description_string.split(',')
            gender = description_list[0].split('Substantiv')[1]
            # inflections.append({id: gender})
            inflections[id] = gender
            # print({id: gender})
            # print(f"{item.attrib['id']} {gender}")
    return inflections

def generate_form(input, input_plural=""):
    inflections = get_inflections()
    words = get_words()
    
    # input = "perete"
    
    template = """
    (deftemplate word
        (slot base (type STRING))
        (slot plural (type STRING))
        (slot gender (type STRING))
    )
    """
    
    FUNCTIONS = [
        """
        (deffunction get-last-char (?str)
            (return (sub-string (str-length ?str) (str-length ?str) ?str))
        )
        """,
        """
        (deffunction get-second-to-last-char (?str)
            (return (sub-string (- (str-length ?str) 1) (- (str-length ?str) 1) ?str))
        )
        """
    ]
    
    RULES = [
        """
        (defrule first_feminine_a
            ?w <- (word (base ?base) (gender "feminin"))
            (test (eq "a" (get-last-char ?base)))
            (test (neq "e" (get-second-to-last-char ?base)))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "ua") crlf)
            (printout t "N-A p " (str-cat ?base "le") " " (str-cat ?base "lele") crlf)
            (printout t "G-D s " (str-cat ?base "le") " " (str-cat ?base "lei") crlf)
            (printout t "G-D p " (str-cat ?base "le") " " (str-cat ?base "lelor") crlf)
        )
        """,
        """
        (defrule first_feminine_ad_plural_empty
            ?w <- (word (base ?base) (plural "") (gender "feminin"))
            (test (eq "ă" (get-last-char ?base)))
            =>
            (printout t "Plural form required for this word" crlf)
        )
        """,
        """
        (defrule first_feminine_ad
            ?w <- (word (base ?base) (plural ?plural) (gender "feminin"))
            (test (eq "ă" (get-last-char ?base)))
            (test (neq "" ?plural))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "a") crlf)
            (printout t "N-A p " ?plural " " (str-cat ?plural "le") crlf)
            (printout t "G-D s " ?plural " " (str-cat ?plural "i") crlf)
            (printout t "G-D p " ?plural " " (str-cat ?plural "lor") crlf)
        )
        """,
        """
        (defrule first_feminine_ea
            ?w <- (word (base ?base) (gender "feminin"))
            (test (eq "a" (get-last-char ?base)))
            (test (eq "e" (get-second-to-last-char ?base)))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "aua") crlf)
            (printout t "N-A p " (str-cat ?str "le") " " (str-cat ?str "lele") crlf)
            (printout t "G-D s " (str-cat ?str "le") " " (str-cat ?str "lei") crlf)
            (printout t "G-D p " (str-cat ?str "le") " " (str-cat ?str "lelor") crlf)
        )
        """,
        """
        (defrule second_masculine
            ?w <- (word (base ?base) (gender "masculin"))
            (test (eq (str-index (get-last-char ?base) "aeiou") FALSE))
            (test (neq "t" (get-last-char ?base)))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "ul") crlf)
            (printout t "N-A p " (str-cat ?base "i") " " (str-cat ?base "ii") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "ului") crlf)
            (printout t "G-D p " (str-cat ?base "i") " " (str-cat ?base "ilor") crlf)
        )
        """,
        """
        (defrule second_masculine_t
            ?w <- (word (base ?base) (gender "masculin"))
            (test (eq (str-index (get-last-char ?base) "aeiou") FALSE))
            (test (eq "t" (get-last-char ?base)))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?base "ul") crlf)
            (printout t "N-A p " (str-cat ?str "ți") " " (str-cat ?str "ții") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "ului") crlf)
            (printout t "G-D p " (str-cat ?str "ți") " " (str-cat ?str "ților") crlf)
)
        """,
        """
        (defrule second_masculine_u
            ?w <- (word (base ?base) (gender "masculin"))
            (test (eq "u" (get-last-char ?base)))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "ul") crlf)
            (printout t "N-A p " (str-cat ?str "i") " " (str-cat ?str "ii") crlf)
            (printout t "G-D s " ?base " " (str-cat ?str "ului") crlf)
            (printout t "G-D p " (str-cat ?str "i") " " (str-cat ?str "ilor") crlf)
        )
        """,
        """
        (defrule second_masculine_semi_i_u
            ?w <- (word (base ?base) (gender "masculin"))
            (test (eq "i" (get-last-char ?base)))
            (test (neq (str-index (get-second-to-last-char ?base) "aeiou") FALSE))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "ul") crlf)
            (printout t "N-A p " ?base " " (str-cat ?base "i") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "ului") crlf)
            (printout t "G-D p " ?base " " (str-cat ?base "lor") crlf)
        )
        """,
        """
        (defrule second_neuter_u
            ?w <- (word (base ?base) (gender "neutru"))
            (test (eq "u" (get-last-char ?base)))
            (test (eq (str-index (get-second-to-last-char ?base) "aeiou") FALSE))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "ul") crlf)
            (printout t "N-A p " (str-cat ?str "e") " " (str-cat ?str "ele") crlf)
            (printout t "G-D s " ?base " " (str-cat ?str "ului") crlf)
            (printout t "G-D p " (str-cat ?str "e") " " (str-cat ?str "elor") crlf)
        )
        """,
        """
        (defrule second_neuter_semi_u
            ?w <- (word (base ?base) (gender "neutru"))
            (test (eq "u" (get-last-char ?base)))
            (test (neq (str-index (get-second-to-last-char ?base) "aeiou") FALSE))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "l") crlf)
            (printout t "N-A p " (str-cat ?base "ri") " " (str-cat ?base "rile") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "lui") crlf)
            (printout t "G-D p " (str-cat ?base "ri") " " (str-cat ?base "rilor") crlf)
        )
        """,
        """
        (defrule second_neuter_i
            ?w <- (word (base ?base) (gender "neutru"))
            (test (eq "i" (get-last-char ?base)))
            (test (eq (str-index (get-second-to-last-char ?base) "aeiou") FALSE))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "ul") crlf)
            (printout t "N-A p " (str-cat ?base "uri") " " (str-cat ?base "urile") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "ului") crlf)
            (printout t "G-D p " (str-cat ?base "uri") " " (str-cat ?base "urilor") crlf)
        )
        """,
        """
        (defrule second_neuter_plural_empty
            ?w <- (word (base ?base) (plural "") (gender "neutru"))
            (test (eq (str-index (get-last-char ?base) "aeiou") FALSE))
            =>
            (printout t "Plural form required for this word" crlf)
        )
        """,
        """
        (defrule second_neuter
            ?w <- (word (base ?base) (plural ?plural) (gender "neutru"))
            (test (eq (str-index (get-last-char ?base) "aeiou") FALSE))
            (test (neq "" ?plural))
            =>
            (printout t "N-A s " ?base " " (str-cat ?base "ul") crlf)
            (printout t "N-A p " ?plural " " (str-cat ?plural "le") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "ului") crlf)
            (printout t "G-D p " ?plural " " (str-cat ?plural "lor") crlf)
        )
        """,
        """
        (defrule third_plural_empty
            ?w <- (word (base ?base) (plural ""))
            (test (eq "e" (get-last-char ?base)))
            =>
            (printout t "Plural form required for this word" crlf)
        )
        """,
        """
        (defrule third_feminine_e
            ?w <- (word (base ?base) (plural ?plural) (gender "feminin"))
            (test (eq "e" (get-last-char ?base)))
            (test (neq "" ?plural))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?base "a") crlf)
            (printout t "N-A p " ?plural " " (str-cat ?plural "le") crlf)
            (printout t "G-D s " ?plural " " (str-cat ?plural "i") crlf)
            (printout t "G-D p " ?plural " " (str-cat ?plural "lor") crlf)
        )
        """,
        """
        (defrule third_masculine_e
            ?w <- (word (base ?base) (plural ?plural) (gender "masculin"))
            (test (eq "e" (get-last-char ?base)))
            (test (neq "" ?plural))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?base "le") crlf)
            (printout t "N-A p " ?plural " " (str-cat ?plural "i") crlf)
            (printout t "G-D s " ?base " " (str-cat ?base "lui") crlf)
            (printout t "G-D p " ?plural " " (str-cat ?plural "lor") crlf)
        )
        """,
        """
        (defrule third_neuter_e
            ?w <- (word (base ?base) (plural ?plural) (gender "neutru"))
            (test (eq "e" (get-last-char ?base)))
            (test (neq "" ?plural))
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?base "le") crlf)
            (printout t "N-A p " ?plural " " (str-cat ?plural "le") crlf)
            (printout t "G-D s " ?base " " (str-cat ?plural "lui") crlf)
            (printout t "G-D p " ?plural " " (str-cat ?plural "lor") crlf)
        )
        """,
        """
        (defrule third_days_1
            ?w <- (word (base ?base))
            (or
                (test (eq "luni" ?base))
                (test (eq "marți" ?base))
                (test (eq "miercuri" ?base))
                (test (eq "vineri" ?base))
            )
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "ea") crlf)
            (printout t "N-A p " ?base " " (str-cat ?str "ile") crlf)
            (printout t "G-D s " ?base " " (str-cat ?str "ii") crlf)
            (printout t "G-D p " ?base " " (str-cat ?str "ilor") crlf)
        )
        """,
        """
        (defrule third_days_2 
            ?w <- (word (base ?base))
            (or
                (test (eq "joi" ?base))
            )
            =>
            (bind ?str (sub-string 1 (- (str-length ?base) 1) ?base))
            (printout t "N-A s " ?base " " (str-cat ?str "ia") crlf)
            (printout t "N-A p " ?base " " (str-cat ?str "ile") crlf)
            (printout t "G-D s " ?base " " (str-cat ?str "ii") crlf)
            (printout t "G-D p " ?base " " (str-cat ?str "ilor") crlf)
        )
        """
    ]

    # ,
    #     """
    #     (defrule teststr
    #         ?w <- (word (base ?base))
    #         =>
    #         (printout t "last char: " (get-last-char ?base))
    #         (printout t (str-index (get-last-char ?base) "aeiou") crlf)
    #         (printout t "sec-to-last char: " (get-second-to-last-char ?base))
    #         (printout t (str-index (get-second-to-last-char ?base) "aeiou") crlf)
    #     )
    #     """
    
    env = clips.Environment()
    env.build(template)
    
    for func in FUNCTIONS:
        env.build(func)
    
    for rule in RULES:
        env.build(rule)

    c_template = env.find_template('word')
    
    for word in words:
        if word['form'] == input:
            print(f"{word['form']} found in lexems")
            gender = inflections[int(word['inflectionid'])]

            c_template.assert_fact(base=word['form'], plural=input_plural, gender=gender)
            break
    
    for fact in env.facts():
         print(fact)

    env.run()

def check_word(input):
    words = get_words()
    
    template = """
    (deftemplate word
        (slot base (type STRING))
    )
    """
    
    FUNCTIONS = [
        """
        

        (deffunction get-last-char (?str)
            (return (sub-string (str-length ?str) (str-length ?str) ?str))
            
        )
        """,
        """
        (deffunction get-second-to-last-char (?str)
            (return (sub-string (- (str-length ?str) 1) (- (str-length ?str) 1) ?str))
        )
        """,
        """
        
        (deffunction str-ends-with (?str1 ?str2)
            (bind ?len1 (str-length ?str1))
            (bind ?len2 (str-length ?str2))
            (if (<= ?len2 ?len1) then
                (bind ?start-index (- ?len1 ?len2))
                
                (if (eq (sub-string (+ ?start-index 1) ?len1 ?str1) ?str2) then
                    (return TRUE)
                else
                    (return FALSE)
                )
            else
                (return FALSE)
            )
        )  
        """,
        """ 

        (deffunction str-doesnt-end-with (?str1 ?str2)
            (bind ?len1 (str-length ?str1))
            (bind ?len2 (str-length ?str2))
            (if (<= ?len2 ?len1) then
                (bind ?start-index (- ?len1 ?len2))
                
                (if (neq (sub-string (+ ?start-index 1) ?len1 ?str1) ?str2) then
                    (return TRUE)
                else
                    (return FALSE)
                )
            else
                (return FALSE)
            )
        ) 

        """
    ]
    
    RULES = [
        """
        (defrule ends_with_suffix
            
            ?w <- (word (base ?base))
            (test (or 
                (str-ends-with ?base "itele")
                (str-ends-with ?base "itelor")
                (str-ends-with ?base "ițe")
                (str-ends-with ?base "are")
                (str-ends-with ?base "arele")
                (str-ends-with ?base "le")
                (str-ends-with ?base "oși")
                (str-ends-with ?base "ase")
                (str-ends-with ?base "area")
                (str-ends-with ?base "ărilor")
                (str-ends-with ?base "ările")
                (str-ends-with ?base "ul")
                (str-ends-with ?base "imi")
                (str-ends-with ?base "imea")
                (str-ends-with ?base "ism")
                (str-ends-with ?base "ismului")
                (str-ends-with ?base "iști")
                (str-ends-with ?base "ere")
                (str-ends-with ?base "it")
                (str-ends-with ?base "ilor")
                (str-ends-with ?base "ușă")
                (str-ends-with ?base "itului")
                (str-ends-with ?base "ișurilor")
                (str-ends-with ?base "ului")
                (str-ends-with ?base "ile")))  
=>
            (if (str-ends-with ?base "ile")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "ilor")
                then (bind ?base (sub-string 1 (- (str-length ?base) 4) ?base)))
            (if (str-ends-with ?base "ușă")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "iți")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "ișurilor")
                then (bind ?base (sub-string 1 (- (str-length ?base) 8) ?base)))
            (if (str-ends-with ?base "it")
                then (bind ?base (sub-string 1 (- (str-length ?base) 2) ?base)))
            (if (str-ends-with ?base "itului")
                then (bind ?base (sub-string 1 (- (str-length ?base) 6) ?base)))
            (if (str-ends-with ?base "itele")
                then (bind ?base (sub-string 1 (- (str-length ?base) 5) ?base)))
            (if (str-ends-with ?base "itelor")
                then (bind ?base (sub-string 1 (- (str-length ?base) 6) ?base)))
            (if (str-ends-with ?base "ițe")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "are")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "arele")
                then (bind ?base (sub-string 1 (- (str-length ?base) 5) ?base)))
            (if (str-ends-with ?base "le")
                then (bind ?base (sub-string 1 (- (str-length ?base) 2) ?base)))
            (if (str-ends-with ?base "oși")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "ase")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "area")
                then (bind ?base (sub-string 1 (- (str-length ?base) 4) ?base)))
            (if (str-ends-with ?base "ările")
                then (bind ?base (sub-string 1 (- (str-length ?base) 5) ?base)))
            (if (str-ends-with ?base "ărilor")
                then (bind ?base (sub-string 1 (- (str-length ?base) 6) ?base)))
            (if (str-ends-with ?base "ul")
                then (bind ?base (sub-string 1 (- (str-length ?base) 2) ?base)))
            (if (str-ends-with ?base "imi")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "imea")
                then (bind ?base (sub-string 1 (- (str-length ?base) 4) ?base)))
            (if (str-ends-with ?base "ism")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "ismului")
                then (bind ?base (sub-string 1 (- (str-length ?base) 7) ?base)))
            (if (str-ends-with ?base "iști")
                then (bind ?base (sub-string 1 (- (str-length ?base) 4) ?base)))
            (if (str-ends-with ?base "ere")
                then (bind ?base (sub-string 1 (- (str-length ?base) 3) ?base)))
            (if (str-ends-with ?base "ului")
                then (bind ?base (sub-string 1 (- (str-length ?base) 4) ?base)))
            
            
            (assert (word (base ?base)))
            (assert (first-rule-executed))
            
    )

        """,
        """
        (defrule doesnt_end_with_suffix
        (not (first-rule-executed))
        ?w <- (word (base ?base))
        (test (or 
            (str-doesnt-end-with ?base "itele")
            (str-doesnt-end-with ?base "itelor")
            (str-doesnt-end-with ?base "ițe")
            (str-doesnt-end-with ?base "are")
            (str-doesnt-end-with ?base "arele")
            (str-doesnt-end-with ?base "le")
            (str-doesnt-end-with ?base "oși")
            (str-doesnt-end-with ?base "ase")
            (str-doesnt-end-with ?base "area")
            (str-doesnt-end-with ?base "ărilor")
            (str-doesnt-end-with ?base "ările")
            (str-doesnt-end-with ?base "ul")
            (str-doesnt-end-with ?base "imi")
            (str-doesnt-end-with ?base "imea")
            (str-doesnt-end-with ?base "ism")
            (str-doesnt-end-with ?base "ismului")
            (str-doesnt-end-with ?base "iști")
            (str-doesnt-end-with ?base "ere")
            (str-doesnt-end-with ?base "it")
            (str-doesnt-end-with ?base "ilor")
            (str-doesnt-end-with ?base "ușă")
            (str-doesnt-end-with ?base "itului")
            (str-doesnt-end-with ?base "ișurilor")
            (str-doesnt-end-with ?base "ile")))
            
=>
        
        (assert (word (base ?base)))
           
    )

        """
    ]
    
    env = clips.Environment()
    env.build(template)
    
    for func in FUNCTIONS:
        env.build(func)
    
    for rule in RULES:
        env.build(rule)

    c_template = env.find_template('word')
    c_template.assert_fact(base=input)
    
    env.run()

    result=[]
    for fact in env.facts():
        if fact.template.name == 'word':
            for slot in fact:
                result.append(slot[1])
        # print(fact)
                #print(f"{slot[0]}: {slot[1]}")
    # return result
    for word in words:
        if word['form'] in result:
            print(f'Valid word\n rădăcină: {word['form']}\n')
            return
    print('Invalid word')


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--form", nargs='+', type=str, help="Generate all forms of the word")
    parser.add_argument("-c", "--check", type=str, help="Check if the word is valid")
    args = parser.parse_args()
    if args.form:
        match len(args.form):
            case 1:
                # print("### 1.ESTE CUVANT VALID? ###\n")
                generate_form(args.form[0])
            
            case 2:                
                # print("### 2.TOATE FORMELE CUVANT ###\n")
                generate_form(args.form[0], args.form[1])
                
            case _:
                parser.error("Invalid number of arguments for option [-f]")
                
        # print(args.form)
    
    if args.check:
        results = check_word(args.check)
        # print(args.check)
    
    if args.form is None and args.check is None:
        parser.error("At least one argument is required")