# Overloading User-Defined Functions

class Student:
    def hello(self, name=None):
        if name is not None:
            print('Hey ' + name)
        else:
            print('Hey ')

# Creating a class instance
std = Student()

# Call the method
std.hello()

# Call the method and pass a parameter
std.hello('Nicholas')

# ---------------------------------------

class Purchase:
    def __init__(self, basket, buyer):
        self.basket = list(basket)
        self.buyer = buyer

    def __len__(self):
        return 10

purchase = Purchase(['pen', 'book', 'pencil'], 'Python')
print(len(purchase))