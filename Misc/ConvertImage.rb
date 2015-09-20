#!/usr/bin/ruby
require './pixel_color'
require "rmagick"
include Magick

# Load the image
file = ARGV[0]
imageList = ImageList.new(file)
image = imageList[0]
width = image.columns
height = image.rows

# Define the CoCo palette here
cocoPalette = [ 63, 36, 46, 52, 25, 18, 38, 56, 7, 54, 40, 34, 11, 41, 9, 0 ]

# Output data structure
puts 'byte image[] = {'
(0..(image.rows-1)).each do |row|
  oldPixelColorIndex = 0
  (0..(image.columns-1)).each do |column|
    pixel = image.get_pixels(column, row, 1, 1)[0]
    pixelColor = PixelColor.new pixel.red, pixel.green, pixel.blue
    pixelColorIndex = pixelColor.index_of_closest_coco3_color cocoPalette
    if ((column & 1) == 1)
      byte = (oldPixelColorIndex << 4) | pixelColorIndex
      print "0x#{byte.to_s(16)},"
    else
      oldPixelColorIndex = pixelColorIndex
    end
  end
  puts '0xff,'
end
puts '0xff'

puts '};'

