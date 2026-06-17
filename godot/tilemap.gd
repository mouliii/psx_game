@tool
extends Node2D

@export_tool_button("generate", "AssetLib") var action = Generate

@onready var bgLayer: TileMapLayer = $TileMapLayer
@onready var fgLayer: TileMapLayer = $TileMapLayer2

@export var fileName:String = "MAP.DAT"
@export var version:int = 1
@export var tileSize := 16
@export var textureWidth := 256 # TileSetAtlasSource, getter ?
#@export var mapWidth = 32
#@export var mapHeight = 32

func Generate()->void:
	var file = FileAccess.open(fileName.to_upper(), FileAccess.WRITE)
	if !file:
		push_error("unable to open file!")
		return
	GenerateBG(file)
	GenerateFG(file)
	file.close()

func GenerateBG(file:FileAccess)->void:
		# id
	file.store_8(125)
	# version
	file.store_8(version)
	# tile size
	file.store_8(tileSize)
	# width
	file.store_8(bgLayer.get_used_rect().size.x)
	# height
	file.store_8(bgLayer.get_used_rect().size.y)
	# nLayers
	file.store_8(get_child_count())
	var mapWidth = bgLayer.get_used_rect().size.x
	var mapHeight = bgLayer.get_used_rect().size.y
	# layer size
	#file.store_16(mapWidth * mapHeight)
	# data
	@warning_ignore("integer_division")
	var spriteSheetWidthInTiles := textureWidth / tileSize
	for y in mapHeight:
		for x in mapWidth:
			var coords := bgLayer.get_cell_atlas_coords(Vector2i(x, y))
			if coords.x < 0 || coords.y < 0:
				file.store_8(255)
			else:
				file.store_8(coords.y * spriteSheetWidthInTiles + coords.x)

func GenerateFG(file:FileAccess)->void:
	#TODO: map pos - size
	var mapWidth = fgLayer.get_used_rect().size.x
	var mapHeight = fgLayer.get_used_rect().size.y
	@warning_ignore("integer_division")
	var spriteSheetWidthInTiles := textureWidth / tileSize
	var nTiles:int = 0
	var tiles:Array = []
	for y in mapHeight:
		for x in mapWidth:
			var coords := fgLayer.get_cell_atlas_coords(Vector2i(x, y))
			if coords.x >= 0 && coords.y >= 0:
				nTiles += 1
				tiles.append([x, y, coords.y * spriteSheetWidthInTiles + coords.x])
	file.store_16(nTiles)
	file.store_8(0xFF)
	print(nTiles)
	for t in tiles:
		file.store_8(t[0])
		file.store_8(t[1])
		file.store_8(t[2])
