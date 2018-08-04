// Object IDs
// Every object in the game has a unique ID, good thing to know how to identify an object
// Good for object collision later on, since it can be called to see the objects collision boxes
enum IDS {
	Player,
	Enemy,
	Terrain
};

struct Hero {
	int ID;
	int x; //X position on display
	int y; //Y position on display
	int lives; // Number of lives left
	int speed; // unit speed
	int boundx; // Unit X boundaries for collision
	int boundy; // Unit Y boundaries for collision
};