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

  def to_s()
    return "\##{red.to_s(16).rjust(4, '0')}#{green.to_s(16).rjust(4, '0')}#{blue.to_s(16).rjust(4, '0')}"
  end

end

