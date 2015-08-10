#!/usr/bin/ruby
require "rmagick"
include Magick


class PixelColor
  attr_reader :red, :green, :blue
  attr_reader :hue, :saturation, :value

  def self.from_coco3(coco3Color)
    red = ((coco3Color & 0x04) >> 2) | ((coco3Color & 0x20) >> 4)
    green = ((coco3Color & 0x02) >> 1) | ((coco3Color & 0x10) >> 3)
    blue = (coco3Color & 0x01) | ((coco3Color & 0x08) >> 2)
    red = ((0xffff / 3.0) * red).to_int
    green = ((0xffff / 3.0) * green).to_int
    blue = ((0xffff / 3.0) * blue).to_int

    PixelColor.new(red, green, blue)
  end

  def initialize(red, green, blue)
    @red = red
    @green = green
    @blue = blue

    colors = [red, green, blue]
    cmax = 1.0 * colors.max
    cmin = 1.0 * colors.min
    delta = cmax - cmin

    if (delta == 0)
      @hue = 0.0
    elsif (red == cmax)
      @hue = 60.0 * (((green - blue)/delta) % 6)
    elsif (green == cmax)
      @hue = 60.0 * (((blue - red)/delta) + 2)
    else
      @hue = 60.0 * (((red - green)/delta) + 4)
    end

    @saturation = (cmax == 0.0) ? 0.0 : delta / cmax
    @value = cmax
  end

  def index_of_closest_coco3_color(colors)
    current_distance = Float::INFINITY
    current_index = -1
    index = 0
    colors.each do |coco3_color|
      color = PixelColor::from_coco3 coco3_color
      new_distance = distance(color)
      if (new_distance < current_distance)
        current_distance = new_distance
        current_index = index
      end
      index = index + 1
    end

    current_index
  end

  def distance(color)
    Math.sqrt(((@hue - color.hue) ** 2) + (((@saturation - color.saturation)*200) ** 2) + (((@value - color.value)/512) ** 2))
  end

end

width = 92
height = 46
image = Image.new width, height

rectangle = Draw.new
rectangle.stroke_antialias false
rectangle.fill "#0040FF"
rectangle.rectangle 0, 0, width, height
rectangle.draw image

# Rainbow colors
colors = ["#FF0000", "#FF8000", "#FFF000", "#00FF00", "#0080FF", "#0000FF", "#FF00FF", "#0040FF"]
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

