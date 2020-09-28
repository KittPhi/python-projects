# Different ways to print 2D Array via Lists
# Array=[y][x]
Array9x4 = [['a','b','c','d'],
            ['e','f','g','h'],
            ['i','j','k','l'],
            ['m','n','o','p'],
            ['q','r','s','t'],
            ['u','v','w','x'],
            ['y','z','0','1'],
            ['2','3','4','5'],
            ['6','7','8','9']]

print('\nPrint bottom right corner\n')
print(Array9x4[8][3])

print('\nPrint bottom left corner\n')
print(Array9x4[8][0])
# print('\nPrint 9x4 array', Array9x4, '\n')

# print('\nPrint row1 (x values), where y=0\n')
# for x in range(0,4):  # 0...3
#     print(Array9x4[0][x])

# print('\nPrint column1 (y values), where x=0\n')
# for y in range(0,9):    # 0...8
#     print(Array9x4[y][0])

# print('\nPrint left to right, top to bottom\n')
# for y in range(0,9):    # start at column1
#     for x in range(0,4):
#         print(Array9x4[y][x])
#     print('\n')

# print('\nPrint top to bottom, left to right\n')
# for x in range(0,4):
#     for y in range(0,9):
#         print(Array9x4[y][x])
#     print('\n')