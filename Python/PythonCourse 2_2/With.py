f = open('/etc/passwd',r')
try:
    print f.getline()
    # lots of code
finally:
    f.close()


# Same as before: context manager (open must be programmed to be used with "with"
with open('/etc/passwd',r') as f:
    print f.getline()
    # lots of code
