import hashlib
import sys

tocheck = sys.argv[1]


alpha = 'abcdefghijklmnopqrstuvwxyz'

def search():
    for c1 in alpha:
        for c2 in alpha:
            for c3 in alpha:
                for c4 in alpha:
                    s = c1 + c2 + c3 + c4
                    hashed = hashlib.md5(s.encode()).hexdigest()
                    if hashed == tocheck:
                        print("Found it! %s = %s" % (s, hashed))
                        return
    print("Did not find it")


search()


