#!/usr/bin/python
import math

from colormath.color_objects import LabColor, sRGBColor
from colormath.color_conversions import convert_color
from colormath.color_diff import delta_e_cie2000


def cmp_to_rgb8():
  """
  Returns an array of (r, g, b) tuples such that array[ii] is the ideal map
  from CMP palette ii to an 8-bit RGB color table.  Pretty much a direct
  translation of the algorithm by John Kowalski and Erik Gavriluk. See:
  https://github.com/milliluk/mame/commit/606270218d4a324c42330932b46c4ef07d0b0e83
  """
  r = [0.0] * 64
  g = [0.0] * 64
  b = [0.0] * 64
  l = [0.0] * 4
  g = [0.0] * 64

  sat = 92.0
  con = 70.0
  bri = -50
  l[0] = 0.0
  l[1] = 47.0
  l[2] = 120.0
  l[3] = 255.0

  w = 0.4195456981879 * 1.01
  a = w * 9.2
  s = a + w * 5
  d = s + w * 5
  p = 0
  vals = []
  for h in range(0, 4):
    p = p + 1
    bri = bri + con
    for gg in range(1, 16):
      r[p] = (math.cos(a) * sat) * 1 + bri
      g[p] = (math.cos(s) * sat) * 1 + bri
      b[p] = (math.cos(d) * sat) * 1 + bri
      p = p + 1
      a = a + w
      s = s + w
      d = d + w
    r[p - 16] = l[h]
    g[p - 16] = l[h]
    b[p - 16] = l[h]
  r[63] = r[48]
  g[63] = g[48]
  b[63] = b[48]

  for h in range(0, 64):
    rr = int(r[h])
    gg = int(g[h])
    bb = int(b[h])
    rr = 0 if rr < 0 else rr
    gg = 0 if gg < 0 else gg
    bb = 0 if bb < 0 else bb
    rr = 255 if rr > 255 else rr
    gg = 255 if gg > 255 else gg
    bb = 255 if bb > 255 else bb
    vals.append((rr, gg, bb))
  return vals 


def rgb_to_rgb8():
  """
  Returns an array of (r, g, b) tuples such that array[ii] is the ideal map
  from RGB palette ii to an 8-bit RGB color table.
  """
  def map_component(vv, v):
    val = (vv * 2) + v
    if (val >= 3):
      return 255
    if (val == 2):
      return 120
    if (val == 1):
      return 47
    return 0

  vals = []
  for rr in xrange(0, 2):
    for gg in xrange(0, 2):
      for bb in xrange(0, 2):
        for r in xrange(0, 2):
          for g in xrange(0, 2):
            for b in xrange(0, 2):
              red = map_component(rr, r)
              green = map_component(gg, g)
              blue = map_component(bb, b)

              vals.append((red, green, blue))
  return vals


def map_colors(original_colors, other_colors):
  """
  Returns an array such that array[ii] provides the index to the closest color
  in other_colors to the closest color in original_colors[ii]. original_colors
  and other_colors are assumed to contain LAB colors and are the same length
  """
  vals = []
  for other_color in other_colors:
    deltas = [delta_e_cie2000(original_color, other_color) 
                for original_color in original_colors]
    min_delta = min(deltas)
    vals.append(deltas.index(min_delta))
  return vals


def output_mappings():
  cmp_rgb_colors = [sRGBColor(r / 255.0, g / 255.0, b / 255.0)
                      for (r, g, b) in cmp_to_rgb8()]
  rgb_rgb_colors = [sRGBColor(r / 255.0, g / 255.0, b / 255.0)
                      for (r, g, b) in rgb_to_rgb8()]
  rgb_lab_colors = [convert_color(rgb_color, LabColor)
                      for rgb_color in rgb_rgb_colors]
  cmp_lab_colors = [convert_color(rgb_color, LabColor)
                      for rgb_color in cmp_rgb_colors]

  c2r = map_colors(rgb_lab_colors, cmp_lab_colors)
  r2c = map_colors(cmp_lab_colors, rgb_lab_colors)

  print 'ii\tr2c\tc2r'
  print '--\t---\t---'
  for ii in xrange(0, len(r2c)):
    print '{}\t{}\t{}'.format(ii, r2c[ii], c2r[ii])

output_mappings()

