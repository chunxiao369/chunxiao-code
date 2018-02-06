
#!/usr/bin/python
print range(11)
print range(1,11)

min(range(1,11))
max(range(1,11))

print zip([1,2,3,4,5,6],[0,1,2,3,4,5])
print map(lambda x: x * 2, range(1,11))
print reduce(lambda x,y: x + y, range(1,11))
print filter(lambda x: x % 3 == 0, range(1,11))
print sum(range(1,11))

tweet = "This is an example tweet talking about scala and sbt"
wordlist = ["scala", "akka", "play framework", "sbt", "typesafe"]
print map(lambda x: x in tweet.split(), wordlist)

n=50
print sorted(set(range(2,n+1)).difference(set((p * f) for p in range(2,int(n**0.5) + 2) for f in range(2,(n/p)+1))))
