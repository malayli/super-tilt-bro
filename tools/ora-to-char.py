#!/usr/bin/env python
"""
Utility to update a character with animations present in an Open Raster file (.ora)

Dependencies:
	* stblib
	* Open Raster Python lib: https://github.com/sgadrat/python-open-raster

Layers in the .ora file must have the following layout:
	`- palettes
	`- origin
	`- anims
		`- anim.victory
		|	`- anim.victory.frame1
		|		`- anim.victory.frame1.sprites
		|		|	`- sprite_layer
		|		|	`- sprite_layer
		|		|	`- ...
		|		`- anim.victory.frame2.sprites
		|		|	`- ...
		|		`- ...
		`- anim.defeat
			`- anim.defeat.frame1
			|	`- anim.defeat.frame1.sprites
			|		`- ...
			`- ...

"palettes" is a layer on which each pixel is a palette entry.
"origin" is a layer positioned in the image at character's origin.
"sprite_layer" are 8x8 layers, each of their pixel must be in a color present in "palettes". These layers can be named freely.
"""

import ora
import os
import re
from stblib import ensure
import stblib.animations
import stblib.character
import stblib.jsonformat
import stblib.tiles
import sys

def ora_to_character(image_file, char_name):
	character = stblib.character.Character(name = char_name)

	# Read Open Raster file
	image = ora.read_ora(image_file)

	# Find interesting elements
	animations_stack = None
	origin_layer = None
	palettes_layer = None
	for child in image['root']['childs']:
		if child.get('name') == 'anims':
			animations_stack = child
			ensure(animations_stack['type'] == 'stack', '"anims" is not a stack')
		elif child.get('name') == 'origin':
			origin_layer = child
			ensure(origin_layer['type'] == 'layer', '"origin" is not a layer')
		elif child.get('name') == 'palettes':
			palettes_layer = child
			ensure(palettes_layer['type'] == 'layer', '"palettes" is not a layer')

	# Convert origin info to a usable form
	ensure(origin_layer is not None, 'No origin layer found')
	origin = {
		'x': origin_layer['x'],
		'y': origin_layer['y']
	}

	# Convert palettes info to a usable form
	ensure(palettes_layer is not None, 'No palettes layer found')
	palettes = []
	for color in palettes_layer['raster'].getdata():
		palettes.append(_uniq_transparent(color))

	# Construct animations and tileset
	for animation_stack in animations_stack['childs']:
		# Parse animation
		animation = stblib.animations.Animation()

		m = re.match('^anims\.(?P<anim>[a-z0-9_]+)$', animation_stack['name'])
		ensure(m is not None, 'invalid animation stack name "{}"'.format(animation_stack['name']))
		anim_name = m.group('anim')
		animation.name = 'anim_{}_{}'.format(character.name, anim_name)

		for frame_stack in animation_stack['childs']:
			frame = stblib.animations.Frame()

			m = re.match('^anims\.(?P<anim>[a-z0-9_]+)\.frame(?P<frame>[0-9]+)$', frame_stack['name'])
			ensure(m is not None, 'invalid frame stack name "{}"'.format(frame_stack['name']))
			ensure(m.group('anim') == anim_name, 'frame stack "{}" is named after animation "{}" while in animation "{}"'.format(frame_stack['name'], m.group('anim'), anim_name))
			frame_id = m.group('frame')

			for frame_child in frame_stack['childs']:
				if frame_child['name'] == 'anims.{}.frame{}.hurtbox'.format(anim_name, frame_id):
					# Parse hurtbox
					ensure(frame_child['type'] == 'layer', '{} is note a layer'.format(frame_child['name']))

					hurtbox = stblib.animations.Hurtbox(
						left = frame_child['x'] - origin['x'],
						top = frame_child['y'] - origin['y']
					)
					hurtbox.right = hurtbox.left + frame_child['raster'].size[0]
					hurtbox.bottom = hurtbox.top + frame_child['raster'].size[1]
					frame.hurtbox = hurtbox

				elif frame_child['name'] == 'anims.{}.frame{}.sprites'.format(anim_name, frame_id):
					# Parse sprites
					ensure(frame_child['type'] == 'stack', '{} is not a stack'.format(frame_child['name']))
					for sprite_layer in frame_child['childs']:
						ensure(sprite_layer['type'] == 'layer', 'unexpected non-layer in {}: "{}"'.format(frame_child['name'], sprite_layer['name']))
						ensure(sprite_layer['raster'].size == (8, 8), 'unexpected sprite size of {}x{} in {}: "{}"'.format(
							sprite_layer['raster'].size[0], sprite_layer['raster'].size[1],
							frame_child['name'], sprite_layer['name']
						))

						# Parse tile
						tile = stblib.tiles.Tile()
						for y in range(8):
							for x in range(8):
								# Convert pixel data to palette index
								try:
									color_index = palettes.index(_uniq_transparent(sprite_layer['raster'].getpixel((x, y)))) % 4
								except ValueError:
									ensure(False, 'a sprite in {} use a color not found in palettes: sprite "{}", position "{}x{}", color "{}", palettes: {}'.format(frame_child['name'], sprite_layer['name'], x, y, sprite_layer['raster'].getpixel((x, y)), palettes))

								# Store pixel in tile
								tile._representation[y][x] = color_index

						# Add tile to tileset if needed, get its name and attributes
						tile_name = None
						flip = None
						for try_flip in [{'v': False, 'h': False}, {'v': False, 'h': True}, {'v': True, 'h': False}, {'v': True, 'h': True}]:
							flipped = stblib.tiles.Tile(representation = tile._representation.copy())
							if try_flip['v']:
								flipped.flip_v()
							if try_flip['h']:
								flipped.flip_h()
							try:
								tile_index = character.tileset.tiles.index(flipped)
								tile_name = character.tileset.tilenames[tile_index]
								flip = try_flip
								break
							except ValueError:
								pass

						if tile_name is None:
							tile_name = '{}_TILE_{}'.format(character.name.upper(), len(character.tileset.tiles))
							flip = {'v': False, 'h': False}
							character.tileset.tiles.append(tile)
							character.tileset.tilenames.append(tile_name)

						# Add sprite to frame
						numeric_attr = 0
						if flip['v']:
							numeric_attr += 0x80
						if flip['h']:
							numeric_attr += 0x40
						sprite = stblib.animations.Sprite(
							y = sprite_layer['y'] - origin['y'],
							tile = tile_name,
							attr = numeric_attr,
							x = sprite_layer['x'] - origin['x']
						)
						frame.sprites.append(sprite)
				else:
					# Refuse unknown children in a frame, it is certainly a naming error or something not yet supported
					ensure(False, 'unknown frame child in {}: "{}"'.format(frame_stack['name'], frame_child['name']))

			animation.frames.append(frame)

		# Store parsed animation in character
		if anim_name == 'victory':
			character.victory_animation = animation
		elif anim_name == 'defeat':
			character.defeat_animation = animation
		elif anim_name == 'menu_select':
			character.menu_select_animation = animation
		else:
			character.animations.append(animation)

	return character

def _uniq_transparent(color):
	ensure(isinstance(color, tuple) and len(color) == 4, 'expected RGBA images, but seems not to be the case')
	if color[3] == 0:
		return (0, 0, 0, 0)
	else:
		return color

# Check parameters
if len(sys.argv) != 4 or sys.argv[1] == '-h' or sys.argv[1] == '--help':
	print('usage: {} ora-file mod-path character-name'.format(sys.argv[0]))
	sys.exit(1)

ora_file_path = sys.argv[1]
base_path = sys.argv[2]
character_name = sys.argv[3]

# Generate character
character = ora_to_character(sys.argv[1], character_name)

# Load exisitng character data
character_main_file_path = '{base}/characters/{char}/{char}.json'.format(base = base_path, char = character_name)
orig_character = None
if os.path.exists(character_main_file_path):
	with open(character_main_file_path, 'r') as orig_character_file:
		orig_character = stblib.jsonformat.import_from_json(orig_character_file, base_path)

# Merge character read from ora into existing character
orig_character.tileset = character.tileset
orig_character.animations = character.animations
orig_character.victory_animation = character.victory_animation
orig_character.defeat_animation = character.defeat_animation
orig_character.menu_select_animation = character.menu_select_animation

# Write character's json
os.makedirs('{}/characters/{}'.format(base_path, character_name), exist_ok=True)
with open(character_main_file_path, 'w') as char_file:
	stblib.jsonformat.export_to_json(orig_character, char_file, base_path)
