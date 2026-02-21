#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

void initAllegro(void);
void  loadMap(const char* filename);

ALLEGRO_DISPLAY* display;

typedef struct mapData
{
	unsigned char height;
	unsigned char biome;
	unsigned char water;
	unsigned char influence;
	unsigned char city;
	unsigned char cityEra;
	unsigned char cityPopulation;
	unsigned char terraformer;
	unsigned char terraformerPower;
	unsigned char terraformerType;

}MAPDATA;

MAPDATA map[128][64];



main()
{
	initAllegro();
	loadMap("C:\\Users\\halku\\Games\\PC\\EARTHWIN\\lab.pla");
	while (1);

}

void initAllegro(void)
{

	if (!al_init()) {
		printf("Could not init Allegro.\n");
		exit(1);
	}
	if (!al_install_keyboard()) {
		printf("Error installing keyboard.\n");
		exit(1);
	}
	if (!al_init_primitives_addon()) {
		printf("Error installing keyboard.\n");
		exit(1);
	}



	display = al_create_display(1280, 742);
	if (!display)
	{
		printf("Failed to create display!\n");
		exit(1);
	}

}

void loadMap(const char* filename)
{
	int mapX;
	int mapY;
	char megaobjectType;
	uint16_t mapBitplane;
	ALLEGRO_FILE* mapFile;
	mapFile = al_fopen(filename, "r");
	if (mapFile == NULL) { printf("file not found!\n"); exit(1); }
	al_fseek(mapFile, 0x80, ALLEGRO_SEEK_SET);


	for (mapX = 0; mapX < 128; mapX++)
	{
		for (mapY = 0; mapY < 64; mapY++)
		{
			mapBitplane = al_fread16be(mapFile);

			map[mapX][mapY].biome =		(mapBitplane & ~(0b0001111111111111)) >> 13;
			map[mapX][mapY].height =	(mapBitplane & ~(0b1110000011111111)) >> 5; //shifted to make it 0-255 
			map[mapX][mapY].water =		(mapBitplane & ~(0b1111111101111111)) >> 7; // 0 = Land ; 1= water
			map[mapX][mapY].influence = (mapBitplane & ~(0b1111111110111111)) >> 6; 



			megaobjectType =		mapBitplane & ~(0b1111111111000000);

			if (megaobjectType == 0)  //Nothing here
	{
		map[mapX][mapY].city = 0;
		map[mapX][mapY].cityEra = 0;
		map[mapX][mapY].cityPopulation = 0;
		map[mapX][mapY].terraformer = 0;
		map[mapX][mapY].terraformerPower = 0;
		map[mapX][mapY].terraformerType = 0;
	}

			if (megaobjectType >= 1  && megaobjectType <= 31) //City
			{
				map[mapX][mapY].city = 1;
				map[mapX][mapY].cityEra =			(mapBitplane & ~(0b1111111111110011)) >> 2;
				map[mapX][mapY].cityPopulation =	mapBitplane & ~(0b1111111111111100); // 0 = Travelling
				map[mapX][mapY].terraformer = 0;
				map[mapX][mapY].terraformerPower = 0;
				map[mapX][mapY].terraformerType = 0;
			}
			if (megaobjectType >= 32)  //Terriformer
			{
				map[mapX][mapY].city = 0;
				map[mapX][mapY].cityEra = 0;
				map[mapX][mapY].cityPopulation = 0;
				map[mapX][mapY].terraformer = 1;
				map[mapX][mapY].terraformerPower =	(mapBitplane & ~(0b1111111111110011) >> 2);
				map[mapX][mapY].terraformerType =	(mapBitplane & ~(0b1111111111111100));
				//0 = Terriformer
				//1 = Biome factory
				//2 = Oxygenator
				//3 = N2 Generator
				//4 = Vaporator
				//5 = CO2 generator
				//6 = Monolith
				//7 = Ice Metor(daisy)
			}


			al_draw_filled_rectangle((mapX * 10), (mapY * 10), (mapX * 10) + 10, (mapY * 10) + 10, 
				al_map_rgb(map[mapX][mapY].height, map[mapX][mapY].height, map[mapX][mapY].height));

			al_flip_display();
		}
	}


}