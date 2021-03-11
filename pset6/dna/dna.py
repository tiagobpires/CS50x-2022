from sys import exit, argv
import csv


def main():
    # Checks if the number of arguments is correct
    if len(argv) != 3:
        print("Command-line argument: python dna.py file.csv file.txt")
        exit(1)

    # Create a list (database) of dictionaries with information about each person in the csv file
    database = []
    with open(argv[1], "r") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            # Make the values to int, with the exception of the name
            for key in row.keys():
                if key == "name":
                    continue
                row[key] = int(row[key])

            database.append(row)
        # Nucleotide sequences in the database
        sequences = (reader.fieldnames)
        del(sequences[0])

    # Reads the txt file that contains the requested person's DNA
    with open(argv[2], "r") as txtfile:
        person_dna = txtfile.readline()

    # Number of matches with nucleotide sequence from the database
    person_match = {}
    for nucleotides in sequences:
        person_match.update({nucleotides: max_match(person_dna, nucleotides)})
    
    print(check(database, sequences, person_match))


# Calculates the maximum number of nucleotides in sequence in DNA
def max_match(dna, nucleotides):
    match = 0
    for i in range(dna.find(nucleotides), len(dna)):
        # Checks the number of consecutive matches and stores the maximum value
        # i:i+len(nucleotides) returns the nucleotide substring
        if dna[i:i+len(nucleotides)] == nucleotides:
            i, tmp = number_match(i, dna, nucleotides)
            if tmp > match:
                match = tmp
    return match


# Calculates the number of consecutive matches and returns along with the cursor position
def number_match(cursor, dna, nucleotides):
    number = 0
    while(dna[cursor:cursor+len(nucleotides)] == nucleotides):
        number += 1
        cursor += len(nucleotides)

    return cursor, number


# Checks if the person is in the database
def check(database, sequences, person_match):
    cont = 0
    for dic in database:
        for key in sequences:
            # If get the same number of a certain nucleotide, increase by 1
            if dic[key] == person_match[key]:
                cont += 1
        # If the number of nucleotides is equal to the size of the base sequence, we find the person 
        if cont == len(sequences):
            return dic["name"]
        cont = 0
    return "No match"
    

if __name__ == "__main__":
    main()