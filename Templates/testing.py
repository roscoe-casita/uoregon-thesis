#from pylab import *

import turtle

def old_main():
    draw = Drawer()
    draw.draw_simple(4)


def main():
    plot([1,2,3])
    show()

class Drawer:
    def __init__(self):
        self.my_turtle = turtle.Turtle()
        self.my_screen = turtle.Screen()

    def draw_simple(self,n):

        self.my_screen.setworldcoordinates(0,0,n,n)
        self.my_turtle.penup()
        self.my_turtle.goto(0,0)
        self.my_turtle.pendown()
        self.my_turtle.speed(0)

        draw_list = self.draw_hilbert(0,0,n,0,0,n,n)

        for dl in draw_list:
            (x,y) = dl
            self.my_turtle.speed(0)
            self.my_turtle.goto(x,y)



    def draw_hilbert(self,x,y,xi,xj,yi,yj,n):
        if n <= 0 :
            return [(x + (xi + yi) / 2,y + (xj + yj) / 2)]
        else:
            returnValue = []
            returnValue.extend(self.draw_hilbert(x, y, yi / 2,yj / 2,xi / 2,xj / 2,n - 1))
            returnValue.extend(self.draw_hilbert(x + xi / 2,y + xj / 2,xi / 2,xj / 2,yi / 2,yj / 2,n - 1))
            returnValue.extend(self.draw_hilbert(x + xi / 2 + yi / 2,y + xj / 2 + yj / 2,xi / 2,xj / 2,yi / 2,yj / 2,n - 1))
            returnValue.extend(self.draw_hilbert(x + xi / 2 + yi,y + xj / 2 + yj,-yi / 2,-yj / 2,-xi / 2,-xj / 2,n - 1))
            return returnValue


if __name__ == "__main__":
    old_main()
    #main()