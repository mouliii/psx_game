@tool
extends Node

@export_tool_button("generate", "Bake") var act = Generate


func Generate()->void:
	var file = FileAccess.open("level.dat", FileAccess.WRITE)
	# header id
	file.store_8(123)
	# version
	file.store_8(1)
	# nollia loput 16 yht 32
	file.store_16(0)
	# TEXTURE #
	# id
	file.store_16(100)
	# 
	
	
	file.close()
