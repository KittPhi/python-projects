def checkType(a_list):
    print('The List', a_list)
    for element in a_list:
        if isinstance(element, int):
            print("Found a Integer: ", a_list[element])
        if isinstance(element, str):
            print("Found a String")
        if isinstance(element, float):
            print("Found a Float: ")

if __name__ == '__main__':
    a_list = ['A', 1, '!', 'a', 10.01 ]

    checkType(a_list)