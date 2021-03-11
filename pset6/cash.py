from cs50 import get_float

# Checks if cash is positive and re-prompt if it's not
while True:
    cash = get_float("Change owed: ")
    if cash > 0:
        break

# Convert and rounds to an integer number
cents = int(round(cash * 100))
coins = 0

# Check if it is possible to decrease in every case
while cents >= 25:
    cents -= 25
    # Count the number of change coins
    coins += 1

while cents >= 10:
    cents -= 10
    coins += 1
    
while cents >= 5:
    cents -= 5
    coins += 1

while cents >= 1:
    cents -= 1
    coins += 1

print(f"{coins}")s