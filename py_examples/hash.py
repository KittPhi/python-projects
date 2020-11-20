# Hash produces a private key

import sha3

# Sender
data = 'Hello World'

s = sha3.sha3_224(data.encode('utf-8')).hexdigest()

print(s)

# Receiver
if (s == '8e800079a0b311788bf29353f400eff969b650a3597c91efd9aa5b38'):
    print('Hello World')

