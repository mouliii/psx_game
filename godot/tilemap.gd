@tool
extends Node2D

@export_tool_button("generate", "AssetLib") var action = Generate

@onready var tile_map_layer: TileMapLayer = $TileMapLayer


@export var fileName:String = "MAP.DAT"
@export var version:int = 1
@export var tileSize = 16
@export var textureWidth := 256 # TileSetAtlasSource, getter ?
#@export var mapWidth = 32
#@export var mapHeight = 32

func Generate()->void:
	var file = FileAccess.open(fileName.to_upper(), FileAccess.WRITE)
	if !file:
		push_error("unable to open file!")
		return
	# id
	file.store_8(125)
	# version
	file.store_8(version)
	# tile size
	file.store_8(tileSize)
	# width
	file.store_8(tile_map_layer.get_used_rect().size.x)
	# height
	file.store_8(tile_map_layer.get_used_rect().size.y)
	# nLayers
	# TODO
	# data
	var mapWidth = tile_map_layer.get_used_rect().size.x
	var mapHeight = tile_map_layer.get_used_rect().size.y
	var spriteSheetWidthInTiles:int = textureWidth / tileSize
	for y in mapHeight:
		for x in mapWidth:
			var coords := tile_map_layer.get_cell_atlas_coords(Vector2i(x, y))
			if coords.x < 0 || coords.y < 0:
				file.store_8(255)
			else:
				file.store_8(coords.y * spriteSheetWidthInTiles + coords.x)
	file.close()
