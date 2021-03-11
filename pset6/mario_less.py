from cs50 import get_int

# Checks if n is between 1 and 8 and re-prompt if it's not
while True:
    n = get_int("Height: ")
    if n >= 1 and n <= 8:
        break
    
# Create n lines
for i in range(n):
    
    # Create columns
    for j in range(n - 1, i - 1, -1):
        # Print white spaces
        if j > i:
            print(" ", end="")
        
        if j == i:
            # Print # diagonal
            print("#", end="")
            # Print # in the remaining spaces and jump one line
            print("#" * i)