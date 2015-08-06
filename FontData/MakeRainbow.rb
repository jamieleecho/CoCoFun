#!/usr/bin/ruby
require "rmagick"
include Magick

width = 92
height = 46
image = Image.new width, height

rectangle = Draw.new
rectangle.stroke_antialias false
rectangle.fill "light blue"
rectangle.rectangle 0, 0, width, height
rectangle.draw image

# Rainbow colors
colors = ["red", "orange", "yellow", "green", "cyan", "blue", "violet", "light blue"]
offset = 0
colors.each do |color|
  circle = Draw.new
  circle.fill color
  circle.stroke_antialias false
  circle.circle width/2, height, width/2, offset
  circle.draw image
  offset = offset + 5
end

# Save
image.write("rainbow.png")


