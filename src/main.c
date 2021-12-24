#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

void initAllegro(void);
void  loadMap(const char* filename);

ALLEGRO_DISPLAY* display;

unsigned char heightmap[128][64];
unsigned char biomemap[128][64];
unsigned char watermap[128][64];
unsigned char influencemap[128][64];
unsigned char city[128][64];
unsigned char cityEra[128][64];
unsigned char cityPopulation[128][64];
unsigned char terraformer[128][64];
unsigned char terraformerPower[128][64];
unsigned char terraformerType[128][64];



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
	//unsigned char byte1;
	//unsigned char byte2;
	//unsigned char testvar;
	uint16_t mapData;
	ALLEGRO_FILE* mapFile;
	mapFile = al_fopen(filename, "r");
	if (mapFile == NULL) { printf("file not found!\n"); exit(1); }
	al_fseek(mapFile, 0x80, ALLEGRO_SEEK_SET);

	//mapData = al_fread16be(mapFile);
	//testvar = (mapData & ~(0b0001111111111111)) >> 13;
	//printf("%x\n", testvar);

	//byte1 = al_fgetc(mapFile);
	//byte2 = al_fgetc(mapFile);
	for (mapX = 0; mapX < 128; mapX++)
	{
		for (mapY = 0; mapY < 64; mapY++)
		{
			mapData = al_fread16be(mapFile);

			biomemap[mapX][mapY] =		(mapData & ~(0b0001111111111111)) >> 13;
			heightmap[mapX][mapY] =		(mapData & ~(0b1110000011111111)) >> 5; //0-255
			watermap[mapX][mapY] =		(mapData & ~(0b1111111101111111)) >> 7; // 0 = Land ; 1= water
			influencemap[mapX][mapY] =	(mapData & ~(0b1111111110111111)) >> 6;



			megaobjectType =		mapData & ~(0b1111111111000000);

			if (megaobjectType == 0) //Nothing here
			{
				city[mapX][mapY] = 0;
				cityEra[mapX][mapY] = 0;
				cityPopulation[mapX][mapY] = 0;
				terraformer[mapX][mapY] = 0;
				terraformerPower[mapX][mapY] = 0;
				terraformerType[mapX][mapY] = 0;
			}
			if (megaobjectType >= 1 && megaobjectType <= 31) //City
			{
				city[mapX][mapY] = 1;
				cityEra[mapX][mapY] =			(mapData & ~(0b1111111111110011)) >> 2;
				cityPopulation[mapX][mapY] =	mapData & ~(0b1111111111111100); // 0 = Travelling
				terraformer[mapX][mapY] = 0;
				terraformerPower[mapX][mapY] = 0;
				terraformerType[mapX][mapY] = 0;
			}
			else  //Terriformer
			{
				city[mapX][mapY] = 0;
				cityEra[mapX][mapY] = 0;
				cityPopulation[mapX][mapY] = 0;
				terraformer[mapX][mapY] = 1;
				terraformerPower[mapX][mapY] =	(mapData & ~(0b1111111111110011) >> 2); 
				terraformerType[mapX][mapY] =	(mapData & ~(0b1111111111111100));
				//0 = Terriformer
				//1 = Biome factory
				//2 = Oxygenator
				//3 = N2 Generator
				//4 = Vaporator
				//5 = CO2 generator
				//6 = Monolith
				//7 = Ice Metor(daisy)
			}



			
	//		civmap[mapX][mapY] = byte2;
			al_draw_filled_rectangle((mapX * 10), (mapY * 10), (mapX * 10) + 10, (mapY * 10) + 10, 
				al_map_rgb(heightmap[mapX][mapY], heightmap[mapX][mapY], heightmap[mapX][mapY]), 0);
			//al_draw_pixel(mapX*10, mapY*10, al_map_rgb(value, value, value));
			//al_draw_filled_rectangle(mapX * 10, mapY * 10, mapX * 10 + 10, mapY * 10 + 10, al_map_rgb(value, value, value), 0);
			al_flip_display();
		}
	}


}