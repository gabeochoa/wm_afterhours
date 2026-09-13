The text-free world map uses Natural Earth 1:110m land polygons, public-domain data:
https://github.com/nvkelso/natural-earth-vector/blob/master/geojson/ne_110m_land.geojson

`world_map.svg` projects longitude -180..180 and latitude 85..-60 to a 740×350
canvas. `world_map.png` is its transparent 2× CairoSVG export. Antarctica is omitted.
Markers use city longitude and latitude in the same projection. Selected locations,
flags, controls and labels remain native UI elements in SecureTunnelMockup.h.
