class User(object):                     # 3
    def __init__(self, something):
        print("User Init called.")
        self.something = something

    def method(self):
        return self.something 

class Student(User):                    # 1
    def __init__(self, something):
        User.__init__(self, something)  # 2
        print("Student Init called.")
        self.something = something

    def method(self):
        return self.something 

my_object = Student('Jetty')