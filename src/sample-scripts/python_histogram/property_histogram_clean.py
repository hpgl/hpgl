#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *

import sys
import os

import pygtk
import gtk
import gtk.glade

import matplotlib   
matplotlib.use('GTK') 
from matplotlib.figure import Figure   
from matplotlib.axes import Subplot
from matplotlib.backends.backend_gtk import FigureCanvasGTK, NavigationToolbar   

# property working - for now it's python function (reading from bos/eclipse file)
from python_property_hpgl import *


class App:
	def __init__(self):
		self.gladefile = "property_hist.glade"
		
		self.widgetsTree = gtk.glade.XML(self.gladefile)
		
		dic = {
				"new_bins_number_selected"      :       self.new_bins_number_selected,
				"on_range_check_toggled"	:	self.on_range_check_toggled,
				"on_min_cutoff_box_activate"	:	self.on_min_cutoff_box_activate,
				"on_max_cutoff_box_activate"	:	self.on_max_cutoff_box_activate,
				"on_cumulative_check_toggled"	:	self.on_cumulative_check_toggled,
				"on_normalized_check_toggled"	:	self.on_normalized_check_toggled,
				"on_log_check_toggled"		:	self.on_log_check_toggled,
				"on_obj_combo_changed"		:	self.on_obj_combo_changed,
                                "on_save_file_clicked"          :       self.on_save_file_clicked,
			}
	
		self.widgetsTree.signal_autoconnect(dic)
		
		self.window = self.widgetsTree.get_widget("property_hist")
		
		if(self.window):
			self.window.connect("destroy", self.close_app)
		
		# -- initialization
		self.bins = 50
		
		self.cumulative = False
		self.cumulative_check = self.widgetsTree.get_widget("cumulative_check")
		
		self.normalized = False
		self.normalized_check = self.widgetsTree.get_widget("normalized_check")
		
		self.log = False
		self.log_check = self.widgetsTree.get_widget("log_check")
		
		self.undefined_value = -99
		self.property_names = [ "BIG_SOFT_DATA_CONT", "BIG_SOFT_IND_DATA", "SOME_OTHER_OBJECT"]
		
		# objects initialization
		self.properties = []
		self.properties.append(load_property_python_hpgl(166,141,20,"GAUSSIAN_PROP.INC",intype=True))
		self.properties.append(load_property_python_hpgl(166,141,20,"BIG_SOFT_DATA_160_141_20.INC",intype=True))
		
		#default object is 0
		self.property = self.properties[0]
		self.property_name = self.property_names[0]
		self.property_defined_only = self.property[self.property != self.undefined_value]
		
		self.bins_editbox = self.widgetsTree.get_widget("bins_editbox")
		self.bins_editbox.insert_text(str(self.bins), position = 0)
		
		# objects box
		cell = []
		self.object_box = gtk.ListStore(str)
		self.cb = self.widgetsTree.get_widget("obj_combo")
		self.cb.set_model(self.object_box)

		for k in xrange(len(self.property_names)):
			self.object_box.append([self.property_names[k]])
			cell.append(gtk.CellRendererText())
			self.cb.pack_start(cell[k])
			self.cb.add_attribute(cell[k],'text',k)
		
		self.cb.set_active(0)
		
		
		# -- histogram drawing
		self.figure = Figure(figsize=(6,4), dpi=72)
		self.axis = self.figure.add_subplot(111)   
		self.axis.hist(self.property_defined_only, bins = self.bins, range = self.range, normed = self.normalized, cumulative = self.cumulative, log = self.log)
		self.axis.set_xlabel('Values')   
		self.axis.set_ylabel('Count')   
		self.axis.set_title(self.property_names[self.cb.get_active()])   
		self.axis.grid(True)   
		self.canvas = FigureCanvasGTK(self.figure) # a gtk.DrawingArea   
		self.canvas.show()   
		self.graphview = self.widgetsTree.get_widget("draw_area")   
		self.graphview.pack_start(self.canvas, True, True) 	
		
		# range elements 
		self.max_cutoff = self.widgetsTree.get_widget("max_cutoff_box")
		self.max_cutoff.hide()
		self.min_cutoff = self.widgetsTree.get_widget("min_cutoff_box")
		self.min_cutoff.hide()
		
		self.min_cutoff_text = self.widgetsTree.get_widget("min_cutoff_text")
		self.min_cutoff_text.hide()
		self.max_cutoff_text = self.widgetsTree.get_widget("max_cutoff_text")
		self.max_cutoff_text.hide()
		
		self.range_check = self.widgetsTree.get_widget("range_check")

		self.save_file = self.widgetsTree.get_widget("save_file")
                # statistic calculation
		self.init()
		
		self.window.show()
		
	def new_bins_number_selected(self, widget):
		self.bins = int(self.bins_editbox.get_chars(0, -1))
		self.repaint_hist()
		
	def repaint_hist(self):
		self.axis.clear()
		self.axis.set_xlabel('Values')   
		if self.normalized:
			self.axis.set_ylabel('Frequency')   
		else:
			self.axis.set_ylabel('Count')   
		self.axis.hist(self.property_defined_only, bins = self.bins, range = self.range, normed = self.normalized, cumulative = self.cumulative, log = self.log)
		self.axis.set_title(self.property_name)   
		self.axis.grid(True)   
		self.graphview.hide_all()
		self.graphview.show_all()
		
	def on_range_check_toggled(self, widget):
		if self.range_check.get_active():
			self.max_cutoff.show()
			self.min_cutoff.show()
			self.min_cutoff_text.show()
			self.max_cutoff_text.show()
			self.repaint_hist()
		else:
			self.range_state = False
			self.max_cutoff.hide()
			self.min_cutoff.hide()	
			self.min_cutoff_text.hide()
			self.max_cutoff_text.hide()
			self.repaint_hist()
	
	def on_max_cutoff_box_activate(self,widget):
		self.range[1] = double(self.max_cutoff.get_text())
		self.repaint_hist()
	
	def on_min_cutoff_box_activate(self,widget):
		self.range[0] = double(self.min_cutoff.get_text())
		self.repaint_hist()
		
	def on_cumulative_check_toggled(self, widget):
		if self.cumulative_check.get_active():
			self.cumulative = True
			self.repaint_hist()
		else:
			self.cumulative = False
			self.repaint_hist()
	
	def on_normalized_check_toggled(self, widget):
		if self.normalized_check.get_active():
			self.normalized = True
			self.repaint_hist()
		else:
			self.normalized = False
			self.repaint_hist()	

	def on_log_check_toggled(self, widget):
		if self.log_check.get_active():
			self.log = True
			self.repaint_hist()
		else:
			self.log = False
			self.repaint_hist()	
	
	def on_obj_combo_changed(self, widget):
		del(self.property)
		del(self.property_name)
		self.property_name = self.property_names[self.cb.get_active()]
		self.property = self.properties[self.cb.get_active()]
		self.property_defined_only = self.property[self.property != self.undefined_value]
		self.init()
		self.repaint_hist()

	# save file
	def on_save_file_clicked(self, widget):
		print "saving..."
		self.save_file_dialog = gtk.FileChooserDialog(parent = self.window, action = gtk.FILE_CHOOSER_ACTION_SAVE, buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, gtk.STOCK_SAVE, gtk.RESPONSE_OK))
		response = self.save_file_dialog.run()
		filter = gtk.FileFilter()
		filter.set_name("png image")
		filter.add_pattern("*."+"png")
		self.save_file_dialog.add_filter(filter)
		self.figure.savefig(self.save_file_dialog.get_filename())
		self.save_file_dialog.destroy()

	def init(self):
		# -- statistics
		
		
		# mean 
		self.mean_editbox = self.widgetsTree.get_widget("mean_box")
		self.mean_editbox.set_text(str(self.property_defined_only.mean()))
		
		# median
		self.median_editbox = self.widgetsTree.get_widget("median_box")
		self.median_editbox.set_text(str(median(self.property_defined_only)))
		
		# variance
		self.var_box = self.widgetsTree.get_widget("var_box")
		self.var_box.set_text(str(self.property_defined_only.var()))
		
		# max & min
		self.max_box = self.widgetsTree.get_widget("max_box")
		self.max_box.set_text(str(self.property_defined_only.max()))
		
		self.min_box = self.widgetsTree.get_widget("min_box")
		self.min_box.set_text(str(self.property_defined_only.min()))
		
		# now we can set ranges :)
		self.range = [self.property_defined_only.min(), self.property_defined_only.max()]
		
		# number of all points
		self.min_box = self.widgetsTree.get_widget("points_num_box")
		self.min_box.set_text(str(self.property.size))
		
		# number of defined points
		self.min_box = self.widgetsTree.get_widget("def_points_num_box")
		self.min_box.set_text(str(self.property_defined_only.size))
		
		self.max_cutoff.set_text(str(self.range[1]))
		self.min_cutoff.set_text(str(self.range[0]))

	def close_app(self, widget):
		gtk.main_quit()

if __name__ == "__main__":
	app = App()
	gtk.main()
	
