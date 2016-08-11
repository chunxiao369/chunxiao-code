# http://ginstrom.com/scribbles/2007/10/08/design-patterns-python-style/

"""Implementation of the abstract factory pattern"""

import random


class PetShop:
    """A pet shop"""

    def __init__(self, animal_factory=None):
        """pet_factory is our abstract factory.
        We can set it at will."""

        self.pet_factory = animal_factory

    def show_pet(self):
        """Creates and shows a pet using the
        abstract factory"""

        pet = self.pet_factory.get_pet()
        print("This is a lovely", pet)
        print("It says", pet.speak())
        print("It eats", self.pet_factory.get_food())


# Stuff that our factory makes
class Animal:
    def speak(self):
        return "......"

    def __str__(self):
        return "I'm Animal"


class Dog(Animal):
    def speak(self):
        return "woof"

    def __str__(self):
        return "Dog"


class Cat(Animal):
    def speak(self):
        return "meow"

    def __str__(self):
        return "Cat"


# Factory classes

class DogFactory:
    def get_pet(self):
        return Dog()

    def get_food(self):
        return "dog food"


class CatFactory:
    def get_pet(self):
        return Cat()

    def get_food(self):
        return "cat food"


# Show pets with various factories
if __name__ == "__main__":
    shop = PetShop()

    shop.pet_factory = DogFactory()
    shop.show_pet()
    print("=" * 20)

    shop.pet_factory = CatFactory()
    shop.show_pet()
    print("=" * 20)
