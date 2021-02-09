#ifndef FIELD_H
#define FIELD_H

enum FieldType{EMPTY = 0, SHIP = 1, HIT = 2, MISS = 3, SUNKEN = 4, SHIP_PICK_START = 5, AVAILABLE_SHIP_END = 6};

struct Field{
	int  x;
	int y;
	enum FieldType type;
};

#endif

