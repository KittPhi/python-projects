global_x = 'will reassigned'
local_x = 'wont be reassined'

def some_func():
    global gobal_x   # <<<<<<< MUST DO
    print(global_x)
    global_x = 'global is reassigned'
    local_x = 'global not reassiged'

some_func()
print(global_x) # reassigned by local
print(local_x)  # not re-assign by local