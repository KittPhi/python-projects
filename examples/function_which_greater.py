def whichIsGreater(a,b):
    if a != b:
        if a > b:
            print(f'{a} is greater')
        if b > a:
            print(f'{b} is greater')
    else:
        print(f'{a} and {b} are equal')


whichIsGreater(10,20)