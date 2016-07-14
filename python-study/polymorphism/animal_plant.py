#!/usr/bin/python
#coding=utf-8
class Animal(object):
    def run(self):
        print 'Animal is running...'

class Dog(Animal):
    def run(self):
        print 'Dog is running...'

class Cat(Animal):
    def run(self):
        print 'Cat is running...'

class Tortoise(Animal):
    def run(self):
        print 'Tortoise is running slowly...'

def run_once(animal):
    animal.run()

'''
dog is animal
animal is not dog
'''
if __name__=='__main__':
    dog = Dog()
    cat = Cat()
    tort = Tortoise()
    animals = [dog, cat, tort]
    for ani in animals:
        run_once(ani)
