#!/usr/bin/python
import math

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
  map = []
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
    map.append((rr, gg, bb))
  return map


def cmp_to_rgb():
  """
  Returns an array that maps CMP palette value to rgb palette values.
  """
  return [((((r / 64) & 2) << 2) + (((r / 64) & 1))) + \
          ((((g / 64) & 2) << 3) + (((g / 64) & 1) << 1)) + \
          ((((b / 64) & 2) << 4) + (((b / 64) & 1) << 2)) \
            for (r, g, b) in cmp_to_rgb8()]

print cmp_to_rgb8()
print cmp_to_rgb()
