list = [1,2,3,5,7,8,9]

for i in list:
    if (i % 2 == 0):
        print('found even', i)
    elif (i % 2 != 0):
        print('found odd', i)
    else:
        print('found something else...', i)