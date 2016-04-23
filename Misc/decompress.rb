#!/usr/bin/ruby

# Script to convert compressed Pix.bin data to GIF
# decompress infile out.gif pal0 pal1 ...pal15

require './pixel_color'
require "rmagick"
include Magick

# Decompresses data in infile and returns decompressed data as string
def decompress(infile)
  compressed_data = File.binread(infile)
  ii = 5
  uncompressed_data = ''

  while ii < compressed_data.length - 5 and uncompressed_data.length < 24576
    c = compressed_data[ii]
    ii = ii + 1

    # Run Length Encoded data
    if c.ord == 0xff 
      # Get the length
      count_hi = compressed_data[ii].ord
      ii = ii + 1
      count_lo = compressed_data[ii].ord
      ii = ii + 1
      count = (count_hi << 8) + count_lo

      # Get the repeating byte
      c = compressed_data[ii]
      ii = ii + 1

      # Write out the repeating character
      while count > 0
        count = count - 1
        uncompressed_data << c
      end

    # Uncompressed data
    else
      uncompressed_data << c
    end
  end

  return uncompressed_data
end


# Given a 16-color rawimage buffer of width and height
# creates and saves the corresponding gif file.
def rawimage_to_gif(rawimage, width, height, palette, image_path)
  image = Image.new width, height
  draw = Draw.new
  ii = 0
  (0...height).each do |yy|
    (0...width).step(2).each do |xx|
      c0 = PixelColor.from_coco3(palette[(rawimage[ii].ord & 0xf0) >> 4])
      draw.fill(c0.to_s())
      draw.point(xx, yy)
      c1 = PixelColor.from_coco3(palette[rawimage[ii].ord & 0xf])
      draw.fill(c1.to_s())
      draw.point(xx + 1, yy)
      ii = ii + 1
    end
  end
  draw.draw image
  image.write image_path
end


rawimage = decompress(ARGV[0])
colors = ARGV[2...ARGV.length].map {|c| c.to_i(16) >> 0}
rawimage_to_gif(rawimage, 256, 192, colors, ARGV[1])


