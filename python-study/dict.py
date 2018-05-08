#!/usr/bin/env python
# Filename: using_dict.py

# 'ab' is short for 'a'ddress'b'ook

def aaa():
    print "lixiao"
def aab():
    print "jun"
ab={'Swaroop'   : 'swaroopch@byteofpython.info',
    'Larry'     : 'larry@wall.org',
    'Matsumoto' : 'matz@ruby-lang.org',
    'Spammer'   : 'spammer@hotmail.com'
   }

abc={'aaa'   : aaa,
     'aab'   : aab
   }
print "Swaroop's address is %s" %ab['Swaroop']

# Adding a key/value pair
ab['Guido']='guido@python.org'

# Deleting a key/value pair
del ab['Spammer']

print '\nThere are %d contacts in the address-book\n' %len(ab)
for name,address in ab.items():
    print 'Contact %s at %s' %(name,address)

for name,func in abc.items():
    print 'Contact %s' %(name)
    func()

if 'Guido' in ab: # OR ab.has_key('Guido')
    print "\nGuido's address is %s" %ab['Guido']


