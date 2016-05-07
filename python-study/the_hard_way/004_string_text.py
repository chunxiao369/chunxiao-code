#!/usr/bin/python
x = "There are %d types of people." % 10
binary = "binary"
do_not = "don't"
y = "Those who know %s and those who %s." % (binary, do_not)
print x
print y
print "I said: %r." % x
print "I also said: '%s'." % y
hilarious = False
joke_evaluation = "Isn't that joke so funny?! %r"
print joke_evaluation % hilarious
w = "This is the left side of..."
e = "a string with a right side."
print w + e

tabby_cat = "\tI'm tabbed in."
persian_cat = "I'm split\non a line."
backslash_cat = "I'm \\ a \\ cat."
fat_cat = """
I'll do a list:
\t* Cat food
\t* Fishies
\t* Catnip\n\t* Grass
"""

print tabby_cat
print persian_cat
print backslash_cat
print fat_cat

while True:
    for i in ["/","-","|","\\","|"]:
        print "%s\r" % i,
