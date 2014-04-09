all:
	gcc w2obj.c -o w2obj

run:
	touch meh.obj && touch meh.obj.mtl && rm meh.obj meh.obj.mtl && ./w2obj bergen_1024x918.bin bergen_terrain_texture.png meh.obj

clean:
	rm -f w2obj
