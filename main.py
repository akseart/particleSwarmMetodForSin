#coding=utf-8
import matplotlib.pyplot as plt
import math
 
# Функция для отображения
def func(x):
   return [math.sin(i) for i in x]
 
if __name__ == '__main__':
   # Значения по X
   x = [i for i in range(-10, 10)]
   # Значения по Y
   y = func(x)
 
   plt.title("X^2 plot")
   plt.xlabel("X")
   plt.ylabel("Y")
   # представляем точки (х,у) кружочками диаметра 10
   plt.plot(x, y, 'r')
 
   # Сетка на фоне для улучшения восприятия
   plt.grid(True, linestyle='-', color='0.75')
 
   plt.show()