def function(x,y):
    """Figure out which of the two inputs is larger

    Parameters
    ----------
    x : int or float
    y : int or float

    return int or float
    """
    
    z = x if x>y else y

    return z

print(function(2,4), "is larger")