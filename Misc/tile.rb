#!/usr/bin/ruby

# tile filename k
# Script that breaks up an image into NxM sized tiles
# and applies k-means to get an optimal tile set of K
# tiles. 

require 'fileutils'
require 'rubygems'
require 'set'
require 'rmagick'
require 'k_means'

require './pixel_color'

include Magick


# Draws in image of size width, height by drawing the tiles in tiles.
def image_from_tiles(filename, width, height, tiles, tile_width, tile_height)
  draw = Draw.new
  ii = 0
  (0...height).step(tile_height).each do |yy|
    (0...width).step(tile_width).each do |xx|
      tile = tiles[ii]
      ii = ii + 1
      draw_tile(draw, tile, tile_width, tile_height, xx, yy)
    end
  end

  image = Image.new width, height
  draw.draw image
  image.write filename
end


# Draws tile at x, y
def draw_tile(draw, tile, tile_width, tile_height, x, y)
  ii = 0
  (0...tile_height).each do |yy|
    (0...tile_width).each do |xx|
      c = Pixel.from_hsla(tile[ii] * 360, tile[ii + 1] * 255, tile[ii + 2] * 255)
      ii = ii + 3
      draw.fill(c.to_color)
      draw.point(x + xx, y + yy)
    end
  end
end


# Returns the list of tiles made by breaking image_path into tiles
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
      color = sample.pixel_color(xx, yy).to_hsla
      tile << color[0] / 360.0
      tile << color[1] / 255.0
      tile << color[2] / 255.0
    end
  end
  return tile
end


class Array
  def hsv_distance(other)
    sum = 0
    ii = 0 
    other.each do |x|
      delta = self[ii] - x
      ii = ii + 1
      delta = ((ii % 3 == 0) and delta.abs > 0.5) ? 1 - delta.abs : delta 
      sum = sum + delta * delta
    end
    return Math.sqrt(sum)
  end
end


def save_tile(tile, width, height, filename)
  draw = Draw.new
  ii = 0
  (0...height).each do |yy|
    (0...width).each do |xx|
      c = Pixel.from_hsla(tile[ii] * 360, tile[ii + 1] * 255, tile[ii + 2] * 255)
      ii = ii + 3
      draw.fill(c.to_color)
      draw.point(xx, yy)
    end
  end

  image = Image.new width, height
  draw.draw image
  image.write filename
end


TILE_SIZE = 16

# Break the image into unique images
tiles = tiles_from_image(ARGV[0], TILE_SIZE, TILE_SIZE)
stiles = Set.new(tiles).to_a
tile_to_stile = {}
(0...tiles.length).each do |ii|
  tile_to_stile[ii] = stiles.index(tiles[ii])
end

# Perform k-means
kmeans = KMeans.new(stiles, :centroids => ARGV[1].to_i, :distance_measure => :hsv_distance)

# Save the different groups identified by k-means
jj = 0
stile_to_group = {}
groups = []
kmeans.view.each do |group_indices|
  group_name = "groups/group_#{jj.to_s.rjust(3, '0')}"
  centroid = kmeans.centroids[jj]
  FileUtils.mkpath group_name
  groups << centroid.position
  if group_indices.length > 0
    save_tile centroid.position, TILE_SIZE, TILE_SIZE, "#{group_name}/tile.gif"
  end
  group_indices.each do |ii|
    save_tile stiles[ii], TILE_SIZE, TILE_SIZE, "#{group_name}/tile_#{ii.to_s.rjust(3, '0')}.gif"
    stile_to_group[ii] = jj
  end
  jj = jj + 1
end


# Recontruct image based on groups
image = Image.read(ARGV[0])
sample = image.sample
new_tiles = []
ii = 0
(0...sample.rows).step(TILE_SIZE).each do |yy|
  (0...sample.columns).step(TILE_SIZE).each do |xx|
    new_tiles << groups[stile_to_group[tile_to_stile[ii]]]
    ii = ii + 1
  end
end
image_from_tiles("groups/image.gif", sample.columns, sample.rows, new_tiles, TILE_SIZE, TILE_SIZE)



