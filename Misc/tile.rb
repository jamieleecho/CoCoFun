#!/usr/bin/ruby

# Script that breaks up an image into NxM sized tiles
# and applies k-means to get an optimal tile set of K
# tiles. Outputs the tiles as gifs (tile0.gif,
# tile2.gif ... tilek-1.gif) and tiles.json which is a list
# of lists of tile numbers.

require './pixel_color'
require "rmagick"
require "set"
include Magick


def tiles_from_image(image_path, width, height)
  image = Image.read(image_path)
  sample = image.sample
  tiles = []
  (0...sample.rows).step(height).each do |yy|
    (0...sample.columns).step(width).each do |xx|
      tiles << tile_from_image(sample, xx, yy, width, height)
    end
  end
  return tiles
end

# extracts the tile(x, y, width, height) from image as an
# array of flattened HSV tuples.
def tile_from_image(sample, x, y, width, height)
  tile = []
  (y...y + height).each do |yy|
    (x... x + width).each do |xx|
      color = sample.pixel_color(xx, yy).to_HSL
      tile << color[0]
      tile << color[1]
      tile << color[2]
    end
  end
  return tile
end

tiles = tiles_from_image(ARGV[0], 16, 16)
puts tiles.length
tile_set = Set.new(tiles)
puts tile_set.length

