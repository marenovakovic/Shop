#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_STORE "prodavnica.txt"
#define FILE_NAME_NEEDED "potrepstine.txt"
#define FILE_NAME_EXIT "izlazni.txt"

#define NUMBER_OF_VALUES_TO_READ_FOR_FOOD 4
#define NUMBER_OF_VALUES_TO_READ_FOR_NEEDED 3

#define TYPE_FOOD 0
#define TYPE_NEEDED 1

typedef struct {
	char *name;
	char mark;
	int howMuch;
	float price;
} FOOD;

typedef struct {
	FOOD *food;
	int size;
} ARRAY_AND_SIZE;

void printError();

ARRAY_AND_SIZE readFile(char *fileName, int type);

void checkAndWriteToFile(ARRAY_AND_SIZE foodAndSize, ARRAY_AND_SIZE needAndSize, char *writeWhere);

void realiseMemory(ARRAY_AND_SIZE foodAndSize);

int main() {
	FOOD *food;
	ARRAY_AND_SIZE foodAndSize, neededAndSize;
	FOOD *needed;
	int foodNumber, neededNumber;

	foodAndSize = readFile(FILE_NAME_STORE, TYPE_FOOD);

	food = foodAndSize.food;
	foodNumber = foodAndSize.size;

	for (int i = 0; i < foodNumber; ++i) {
		printf("%s %c %d %f\n", food[i].name, food[i].mark, food[i].howMuch, food[i].price);
	}

	neededAndSize = readFile(FILE_NAME_NEEDED, TYPE_NEEDED);

	needed = neededAndSize.food;
	neededNumber = neededAndSize.size;

	for (int i = 0; i < neededNumber; i++) {
		printf("%s %c %d\n", needed[i].name, needed[i].mark, needed[i].howMuch);
	}

	checkAndWriteToFile(foodAndSize, neededAndSize, FILE_NAME_EXIT);

	realiseMemory(foodAndSize);
	realiseMemory(neededAndSize);

	return 0;
}

ARRAY_AND_SIZE readFile(char *fileName, int type) {
	FOOD *food;
	int n;
	int len;
	int numberOfReadValues;
	char name[10];
	FILE *file;

	file = fopen(fileName, "r");

	fscanf(file, "%d", &n);

	if ((food = (FOOD *) malloc(n * sizeof(FOOD))) == NULL)
		printError();

	for (int i = 0; i < n; i++) {
		numberOfReadValues = fscanf(file, "%s", name);
		fgetc(file);
		numberOfReadValues += fscanf(file, "%c", &food[i].mark);
		numberOfReadValues += fscanf(file, "%d", &food[i].howMuch);

		if (type == TYPE_FOOD) {
			numberOfReadValues += fscanf(file, "%f", &food[i].price);
		}

		if (numberOfReadValues !=
				(type == TYPE_FOOD ? NUMBER_OF_VALUES_TO_READ_FOR_FOOD : NUMBER_OF_VALUES_TO_READ_FOR_NEEDED)) {
			printError();
		}

		len = strlen(name);

		if ((food[i].name = (char *) malloc(len * sizeof(char))) == NULL) {
			printError();
		}

		strcpy(food[i].name, name);
	}

	ARRAY_AND_SIZE foodAndSize;
	foodAndSize.food = food;
	foodAndSize.size = n;

	fclose(file);

	return foodAndSize;
}

void checkAndWriteToFile(ARRAY_AND_SIZE foodAndSize, ARRAY_AND_SIZE needAndSize, char *writeWhere) {
	FILE *file;

	int numberOfFood = foodAndSize.size;
	int numberOfNeeded = needAndSize.size;

	FOOD *food = foodAndSize.food;
	FOOD *needed = needAndSize.food;

	file = fopen(writeWhere, "w");

	for (int i = 0; i < numberOfNeeded; i++) {
		for (int j = 0; j < numberOfFood; j++) {
			if (strcmp(needed[i].name, food[j].name) == 0) {
				if (needed[i].howMuch <= food[j].howMuch) {
					float priceOfNeededFoodThatCanBeBought = needed[i].howMuch * food[j].price;
					fprintf(file, "%s %f %f\n", needed[i].name, food[j].price, priceOfNeededFoodThatCanBeBought);
				}
			}
		}
	}

	fclose(file);
}

void printError() {
	printf("ERROR\n");
	EXIT_FAILURE;
}

void realiseMemory(ARRAY_AND_SIZE foodAndSize) {
	for (int i = 0; i < foodAndSize.size; ++i) {
		free(foodAndSize.food[i].name);
	}

	free(foodAndSize.food);
}