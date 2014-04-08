all:
	gcc w2obj.c -o w2obj

run:
	rm meh.obj && ./w2obj data/bergen_1024x918.bin data/bergen_terrain_texture.png meh.obj

clean:
	rm -f w2obj
