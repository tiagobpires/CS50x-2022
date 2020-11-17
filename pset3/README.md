You can check the problem sets [here](https://cs50.harvard.edu/x/2020/psets/3/)

## Plurality
> Runs a plurality election </br>
> Every voter gets to vote for one candidate. At the end of the election, whichever candidate has the greatest number of votes is declared the winner.
```
$ ./plurality Alice Bob Charlie
Number of voters: 4
Vote: Alice
Vote: Bob
Vote: Charlie
Vote: Alice
Alice
```

## Runoff
> Runs a runoff election </br>
> It's a ranked-choice voting system, in which voters can vote for more than one candidate
```
$ ./runoff Alice Bob Charlie
Number of voters: 3
Rank 1: Alice
Rank 2: Bob
Rank 3: Charlie

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Alice
Rank 3: Charlie

Alice
```
