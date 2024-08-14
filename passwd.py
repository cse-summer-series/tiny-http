import hashlib
import sys

tocheck = sys.argv[1]
tocheck = bytes.fromhex(tocheck)


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


def recsearch(prefix, l):
    if len(prefix) >= l:
        hashed = hashlib.md5(prefix.encode()).digest()
        if hashed == tocheck:
            print("Found it! %s = %s" % (prefix, hashlib.md5(prefix.encode()).hexdigest()))
            return True
        else:
            return False

    for c in alpha:
        s = prefix + c
        if recsearch(s, l): return True

for i in range(4, 20):
    result = recsearch("", i)
    if result: break

if not result:
    print("Did not find it")



