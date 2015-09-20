#!/usr/bin/ruby

require 'rmagick'
include Magick

class String
  def is_upper?
    !!self.match(/\p{Upper}/)
  end

  def is_lower?
    !!self.match(/\p{Lower}/)
    # or: !self.is_upper?
  end
end

# Factor to use to reduce size of fonts
reductionFactor = 5

# Maps a character to a hash
# Each hash contains
# :width     => character width in pixels
# :height    => character width in pixels
# :num_bytes => total number of bytes for that character
# :bytes     => array representing bytes for that character
char2CharData = {}

# Iterate through all files
Dir.foreach(".") do |file|
  # Only look at png files
  if file !~ /.+\.png/
    next
  end

  # Open the image
  puts "Processing #{file}"
  glyphImageList = ImageList.new(file)
  glyphImage = glyphImageList[0]
  glyphImagePixels = glyphImage.export_pixels_to_str

  # Convert each pixel in the image to 0 and 1
  numXBits = (glyphImage.columns + reductionFactor - 1)/reductionFactor
  numYBits = (glyphImage.rows + reductionFactor - 1)/reductionFactor
  puts "#{numXBits} x #{numYBits}"
  bits = Array.new((numXBits + 7)/8) { Array.new(numYBits) { 0 } }
  pixels = Array.new(numXBits) { Array.new(numYBits) { ' ' } }
  glyphImagePixelsIndex = 0
  (0..(glyphImage.rows-1)).each do |row|
    # Skip rows as needed
    if ((row % reductionFactor) != 0)
      glyphImagePixelsIndex += 3 * glyphImage.columns
      next
    end

    bitMask = 0
    colCount = 0
    (0..(glyphImage.columns-1)).each do |column|
      # Skip columns as needed
      if ((column % reductionFactor) != 0)
        glyphImagePixelsIndex += 3
        next
      end

      # Compute whether or not the pixel is set
      sum = glyphImagePixels.getbyte(glyphImagePixelsIndex)
      sum += glyphImagePixels.getbyte(glyphImagePixelsIndex + 1)
      sum += glyphImagePixels.getbyte(glyphImagePixelsIndex + 2)
      glyphImagePixelsIndex += 3
      pixelIsSet = (sum < 0x27d)

      # If the pixel is set, put it in the array
      pixels[column / reductionFactor][row / reductionFactor] = pixelIsSet ? 1 : 0
      bitMask = bitMask | (pixelIsSet ? 0x1 : 0)
      print pixelIsSet ? '#' : ' '

      # Stuff the bits
      colCount = colCount + 1
      if ((colCount % 8) == 0)
        bits[(colCount / 8) - 1][row / reductionFactor] = bitMask
        bitMask = 0
      else
        bitMask = (bitMask << 1)
      end
    end

    # Update bits array
    bitsToShift = 8 - (numXBits % 8)
    if (bitsToShift < 8)
      bitMask = bitMask << (bitsToShift - 1)
      bits[bits.length-1][row / reductionFactor] = bitMask
    end

    # Start a new line
    puts
  end

  # Put character information into data structure
  charData = {
    :width => pixels.length,
    :height => pixels[0].length,
    :num_bytes => bits.length * bits[0].length + 2,
    :num_width_bytes => bits.length,
    :num_height_bytes => bits[0].length,
    :offset => 0,
    :bytes => bits,
    :skip => :none
  }
  char2CharData[file[0]] = charData
end

# Iterate through [a, z] mapping missing upper/lower cases chars
(97..122).each do |x|
  myChar = [x].pack('c*')
  if ((char2CharData[myChar] != nil) && (char2CharData[myChar.upcase] == nil))
    char2CharData[myChar.upcase] = char2CharData[myChar]
    char2CharData[myChar][:skip] = :upper
  elsif ((char2CharData[myChar.upcase] != nil) && (char2CharData[myChar] == nil))
    char2CharData[myChar] = char2CharData[myChar.upcase]
    char2CharData[myChar][:skip] = :lower
  end
end

# Iterate through all of the visible ASCII characters and record the number of bytes required to
# store each char.
offset = 0
offsets = []
chars = (32..127).map{ |x| [x].pack('c*') }
chars.each do |myChar|
  charData = char2CharData[myChar]
  if (charData == nil)
    offsets << -1
    next
  end

  # Update the offset for the given character
  charData[:offset] = offset
  offsets << offset
  if (myChar.is_lower? && (charData[:skip] == :lower)) ||
     (myChar.is_upper? && (charData[:skip] == :upper))
     next
  end
  offset = offset + charData[:num_bytes]
end

# The offsets for the uppercase characters not valid if they
# should have been skipped. Fix them here.
upperChars = (65..90).each do |charVal|
  myChar = [charVal].pack('c*')
  charData = char2CharData[myChar]
  if (charData == nil) || (charData[:skip] != :upper)
    next
  end
  offset = char2CharData[myChar.downcase][:offset]
  charData[:offset] = offset
  offsets[charVal - 32] = offset
end

# Output the C data structure for the font index that maps a character to its data structure offset
puts '// Index into fontData. Maps chars 32->127 to offsets into fontData.'
puts '// -1 imples a glyph is not defined for that character.'
puts "unsigned FontDataFontIndex[] = {#{offsets.join(",")}};\n"
puts
puts

# Output the C data structure for each character
puts '// Each glyph is preceded by 2 bytes that define the rows and columns per font'
puts '// The remaining bytes define 1-bit data organized as bytes for each glyph'
puts "byte FontDataFontData[] = {"
firstChar = true
chars.each do |myChar| 
  # Make sure a glyph is defined
  charData = char2CharData[myChar]
  if (charData == nil)
    next
  end

  if (myChar.is_lower? && (charData[:skip] == :lower)) ||
     (myChar.is_upper? && (charData[:skip] == :upper))
    next
  end
  offset = offset + charData[:num_bytes]

  if (!firstChar)
    puts ','
  end
  firstChar = false

  puts
  puts "// Font Data for '#{myChar}' #{charData[:width]}x#{charData[:height]}"
  puts "#{charData[:width]}, #{charData[:height]}"
  firstByte = true
  charBytes = charData[:bytes]
  (0..charData[:num_height_bytes]-1).each do |row|
    (0..charData[:num_width_bytes]-1).each do |column|
      print ",0x#{charBytes[column][row].to_s(16)} "
    end
    puts
  end

end
puts "};"

