#!/usr/bin/env python3

from enum import Enum
class Token(Enum):
    NUMBER = 1
    MINUS = 2
    LEFT_PAREN = 3
    PLUS = 4
    RIGHT_PAREN = 5

def scan(cmd: str) -> list:
    """Function to extract unique tokens from command string

    Parameters
    ----------
    cmd : str
        Command string including numerical and arithmetic symbols

    Returns
    -------
    list
        A list of token tuples
    
    """
    tokens = []

    # this will be useful for handling multi-character tokens
    buffer = [] 
    
    for c in cmd:
        if c.isnumeric(): # a number
            tokens.append((Token.NUMBER, c))
        elif c == '-':    # a minus sign
            tokens.append((Token.MINUS, c))                        
        elif c == '+':    # a plus sign
            tokens.append((Token.PLUS, c))            
        elif c == '(':    # left paren
            tokens.append((Token.LEFT_PAREN, c))
        elif c == ')':    # right paren
            tokens.append((Token.RIGHT_PAREN, c))
        else:             # right paren
            raise Exception("Unknown token encountered!")

    print(tokens)

def main():

    # this example does not have whitespace and uses single-digit integers
    scan("1-(2+3)")

if __name__ == "__main__":
    main()
