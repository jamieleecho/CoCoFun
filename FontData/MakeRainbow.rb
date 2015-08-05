#!/usr/bin/ruby
require "rmagick"
include Magick

width = 92
height = 46
image = Image.new width, height
gc = Draw.new
gc.stroke_antialias false

# Rainbow colors
colors = ["red", "orange", "yellow", "green", "cyan", "blue", "violet", "white"]
offset = 0
colors.each do |color|
  circle = Draw.new
  circle.fill color
  circle.circle width/2, height, width/2, offset
  circle.draw image
  offset = offset + 5
end

# Save
image.write("rainbow.png")


