

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person
{
	int		id;
	char	name[24];
	char	last_name[24];
	int		age;
	char	country[24];
	char	city[24];
	char	profession[24];
	char	e_mail[24];
	int		no_of_kids;
	char	fav_food[24];
	struct _person *prev;
	struct _person *next;
} person;

void add_person( person *database )
{
	char buf[1000];
	person *new_person = malloc( sizeof(person) );
	person *ptr = database->prev;
	printf("Adding a person to the database: \n");

	printf("Name: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->name );

	printf("Last name: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->last_name );

	printf("Age: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%i", &new_person->age);
	
	printf( "Country: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->country);
	
	printf( "City: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->city);

	printf( "Profession: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->profession);
	
	printf( "E-mail: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->e_mail);
	
	printf( "Number of kids: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%i", &new_person->no_of_kids);
	
	printf( "Favourite food: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%23[^\n]s", new_person->fav_food);
	
	new_person->id = (database->prev->id) + 1;

	database->prev->next = new_person;
	database->prev = new_person;
	new_person->prev = ptr;
	new_person->next = database;
	
}

void delete_person( person *database)
{
	char buf[1000];
	int temp;
	person *person = database->next;
	printf("Which person do you want to delete form the database[give ID]: ");
	fgets( buf, sizeof(buf), stdin );
	sscanf( buf, "%i", &temp);
	
	while( person != database )
	{
		if( person->id == temp )
		{
			person->next->prev = person->prev;
			person->prev->next = person->next;
			free(person);
			break;
		}
		person = person->next;
	}
}

void show( person *database )
{
	person *person = database->next;
	
	
	while( person != database )
	{
		printf( "ID %i:\n", person->id );
		printf("Name: %s", person->name);
		
		printf("\nLast name: %s", person->last_name);
		
		printf("\nAge: %i", person->age);

		printf("\nCountry: %s", person->country);
	
		printf("\nCity: %s", person->city);
	
		printf("\nProfession: %s", person->profession);

		printf("\nE-mail: %s", person->e_mail);
	
		printf("\nno_of_kids: %i", person->no_of_kids);

		printf("\nFavourite food: %s\n", person->fav_food);
		person = person->next;
	}
}


int main(int argc, char *argv[]) 
{
	
	person *database;
	//printf("%lu\n", sizeof(person));
	database = malloc(sizeof(person));
	database->prev = database->next = database;
	database->id = 0;

	char instruction[1000];

	do
	{
		printf("What do you want to do?\n[1] Add person to the database\n[2] Delete person from the database\n[3] Show entire database\n[4] Exit\n");
		fgets(instruction, 1000, stdin);
		if(instruction[1] != '\n')
		{
			printf("Too many characters\n" );
			continue;
		}
		switch(*instruction)
		{
			case '1':
				add_person( database );
				break;
			case '2':
				delete_person( database );
				break;
			case '3':
				show( database );
				break;
			case '4':
				break;
			default:
				printf("Unknown choice '%c'\n", *instruction );
		}	
	} while( *instruction != '4' );	
	exit(0);
}