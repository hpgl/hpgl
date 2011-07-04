import sys
import os

import pygtk
import gtk
import gtk.glade

class App:
	# инициализация
	def __init__(self):
		# загружаем файл с интерфейсом
		self.gladefile = "property_hist.glade"
		
		# дерево элементов интерфейса
		self.widgetsTree = gtk.glade.XML(self.gladefile)
		
		# словарь связей < событие - обработчик >
		dic =
			{
				# пользователь выбрал новое количество столбцов
				"new_bins_number_selected" : self.new_bins_number_selected,
			}
	
		# соединение сигналов и обработчиков
		self.widgetsTree.signal.autoconnect(dic)
		
		# соединение события закрытие окна и функции завершения приложения
		self.window = self.widgetsTree.get_widget("property_hist")
		if(self.window):
			self.window.connect("destroy", self.close_app)
		
		# параметры графиков по умолчанию
		self.bins = 50
		
		# вывод графика на экран
		#self.graphview = self.wTree.get_widget("histogram_box")
		#self.graphview.pack_start(self.canvas, True, True)
		
	def new_bins_number_selected(self, widget):
		# здесь нужно забрать из текстового поля число и перерисовать график матплотлиба

		
	# закрытие приложения
	def close_app(self, widget):
		gtk.main_quit()

if __name__ == "__main__":
	app = App()
	gtk.main()